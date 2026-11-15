/* Copyright 2024 ~ 2026 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"

enum layers {
    WIN_BASE,
    WIN_FN,
};

#define FN_WIN MO(WIN_FN)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_109_iso(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_F14,   UG_TOGG,  DM_PLY1,  DM_PLY2,  QK_LOCK,  _______,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,            KC_DEL,   KC_END,   KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,                                 KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,            KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN,  FN_WIN,   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT,  KC_PENT),

    [WIN_FN] = LAYOUT_109_iso(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  _______,  _______,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  _______,  _______,  DM_REC1,  DM_REC2,  _______,  _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                                _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  QK_BOOT,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,            _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______),
};
// clang-format on

// see: quantum/keycodes.h
#define IS_LETTER(code) ((code) >= KC_A && (code) <= KC_Z)
#define IS_NUMPAD(code) ((code) >= KC_NUM_LOCK && (code) <= KC_KP_DOT)
#define IS_SPECIAL(code) ((code) >= KC_ENTER && (code) <= KC_F12)

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    const uint8_t layer     = get_highest_layer(layer_state);
    const led_t   led_state = host_keyboard_led_state();
    const uint8_t mods      = get_mods();
    const bool    is_shift  = mods & MOD_MASK_SHIFT;
    const bool    is_ctrl   = mods & MOD_MASK_CTRL;

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            const uint8_t index = g_led_config.matrix_co[row][col];
            if (index == NO_LED || index < led_min || index >= led_max) {
                continue;
            }

            // FN layer => keys defined on it green
            if (layer > WIN_BASE) {
                if (keymap_key_to_keycode(layer, (keypos_t){col, row}) != KC_TRNS) {
                    rgb_matrix_set_color(index, RGB_GREEN);
                }
                continue;
            }

            const uint16_t keycode = keymap_key_to_keycode(WIN_BASE, (keypos_t){col, row});

            // caps lock => letters red
            if (led_state.caps_lock && IS_LETTER(keycode)) {
                rgb_matrix_set_color(index, RGB_RED);
            }
            // shift => letters yellow
            if (is_shift && IS_LETTER(keycode)) {
                rgb_matrix_set_color(index, RGB_YELLOW);
            }
            // ctrl => special keys turquoise
            if (is_ctrl && IS_SPECIAL(keycode)) {
                rgb_matrix_set_color(index, RGB_TURQUOISE);
            }
            // num lock off => numpad red
            if (!led_state.num_lock && IS_NUMPAD(keycode)) {
                rgb_matrix_set_color(index, RGB_RED);
            }
        }
    }
    return true;
}

void heatmap_blur_key_event(uint8_t row, uint8_t col, bool pressed);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (IS_KEYEVENT(record->event)) {
        heatmap_blur_key_event(record->event.key.row, record->event.key.col, record->event.pressed);
    }
    return true;
}

void keyboard_post_init_user(void) {
    // Heatmap Blur is the only mode; override whatever EEPROM holds.
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_heatmap_blur);
}
