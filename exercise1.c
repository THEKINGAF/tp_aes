#include "tp-utils.h"

void aes3rounds_keyed_function (uint8_t block[AES_BLOCK_SIZE], const uint8_t key1[AES_128_KEY_SIZE], const uint8_t key2[AES_128_KEY_SIZE]) {
    uint8_t block1[AES_BLOCK_SIZE], block2[AES_BLOCK_SIZE];
    unsigned int i;

    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        block1[i] = block[i];
        block2[i] = block[i];
    }

    aes128_enc(block1, key1, 3, 1);
    aes128_enc(block2, key2, 3, 1);

    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        block[i] = block1[i] ^ block2[i];
    }
}

int main () {
     uint8_t block[AES_BLOCK_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t key1[AES_128_KEY_SIZE] = {0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    uint8_t key2[AES_128_KEY_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    unsigned int i;
 
    printf("in  = "); print_block(block); printf("\n");

    aes3rounds_keyed_function (block, key1, key2);

    printf("out = "); print_block(block); printf("\n");

    return 0;
}
