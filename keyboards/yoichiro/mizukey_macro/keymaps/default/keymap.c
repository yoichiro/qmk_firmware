/* Copyright 2020 Yoichiro Tanaka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "song_list.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _BROWSER
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  BASE = SAFE_RANGE,
  BROWSER
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_PGUP,      KC_HOME,    KC_UP,      KC_END, \
        KC_PGDN,      KC_LEFT,    KC_DOWN,    KC_RGHT, \
        MO(_BROWSER), KC_NO,      KC_ESC \
    ),
    [_BROWSER] = LAYOUT(
        KC_WH_U,      KC_MS_BTN1, KC_MS_U,    KC_MS_BTN2, \
        KC_WH_D,      KC_MS_L,    KC_MS_D,    KC_MS_R, \
        KC_TRNS,      _______,    _______ \
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//  if (record->event.pressed) {
//    float my_song[][2] = SONG(GUITAR_SOUND);
//    PLAY_SONG(my_song);
//  }
//  switch (keycode) {
//    case BASE:
//      if (record->event.pressed) {
//        set_single_persistent_default_layer(_BASE);
//      }
//      return false;
//      break;
//    case BROWSER:
//      if (record->event.pressed) {
//        layer_on(_BROWSER);
//      } else {
//        layer_off(_BROWSER);
//      }
//      return false;
//      break;
//  }
  return true;
//    switch (keycode) {
//        case QMKBEST:
//            if (record->event.pressed) {
//                // when keycode QMKBEST is pressed
//                SEND_STRING("QMK is the best thing ever!");
//            } else {
//                // when keycode QMKBEST is released
//            }
//            break;
//        case QMKURL:
//            if (record->event.pressed) {
//                // when keycode QMKURL is pressed
//                SEND_STRING("https://qmk.fm/\n");
//            } else {
//                // when keycode QMKURL is released
//            }
//            break;
//    }
//    return true;
}
