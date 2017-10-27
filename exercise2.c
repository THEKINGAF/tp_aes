#include "tp-utils.h"
#include <stdlib.h>

void reverse_halfround(uint8_t * block, uint8_t * key) {
    unsigned int i;
    uint8_t tmp;

    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        block[i] ^= key[i];
    }

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

int presentFalsePositive (uint8_t ** potentials_keys) {
    unsigned int i, j, sum;

    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
	sum = 0;
	for (j = 0; j < 256; ++j) {
	    sum += potentials_keys[i][j];
	    if (sum > 1)
		return 1;
	}
    }

    return 0;
}

int main () {
    uint8_t guessed_key[AES_128_KEY_SIZE], temp_key[AES_128_KEY_SIZE];
    uint8_t ** potentials_keys;
    uint8_t reversed_byte, sum;
    unsigned int i, j, k, l;
    int falsePositiveFlag; 
    uint8_t ** block_set;

    uint8_t key[AES_128_KEY_SIZE] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
	0xcf, 0x4f, 0x3c};

    /* allocating memory for the set of blocks */
    block_set = malloc(256*sizeof(uint8_t *));
    block_set[0] = malloc(AES_BLOCK_SIZE*256*sizeof(uint8_t));
    for (i = 0; i < 256; ++i)
	block_set[i+1] = block_set[i] + AES_BLOCK_SIZE;

    /* allocating memory for potentials keys */
    potentials_keys = malloc(AES_BLOCK_SIZE*sizeof(uint8_t *));
    potentials_keys[0] = malloc(256*AES_BLOCK_SIZE*sizeof(uint8_t));
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
	if (i)
	    potentials_keys[i] = potentials_keys[i - 1] + 256; 
	for (j = 0; j < 256; ++j)
	    potentials_keys[i][j] = 1;
    }

    l = 0;
    do {
	/* generating set of ciphered blocks */
	for (i = 0; i < 256; ++i) {
	    for (k = 1; k < AES_BLOCK_SIZE; ++k)
		block_set[i][k] = l;
	    block_set[i][0] = i;
	    aes128_enc(block_set[i], key, 4, 0);
	}

    
	/* attack */
	for (j = 0; j < AES_BLOCK_SIZE; ++j) {
	    for (i = 0; i < 256; ++i) {
		sum = 0;

		for (k = 0; k < 256; ++k) {
		    reversed_byte = block_set[k][j] ^ i; 
		    reversed_byte = Sinv[reversed_byte];
		    sum ^= reversed_byte;
		}

		/* eliminate candidate for key byte j */
		if (sum)
		    potentials_keys[j][i] = 0;
	    }
	}
	falsePositiveFlag = presentFalsePositive(potentials_keys);
	if (falsePositiveFlag)
	    printf("False positive present after attack %d, changing constant and retrying...\n", l + 1);
	++l;
    } while (falsePositiveFlag && (l < 256));

    if (falsePositiveFlag)
	return 1;

    printf("\n=============\n  KEY FOUND\n=============\n\n");

    for (i = 0; i < AES_BLOCK_SIZE; ++i)
	for (j = 0; j < 256; ++j)
	    if(potentials_keys[i][j])
		guessed_key[i] = j;

    printf("key_schedule[last_round] :\n    "); print_block(guessed_key); printf("\n");

    /* Reversing key schedule to find the initial key*/
    prev_aes128_round_key(guessed_key, temp_key, 4);
    prev_aes128_round_key(temp_key, guessed_key, 3);
    prev_aes128_round_key(guessed_key, temp_key, 2);
    prev_aes128_round_key(temp_key, guessed_key, 1);
    printf("found key :\n    "); print_block(guessed_key); printf("\n");

    free(block_set[0]);
    free(block_set);
    free(potentials_keys[0]);
    free(potentials_keys);

    return 0;
}
