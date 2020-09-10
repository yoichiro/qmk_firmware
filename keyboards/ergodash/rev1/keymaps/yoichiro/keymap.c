#include QMK_KEYBOARD_H
/* #include "keymap_jp.h" */


#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 16

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
};

#define EISU LALT(KC_GRV)
#define C_TAB LCTL_T(KC_TAB)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT( \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                      KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_HOME,                   KC_PGUP, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   C_TAB,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_END,                    KC_PGDN, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_SPC,                    KC_ENT,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_UP,   RSFT_T(KC_SLSH), \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
   ADJUST,  KC_LGUI, KC_LALT,          KC_HENK, LOWER,   KC_SPC,  KC_SPC,  KC_ENT,  KC_ENT,  RAISE,   KC_HENK,          KC_LEFT, KC_DOWN, KC_RGHT  \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
  ),

  [_LOWER] = LAYOUT(
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                      KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC,                   KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, KC_DQT, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_LSFT, KC_Z,    KC_X,    KC_PIPE, KC_LBRC, KC_RBRC, KC_PSCR,                   KC_PSCR, KC_LCBR, KC_RCBR, KC_LT,   KC_GT,   KC_PGUP, KC_QUES, \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
   KC_CAPS, KC_LGUI, KC_LALT,          KC_MHEN, LOWER,   KC_SPC,  KC_SPC,  KC_ENT,  KC_ENT,  RAISE,   KC_MHEN,          KC_HOME, KC_PGDN, KC_END   \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
  ),

  [_RAISE] = LAYOUT(
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                      KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC,                   KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, KC_DQT, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   KC_LSFT, KC_Z,    KC_X,    KC_PIPE, KC_LBRC, KC_RBRC, KC_PSCR,                   KC_PSCR, KC_LCBR, KC_RCBR, KC_LT,   KC_GT,   KC_PGUP, KC_QUES, \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
   KC_CAPS, KC_LGUI, KC_LALT,          KC_MHEN, LOWER,   KC_SPC,  KC_SPC,  KC_ENT,  KC_ENT,  RAISE,   KC_MHEN,          KC_HOME, KC_PGDN, KC_END   \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
  ),

  [_ADJUST] = LAYOUT(
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   _______, RESET  , RGB_TOG, RGB_MOD, RGB_HUD, RGB_HUI, _______,                   _______, RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI, _______, _______, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   _______, _______, BL_TOGG, BL_BRTG, BL_INC , BL_DEC , _______,                   _______, _______, _______, _______, _______, _______, _______, \
//+--------+--------+--------+--------+--------+--------+--------+                 +--------+--------+--------+--------+--------+--------+--------+
   _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
   _______, _______, _______,          _______, _______, _______,_______,  _______, _______, _______, _______,          _______, _______, _______  \
//+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
  )
};

#ifdef AUDIO_ENABLE
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
#endif

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

static bool lower_pressed = false;
static bool raise_pressed = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
         print("mode just switched to qwerty and this is a huge string\n");
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        lower_pressed = true;
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
        if (lower_pressed) {
          register_code(KC_MHEN);
          unregister_code(KC_MHEN);
        }
        lower_pressed = false;
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        raise_pressed = true;
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
        if (raise_pressed) {
          register_code(KC_HENK);
          unregister_code(KC_HENK);
        }
        raise_pressed = false;
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
    default:
      if (record->event.pressed) {
        lower_pressed = false;
        raise_pressed = false;
      }
      break;
  }
  return true;
}
