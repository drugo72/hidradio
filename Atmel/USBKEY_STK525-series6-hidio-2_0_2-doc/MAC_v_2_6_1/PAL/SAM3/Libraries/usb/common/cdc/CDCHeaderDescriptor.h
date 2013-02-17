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
 * $Id: CDCHeaderDescriptor.h 23837 2010-10-13 13:45:05Z mahendran.p $
 */

/**\file
 *
 * \section Purpose
 *
 * Definition of the CDCHeaderDescriptor class.
 *
 * \section Usage
 *
 * Should be included in a USB configuration descriptor.
*/

#ifndef CDCHEADERDESCRIPTOR_H
#define CDCHEADERDESCRIPTOR_H
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
 * \typedef CDCHeaderDescriptor
 * \brief Marks the beginning of the concatenated set of functional descriptors
 *        for the interface.
 */
typedef struct _CDCHeaderDescriptor {

    /** Size of this descriptor in bytes. */
    unsigned char bFunctionLength;
    /** Descriptor type . */
    unsigned char bDescriptorType;
    /** Descriptor sub-type . */
    unsigned char bDescriptorSubtype;
    /** USB CDC specification release number. */
    unsigned short bcdCDC;

} __attribute__ ((packed)) CDCHeaderDescriptor; /* GCC*/

#ifdef __ICCARM__          /* IAR */
#pragma pack()             /* IAR */
#endif                     /* IAR */

/**@}*/
#endif /*#ifndef CDCHEADERDESCRIPTOR_H*/


