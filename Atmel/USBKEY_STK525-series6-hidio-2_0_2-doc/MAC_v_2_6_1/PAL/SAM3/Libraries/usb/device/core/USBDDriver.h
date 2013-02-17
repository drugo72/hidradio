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
 * $Id: USBDDriver.h 23837 2010-10-13 13:45:05Z mahendran.p $
 */

/**
 * \file
 *
 * \section Purpose
 *
 *    USB Device Driver class definition.
 *
 * \section Usage
 *
 *    -# Instanciate a USBDDriver object and initialize it using
 *       USBDDriver_Initialize.
 *    -# When a USB SETUP request is received, forward it to the standard
 *       driver using USBDDriver_RequestHandler.
 *    -# Check the Remote Wakeup setting via USBDDriver_IsRemoteWakeUpEnabled.
 */

#ifndef USBDDRIVER_H
#define USBDDRIVER_H


/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/

/* These headers were introduced in C99 by working group
 * ISO/IEC JTC1/SC22/WG14.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "USBDDriverDescriptors.h"
#include <usb/common/core/USBGenericRequest.h>

/*------------------------------------------------------------------------------
 *         Types
 *------------------------------------------------------------------------------*/

/**
 * \typedef USBDDriver
 * \brief USB device driver structure, holding a list of descriptors identifying
 *        the device as well as the driver current state.
 */
typedef struct _USBDDriver {

    /** List of descriptors used by the device. */
    const USBDDriverDescriptors *pDescriptors;
    /** Current setting for each interface. */
    uint8_t *pInterfaces;
    /** Current configuration number (0 -> device is not configured). */
    uint8_t cfgnum;
    /** Indicates if remote wake up has been enabled by the host. */
    uint8_t isRemoteWakeUpEnabled;
#if defined(CHIP_USB_OTGHS)
    /** Features supported by OTG */
    uint8_t otg_features_supported;
#endif
} USBDDriver;

/*------------------------------------------------------------------------------
 *         Exported functions
 *------------------------------------------------------------------------------*/

extern void USBDDriver_Initialize(
    USBDDriver *pDriver,
    const USBDDriverDescriptors *pDescriptors,
    uint8_t *pInterfaces);

extern void USBDDriver_RequestHandler(
    USBDDriver *pDriver,
    const USBGenericRequest *pRequest);

extern uint8_t USBDDriver_IsRemoteWakeUpEnabled(const USBDDriver *pDriver);

#if defined(CHIP_USB_OTGHS)
extern uint8_t USBDDriver_returnOTGFeatures(void);
extern void USBDDriver_clearOTGFeatures(void);
#endif

/**@}*/

#endif /*#ifndef USBDDRIVER_H*/

