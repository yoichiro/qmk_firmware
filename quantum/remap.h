// Copyright 2026 Yoichiro Tanaka
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "action.h"

// Remap-specific command IDs (0x80-0xEF namespace).
// Commands 0x81-0xEF mirror the behavior of the corresponding VIA commands
// (VIA id N is exposed as Remap id N + 0x80). Behavior is intended to be
// identical to quantum/via.c raw_hid_receive as of QMK 0.34.0.
enum remap_command_id {
    id_remap_identify                             = 0x80,
    // 0x81 reserved (VIA id_get_protocol_version is not mirrored; use
    // id_remap_identify's REMAP_PROTOCOL_VERSION instead).
    id_remap_get_keyboard_value                   = 0x82,
    id_remap_set_keyboard_value                   = 0x83,
    id_remap_dynamic_keymap_get_keycode           = 0x84,
    id_remap_dynamic_keymap_set_keycode           = 0x85,
    id_remap_dynamic_keymap_reset                 = 0x86,
    id_remap_custom_set_value                     = 0x87,
    id_remap_custom_get_value                     = 0x88,
    id_remap_custom_save                          = 0x89,
    id_remap_dynamic_keymap_macro_get_count       = 0x8C,
    id_remap_dynamic_keymap_macro_get_buffer_size = 0x8D,
    id_remap_dynamic_keymap_macro_get_buffer      = 0x8E,
    id_remap_dynamic_keymap_macro_set_buffer      = 0x8F,
    id_remap_dynamic_keymap_get_layer_count       = 0x91,
    id_remap_dynamic_keymap_get_buffer            = 0x92,
    id_remap_dynamic_keymap_get_encoder           = 0x94,
    id_remap_dynamic_keymap_set_encoder           = 0x95,
    id_remap_get_definition_size                  = 0x96,
    id_remap_get_definition_chunk                 = 0x97,
    // 0x98-0xEF reserved for future Remap extensions.
    id_remap_unhandled                            = 0xFF,
};

enum remap_keyboard_value_id {
    id_remap_uptime              = 0x01,
    id_remap_layout_options      = 0x02,
    id_remap_switch_matrix_state = 0x03,
    id_remap_firmware_version    = 0x04,
    id_remap_device_indication   = 0x05,
    id_remap_keycodes_version    = 0x06,
};

enum remap_channel_id {
    id_remap_custom_channel         = 0,
    id_remap_qmk_backlight_channel  = 1,
    id_remap_qmk_rgblight_channel   = 2,
    id_remap_qmk_rgb_matrix_channel = 3,
    id_remap_qmk_audio_channel      = 4,
    id_remap_qmk_led_matrix_channel = 5,
};

enum remap_qmk_backlight_value {
    id_remap_qmk_backlight_brightness = 1,
    id_remap_qmk_backlight_effect     = 2,
};

enum remap_qmk_rgblight_value {
    id_remap_qmk_rgblight_brightness   = 1,
    id_remap_qmk_rgblight_effect       = 2,
    id_remap_qmk_rgblight_effect_speed = 3,
    id_remap_qmk_rgblight_color        = 4,
};

// Magic bytes returned by id_remap_identify
#define REMAP_MAGIC_0 'R'
#define REMAP_MAGIC_1 'M'
#define REMAP_MAGIC_2 'P'

// Remap firmware protocol version (16-bit, big-endian on wire)
#define REMAP_PROTOCOL_VERSION 0x0001

// Layout options are stored in EEPROM as a big-endian integer of this size.
#ifndef REMAP_EEPROM_LAYOUT_OPTIONS_SIZE
#    define REMAP_EEPROM_LAYOUT_OPTIONS_SIZE 1
#endif

// Default value of layout options in EEPROM after reset.
#ifndef REMAP_EEPROM_LAYOUT_OPTIONS_DEFAULT
#    define REMAP_EEPROM_LAYOUT_OPTIONS_DEFAULT 0x00000000
#endif

// Keyboard-defined EEPROM area past the Remap-managed config.
#ifndef REMAP_EEPROM_CUSTOM_CONFIG_SIZE
#    define REMAP_EEPROM_CUSTOM_CONFIG_SIZE 0
#endif

// Firmware version reported via id_remap_get_keyboard_value +
// id_remap_firmware_version.
#ifndef REMAP_FIRMWARE_VERSION
#    define REMAP_FIRMWARE_VERSION 0x00000000
#endif

// Can be called in an overriding remap_init_kb() to test if keyboard level
// code usage of EEPROM is invalid and use/save defaults.
bool remap_eeprom_is_valid(void);

// Sets Remap/keyboard level usage of EEPROM to valid/invalid.
// Keyboard level code (eg. remap_init_kb()) should not call this.
void remap_eeprom_set_valid(bool valid);

// Called by QMK core to initialize dynamic keymaps etc.
void eeconfig_init_remap(void);
void remap_init(void);

// Layout options accessors.
uint32_t remap_get_layout_options(void);
void     remap_set_layout_options(uint32_t value);
void     remap_set_layout_options_kb(uint32_t value);

uint32_t remap_read_custom_config(void *buf, uint32_t offset, uint32_t length);
uint32_t remap_update_custom_config(const void *buf, uint32_t offset, uint32_t length);

void remap_set_device_indication(uint8_t value);

bool process_record_remap(uint16_t keycode, keyrecord_t *record);

void remap_custom_value_command(uint8_t *data, uint8_t length);
void remap_custom_value_command_kb(uint8_t *data, uint8_t length);

#if defined(BACKLIGHT_ENABLE)
void remap_qmk_backlight_command(uint8_t *data, uint8_t length);
void remap_qmk_backlight_set_value(uint8_t *data);
void remap_qmk_backlight_get_value(uint8_t *data);
void remap_qmk_backlight_save(void);
#endif

#if defined(RGBLIGHT_ENABLE)
void remap_qmk_rgblight_command(uint8_t *data, uint8_t length);
void remap_qmk_rgblight_set_value(uint8_t *data);
void remap_qmk_rgblight_get_value(uint8_t *data);
void remap_qmk_rgblight_save(void);
#endif

// Keyboard-level override hook. Returning true from this function means the
// command was fully handled (including calling raw_hid_send).
bool remap_command_kb(uint8_t *data, uint8_t length);
