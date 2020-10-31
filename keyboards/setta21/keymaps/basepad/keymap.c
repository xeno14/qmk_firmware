#include QMK_KEYBOARD_H
// #include "keymap_jp.h"


#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

#ifdef OLED_DRIVER_ENABLE
static uint32_t        oled_timer = 0;
#endif

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
  _BASE = 0,
};

enum custom_keycodes {
  BP_ENT = SAFE_RANGE,
  BP_IN,
  BP_OUT,
  BP_ESC,
  BP_DEL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_numpad_6x4(
      //,-----------------------------------|
            KC_D,   KC_E,   KC_F,   BP_ESC,
      //|--------+--------+--------+--------|
            KC_A,   KC_B,   KC_C,   BP_OUT,
      //|--------+--------+--------+--------|
            KC_7,   KC_8,   KC_9,
      //|--------+--------+--------+--------|
            KC_4,   KC_5,   KC_6,   BP_IN,
      //|--------+--------+--------+--------|
            KC_1,   KC_2,   KC_3,
      //|--------+--------+--------+--------|
            KC_0,           BP_DEL, BP_ENT
      //`-----------------------------------'
  )
};


// //A description for expressing the layer position in LED mode.
// layer_state_t layer_state_set_user(layer_state_t state) {
//   return update_tri_layer_state(state, _ARROW, _MACRO, _ADJUST);
// }

// When add source files to SRC in rules.mk, you can use functions.
void basepad_input(uint16_t);
const char* basepad_get_output(void);
const char* basepad_get_buffer2(void);
const char* basepad_get_buffer10(void);
const char* basepad_get_buffer16(void);
void basepad_delete(void);
void basepad_clear(void);
void basepad_next_input_base_type(void);
void basepad_next_output_base_type(void);

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    bool result = false;  // don't send key by default
    if (record->event.pressed) {
#ifdef OLED_DRIVER_ENABLE
        oled_timer = timer_read32();
#endif
    }
    switch (keycode) {
        case BP_IN:
            basepad_next_input_base_type();
            break;
        case BP_OUT:
            basepad_next_output_base_type();
            break;
        case BP_ENT:
            send_string(basepad_get_output());
            basepad_clear();
            result = true;
            break;
        case BP_ESC:
            basepad_clear();
            break;
        case BP_DEL:
            basepad_delete();
            break;
        default:
            basepad_input(keycode);
            break;
    }
    return result;
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
// #ifdef SSD1306OLED

// // void matrix_scan_user(void) {
// //    iota_gfx_task();
// // }

// void matrix_render_user(struct CharacterMatrix *matrix) {
//     matrix_write_ln(matrix, basepad_get_buffer2());
//     matrix_write_ln(matrix, basepad_get_buffer10());
//     matrix_write_ln(matrix, basepad_get_buffer16());
// }

// void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
//   if (memcmp(dest->display, source->display, sizeof(dest->display))) {
//     memcpy(dest->display, source->display, sizeof(dest->display));
//     dest->dirty = true;
//   }
// }

// void iota_gfx_task_user(void) {
//   struct CharacterMatrix matrix;
//   matrix_clear(&matrix);
//   matrix_render_user(&matrix);
//   matrix_update(&display, &matrix);
// }
// #endif//SSD1306OLED
