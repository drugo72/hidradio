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
 * $Id: CDCAbstractControlManagementDescriptor.h 23837 2010-10-13 13:45:05Z mahendran.p $
 */

/**\file
 *
 * \section Purpose
 *
 * Definition of a class for manipulating CDC abstract control management
 * descriptors.
 *
 * \section Usage
 *
 * Should be included in a list of USB configuration descriptors.
 */

#ifndef CDCABSTRACTCONTROLMANAGEMENTDESCRIPTOR_H
#define CDCABSTRACTCONTROLMANAGEMENTDESCRIPTOR_H
/** \addtogroup usb_cdc
 *@{
 */

/*------------------------------------------------------------------------------
 *         Definitions
 *------------------------------------------------------------------------------*/

/** \addtogroup usb_cdc_acm USB CDC ACM Capabilities
 * @{
 *
 * This section lists the capabilities of the CDC ACM.
 * - \ref CDCAbstractControlManagementDescriptor_COMMFEATURE
 * - \ref CDCAbstractControlManagementDescriptor_LINE
 * - \ref CDCAbstractControlManagementDescriptor_SENDBREAK
 * - \ref CDCAbstractControlManagementDescriptor_NETWORKCONNECTION
 */

/** Device supports the request combination of SetCommFeature, ClearCommFeature
    and GetCommFeature. */
#define CDCAbstractControlManagementDescriptor_COMMFEATURE          (1 << 0)
/** Device supports the request combination of SetLineCoding, GetLineCoding and
    SetControlLineState. */
#define CDCAbstractControlManagementDescriptor_LINE                 (1 << 1)
/** Device supports the SendBreak request. */
#define CDCAbstractControlManagementDescriptor_SENDBREAK            (1 << 2)
/** Device supports the NetworkConnection notification. */
#define CDCAbstractControlManagementDescriptor_NETWORKCONNECTION    (1 << 3)
/** @}*/

/*------------------------------------------------------------------------------
 *         Types
 *----------------------------------------------------------------------------*/

#ifdef __ICCARM__          /* IAR */
#pragma pack(1)            /* IAR */
#define __attribute__(...) /* IAR */
#endif                     /* IAR */

/**
 * \typedef CDCAbstractControlManagementDescriptor
 * \brief Describes the command supported by the communication interface class
 *        with the Abstract Control Model subclass code.
 */
typedef struct _CDCAbstractControlManagementDescriptor {

    /** Size of this descriptor in bytes. */
    unsigned char bFunctionLength;
    /** Descriptor type . */
    unsigned char bDescriptorType;
    /** Descriptor subtype . */
    unsigned char bDescriptorSubtype;
    /** Configuration capabilities.
        \sa usb_cdc_acm CDC ACM Capabilities. */
    unsigned char bmCapabilities;

} __attribute__ ((packed)) CDCAbstractControlManagementDescriptor; // GCC

#ifdef __ICCARM__          /* IAR */
#pragma pack()             /* IAR */
#endif                     /* IAR */

/**@}*/
#endif /*#ifndef CDCABSTRACTCONTROLMANAGEMENTDESCRIPTOR_H*/

