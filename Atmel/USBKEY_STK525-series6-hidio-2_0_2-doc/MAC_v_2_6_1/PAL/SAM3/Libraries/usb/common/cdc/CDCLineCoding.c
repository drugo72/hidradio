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
 * $Id: CDCLineCoding.c 24055 2010-11-01 11:27:04Z mahendran.p $
 */

/*
    Title: CDCLineCoding

    About: Purpose
        Implementation of the CDCLineCoding class.
*/

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "CDCLineCoding.h"

/*----------------------------------------------------------------------------
 *         Exported functions
 *----------------------------------------------------------------------------*/

/**
 *  Initializes the bitrate, number of stop bits, parity checking and
 *  number of data bits of a CDCLineCoding object.
 *  \param lineCoding Pointer to a CDCLineCoding instance.
 *  \param bitrate Bitrate of the virtual COM connection.
 *  \param stopbits Number of stop bits
 *                  (\ref usb_cdc_stop CDC LineCoding StopBits).
 *  \param parity Parity check type
 *                  (\ref usb_cdc_parity CDC LineCoding ParityChecking).
 *  \param databits Number of data bits.
 */
void CDCLineCoding_Initialize(CDCLineCoding *lineCoding,
                              unsigned int bitrate,
                              unsigned char stopbits,
                              unsigned char parity,
                              unsigned char databits)
{
    lineCoding->dwDTERate = bitrate;
    lineCoding->bCharFormat = stopbits;
    lineCoding->bParityType = parity;
    lineCoding->bDataBits = databits;
}

/**@}*/

