#include "tp-utils.h"

void print_block (uint8_t * block) {
    unsigned int i;

    for (i = 0; i < AES_BLOCK_SIZE; ++i)
        printf("%02x ", block[i]);
}
