#include QMK_KEYBOARD_H

enum layer_names {
  L_QWERTY,
  L_SYMBOL,
  L_FUNCTION,
  L_READLINE
};

enum macros {
  M_CTRL_A = SAFE_RANGE,
  M_CTRL_E,
  M_CTRL_W,
  M_CTRL_U
};

enum tap_dances {
  TD_LCTL
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!record->event.pressed) {
    return true;
  }

  switch (keycode) {
    case M_CTRL_A:
      register_code(KC_LGUI);
      register_code(KC_LEFT);
      unregister_code(KC_LEFT);
      unregister_code(KC_LGUI);
      return false;

    case M_CTRL_E:
      register_code(KC_LGUI);
      register_code(KC_RGHT);
      unregister_code(KC_RGHT);
      unregister_code(KC_LGUI);
      return false;

    case M_CTRL_W:
      register_code(KC_LALT);
      register_code(KC_DEL);
      unregister_code(KC_DEL);
      unregister_code(KC_LALT);
      return false;

    case M_CTRL_U:
      register_code(KC_LGUI);
      register_code(KC_DEL);
      unregister_code(KC_DEL);
      unregister_code(KC_LGUI);
      return false;
  }

  return true;
}

void td_lctl_finished(tap_dance_state_t *state, void *data) {
  if (state->count == 1) register_code(KC_LCTL);
  if (state->count == 2) layer_on(L_READLINE);
}

void td_lctl_reset(tap_dance_state_t *state, void *data) {
  if (state->count == 1) unregister_code(KC_LCTL);
  if (state->count == 2) layer_off(L_READLINE);
}

tap_dance_action_t tap_dance_actions[] = {
  [TD_LCTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_lctl_finished, td_lctl_reset)
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT(1, KC_SPC):
      return TAPPING_TERM + 25;
    case TD(TD_LCTL):
      return TAPPING_TERM + 50;
    default:
      return TAPPING_TERM;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_QWERTY] = LAYOUT_ortho_4x12_2x2u(
    KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,          KC_T,    KC_Y,    KC_U,          KC_I,    KC_O,    KC_P,    KC_MINS,
    TD(TD_LCTL), KC_A,    KC_S,    KC_D,    KC_F,          KC_G,    KC_H,    KC_J,          KC_K,    KC_L,    KC_LBRC, MT(MOD_LCTL, KC_RBRC),
    KC_LSFT,     KC_Z,    KC_X,    KC_C,    KC_V,          KC_B,    KC_N,    KC_M,          KC_COMM, KC_DOT,  KC_SLSH, MT(MOD_LSFT, KC_BSLS),
    KC_F6,       KC_MPLY, KC_LALT, KC_LGUI, LT(1, KC_SPC),                   LT(2, KC_SPC), KC_ENT,  KC_BSPC, KC_VOLD, KC_VOLU
  ),
  [L_SYMBOL] = LAYOUT_ortho_4x12_2x2u(
    KC_GRV,  KC_1,  KC_2,  KC_3,  KC_4,   KC_5,  KC_6,  KC_7,   KC_8,  KC_9,  KC_0,    KC_EQL,
    KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO, KC_NO, KC_NO,  KC_NO, KC_NO, KC_SCLN, KC_QUOT,
    KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO,  KC_NO, KC_NO, KC_NO,  KC_NO, KC_NO, KC_NO,   KC_RSFT,
    KC_NO,   KC_NO, KC_NO, KC_NO, KC_SPC,               KC_SPC, KC_NO, KC_NO, KC_NO,   KC_NO
  ),
  [L_FUNCTION] = LAYOUT_ortho_4x12_2x2u(
    KC_GRV,  KC_1,    KC_2,     KC_3,     KC_4,     KC_5,  KC_6,    KC_7,     KC_8,   KC_9,    KC_0,    KC_EQL,
    KC_NO,   KC_NO,   RGB_RMOD, RGB_MOD,  RGB_TOG,  KC_NO, KC_LEFT, KC_DOWN,  KC_UP,  KC_RGHT, KC_SCLN, KC_QUOT,
    KC_LSFT, RGB_SPD, RGB_SPI,  RGB_VAD,  RGB_VAI,  KC_NO, KC_NO,   KC_NO,    KC_NO,  KC_NO,   KC_NO,   KC_RSFT,
    KC_MRWD, KC_MFFD, KC_NO,    QK_BOOT,  KC_SPC,                   KC_SPC,   KC_ESC, KC_CAPS, KC_BRID, KC_BRIU
  ),
  [L_READLINE] = LAYOUT_ortho_4x12_2x2u(
    KC_NO, KC_NO,    M_CTRL_W, M_CTRL_E, KC_NO,   KC_NO,   KC_NO,   M_CTRL_U, KC_NO, KC_NO, KC_UP,  KC_NO,
    KC_NO, M_CTRL_A, KC_NO,    KC_NO,    KC_RGHT, KC_NO,   KC_BSPC, KC_ENT,   KC_NO, KC_NO, KC_ESC, KC_NO,
    KC_NO, KC_NO,    KC_NO,    KC_NO,    KC_NO,   KC_LEFT, KC_DOWN, KC_ENT,   KC_NO, KC_NO, KC_NO,  KC_NO,
    KC_NO, KC_NO,    KC_NO,    KC_NO,    KC_NO,                     KC_NO,    KC_NO, KC_NO, KC_NO,  KC_NO
  )
};
