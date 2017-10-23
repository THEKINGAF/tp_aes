#include "tp-utils.h"

void print_block (uint8_t block[AES_BLOCK_SIZE]) {
    unsigned int i;

    for (i = 0; i < AES_BLOCK_SIZE; ++i)
        printf("%02x ", block[i]);
}

int blocks_are_equals(uint8_t block1[AES_BLOCK_SIZE], uint8_t block2[AES_BLOCK_SIZE]) {
    unsigned int i;

    for (i = 0; i < AES_BLOCK_SIZE; ++i)
	if (block1[i] != block2[i])
	    return 0;

    return 1;
}

void block_copy(uint8_t from[AES_BLOCK_SIZE], uint8_t to[AES_BLOCK_SIZE]) {
    unsigned int i;

    for (i = 0; i < AES_BLOCK_SIZE; ++i)
	to[i] = from[i];
}
