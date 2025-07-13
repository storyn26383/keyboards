#include QMK_KEYBOARD_H

enum tap_dance_states {
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_TAP,
  DOUBLE_HOLD
};

enum layer_names {
  L_QWERTY,
  L_NUMBER,
  L_FUNCTION,
  L_READLINE,
  L_SYMBOL
};

enum macros {
  M_CTL_A = SAFE_RANGE,
  M_CTL_E,
  M_CTL_W,
  M_CTL_U
};

enum tap_dances {
  TD_LCTL,
  TD_LSFT
};

int cur_dance_state(tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted && state->pressed) return SINGLE_HOLD;
    if (state->interrupted || !state->pressed) return SINGLE_TAP;
    return SINGLE_HOLD;
  }

  if (state->count == 2) {
    if (state->interrupted && state->pressed) return DOUBLE_HOLD;
    if (state->interrupted || !state->pressed) return DOUBLE_TAP;
    return DOUBLE_HOLD;
  }

  return 0;
}

static int td_lctl_state;
void td_lctl_finished(tap_dance_state_t *state, void *user_data) {
  td_lctl_state = cur_dance_state(state);

  switch (td_lctl_state) {
    case SINGLE_TAP: tap_code(KC_LCTL); break;
    case SINGLE_HOLD: register_code(KC_LCTL); break;
    case DOUBLE_TAP: tap_code(KC_ESCAPE); break;
    case DOUBLE_HOLD: layer_on(L_READLINE); break;
  }
}
void td_lctl_reset(tap_dance_state_t *state, void *user_data) {
  switch (td_lctl_state) {
    case SINGLE_TAP: break;
    case SINGLE_HOLD: unregister_code(KC_LCTL); break;
    case DOUBLE_TAP: break;
    case DOUBLE_HOLD: layer_off(L_READLINE); break;
  }

  td_lctl_state = 0;
}

static int td_lsft_state;
void td_lsft_finished(tap_dance_state_t *state, void *user_data) {
  td_lsft_state = cur_dance_state(state);

  switch (td_lsft_state) {
    case SINGLE_TAP: tap_code(KC_LSFT); break;
    case SINGLE_HOLD: register_code(KC_LSFT); break;
    case DOUBLE_TAP: layer_on(L_SYMBOL); break;
    case DOUBLE_HOLD: layer_on(L_SYMBOL); break;
  }
}
void td_lsft_reset(tap_dance_state_t *state, void *user_data) {
  switch (td_lsft_state) {
    case SINGLE_TAP: break;
    case SINGLE_HOLD: unregister_code(KC_LSFT); break;
    case DOUBLE_TAP: layer_off(L_SYMBOL); break;
    case DOUBLE_HOLD: layer_off(L_SYMBOL); break;
  }

  td_lsft_state = 0;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    default:
      return TAPPING_TERM;
  }
}

tap_dance_action_t tap_dance_actions[] = {
  [TD_LCTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_lctl_finished, td_lctl_reset),
  [TD_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_lsft_finished, td_lsft_reset)
};

void rgb_matrix_random_sethsv_noeeprom(void) {
  uint8_t h = rand() % 256;
  uint8_t s = 200 + (rand() % 56);
  uint8_t v = 100;

  rgb_matrix_sethsv_noeeprom(h, s, v);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!record->event.pressed) {
    return true;
  }

  // rgb
  if (get_highest_layer(layer_state) == L_QWERTY) {
    rgb_matrix_random_sethsv_noeeprom();
  }

  // macro
  switch (keycode) {
    case M_CTL_A:
      register_code(KC_LGUI);
      register_code(KC_LEFT);
      unregister_code(KC_LEFT);
      unregister_code(KC_LGUI);
      return false;

    case M_CTL_E:
      register_code(KC_LGUI);
      register_code(KC_RGHT);
      unregister_code(KC_RGHT);
      unregister_code(KC_LGUI);
      return false;

    case M_CTL_W:
      register_code(KC_LALT);
      register_code(KC_DEL);
      unregister_code(KC_DEL);
      unregister_code(KC_LALT);
      return false;

    case M_CTL_U:
      register_code(KC_LGUI);
      register_code(KC_DEL);
      unregister_code(KC_DEL);
      unregister_code(KC_LGUI);
      return false;
  }

  return true;
}

