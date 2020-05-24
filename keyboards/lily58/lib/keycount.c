#include <stdio.h>
#include "lily58.h"

char typecount_msg[64];
uint32_t total_count = 0;
uint32_t bs_count = 0;
uint32_t del_count = 0;

void set_typecount(uint16_t keycode, keyrecord_t *record) {
    total_count++;
    bs_count += (keycode == (uint16_t)KC_BSPC);
    del_count += (keycode == (uint16_t)KC_DEL);

    snprintf(typecount_msg, sizeof(typecount_msg), 
        "Tot: %lu B: %lu D: %lu", total_count, bs_count, del_count);
}

const char *read_typecount(void) {
    return typecount_msg;
} 