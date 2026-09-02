// Copyright 2026 Yoichiro Tanaka
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Remap firmware Raw HID handler.
//
// Commands 0x81-0xEF mirror the behavior of the corresponding VIA commands
// (Remap id N is VIA id N - 0x80). This file is a vendored copy of the
// switch in quantum/via.c raw_hid_receive as of QMK 0.34.0, calling the same
// public helpers so behavior stays in lockstep with VIA without depending on
// quantum/via.c. When bumping the QMK base version, review via.c for
// behavioral changes to the mirrored commands.

#ifdef REMAP_ENABLE

#ifndef RAW_ENABLE
#    error "RAW_ENABLE is not enabled"
#endif

#ifndef DYNAMIC_KEYMAP_ENABLE
#    error "DYNAMIC_KEYMAP_ENABLE is not enabled"
#endif

#include "remap.h"

#include "raw_hid.h"
#include "dynamic_keymap.h"
#include "eeconfig.h"
#include "keycodes.h"
#include "matrix.h"
#include "timer.h"
#include "wait.h"
#include "version.h" // for QMK_BUILDDATE used in EEPROM magic
#include "nvm_remap.h"

#if defined(AUDIO_ENABLE)
#    include "audio.h"
#endif

#if defined(BACKLIGHT_ENABLE)
#    include "backlight.h"
#endif

#if defined(RGBLIGHT_ENABLE)
#    include "rgblight.h"
#endif

// Embedded Remap keyboard definition, generated at build time by
// util/generate_remap_definition.py into .build/*/src/remap_definition.c.
extern const uint32_t remap_definition_size;
extern const uint8_t  remap_definition_bytes[];

// Can be called in an overriding remap_init_kb() to test if keyboard level
// code usage of EEPROM is invalid and use/save defaults.
bool remap_eeprom_is_valid(void) {
    char   *p      = QMK_BUILDDATE; // e.g. "2019-11-05-11:29:54"
    uint8_t magic0 = ((p[2] & 0x0F) << 4) | (p[3] & 0x0F);
    uint8_t magic1 = ((p[5] & 0x0F) << 4) | (p[6] & 0x0F);
    uint8_t magic2 = ((p[8] & 0x0F) << 4) | (p[9] & 0x0F);

    uint8_t ee_magic0;
    uint8_t ee_magic1;
    uint8_t ee_magic2;
    nvm_remap_read_magic(&ee_magic0, &ee_magic1, &ee_magic2);

    return ee_magic0 == magic0 && ee_magic1 == magic1 && ee_magic2 == magic2;
}

void remap_eeprom_set_valid(bool valid) {
    if (valid) {
        char   *p      = QMK_BUILDDATE;
        uint8_t magic0 = ((p[2] & 0x0F) << 4) | (p[3] & 0x0F);
        uint8_t magic1 = ((p[5] & 0x0F) << 4) | (p[6] & 0x0F);
        uint8_t magic2 = ((p[8] & 0x0F) << 4) | (p[9] & 0x0F);
        nvm_remap_update_magic(magic0, magic1, magic2);
    } else {
        nvm_remap_update_magic(0xFF, 0xFF, 0xFF);
    }
}

// Override this at the keyboard code level to check Remap's EEPROM valid
// state and reset to defaults as needed. See via_init_kb() for the VIA
// equivalent.
__attribute__((weak)) void remap_init_kb(void) {}

void remap_init(void) {
    remap_init_kb();
    remap_set_layout_options_kb(remap_get_layout_options());

    if (!remap_eeprom_is_valid()) {
        eeconfig_init_remap();
    }
}

void eeconfig_init_remap(void) {
    nvm_remap_erase();
    remap_eeprom_set_valid(false);
    remap_set_layout_options(REMAP_EEPROM_LAYOUT_OPTIONS_DEFAULT);
    dynamic_keymap_reset();
    dynamic_keymap_macro_reset();
    remap_eeprom_set_valid(true);
}

uint32_t remap_get_layout_options(void) {
    return nvm_remap_read_layout_options();
}

__attribute__((weak)) void remap_set_layout_options_kb(uint32_t value) {}

void remap_set_layout_options(uint32_t value) {
    remap_set_layout_options_kb(value);
    nvm_remap_update_layout_options(value);
}

#if REMAP_EEPROM_CUSTOM_CONFIG_SIZE > 0
uint32_t remap_read_custom_config(void *buf, uint32_t offset, uint32_t length) {
    return nvm_remap_read_custom_config(buf, offset, length);
}
uint32_t remap_update_custom_config(const void *buf, uint32_t offset, uint32_t length) {
    return nvm_remap_update_custom_config(buf, offset, length);
}
#endif

