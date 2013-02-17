/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */
/*
 * $Id: CDCUnionDescriptor.h 23837 2010-10-13 13:45:05Z mahendran.p $
 */

/**
 \unit

 !!!Purpose

 Definition of a class for manipulating CDC union descriptors.

 !!!Usage

 Should be included in the list of USB descriptor used for a device
 configuration.
*/

#ifndef CDCUNIONDESCRIPTOR_H
#define CDCUNIONDESCRIPTOR_H

/** \addtogroup usb_cdc
 *@{
 */

/*----------------------------------------------------------------------------
 *         Types
 *----------------------------------------------------------------------------*/

#ifdef __ICCARM__          /* IAR */
#pragma pack(1)            /* IAR */
#define __attribute__(...) /* IAR */
#endif                     /* IAR */

/**
 * \typedef CDCUnionDescriptor
 * \brief Describes the relationship between a group of interfaces that can
 *        be considered to form a functional unit.
 */
typedef struct _CDCUnionDescriptor {

    /** Size of the descriptor in bytes. */
    unsigned char bFunctionLength;
    /** Descriptor type . */
    unsigned char bDescriptorType;
    /** Descriptor subtype . */
    unsigned char bDescriptorSubtype;
    /** Number of the master interface for this union. */
    unsigned char bMasterInterface;
    /** Number of the first slave interface for this union. */
    unsigned char bSlaveInterface0;

} __attribute__ ((packed)) CDCUnionDescriptor; /* GCC */

#ifdef __ICCARM__          /* IAR */
#pragma pack()             /* IAR */
#endif                     /* IAR */

/**@}*/

#endif /*#ifndef CDCUNIONDESCRIPTOR_H */

