/**
 * @file pal_flash
 *
 * @brief PAL flash internal functions prototypes
 *
 * This header has the flash specific stuctures, macros and
 * internal functions.
 *
 * $Id: pal_flash.h 25440 2011-02-08 13:52:04Z mahendran.p $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef PAL_FLASH_H
#define PAL_FLASH_H

#if (defined INTERN_FLASH_ENABLED)
/* === Includes =============================================================*/

/* === Globals ============================================================= */

/* === Macros ============================================================= */
/* EFC command */
#define EFC_FCMD_GETD    0x00
#define EFC_FCMD_WP      0x01
#define EFC_FCMD_WPL     0x02
#define EFC_FCMD_EWP     0x03
#define EFC_FCMD_EWPL    0x04
#define EFC_FCMD_EA      0x05
#define EFC_FCMD_SLB     0x08
#define EFC_FCMD_CLB     0x09
#define EFC_FCMD_GLB     0x0A
#define EFC_FCMD_SFB     0x0B
#define EFC_FCMD_CFB     0x0C
#define EFC_FCMD_GFB     0x0D
#define EFC_FCMD_STUI    0x0E
#define EFC_FCMD_SPUI    0x0F

/* === Prototypes =========================================================== */
uint8_t pal_flash_unlock_page(uint16_t pagenum);
uint8_t pal_flash_write(uint32_t start_address, const void *pBuffer, uint32_t size);
uint8_t pal_flash_read(uint16_t start_addr, uint16_t length, void *value);

#endif/* #if (defined INTERN_FLASH_ENABLED) */
#endif  /* PAL_FLASH_H */

/* EOF */
