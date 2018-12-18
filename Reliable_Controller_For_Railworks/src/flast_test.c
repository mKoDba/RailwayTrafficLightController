/*
 * flast_test.c
 *
 *  Created on: 18. pro 2018.
 *      Author: Mario
 */

#include "flash_test.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#define N_BYTES_FLASH (2632U)

uint16_t EEMEM eptr_eeprom_code_hash = 8056U;


uint16_t flash_test_calculate_xor(void) {
    uint16_t hash = 0;

    for (uint16_t k = 0; k < N_BYTES_FLASH - 1; k += 2) {
        uint16_t value = pgm_read_word_near(k);
        hash ^= value;
    }
    return hash;
}

uint8_t flash_test(void) {
    uint16_t hash = flash_test_calculate_xor();
    uint16_t true_hash = eeprom_read_word(&eptr_eeprom_code_hash);
    if(hash == true_hash) return 0;
    return 1;
}
