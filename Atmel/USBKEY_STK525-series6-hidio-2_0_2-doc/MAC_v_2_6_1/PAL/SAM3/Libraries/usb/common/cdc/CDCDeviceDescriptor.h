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
 * $Id: CDCDeviceDescriptor.h 23837 2010-10-13 13:45:05Z mahendran.p $
 */


/**\file
 *
 * \section Purpose
 *
 * Definition of several constants used when declaring USB CDC device
 * descriptors.
*/

#ifndef CDCDEVICEDESCRIPTOR_H
#define CDCDEVICEDESCRIPTOR_H
/** \addtogroup usb_cdc
 *@{
 */

/*----------------------------------------------------------------------------
 *         Definitions
 *----------------------------------------------------------------------------*/

/** \addtogroup usb_cdc_descriptor USB CDC Device Descriptor Values
 * @{
 * This section lists the values for CDC Device Descriptor.
 * - \ref CDCDeviceDescriptor_CLASS
 * - \ref CDCDeviceDescriptor_SUBCLASS
 * - \ref CDCDeviceDescriptor_PROTOCOL
 */
/** Device class code when using the CDC class. */
#define CDCDeviceDescriptor_CLASS               0x02
/** Device subclass code when using the CDC class. */
#define CDCDeviceDescriptor_SUBCLASS            0x00
/** Device protocol code when using the CDC class. */
#define CDCDeviceDescriptor_PROTOCOL            0x00
/** @}*/

/**@}*/
#endif /*#ifndef CDCDEVICEDESCRIPTOR_H*/
