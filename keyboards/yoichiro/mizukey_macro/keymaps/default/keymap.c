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
//#include "song_list.h"
//#include "musical_notes.h"
//#include <print.h>

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _CURSOR,
    _BROWSER,
    _NUMBERS
};

// Defines the keycodes used by our macros in process_record_user
//enum custom_keycodes {
//  CURSOR = SAFE_RANGE,
//  BROWSER,
//  NUMBERS
//};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_CURSOR] = LAYOUT(
        KC_PGUP,      KC_HOME,    KC_UP,       KC_END, \
        KC_PGDN,      KC_LEFT,    KC_DOWN,     KC_RGHT, \
        KC_LSFT,      KC_ESC,     DF(_BROWSER) \
    ),
    [_BROWSER] = LAYOUT(
        KC_WH_U,      KC_MS_BTN1, KC_MS_U,     KC_MS_BTN2, \
        KC_WH_D,      KC_MS_L,    KC_MS_D,     KC_MS_R, \
        KC_WBAK,      KC_WFWD,    DF(_NUMBERS) \
    ),
    [_NUMBERS] = LAYOUT(
        KC_1,         KC_2,       KC_3,        KC_4, \
        KC_5,         KC_6,       KC_7,        KC_8, \
        KC_9,         KC_0,       DF(_CURSOR) \
    )
};

const rgblight_segment_t PROGMEM rgb_cursor_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_RED}
);
const rgblight_segment_t PROGMEM rgb_browser_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_BLUE}
);
const rgblight_segment_t PROGMEM rgb_numbers_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_GREEN}
);
const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    rgb_cursor_layer,
    rgb_browser_layer,
    rgb_numbers_layer
);

void keyboard_post_init_user() {
    rgblight_layers = rgb_layers;
    debug_enable = true;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, 0));
    rgblight_set_layer_state(1, layer_state_cmp(state, 1));
    rgblight_set_layer_state(2, layer_state_cmp(state, 2));
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    float my_song[][2] = SONG(E__NOTE(_A6));
    switch (keycode) {
        case DF(_CURSOR):
        case DF(_BROWSER):
        case DF(_NUMBERS):
            PLAY_SONG(my_song);
            break;
    }
    return true;
}
