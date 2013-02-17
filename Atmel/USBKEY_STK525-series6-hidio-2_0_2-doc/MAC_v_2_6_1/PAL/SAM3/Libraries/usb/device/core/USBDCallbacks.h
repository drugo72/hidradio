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
 * $Id: USBDCallbacks.h 23837 2010-10-13 13:45:05Z mahendran.p $
 */

/**
 * \file
 *
 * \section Purpose
 *
 *    Definitions of callbacks used by the USBD API to notify the user
 *    application of incoming events. These functions are declared as 'weak',
 *    so they can be re-implemented elsewhere in the application in a
 *    transparent way.
 */

#ifndef USBDCALLBACKS_H
#define USBDCALLBACKS_H

/** \addtogroup usbd_interface
 *@{
 */
 /*----------------------------------------------------------------------------
 *         Headers
 *----------------------------------------------------------------------------*/

#include <usb/common/core/USBGenericRequest.h>

/*----------------------------------------------------------------------------
 *         Defines
 *----------------------------------------------------------------------------*/

/* Define WEAK attribute */
#ifndef WEAK
#if defined   ( __CC_ARM   )
    #define WEAK __attribute__ ((weak))
#elif defined ( __ICCARM__ )
    #define WEAK __weak
#elif defined (  __GNUC__  )
    #define WEAK __attribute__ ((weak))
#endif
#endif

/*----------------------------------------------------------------------------
 *         Exported functions
 *----------------------------------------------------------------------------*/

extern WEAK void USBDCallbacks_Initialized(void);

extern WEAK void USBDCallbacks_Reset(void);

extern WEAK void USBDCallbacks_Suspended(void);

extern WEAK void USBDCallbacks_Resumed(void);

extern WEAK void USBDCallbacks_RequestReceived(const USBGenericRequest *request);

/**@}*/

#endif /*#ifndef USBDCALLBACKS_H*/

