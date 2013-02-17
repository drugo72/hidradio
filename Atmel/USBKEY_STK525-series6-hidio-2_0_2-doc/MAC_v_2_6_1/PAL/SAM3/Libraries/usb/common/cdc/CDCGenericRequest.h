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
 * $Id: CDCGenericRequest.h 23837 2010-10-13 13:45:05Z mahendran.p $
 */

/**
 \unit

 !!!Purpose

 Various definitions used for characterizing USB CDC requests.
*/

#ifndef CDCGENERICREQUEST_H
#define CDCGENERICREQUEST_H
/** \addtogroup usb_cdc
 *@{
 */

/*----------------------------------------------------------------------------
 *         Definitions
 *----------------------------------------------------------------------------*/

/** \addtogroup usb_cdc_request USB CDC Request Codes

 * This section lists USB CDC Request Codes.
 * - \ref CDCGenericRequest_SETLINECODING
 * - \ref CDCGenericRequest_GETLINECODING
 * - \ref CDCGenericRequest_SETCONTROLLINESTATE
 */

/** SetLineCoding request code. */
#define CDCGenericRequest_SETLINECODING             0x20
/** GetLineCoding request code. */
#define CDCGenericRequest_GETLINECODING             0x21
/** SetControlLineState request code. */
#define CDCGenericRequest_SETCONTROLLINESTATE       0x22
/** @}*/

/**@}*/
#endif /*#ifndef CDCGENERICREQUEST_H*/
