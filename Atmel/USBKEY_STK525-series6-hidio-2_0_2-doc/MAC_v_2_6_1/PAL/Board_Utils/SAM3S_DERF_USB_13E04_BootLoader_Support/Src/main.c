/**
 * @file main.c
 *
 * @brief Boot_from_ROM
 *
 * This is the source code of boot from ROM. It implements the
 * firmware that can make the board to boot from ROM.
 *
 * $Id: main.c 25479 2011-02-09 11:40:11Z mahendran.p $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2011, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pal.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */
#define GPNVM_BIT0     (0)
#define GPNVM_BIT1     (1)

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */
uint8_t pal_flash_clear_GPNVM(uint8_t gpnvm);

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Main function of the application
 */
int main(void)
{
    /* Clear the GPNVM BIT 1 */
    pal_flash_clear_GPNVM(GPNVM_BIT1);

    /* Main loop */
    while (1)
    {
      /* Do nothing*/
    }
}

/**
 * @brief Clears the selected GPNVM bit.
 *
 * @param gpnvm  GPNVM bit index.
 * @returns 0 if successful; else error code.
 */
uint8_t pal_flash_clear_GPNVM(uint8_t gpnvm)
{
    uint32_t status;
    EFC->EEFC_FCR = (0x5A << 24) | (gpnvm << 8) | 0x0C;
    do {

        status = EFC->EEFC_FSR;
    }
    while ((status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);

    return (status & (EEFC_FSR_FLOCKE | EEFC_FSR_FCMDE));
}

/* EOF */
