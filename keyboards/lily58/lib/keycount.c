#include <stdio.h>
#include "lily58.h"

char typecount_msg[64];
uint32_t type_count = 0;

void set_typecount(void) {
    type_count++;

    snprintf(typecount_msg, sizeof(typecount_msg), "Count: %lu", type_count);
}

const char *read_typecount(void) {
    return typecount_msg;
} 