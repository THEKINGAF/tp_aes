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
    uint8_t key[AES_128_KEY_SIZE], reversed_block[AES_BLOCK_SIZE], ciphered_block[AES_BLOCK_SIZE];
    unsigned int i, j;
    uint8_t plaintext[AES_128_KEY_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t ciphertext[AES_128_KEY_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

    for (j = 0; j < AES_128_KEY_SIZE; ++i) {
	i = 0;
	do {
	    key[j] = i;

	    block_copy(ciphertext, reversed_block);
	    reverse_halfround(reversed_block, key);
	    block_copy(plaintext, ciphered_block);
	    aes128_enc(ciphered_block, key, 3, 0);

	    ++i;
	} while (blocks_are_equals(reversed_block, ciphered_block) && i < 255);
	if (blocks_are_equals(reversed_block, ciphered_block))
	    printf("%02x ", i);
	else {
	    printf("Error : can't find the key\n");
	    return -1;
	}
    }

    return 0;
}
