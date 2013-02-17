/**
 * @file pal_flash.c
 *
 * @brief flash related functions for SAM3 MCUs
 *
 * This file implements flash related functions for SAM3 MCUs.
 *
 * $Id: pal_flash.c 25440 2011-02-08 13:52:04Z mahendran.p $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */
#if (defined INTERN_FLASH_ENABLED)
#include "pal.h"
#include "pal_flash.h"

/* === Types =============================================================== */

/* === Globals ============================================================= */
/* To hold the pagebuffer */
static uint8_t PageBuffer[AT91C_IFLASH_PAGE_SIZE];

/* === Prototypes =========================================================== */
static uint8_t pal_efc_perform_command(Efc *efc,uint8_t command, unsigned short argument);

/* === Implementation ======================================================= */

/**
 * @brief Unlocks the page region
 * @param pagenum  page number.
 */
uint8_t pal_flash_unlock_page(uint16_t pagenum)
{
    return(pal_efc_perform_command(EFC, EFC_FCMD_CLB, pagenum));
}


/**
 * @brief Reads data from internal flash
 *
 * @param[in]  start_addr Start offset within flash
 * @param[in]  length Number of bytes to read from flash
 * @param[out] value Data from persistence storage
 *
 * @return 0 if everything went Ok
 */
uint8_t pal_flash_read(uint16_t start_addr, uint16_t length, void *value)
{
    uint32_t address;
    uint8_t index;
    uint8_t *data_ptr;

    data_ptr = (uint8_t *)(value);
    address = FLASH_PAL_ADDRESS + start_addr;

    for (index = 0; index < length; index++)
    {
        data_ptr[start_addr] = *((uint8_t*)(address + index));
        start_addr++;
    }
    return 0;
}


/**
 * @brief Writes a data buffer in the internal flash
 *
 * This function works in polling mode, and thus only returns when the
 * data has been effectively written.
 * @param start_address  start address.
 * @param pBuffer  Data buffer.
 * @param size  Size of data buffer in bytes.
 * @return 0 if successful, otherwise returns an error code.
 */
uint8_t pal_flash_write(uint32_t start_address, const void *pBuffer,
                                                uint32_t size)
{
    uint16_t page = FLASH_PAL_PAGENUM;
    uint16_t writeoffset;
    uint32_t pageAddress = FLASH_PAL_ADDRESS;
    uint8_t error;
    uint8_t index;
    uint32_t sizeTmp;
    uint32_t *pAlignedDestination;
    uint32_t *pAlignedSource;
    uint32_t address;

    address = FLASH_PAL_ADDRESS + start_address;

    ASSERT(pBuffer);
    ASSERT(address >=AT91C_IFLASH);
    ASSERT((address + size) <= (AT91C_IFLASH + AT91C_IFLASH_SIZE));

    error = pal_flash_unlock_page(FLASH_PAL_PAGENUM);
    ASSERT(!error);

    /* Copy data in temporary buffer to avoid alignment problems */
    for (index = 0; index < AT91C_IFLASH_PAGE_SIZE - 1; index++)
    {
        PageBuffer[index] = *((uint8_t*)(FLASH_PAL_ADDRESS + index));
    }
    writeoffset = FLASH_PAL_ADDRESS - address;
    memcpy(PageBuffer + writeoffset, pBuffer, size);

    /* Write page
    Writing 8-bit and 16-bit data is not allowed
    and may lead to unpredictable data corruption */
    pAlignedDestination = (uint32_t*)pageAddress;
    pAlignedSource = (uint32_t*)PageBuffer;
    sizeTmp = AT91C_IFLASH_PAGE_SIZE;
    while (sizeTmp >= 4)
    {
        *pAlignedDestination++ = *pAlignedSource++;
        sizeTmp -= 4;
    }

    /* Send writing command */
    error = pal_efc_perform_command(EFC, EFC_FCMD_EWP, page);
    ASSERT(!error);
    error = error;
    return 0;
}

/**
 * @brief Performs the given command and wait until its completion (or an error).
 *
 * @param efc  Pointer to a Efc instance
 * @param command  Command to perform.
 * @param argument  Optional command argument.
 * @return 0 if successful, otherwise returns an error code.
 */
static uint8_t pal_efc_perform_command(Efc *efc, uint8_t command, unsigned short argument)
{
    uint32_t status;
    efc->EEFC_FCR = (0x5A << 24) | (argument << 8) | command;
    do {

        status = efc->EEFC_FSR;
    }
    while ((status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);

    return (status & (EEFC_FSR_FLOCKE | EEFC_FSR_FCMDE));
}

#endif
/* EOF */
