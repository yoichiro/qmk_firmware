#include QMK_KEYBOARD_H
#include <stdio.h>

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
};

#define LC_TAB LCTL_T(KC_TAB)
#define LA_MHEN LALT_T(KC_MHEN)
#define RG_HENK RGUI_T(KC_HENK)
#define RS_MINS RSFT_T(KC_MINS)
#define VOLUP KC__VOLUP
#define VOLDOWN KC__VOLDOWN

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT( \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   KC_GESC, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   LC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RS_MINS, \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
                              LA_MHEN, LOWER,   KC_SPC,  XXXXXXX, XXXXXXX, KC_ENT,  RAISE,   RG_HENK                             \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
  ),

  [_LOWER] = LAYOUT( \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   KC_BSLS, KC_CIRC, KC_EXLM, KC_AMPR, KC_PIPE, KC_DLR,                    KC_AT,   KC_EQL,  KC_PLUS, KC_ASTR, KC_PERC, KC_MINS, \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_PSCR, \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
                              _______, LOWER,   _______, _______, _______, _______, RAISE,   _______                             \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
  ),

  [_RAISE] = LAYOUT( \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   KC_BSLS, KC_CIRC, KC_EXLM, KC_AMPR, KC_PIPE, KC_DLR,                    KC_AT,   KC_EQL,  KC_PLUS, KC_ASTR, KC_PERC, KC_MINS, \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   KC_HASH, KC_GRV,  KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN,                   KC_PGUP, KC_HOME, KC_UP,   KC_END,  _______, _______, \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   _______, KC_TILD, _______, _______, KC_LCBR, KC_RCBR,                   KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
                              _______, LOWER,   _______, _______, _______, _______, RAISE,   _______                             \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
  ),

  [_ADJUST] = LAYOUT(
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   _______, RESET,   RGB_TOG, RGB_MOD, RGB_HUD, RGB_HUI,                   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   _______, _______, RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI,                   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
//+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+
   _______, _______, VOLDOWN, VOLUP,   KC__MUTE, _______,                  _______, _______, _______, _______, _______, _______, \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
                              _______, _______, _______, _______,  _______, _______, _______, _______                            \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
  )
};

#ifdef OLED_DRIVER_ENABLE

void render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch(get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjust\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined"), false);
    }
}

void render_logo(void) {
    static const char PROGMEM logo[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0};
    oled_write_P(logo, false);
}


char keylog_str[24]  = {};
char keylogs_str[21] = {};
int  keylogs_str_idx = 0;

const char code_to_name[60] = {' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'R', 'E', 'B', 'T', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ';', '\'', ' ', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
    char name = ' ';
    if (keycode < 60) {
        name = code_to_name[keycode];
    }

    // update keylog
    snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c", record->event.key.row, record->event.key.col, keycode, name);

    // update keylogs
    if (keylogs_str_idx == sizeof(keylogs_str) - 1) {
        keylogs_str_idx = 0;
        for (int i = 0; i < sizeof(keylogs_str) - 1; i++) {
            keylogs_str[i] = ' ';
        }
    }

    keylogs_str[keylogs_str_idx] = name;
    keylogs_str_idx++;
}

const char *read_keylog(void) { return keylog_str; }
const char *read_keylogs(void) { return keylogs_str; }

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_layer_state();
        oled_write_ln(read_keylog(), false);
        oled_write_ln(read_keylogs(), false);
    } else {
        render_logo();
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) return OLED_ROTATION_180;
    return rotation;
}

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

#ifdef OLED_DRIVER_ENABLE
    if (record->event.pressed) {
        set_keylog(keycode, record);
    }
#endif

    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
            break;
    }
    return true;
}
