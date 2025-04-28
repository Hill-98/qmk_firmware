/*
Copyright 2023 @ Nuphy <https://nuphy.com/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "layer_names.h"
#include "user_kb.h"

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Tap dance enums
enum {
    TD_X_CTL,
    TD_MAC_HOME,
    TD_MAC_END,
};

td_state_t cur_dance(tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void x_finished(tap_dance_state_t *state, void *user_data);
void x_reset(tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// layer Mac
[MAC_BASE] = LAYOUT_75_ansi(
    KC_ESC,    KC_F1,     KC_F2,       KC_F3,      KC_F4,      KC_F5,       KC_F6,     KC_F7,       KC_F8,       KC_F9,     KC_F10,       KC_F11,    KC_F12,       PRT_SCR,    KC_INS,     KC_DEL,
    KC_GRV,     KC_1,      KC_2,        KC_3,       KC_4,       KC_5,        KC_6,      KC_7,        KC_8,        KC_9,      KC_0,       KC_MINS,    KC_EQL,                  KC_BSPC, TD(TD_MAC_HOME),
    KC_TAB,     KC_Q,      KC_W,        KC_E,       KC_R,       KC_T,        KC_Y,      KC_U,        KC_I,        KC_O,      KC_P,       KC_LBRC,    KC_RBRC,                 KC_BSLS,    KC_PGUP,
 TD(TD_X_CTL),  KC_A,      KC_S,        KC_D,       KC_F,       KC_G,        KC_H,      KC_J,        KC_K,        KC_L,      KC_SCLN,    KC_QUOT,                             KC_ENT,     KC_PGDN,
    KC_LSFT,               KC_Z,        KC_X,       KC_C,       KC_V,        KC_B,      KC_N,        KC_M,        KC_COMM,   KC_DOT,     KC_SLSH,                 KC_RSFT,    KC_UP,   TD(TD_MAC_END),
    KC_LCTL,    KC_LALT,   KC_LGUI,                                          KC_SPC,                             KC_RGUI,   MO(MAC_FN),      KC_RCTL,            KC_LEFT,    KC_DOWN,    KC_RGHT),

// layer Mac Fn
[MAC_FN] = LAYOUT_75_ansi(
    KEYBORD_LOCK,    KC_BRID,   KC_BRIU,     TASK,       SEARCH,     MAC_VOICE,   MAC_DND,   KC_MPRV,     KC_MPLY,   KC_MNXT,   KC_MUTE,   KC_VOLD,  KC_VOLU, PRT_AREA,   _______,    _______,
    _______,    LNK_BLE1,   LNK_BLE2,   LNK_BLE3,   LNK_RF,    _______,     _______,   _______,     _______,     _______,   _______,    _______,    _______,              _______,    KC_HOME,
    _______,TD(TD_MAC_HOME),KC_UP,TD(TD_MAC_END),  _______,    _______,     _______,   _______,     _______,     _______,   _______,    KC_PGUP,    KC_PGDN,              _______,    _______,
    _______,     KC_LEFT,    KC_DOWN,   KC_RIGHT,  _______,    _______,     _______,   _______,     _______,     _______,   _______,    _______,                          _______,    _______,
    _______, LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V),   _______,     _______,   _______,    _______,     _______,   _______,                       MO(M_LAYER),    RM_VALU,     KC_END,
    _______,    _______,    _______,                                        _______,                       _______,   MO(MAC_FN),      _______,          RM_NEXT,    RM_VALD,    RM_HUEU),

// layer win
[WIN_BASE] = LAYOUT_75_ansi(
    KC_ESC,     KC_F1,     KC_F2,       KC_F3,      KC_F4,      KC_F5,       KC_F6,     KC_F7,       KC_F8,       KC_F9,     KC_F10,     KC_F11,     KC_F12,      KC_PSCR,    KC_INS,     KC_DEL,
    KC_GRV,     KC_1,      KC_2,        KC_3,       KC_4,       KC_5,        KC_6,      KC_7,        KC_8,        KC_9,      KC_0,       KC_MINS,    KC_EQL,                  KC_BSPC,    KC_HOME,
    KC_TAB,     KC_Q,      KC_W,        KC_E,       KC_R,       KC_T,        KC_Y,      KC_U,        KC_I,        KC_O,      KC_P,       KC_LBRC,    KC_RBRC,                 KC_BSLS,    KC_PGUP,
    LCTL_T(KC_CAPS),    KC_A,      KC_S,        KC_D,       KC_F,       KC_G,        KC_H,      KC_J,        KC_K,        KC_L,      KC_SCLN,    KC_QUOT,                     KC_ENT,     KC_PGDN,
    KC_LSFT,               KC_Z,        KC_X,       KC_C,       KC_V,        KC_B,      KC_N,        KC_M,        KC_COMM,   KC_DOT,     KC_SLSH,                  KC_RSFT,    KC_UP,      KC_END,
    KC_LCTL,    KC_LGUI,   KC_LALT,                                          KC_SPC,                              KC_RALT,   MO(WIN_FN),      KC_RCTL,           KC_LEFT,    KC_DOWN,    KC_RGHT),

// layer win Fn
[WIN_FN] = LAYOUT_75_ansi(
    KEYBORD_LOCK,_______,   _______,    _______,    _______,    _______,     _______,   KC_MPRV,     KC_MPLY,     KC_MNXT,   KC_MUTE,    KC_VOLD,    KC_VOLU,     _______,    _______,    _______,
    _______,    LNK_BLE1,   LNK_BLE2,   LNK_BLE3,   LNK_RF,     _______,     _______,   _______,     _______,     _______,   _______,    _______,    _______,                 _______,    _______,
    _______,     KC_HOME,      KC_UP,     KC_END,   _______,    _______,     _______,   _______,     _______,     _______,   _______,    KC_PGUP,    KC_PGDN,                 _______,    _______,
    _______,     KC_LEFT,    KC_DOWN,   KC_RIGHT,   _______,    _______,     _______,   _______,     _______,     _______,   _______,    _______,                             _______,    _______,
    _______, LCTL(KC_Z), LCTL(KC_X), LCTL(KC_C), LCTL(KC_V),    _______,     _______,   _______,     _______,     _______,   _______,                 MO(M_LAYER),            RM_VALU,    _______,
    _______,    _______,    _______,                                         _______,                          _______,   MO(WIN_FN),      _______,            RM_NEXT,    RM_VALD,    RM_HUEU),
// layer 4
[M_LAYER] = LAYOUT_75_ansi(
    QK_REBOOT,  DB_TOGG,    RF_DFU,     _______,    _______,    _______,     _______,   _______,     _______,     _______,   _______,    DEBOUNCE_D,  DEBOUNCE_I, _______,    _______,    DEV_RESET,
    _______,    _______,    _______,    _______,    _______,    _______,     _______,   _______,     _______,     _______,   _______,    SLEEP_D,     SLEEP_I,                SLEEP_MODE,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,     _______,   _______,     _______,     SIDE_1,    _______,    _______,     SLEEP_NOW,              BAT_SHOW,    _______,
    CAPS_WORD,  _______,    _______,    DEBOUNCE_T, _______,    GAME_MODE,   _______,   _______,     SOCD_TOG,    _______,   _______,    _______,                             _______,    _______,
    AS_TOGG,                _______,    _______,    _______,    _______,     _______,   NUMLOCK_IND, _______,     SIDE_SPD,  SIDE_SPI,   _______,             MO(M_LAYER),    SIDE_VAI,   _______,
    _______,    _______,    _______,                                         _______,                             _______,   MO(M_LAYER),      _______,           SIDE_MOD,   SIDE_VAD,   SIDE_HUI),
// layer 5
[GAME_BASE] = LAYOUT_75_ansi(
    KC_ESC,     KC_F1,     KC_F2,       KC_F3,      KC_F4,      KC_F5,       KC_F6,     KC_F7,       KC_F8,       KC_F9,     KC_F10,     KC_F11,     KC_F12,      KC_PSCR,    KC_INS,     KC_DEL,
    KC_GRV,     KC_1,      KC_2,        KC_3,       KC_4,       KC_5,        KC_6,      KC_7,        KC_8,        KC_9,      KC_0,       KC_MINS,    KC_EQL,                  KC_BSPC,    KC_HOME,
    KC_TAB,     KC_Q,      KC_W,        KC_E,       KC_R,       KC_T,        KC_Y,      KC_U,        KC_I,        KC_O,      KC_P,       KC_LBRC,    KC_RBRC,                 KC_BSLS,    KC_PGUP,
    KC_CAPS,    KC_A,      KC_S,        KC_D,       KC_F,       KC_G,        KC_H,      KC_J,        KC_K,        KC_L,      KC_SCLN,    KC_QUOT,                              KC_ENT,    KC_PGDN,
    KC_LSFT,               KC_Z,        KC_X,       KC_C,       KC_V,        KC_B,      KC_N,        KC_M,        KC_COMM,   KC_DOT,     KC_SLSH,                 KC_RSFT,    KC_UP,      KC_END,
    KC_LCTL,    KC_LGUI,   KC_LALT,                                          KC_SPC,                        KC_RALT,   MO(GAME_FN),      KC_RCTL,                 KC_LEFT,    KC_DOWN,    KC_RGHT),
[GAME_FN] = LAYOUT_75_ansi(
    GAME_MODE   ,_______,   _______,    _______,    _______,    _______,     _______,   KC_MPRV,     KC_MPLY,     KC_MNXT,   KC_MUTE,    KC_VOLD,    KC_VOLU,     _______,    _______,    _______,
    _______,    LNK_BLE1,   LNK_BLE2,   LNK_BLE3,   LNK_RF,     _______,     _______,   _______,     _______,     _______,   _______,    _______,    _______,                 _______,    _______,
    _______,    _______,    _______,    _______,    _______,    _______,     _______,   _______,     _______,     _______,   _______,    _______,    _______,              BAT_SHOW,      _______,
    _______,    _______,    _______,    _______,    _______,    _______,     _______,   _______,     _______,     _______,   _______,    _______,                             _______,    _______,
    _______,                _______,    _______,    _______,   _______,     _______,   _______,     _______,      _______,   _______,    _______,                 _______,    _______,    _______,
    _______,    _______,    _______,                                      _______,                               _______,   MO(GAME_FN),  _______,               _______,    _______,    _______),
};


bool get_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    if (game_mode_enable) {
        return false;
    }
    switch (keycode) {
        case KC_HOME:
        case KC_END:
        case KC_MINUS:
        case KC_EQUAL:
        case KC_RIGHT_BRACKET:
        case KC_BACKSLASH:
        case KC_SEMICOLON:
        case KC_QUOTE:
        case KC_COMMA:
        case KC_DOT:
        case KC_LEFT_BRACKET:
        case KC_1...KC_0:
        case KC_A...KC_Z:
            return true;
        default:
            return false;
    }
    return false;
}

 td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t xtap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void x_finished(tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_LGUI); break;
        case TD_SINGLE_HOLD: register_code(KC_LGUI); break;
        case TD_DOUBLE_TAP: register_code(KC_CAPS); break;
        case TD_DOUBLE_HOLD: register_code(KC_LCTL); break;
        case TD_DOUBLE_SINGLE_TAP: register_code(KC_CAPS); break;
        default: break;
    }
}

void x_reset(tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_LGUI); break;
        case TD_SINGLE_HOLD: unregister_code(KC_LGUI); break;
        case TD_DOUBLE_TAP: unregister_code(KC_CAPS); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_LCTL); break;
        case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_CAPS); break;
        default: break;
    }
    xtap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_X_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset),
    [TD_MAC_HOME] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_A), KC_HOME),
    [TD_MAC_END] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_E), KC_END),
};
