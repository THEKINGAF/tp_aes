#include "tp-utils.h"

void reverse_halfround(uint8_t * block, uint8_t * key) {
    unsigned int i;
    uint8_t tmp;

    for (i = 0; i < AES_BLOCK_SIZE; ++i)
        block[i] ^= key[i];

    /* Row 0 */
    block[0]  = Sinv[block[0]];
    block[4]  = Sinv[block[4]];
    block[8]  = Sinv[block[8]];
    block[12] = Sinv[block[12]];

    /* Row 1 */
    tmp = block[13];
    block[13] = Sinv[block[9]];
    block[9]  = Sinv[block[5]];
    block[5]  = Sinv[block[1]];
    block[1]  = Sinv[tmp];

    /* Row 2 */
    tmp = block[14];
    block[14] = Sinv[block[6]];
    block[6]  = Sinv[block[tmp]];
    tmp = block[10];
    block[10] = Sinv[block[2]];
    block[2]  = Sinv[tmp];

    /* Row 3 */
    tmp = block[3];
    block[3]  = Sinv[block[7]];
    block[7]  = Sinv[block[11]];
    block[11] = Sinv[block[15]];
    block[15] = Sinv[tmp];
}

int main () {
    uint8_t block[AES_BLOCK_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t block_cpy[AES_BLOCK_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t key[AES_128_KEY_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    printf("in  = "); print_block(block); printf("\n");

    // 3 1/2 rounds encryption
    aes128_enc(block, key, 4, 0);

    printf("out = "); print_block(block); printf("\n");

    reverse_halfround(block, key);

    printf("1/2 = "); print_block(block); printf("\n");

    // 3 1/2 rounds encryption
    aes128_enc(block_cpy, key, 3, 1);

    printf("out = "); print_block(block_cpy); printf("\n");

    return 0;
}