#if defined(AUDIO_ENABLE)
float remap_device_indication_song[][2] = SONG(STARTUP_SOUND);
#endif

__attribute__((weak)) void remap_set_device_indication(uint8_t value) {
#if defined(BACKLIGHT_ENABLE)
    backlight_toggle();
#endif
#if defined(RGBLIGHT_ENABLE)
    rgblight_toggle_noeeprom();
#endif
#if defined(AUDIO_ENABLE)
    if (value == 0) {
        wait_ms(10);
        PLAY_SONG(remap_device_indication_song);
    }
#endif
}

bool process_record_remap(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (keycode >= QK_MACRO && keycode <= QK_MACRO_MAX) {
            uint8_t id = keycode - QK_MACRO;
            dynamic_keymap_macro_send(id);
            return false;
        }
    }

    return true;
}

__attribute__((weak)) void remap_custom_value_command_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id = &(data[0]);
    *command_id         = id_remap_unhandled;
}

__attribute__((weak)) void remap_custom_value_command(uint8_t *data, uint8_t length) {
    uint8_t *channel_id = &(data[1]);

#if defined(BACKLIGHT_ENABLE)
    if (*channel_id == id_remap_qmk_backlight_channel) {
        remap_qmk_backlight_command(data, length);
        return;
    }
#endif

#if defined(RGBLIGHT_ENABLE)
    if (*channel_id == id_remap_qmk_rgblight_channel) {
        remap_qmk_rgblight_command(data, length);
        return;
    }
#endif

    (void)channel_id;

    remap_custom_value_command_kb(data, length);
}

