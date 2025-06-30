#include QMK_KEYBOARD_H

enum layer_names {
  _QWERTY,
  _SYMBOL,
  _FUNCTION,
  _READLINE
};

enum macros {
  _M_CTRL_A = SAFE_RANGE,
  _M_CTRL_E,
  _M_CTRL_W,
  _M_CTRL_U
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!record->event.pressed) {
    return true;
  }

  switch (keycode) {
    case _M_CTRL_A:
      register_code(KC_LGUI);
      register_code(KC_LEFT);
      unregister_code(KC_LEFT);
      unregister_code(KC_LGUI);
      return false;

    case _M_CTRL_E:
      register_code(KC_LGUI);
      register_code(KC_RGHT);
      unregister_code(KC_RGHT);
      unregister_code(KC_LGUI);
      return false;

    case _M_CTRL_W:
      register_code(KC_LALT);
      register_code(KC_DEL);
      unregister_code(KC_DEL);
      unregister_code(KC_LALT);
      return false;

    case _M_CTRL_U:
      register_code(KC_LGUI);
      register_code(KC_DEL);
      unregister_code(KC_DEL);
      unregister_code(KC_LGUI);
      return false;
  }

  return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_ortho_4x12_2x2u(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,          KC_T,    KC_Y,    KC_U,          KC_I,    KC_O,    KC_P,    KC_MINS,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,          KC_G,    KC_H,    KC_J,          KC_K,    KC_L,    KC_LBRC, MT(MOD_LCTL, KC_RBRC),
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,          KC_B,    KC_N,    KC_M,          KC_COMM, KC_DOT,  KC_SLSH, MT(MOD_LSFT, KC_BSLS),
        KC_F6,   KC_MPLY, KC_LALT, KC_LGUI, LT(1, KC_SPC),                   LT(2, KC_SPC), KC_ENT,  KC_BSPC, KC_VOLD, KC_VOLU
    ),
    [_SYMBOL] = LAYOUT_ortho_4x12_2x2u(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_EQL,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_SCLN, KC_QUOT,
        KC_LSFT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_RSFT,
        _______, _______, _______, _______, KC_SPC,                    KC_SPC,  _______, _______, _______, _______
    ),
    [_FUNCTION] = LAYOUT_ortho_4x12_2x2u(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_EQL,
        _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SCLN, KC_QUOT,
        KC_LSFT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_RSFT,
        KC_MRWD, KC_MFFD, _______, QK_BOOT, KC_SPC,                    KC_SPC,  KC_ESC,  KC_CAPS, KC_BRID, KC_BRIU
    ),
    [_READLINE] = LAYOUT_ortho_4x12_2x2u(
        _______,  _______,   _M_CTRL_W, _M_CTRL_E, _______,  _______, _______, _M_CTRL_U, _______, _______, KC_UP,   _______,
        _______,  _M_CTRL_A, _______,   _______,   KC_RGHT, _______,  KC_BSPC, KC_ENT,    _______, _______, KC_ESC,  _______,
        _______,  _______,   _______,   _______,   _______, KC_LEFT,  KC_DOWN, KC_ENT,    _______, _______, _______, _______,
        _______,  _______,   _______,   _______,   _______,                    _______,   _______, _______, _______, _______
    )
};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _SYMBOL, _FUNCTION, _READLINE);
}
