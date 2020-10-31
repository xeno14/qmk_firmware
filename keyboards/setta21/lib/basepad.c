#include <stdio.h>
#include <string.h>
#include "setta21.h"

#define BASEPAD_BUFFER_SIZE 31
#define BASEPAD_BUFFER_PREFIX_SIZE 4
#define BASEPAD_NUMBER_SIZE (BASEPAD_BUFFER_SIZE - BASEPAD_BUFFER_PREFIX_SIZE)

// clang-format off
const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ';', '\'', ' ', ',', '.', '/', ' ', ' ', ' '};

const uint8_t code_to_int[60] = {
     0, 0, 0, 0, 10, 11, 12, 13, 14, 15,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// clang-format on

enum base_type {
    BINARY = 0,
    DECIMAL,
    HEXADECIMAL,
    BASE_TYPE_NUM  // always last
};

const uint8_t base_type_to_int[] = {2, 10, 16};

static struct {
    char           buffers[BASE_TYPE_NUM][BASEPAD_BUFFER_SIZE];
    uint64_t       value;
    enum base_type input_base_type;
    enum base_type output_base_type;
} _basepad = {{" 2  \0", "10<>\0", "16  \0"}, 0, DECIMAL, DECIMAL};

bool basepad_is_keycode_valid(enum base_type base_type, uint16_t keycode) {
    bool result = false;
    switch (base_type) {
        case BINARY:
            result = (keycode == KC_0 || keycode == KC_1);
            break;
        case DECIMAL:
            switch (keycode) {
                case KC_1 ... KC_0:
                    result = true;
                default:
                    break;
            };
            break;
        case HEXADECIMAL:
            switch (keycode) {
                case KC_1 ... KC_0:
                case KC_A ... KC_F:
                    result = true;
                    break;
                default:
                    break;
            }
        default:
            break;
    }
    return result;
}

enum base_type basepad_get_input_base_type(void) { return _basepad.input_base_type; }

enum base_type basepad_get_output_base_type(void) { return _basepad.output_base_type; }

const char* basepad_get_output_buffer(void) { return _basepad.buffers[_basepad.output_base_type] + BASEPAD_BUFFER_PREFIX_SIZE; }

void basepad_update_buffers(void) {
    // for binary
    {
        char*    buf   = _basepad.buffers[BINARY];
        size_t   len   = 0;
        uint64_t value = _basepad.value;
        while (len < BASE_TYPE_NUM - 1 && value) {
            buf[len++] = (value & 1) ? '1' : '0';
        }
        buf[len] = '\0';
        // reverse
        size_t l = 0;
        size_t r = len - 1;
        while (l < r) {
            char tmp = buf[l];
            buf[l]   = buf[r];
            buf[r]   = tmp;
            l++;
            r--;
        }
    }
    // for decimal
    { snprintf(_basepad.buffers[DECIMAL], BASEPAD_BUFFER_SIZE, "%llu", _basepad.value); }
    // for hex
    { snprintf(_basepad.buffers[HEXADECIMAL], BASEPAD_BUFFER_SIZE, "%llx", _basepad.value); }
}

void basepad_input(uint16_t keycode) {
    enum base_type input_base_type = basepad_get_input_base_type();
    if (!basepad_is_keycode_valid(input_base_type, keycode)) {
        return;
    }

    // update value for the input base type
    _basepad.value = _basepad.value * base_type_to_int[input_base_type] + code_to_int[keycode];

    basepad_update_buffers();
}

void basepad_delete(void) {
    uint8_t base = base_type_to_int[_basepad.input_base_type];
    _basepad.value /= base;

    basepad_update_buffers();
}

const char* basepad_get_output(void) { return _basepad.buffers[_basepad.output_base_type] + BASEPAD_BUFFER_PREFIX_SIZE; }

const char* basepad_get_buffer2(void) { return _basepad.buffers[BINARY]; }

const char* basepad_get_buffer10(void) { return _basepad.buffers[DECIMAL]; }

const char* basepad_get_buffer16(void) { return _basepad.buffers[HEXADECIMAL]; }

void basepad_clear(void) {
    _basepad.value = 0;
    for (int i = 0; i < BASE_TYPE_NUM; i++) {
        _basepad.buffers[i][BASEPAD_BUFFER_PREFIX_SIZE] = '\0';
    }
}

void basepad_next_input_base_type(void) {
    _basepad.input_base_type++;
    _basepad.buffers[_basepad.input_base_type][BASEPAD_BUFFER_PREFIX_SIZE] = ' ';
    if (_basepad.input_base_type == BASE_TYPE_NUM) {
        _basepad.input_base_type = 0;
    }
    _basepad.buffers[_basepad.input_base_type][BASEPAD_BUFFER_PREFIX_SIZE] = '<';
}

void basepad_next_output_base_type(void) {
    _basepad.output_base_type++;
    _basepad.buffers[_basepad.output_base_type][BASEPAD_BUFFER_PREFIX_SIZE] = ' ';
    if (_basepad.output_base_type == BASE_TYPE_NUM) {
        _basepad.output_base_type = 0;
    }
    _basepad.buffers[_basepad.output_base_type][BASEPAD_BUFFER_PREFIX_SIZE] = '>';
}
