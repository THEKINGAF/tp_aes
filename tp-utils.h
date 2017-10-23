#include <stdio.h>
#include "aes-128_enc.h"

void print_block(uint8_t block[AES_BLOCK_SIZE]);
int blocks_are_equals(uint8_t block1[AES_BLOCK_SIZE], uint8_t block2[AES_BLOCK_SIZE]);
void block_copy(uint8_t from[AES_BLOCK_SIZE], uint8_t to[AES_BLOCK_SIZE]);
