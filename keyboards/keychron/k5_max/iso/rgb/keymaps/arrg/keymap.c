/* Copyright 2024 @ Keychron (https://www.keychron.com)
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
#include "keychron_common.h"



enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_109_iso(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU, KC_SNAP,  KC_SIRI,  RGB_MOD, KC_F13,  KC_F14,  KC_F15,  KC_F16,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, KC_INS,   KC_HOME,  KC_PGUP, KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,           KC_DEL,   KC_END,   KC_PGDN, KC_P7,   KC_P8,   KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,                               KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,           KC_UP,             KC_P1,   KC_P2,   KC_P3,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN,MO(MAC_FN),KC_RCTL, KC_LEFT,  KC_DOWN,  KC_RGHT, KC_P0,            KC_PDOT, KC_PENT),

    [MAC_FN] = LAYOUT_109_iso(
        _______,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,  _______,  _______,  RGB_TOG, _______, _______, _______, _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______, _______, _______, _______, _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,           _______,  _______,  _______, _______, _______, _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                              _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,           _______,           _______, _______, _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______, _______,  _______,  _______, _______,          _______, _______),

    [WIN_BASE] = LAYOUT_109_iso(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,  KC_PSCR,  KC_F14,   RGB_TOG, DM_PLY1, DM_PLY2, QK_LOCK, QK_REP,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, KC_INS,   KC_HOME,  KC_PGUP, KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,           KC_DEL,   KC_END,   KC_PGDN, KC_P7,   KC_P8,   KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,                               KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,           KC_UP,             KC_P1,   KC_P2,   KC_P3,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN, MO(WIN_FN),QK_LOCK, KC_LEFT,  KC_DOWN,  KC_RGHT, KC_P0,            KC_PDOT, KC_PENT),

    [WIN_FN] = LAYOUT_109_iso(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU, _______,  _______,  _______, DM_REC1, DM_REC2, _______, _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______, _______, _______, _______, _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,           _______,  _______,  _______, _______, _______, _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                              _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  DB_TOGG,  QK_BOOT,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,           _______,           _______, _______, _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______, _______,  _______,  _______, _______,          _______, _______),
};


static void debug_matix(void) {
    xprintf("==================\n");

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            printf("row: %2u, col: %2u\n", row, col);

            /*
            uint8_t i = g_led_config.matrix_co[row][col];
            if (i == NO_LED) { continue;}

            int16_t x = g_led_config.point[i].x;
            int16_t y = g_led_config.point[i].y;
            xprintf("row: %2u, col: %2u, i: %2u | x/y=%i/%i \n", row, col, i, x, y);
            */
        }
    }

    xprintf("==================\n");
}


// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }

    if (record->event.pressed) {
        // xprintf("KL: kc: 0x%04X, col: %2u, row: %2u, type: %d \n", keycode, record->event.key.col, record->event.key.row, record->event.type);
        // type:
        // 0: TICK_EVENT
        // 1: KEY_EVENT
        // 2: ENCODER_CW_EVENT
        // 3: ENCODER_CCW_EVENT
        // 4: COMBO_EVENT


        if (keycode == KC_RALT) {
            debug_matix();
        }

        // ARRG_BLUR_process_record(keycode, record);
    }

    return true;
}




bool color_defined_keys(uint8_t layer) {

    // light up active layer (if its >0)
    // layer = get_highest_layer(layer_state);
    if (layer > 2) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                // if (index >= led_min && index < led_max && index != NO_LED) {
                uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
                if (keycode != KC_TRNS) {
                    rgb_matrix_set_color(index, RGB_GREEN);
                } // if keycode
                // }
            }
        }
    }
    return true;
}


bool rgb_matrix_indicators_user(void) {
    // uint8_t layer = biton32(layer_state);
    uint8_t layer = get_highest_layer(layer_state);

    if (layer > WIN_BASE) {
        color_defined_keys(layer);
    }

    /*
    switch (layer) {
        case WIN_BASE:
            rgb_matrix_set_color_all(RGB_BLUE);
            break;
        case WIN_FN:
            rgb_matrix_set_color_all(RGB_RED);
            break;
        default:
            rgb_matrix_set_color_all(RGB_WHITE);
            break;
    }
    */
    return true;

}