/**
 * Hue: H / 360 * 255
 * Saturation: S / 100 * 255
 * Value: V / 100 * 255
 */
layer_state_t layer_state_set_user(layer_state_t state) {
  uint8_t layer = get_highest_layer(state);

  switch (layer) {
    case L_QWERTY:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE);
      rgb_matrix_random_sethsv_noeeprom();
      break;

    case L_NUMBER:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv_noeeprom(128, 255, 50);
      break;

    case L_FUNCTION:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv_noeeprom(64, 255, 50);
      break;

    case L_READLINE:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv_noeeprom(213, 255, 50);
      break;

    case L_SYMBOL:
      rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv_noeeprom(30, 218, 50);
      break;
  }

  return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_QWERTY] = LAYOUT_ortho_4x12_2x2u(
    KC_TAB,      KC_Q,  KC_W,    KC_E,    KC_R,          KC_T, KC_Y, KC_U,          KC_I,            KC_O,            KC_P,    KC_BSLS,
    TD(TD_LCTL), KC_A,  KC_S,    KC_D,    KC_F,          KC_G, KC_H, KC_J,          KC_K,            KC_L,            KC_LBRC, KC_RBRC,
    TD(TD_LSFT), KC_Z,  KC_X,    KC_C,    KC_V,          KC_B, KC_N, KC_M,          KC_COMM,         KC_DOT,          KC_SLSH, TD(TD_LSFT),
    KC_NO,       KC_NO, KC_LALT, KC_LGUI, LT(1, KC_SPC),             LT(1, KC_SPC), LT(2, KC_ENTER), LCTL_T(KC_BSPC), KC_NO,   KC_NO
  ),
  [L_NUMBER] = LAYOUT_ortho_4x12_2x2u(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8, KC_9,   KC_0,     _______,
    _______, _______, _______, _______, _______, _______, _______, KC_4,    KC_5, KC_6,   KC_SCLN,  KC_QUOT,
    _______, _______, _______, _______, _______, _______, _______, KC_1,    KC_2, KC_3,   KC_MINUS, KC_EQUAL,
    _______, _______, _______, _______, _______,                   _______, KC_0, KC_DOT, _______,  _______
  ),
  [L_FUNCTION] = LAYOUT_ortho_4x12_2x2u(
    KC_NO, KC_NO,   KC_BRIU, KC_MFFD, KC_VOLU, KC_NO, KC_NO,   KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,
    KC_NO, RGB_TOG, KC_NO,   KC_MPLY, KC_MUTE, KC_F6, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_SCLN, KC_NO,
    KC_NO, KC_NO,   KC_BRID, KC_MRWD, KC_VOLD, KC_NO, KC_NO,   KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,
    KC_NO, KC_NO,   QK_BOOT, KC_NO,   KC_NO,                   KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO
  ),
  [L_READLINE] = LAYOUT_ortho_4x12_2x2u(
    KC_NO, KC_NO,   M_CTL_W, M_CTL_E, KC_NO,   KC_NO,   KC_NO,   M_CTL_U, KC_NO, KC_NO, KC_UP,  KC_NO,
    KC_NO, M_CTL_A, KC_NO,   KC_NO,   KC_RGHT, KC_NO,   KC_BSPC, KC_ENT,  KC_NO, KC_NO, KC_ESC, KC_NO,
    KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_LEFT, KC_DOWN, KC_ENT,  KC_NO, KC_NO, KC_NO,  KC_NO,
    KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,                     KC_NO,   KC_NO, KC_NO, KC_NO,  KC_NO
  ),
  [L_SYMBOL] = LAYOUT_ortho_4x12_2x2u(
    LSFT(KC_GRV), LSFT(KC_1), LSFT(KC_2), LSFT(KC_3), LSFT(KC_4), LSFT(KC_5), LSFT(KC_6), LSFT(KC_7), LSFT(KC_8), LSFT(KC_9), LSFT(KC_0),     _______,
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    LSFT(KC_4), LSFT(KC_5), LSFT(KC_6), LSFT(KC_SCLN),  LSFT(KC_QUOT),
    _______,      _______,    _______,    _______,    _______,    _______,    _______,    LSFT(KC_1), LSFT(KC_2), LSFT(KC_3), LSFT(KC_MINUS), LSFT(KC_EQUAL),
    _______,      _______,    _______,    _______,    _______,                            _______,    _______,    _______,    _______,        _______
  )
};