__attribute__((weak)) bool remap_command_kb(uint8_t *data, uint8_t length) {
    return false;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    uint8_t *command_id   = &data[0];
    uint8_t *command_data = &data[1];

    if (remap_command_kb(data, length)) {
        return;
    }

    switch (*command_id) {
        case id_remap_identify: {
            command_data[0] = REMAP_MAGIC_0;
            command_data[1] = REMAP_MAGIC_1;
            command_data[2] = REMAP_MAGIC_2;
            command_data[3] = (REMAP_PROTOCOL_VERSION >> 8) & 0xFF;
            command_data[4] = REMAP_PROTOCOL_VERSION & 0xFF;
            break;
        }
        case id_remap_get_keyboard_value: {
            switch (command_data[0]) {
                case id_remap_uptime: {
                    uint32_t value  = timer_read32();
                    command_data[1] = (value >> 24) & 0xFF;
                    command_data[2] = (value >> 16) & 0xFF;
                    command_data[3] = (value >> 8) & 0xFF;
                    command_data[4] = value & 0xFF;
                    break;
                }
                case id_remap_layout_options: {
                    uint32_t value  = remap_get_layout_options();
                    command_data[1] = (value >> 24) & 0xFF;
                    command_data[2] = (value >> 16) & 0xFF;
                    command_data[3] = (value >> 8) & 0xFF;
                    command_data[4] = value & 0xFF;
                    break;
                }
                case id_remap_switch_matrix_state: {
                    uint8_t offset = command_data[1];
                    uint8_t rows   = 28 / ((MATRIX_COLS + 7) / 8);
                    uint8_t i      = 2;
                    for (uint8_t row = 0; row < rows && row + offset < MATRIX_ROWS; row++) {
                        matrix_row_t value = matrix_get_row(row + offset);
#if (MATRIX_COLS > 24)
                        command_data[i++] = (value >> 24) & 0xFF;
#endif
#if (MATRIX_COLS > 16)
                        command_data[i++] = (value >> 16) & 0xFF;
#endif
#if (MATRIX_COLS > 8)
                        command_data[i++] = (value >> 8) & 0xFF;
#endif
                        command_data[i++] = value & 0xFF;
                    }
                    break;
                }
                case id_remap_firmware_version: {
                    uint32_t value  = REMAP_FIRMWARE_VERSION;
                    command_data[1] = (value >> 24) & 0xFF;
                    command_data[2] = (value >> 16) & 0xFF;
                    command_data[3] = (value >> 8) & 0xFF;
                    command_data[4] = value & 0xFF;
                    break;
                }
                case id_remap_keycodes_version: {
                    uint32_t value  = QMK_KEYCODES_VERSION_BCD;
                    command_data[1] = (value >> 24) & 0xFF;
                    command_data[2] = (value >> 16) & 0xFF;
                    command_data[3] = (value >> 8) & 0xFF;
                    command_data[4] = value & 0xFF;
                    break;
                }
                default: {
                    *command_id = id_remap_unhandled;
                    break;
                }
            }
            break;
        }
        case id_remap_set_keyboard_value: {
            switch (command_data[0]) {
                case id_remap_layout_options: {
                    uint32_t value = ((uint32_t)command_data[1] << 24) | ((uint32_t)command_data[2] << 16) | ((uint32_t)command_data[3] << 8) | (uint32_t)command_data[4];
                    remap_set_layout_options(value);
                    break;
                }
                case id_remap_device_indication: {
                    uint8_t value = command_data[1];
                    remap_set_device_indication(value);
                    break;
                }
                default: {
                    *command_id = id_remap_unhandled;
                    break;
                }
            }
            break;
        }
        case id_remap_dynamic_keymap_get_keycode: {
            uint16_t keycode = dynamic_keymap_get_keycode(command_data[0], command_data[1], command_data[2]);
            command_data[3]  = keycode >> 8;
            command_data[4]  = keycode & 0xFF;
            break;
        }
        case id_remap_dynamic_keymap_set_keycode: {
            dynamic_keymap_set_keycode(command_data[0], command_data[1], command_data[2], (command_data[3] << 8) | command_data[4]);
            break;
        }
        case id_remap_dynamic_keymap_reset: {
            dynamic_keymap_reset();
            break;
        }
        case id_remap_custom_set_value:
        case id_remap_custom_get_value:
        case id_remap_custom_save: {
            remap_custom_value_command(data, length);
            break;
        }
        case id_remap_dynamic_keymap_macro_get_count: {
            command_data[0] = dynamic_keymap_macro_get_count();
            break;
        }
        case id_remap_dynamic_keymap_macro_get_buffer_size: {
            uint16_t size   = dynamic_keymap_macro_get_buffer_size();
            command_data[0] = size >> 8;
            command_data[1] = size & 0xFF;
            break;
        }
        case id_remap_dynamic_keymap_macro_get_buffer: {
            uint16_t offset = (command_data[0] << 8) | command_data[1];
            uint16_t size   = command_data[2]; // size <= 28
            dynamic_keymap_macro_get_buffer(offset, size, &command_data[3]);
            break;
        }
        case id_remap_dynamic_keymap_macro_set_buffer: {
            uint16_t offset = (command_data[0] << 8) | command_data[1];
            uint16_t size   = command_data[2]; // size <= 28
            dynamic_keymap_macro_set_buffer(offset, size, &command_data[3]);
            break;
        }
        case id_remap_dynamic_keymap_get_layer_count: {
            command_data[0] = dynamic_keymap_get_layer_count();
            break;
        }
        case id_remap_dynamic_keymap_get_buffer: {
            uint16_t offset = (command_data[0] << 8) | command_data[1];
            uint16_t size   = command_data[2]; // size <= 28
            dynamic_keymap_get_buffer(offset, size, &command_data[3]);
            break;
        }
#ifdef ENCODER_MAP_ENABLE
        case id_remap_dynamic_keymap_get_encoder: {
            uint16_t keycode = dynamic_keymap_get_encoder(command_data[0], command_data[1], command_data[2] != 0);
            command_data[3]  = keycode >> 8;
            command_data[4]  = keycode & 0xFF;
            break;
        }
        case id_remap_dynamic_keymap_set_encoder: {
            dynamic_keymap_set_encoder(command_data[0], command_data[1], command_data[2] != 0, (command_data[3] << 8) | command_data[4]);
            break;
        }
#endif
        case id_remap_get_definition_size: {
            command_data[0] = (remap_definition_size >> 24) & 0xFF;
            command_data[1] = (remap_definition_size >> 16) & 0xFF;
            command_data[2] = (remap_definition_size >> 8) & 0xFF;
            command_data[3] = remap_definition_size & 0xFF;
            break;
        }
        case id_remap_get_definition_chunk: {
            uint16_t offset    = ((uint16_t)command_data[0] << 8) | (uint16_t)command_data[1];
            uint8_t  requested = command_data[2];
            // Layout: [id, off_hi, off_lo, size, ...data...] leaves 28 bytes for data.
            if (requested > 28) {
                requested = 28;
            }
            uint32_t remaining = (offset < remap_definition_size) ? (remap_definition_size - offset) : 0;
            uint8_t  actual    = (requested < remaining) ? requested : (uint8_t)remaining;
            command_data[2]    = actual;
            for (uint8_t i = 0; i < actual; i++) {
                command_data[3 + i] = remap_definition_bytes[offset + i];
            }
            break;
        }
        default: {
            *command_id = id_remap_unhandled;
            break;
        }
    }

    raw_hid_send(data, length);
}

#if defined(BACKLIGHT_ENABLE)

