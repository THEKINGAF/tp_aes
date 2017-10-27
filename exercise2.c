#include "tp-utils.h"
#include <stdlib.h>

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
		    /* reverse one state byte for the last half-round */
		    reversed_byte = block_set[k][j] ^ i; 
		    reversed_byte = Sinv[reversed_byte];
		    /* XOR all of the set */
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
