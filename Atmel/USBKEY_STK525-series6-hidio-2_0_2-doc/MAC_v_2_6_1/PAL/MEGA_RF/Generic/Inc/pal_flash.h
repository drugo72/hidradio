/**
 * @file pal_flash.h
 *
 * @brief Flash writting support functionality
 *
 * $Id: pal_flash.h 25462 2011-02-09 07:38:41Z ravikumar.vanteddu $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#if ((defined FLASH_SUPPORT) || (defined DOXYGEN))

/* Prevent double inclusion */
#ifndef FLASH_SUPPORT_H
#define FLASH_SUPPORT_H


/**
 * @brief Fills the temporary page buffer
 *
 * This function fills the temporay flash page buffer.
 *
 * @param start_offset  Offset of start address
 * @param length        Length of provide data buffer
 * @param data          Pointer to data buffer
 *
 * @ingroup apiPalApi
 */
#ifdef __ICCAVR__
#pragma location = "bootloader"
void flash_fill_page_buffer(uint16_t start_offset, uint16_t length, uint8_t *data);
#endif
#ifdef __GNUC__
void flash_fill_page_buffer(uint16_t start_offset, uint16_t length, uint8_t *data)
    __attribute__((section(".bootloader"),noinline));
#endif


/**
 * @brief Writes the actual flash page
 *
 * This function writes the data from the temporary flash page buffer to the
 * actual flash page.
 *
 * @param page_start_addr  Start address of the flash page
 *
 * @ingroup apiPalApi
 */
#ifdef __ICCAVR__
#pragma location = "bootloader"
void flash_program_page(uint32_t page_start_addr);
#endif
#ifdef __GNUC__
void flash_program_page(uint32_t page_start_addr)
    __attribute__((section(".bootloader"),noinline));
#endif


/**
 * @brief Swaps the firmware image
 *
 * This function swapes the image with the current application program.
 *
 * @param image_start_addr  Start address of the image within the flash
 * @param image_size        Size of the firmware image
 *
 * @ingroup apiPalApi
 */
#ifdef __ICCAVR__
#pragma location = "bootloader"
void flash_swap(uint32_t image_start_addr, uint32_t image_size);
#endif
#ifdef __GNUC__
void flash_swap(uint32_t image_start_addr, uint32_t image_size)
    __attribute__((section(".bootloader"),noinline));
#endif


#endif  /* #ifndef FLASH_SUPPORT_H */
#endif /* #if ((defined FLASH_SUPPORT) || (defined DOXYGEN)) */