void remap_qmk_backlight_command(uint8_t *data, uint8_t length) {
    uint8_t *command_id        = &(data[0]);
    uint8_t *value_id_and_data = &(data[2]);

    switch (*command_id) {
        case id_remap_custom_set_value: {
            remap_qmk_backlight_set_value(value_id_and_data);
            break;
        }
        case id_remap_custom_get_value: {
            remap_qmk_backlight_get_value(value_id_and_data);
            break;
        }
        case id_remap_custom_save: {
            remap_qmk_backlight_save();
            break;
        }
        default: {
            *command_id = id_remap_unhandled;
            break;
        }
    }
}

#    if BACKLIGHT_LEVELS == 0
#        error BACKLIGHT_LEVELS == 0
#    endif

void remap_qmk_backlight_get_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);
    switch (*value_id) {
        case id_remap_qmk_backlight_brightness: {
            value_data[0] = ((uint16_t)get_backlight_level() * UINT8_MAX) / BACKLIGHT_LEVELS;
            break;
        }
        case id_remap_qmk_backlight_effect: {
#    ifdef BACKLIGHT_BREATHING
            value_data[0] = is_backlight_breathing() ? 1 : 0;
#    else
            value_data[0] = 0;
#    endif
            break;
        }
    }
}

void remap_qmk_backlight_set_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);
    switch (*value_id) {
        case id_remap_qmk_backlight_brightness: {
            backlight_level_noeeprom(((uint16_t)value_data[0] * BACKLIGHT_LEVELS) / UINT8_MAX);
            break;
        }
        case id_remap_qmk_backlight_effect: {
#    ifdef BACKLIGHT_BREATHING
            if (value_data[0] == 0) {
                backlight_disable_breathing();
            } else {
                backlight_enable_breathing();
            }
#    endif
            break;
        }
    }
}

void remap_qmk_backlight_save(void) {
    eeconfig_update_backlight_current();
}

#endif // BACKLIGHT_ENABLE

#if defined(RGBLIGHT_ENABLE)
#    ifndef RGBLIGHT_LIMIT_VAL
#        define RGBLIGHT_LIMIT_VAL 255
#    endif

void remap_qmk_rgblight_command(uint8_t *data, uint8_t length) {
    uint8_t *command_id        = &(data[0]);
    uint8_t *value_id_and_data = &(data[2]);

    switch (*command_id) {
        case id_remap_custom_set_value: {
            remap_qmk_rgblight_set_value(value_id_and_data);
            break;
        }
        case id_remap_custom_get_value: {
            remap_qmk_rgblight_get_value(value_id_and_data);
            break;
        }
        case id_remap_custom_save: {
            remap_qmk_rgblight_save();
            break;
        }
        default: {
            *command_id = id_remap_unhandled;
            break;
        }
    }
}

void remap_qmk_rgblight_get_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);
    switch (*value_id) {
        case id_remap_qmk_rgblight_brightness: {
            value_data[0] = ((uint16_t)rgblight_get_val() * UINT8_MAX) / RGBLIGHT_LIMIT_VAL;
            break;
        }
        case id_remap_qmk_rgblight_effect: {
            value_data[0] = rgblight_is_enabled() ? rgblight_get_mode() : 0;
            break;
        }
        case id_remap_qmk_rgblight_effect_speed: {
            value_data[0] = rgblight_get_speed();
            break;
        }
        case id_remap_qmk_rgblight_color: {
            value_data[0] = rgblight_get_hue();
            value_data[1] = rgblight_get_sat();
            break;
        }
    }
}

void remap_qmk_rgblight_set_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);
    switch (*value_id) {
        case id_remap_qmk_rgblight_brightness: {
            rgblight_sethsv_noeeprom(rgblight_get_hue(), rgblight_get_sat(), ((uint16_t)value_data[0] * RGBLIGHT_LIMIT_VAL) / UINT8_MAX);
            break;
        }
        case id_remap_qmk_rgblight_effect: {
            if (value_data[0] == 0) {
                rgblight_disable_noeeprom();
            } else {
                rgblight_enable_noeeprom();
                rgblight_mode_noeeprom(value_data[0]);
            }
            break;
        }
        case id_remap_qmk_rgblight_effect_speed: {
            rgblight_set_speed_noeeprom(value_data[0]);
            break;
        }
        case id_remap_qmk_rgblight_color: {
            rgblight_sethsv_noeeprom(value_data[0], value_data[1], rgblight_get_val());
            break;
        }
    }
}

void remap_qmk_rgblight_save(void) {
    eeconfig_update_rgblight_current();
}

#endif // RGBLIGHT_ENABLE

#endif // REMAP_ENABLE
