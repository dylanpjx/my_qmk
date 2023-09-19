#include QMK_KEYBOARD_H

#include "oneshot.h"

#define LA_NUM LT(NUM,KC_BSPC)
#define LA_SYS MO(SYS)
#define LA_NAV LT(NAV,KC_SPC)
#define LA_SYM LT(SYM,KC_ENT)

#define M_LSFT LSFT_T(KC_Z)
#define M_LALT LALT_T(KC_X)
#define M_LGUI LGUI_T(KC_C)
#define M_LCTL LCTL_T(KC_V)

#define M_RCTL RCTL_T(KC_M)
#define M_RSFT RSFT_T(KC_SLSH)
#define M_RGUI RGUI_T(KC_COMM)
#define M_RALT RALT_T(KC_DOT)

const uint16_t PROGMEM JK_COMBO[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM DF_COMBO[] = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM QW_COMBO[] = {KC_Q, KC_W, COMBO_END};
/* const uint16_t PROGMEM LCOLN_COMBO[] = {KC_L, KC_COLN, COMBO_END}; */

combo_t key_combos[] = {
    COMBO(QW_COMBO, CW_TOGG),

    COMBO(DF_COMBO, KC_TAB),
    COMBO(JK_COMBO, KC_ESC),
};

enum layers {
    DEF,
    NUM,
    SYM,
    NAV,
    SYS,
};

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
    CK_NBA,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT_split_3x5_2(
            KC_Q, KC_W, KC_E, KC_R, KC_T,           KC_Y, KC_U, KC_I, KC_O, KC_P,
            KC_A, KC_S, KC_D, KC_F, KC_G,           KC_H, KC_J, KC_K, KC_L, KC_QUOT,
            M_LSFT, M_LALT, M_LGUI, M_LCTL, KC_B,   KC_N, M_RCTL, M_RGUI, M_RALT, M_RSFT,
                               LA_NUM, OSM(MOD_LSFT),      LA_NAV, LA_SYM
    ),

    [NUM] = LAYOUT_split_3x5_2(
            KC_1, KC_2, KC_3, KC_4, KC_5,                  KC_6, KC_7, KC_8, KC_9, KC_0,
            CK_NBA, KC_EQL, KC_MINS, KC_PLUS, KC_NO,       KC_UNDS, KC_LPRN, KC_LBRC, KC_LCBR, KC_NO,
            KC_DQUO, KC_QUOT, KC_SCLN, KC_COLN, KC_NO,     KC_NO, KC_RPRN, KC_RBRC, KC_RCBR, KC_NO,
                                 KC_TRNS, KC_LGUI,         KC_TRNS, MO(SYS)
    ),

    [SYM] = LAYOUT_split_3x5_2(
            KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,      KC_CIRC, KC_AMPR, KC_ASTR, KC_BSLS, KC_PIPE,
            KC_NO, KC_LBRC, KC_LPRN, KC_RPRN, KC_RBRC,     KC_GRV, KC_COLN, KC_SCLN, KC_NO, KC_DEL,
            OS_SHFT, OS_ALT, OS_GUI, OS_CTRL, KC_NO,       KC_TILD, KC_NO, KC_NO, KC_NO, KC_NO,
                                MO(SYS), KC_TRNS,          KC_RGUI, KC_TRNS
    ),

    [NAV] = LAYOUT_split_3x5_2(
            KC_TAB, KC_NO, LCTL(KC_RGHT), KC_NO, KC_NO,       LCTL(KC_INS), KC_PGUP, KC_HOME, KC_NO, LSFT(KC_INS),
            KC_END, KC_NO, KC_PGDN, KC_NO, KC_F6,             KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_ENT,
            OS_SHFT, OS_ALT, OS_GUI, OS_CTRL, LCTL(KC_LEFT),  KC_NO, OS_CTRL, OS_GUI, OS_ALT, OS_SHFT,
                                     KC_TRNS, KC_TRNS,        KC_TRNS, KC_RGUI
    ),

    [SYS] = LAYOUT_split_3x5_2(
            KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,               KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,
            KC_F11, KC_F12, KC_BTN2, KC_BTN1, KC_TRNS,       KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                       KC_TRNS, QK_BOOT,     KC_TRNS, KC_TRNS
    ),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case M_LSFT:
        case M_RSFT:
            return TAPPING_TERM-80;
        default:
            return TAPPING_TERM;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LA_NUM:
        case M_LSFT:
        case M_RSFT:
            // Immediately select the hold action when another key is tapped.
            return true;
        default:
            // Do not select the hold action when another key is tapped.
            return false;
    }
}

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case KC_ESC:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_NUM:
    case LA_SYS:
    case LA_SYM:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
        return true;
    default:
        return false;
    }
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CK_NBA:
            if (record ->event.pressed) {
                SEND_STRING("<=");
            }
            return false;
    };

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_gui_state, KC_LGUI, OS_GUI,
        keycode, record
    );

    return true;
}
