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

#include QMK_KEYBOARD_H  // ok
#include "keychron_common.h"


// Keycodes for umlauts and eszett
#define UML_AE RALT(KC_Q)
#define UML_OE RALT(KC_P)
#define UML_UE RALT(KC_Y)
#define GER_SZ RALT(KC_S)
#define EU_EUR RALT(KC_5)


enum layers {
    WIN_BASE,
    GAMING,
    WIN_FN,
    SPACE_FN,
};



// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /*
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
    */

    [WIN_BASE] = LAYOUT_109_iso(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,  KC_PSCR,  KC_F14,   RGB_TOG, DM_PLY1, DM_PLY2, QK_LOCK, TG(GAMING),
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC, KC_INS,   KC_HOME,  KC_PGUP, KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,           KC_DEL,   KC_END,   KC_PGDN, KC_P7,   KC_P8,   KC_P9,
        _______,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,                               KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,           KC_UP,             KC_P1,   KC_P2,   KC_P3,
        KC_LCTL,  KC_LWIN,  KC_RALT,                         LT(SPACE_FN, KC_SPC),                          KC_RALT,  KC_RWIN,  MO(WIN_FN),QK_LOCK, KC_LEFT, KC_DOWN,  KC_RGHT, KC_P0,            KC_PDOT, KC_PENT),

    /*
        Gaming Layer
        reverts some keys (such as space) back to defaults
    */
    [GAMING] = LAYOUT_109_iso(
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______, _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______, _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,           _______,  _______,  _______, _______, _______, _______,
        KC_CAPS,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                              _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,           _______,           _______, _______, _______,
        _______,  _______,  _______,                                KC_SPC,                                 _______,  _______,  _______,  _______, _______,  _______,  _______, _______,          _______, _______),

    /*
        Regular fn-layer
        used for rgb effects, media keys
        and some mixed qmk features
    */
    [WIN_FN] = LAYOUT_109_iso(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU, _______,  _______,  _______, DM_REC1, DM_REC2, _______, _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______, _______, _______, _______, _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,           _______,  _______,  _______, _______, _______, _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                              _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  DB_TOGG,  QK_BOOT,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,           _______,           _______, _______, _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______, _______,  _______,  _______, _______,          _______, _______),

    /*
        spaceFN:
        - arrow/cursor control keys
        - 
    */
    [SPACE_FN] = LAYOUT_109_iso(
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______, _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, _______,  _______,  _______, _______, _______, _______, _______,
        _______,  _______,  _______,  EU_EUR,   _______,  _______,  _______,  UML_UE,   _______,  UML_OE,   _______,  _______,  _______,           _______,  _______,  _______, _______, _______, _______,
        _______,  UML_AE,   GER_SZ,   _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  _______,  _______,  _______,                              _______, _______, _______, _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,           _______,           _______, _______, _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______, _______,  _______,  _______, _______,          _______, _______),
};




bool color_defined_keys(uint8_t layer) {

    if (layer > 2) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

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
    const uint8_t layer = get_highest_layer(layer_state);
    
    if (layer > WIN_BASE) {
        color_defined_keys(layer);
    }
   
    
    return true;
}



// Helpers
// see: quantum/keycodes.h
#define IS_LETTER(code) ((code >= KC_A && code <= KC_Z))
#define IS_NUMPAD(code) ((code >= KC_NUM_LOCK && code <= KC_KP_DOT))
#define IS_SPECIAL(code) ((code >= KC_ENTER && code <= KC_F12))


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    
    const led_t led_state = host_keyboard_led_state();
    const uint8_t layer = WIN_BASE;  // can we get the current "base" layer?


    const uint8_t mods = get_mods();
    const bool is_shift = mods & MOD_MASK_SHIFT;
    const bool is_ctrl = mods & MOD_MASK_CTRL;


    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            const uint8_t index = g_led_config.matrix_co[row][col];
    
            if (index >= led_min && index < led_max && index != NO_LED) {
                const uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
    
                // caps lock => letters red
                if (led_state.caps_lock && IS_LETTER(keycode)) {
                    rgb_matrix_set_color(index, RGB_RED);
                }
                // shift => letters yellow
                if (is_shift && IS_LETTER(keycode)) {
                    rgb_matrix_set_color(index, RGB_YELLOW);
                }

                // ctrl => special keys => turquoise
                if (is_ctrl && IS_SPECIAL(keycode)) {
                    rgb_matrix_set_color(index, RGB_TURQUOISE);
                }

                // num lock => numpad => red
                if (!led_state.num_lock && IS_NUMPAD(keycode)) {
                    rgb_matrix_set_color(index, RGB_RED);
                }
            } // if index
        } // for col
    } // for row

    return true;

}


/*

    TODO:
    implement this, so we can use the switch to toggle
    between base and gaming layer (low prio)

bool dip_switch_update_user(uint8_t index, bool active) {
    printf("dip_switch_update_user: index=%d active=%d\n", index, active);
    return true;
}
*/
