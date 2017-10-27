#include "tp-utils.h"

/*
    Test key expansion functions
    Output can be compared to FIPS 197, part C.1 AES-128 (Nk=4, Nr=10)
*/

int main () {
    unsigned int i;
    uint8_t prev_key[AES_128_KEY_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    uint8_t next_key[AES_128_KEY_SIZE];
 
    printf("start    : "); print_block(prev_key); printf("\n");

    for (i = 0; i < 10; ++i) {
	next_aes128_round_key(prev_key, next_key, i);
	block_copy(next_key, prev_key);
	printf("round %02d : ", i + 1); print_block(prev_key); printf("\n");
    }

    for (i = 10; i > 0; --i) {
	prev_aes128_round_key(next_key, prev_key, i);
	block_copy(prev_key, next_key);
	printf("round %02d : ", i - 1); print_block(next_key); printf("\n");
    }


    return 0;
}
