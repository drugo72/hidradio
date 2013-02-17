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
 * $Id: USBEndpointDescriptor.h 23837 2010-10-13 13:45:05Z mahendran.p $
 */

/**
 * \file
 *
 * \section Purpose
 *
 *    Definition of a class for handling USB endpoint descriptors.
 *
 * \section Usage
 *
 *    -# Declare USBEndpointDescriptor instance as a part of the
 *       configuration descriptors of a USB device.
 *    -# To get useful information (field values) from the defined USB device
 *       endpoint descriptor (to configure hardware for endpoints, etc), use
 *       - USBEndpointDescriptor_GetNumber
 *       - USBEndpointDescriptor_GetDirection
 *       - USBEndpointDescriptor_GetType
 *       - USBEndpointDescriptor_GetMaxPacketSize
 */

#ifndef USBENDPOINTDESCRIPTOR_H
#define USBENDPOINTDESCRIPTOR_H
/** \addtogroup usb_descriptor
 *@{
 */
/** \addtogroup usb_ep_desc USB Endpoint Descriptor
 * @{
 */

/*----------------------------------------------------------------------------
 *         Definitions
 *----------------------------------------------------------------------------*/

/** \addtogroup usb_ep_define USB Endpoint definitions
 *  @{
 * This section lists definitions and macro for endpoint descriptors.
 * - \ref usb_ep_dir USB Endpoint directions
 *    - \ref USBEndpointDescriptor_OUT
 *    - \ref USBEndpointDescriptor_IN
 *
 * - \ref usb_ep_type USB Endpoint types
 *    - \ref USBEndpointDescriptor_CONTROL
 *    - \ref USBEndpointDescriptor_ISOCHRONOUS
 *    - \ref USBEndpointDescriptor_BULK
 *    - \ref USBEndpointDescriptor_INTERRUPT
 *
 *  - \ref usb_ep_size USB Endpoint maximun sizes
 *     - \ref USBEndpointDescriptor_MAXCTRLSIZE_FS
 *     - \ref USBEndpointDescriptor_MAXCTRLSIZE_HS
 *     - \ref USBEndpointDescriptor_MAXBULKSIZE_FS
 *     - \ref USBEndpointDescriptor_MAXBULKSIZE_HS
 *     - \ref USBEndpointDescriptor_MAXINTERRUPTSIZE_FS
 *     - \ref USBEndpointDescriptor_MAXINTERRUPTSIZE_HS
 *     - \ref USBEndpointDescriptor_MAXISOCHRONOUSSIZE_FS
 *     - \ref USBEndpointDescriptor_MAXISOCHRONOUSSIZE_HS
 *
 *  - \ref usb_ep_addr USB Endpoint address define
 *     - \ref USBEndpointDescriptor_ADDRESS
 */

/** \addtogroup usb_ep_dir USB Endpoint directions
 *      @{
 *  This section lists definitions of USB endpoint directions.
 *  - USBEndpointDescriptor_OUT
 *  - USBEndpointDescriptor_IN
 */
/**  Endpoint receives data from the host. */
#define USBEndpointDescriptor_OUT           0
/**  Endpoint sends data to the host. */
#define USBEndpointDescriptor_IN            1
/**     @}*/

/** \addtogroup usb_ep_type USB Endpoint types
 *      @{
 *  This section lists definitions of USB endpoint types.
 *  - \ref USBEndpointDescriptor_CONTROL
 *  - \ref USBEndpointDescriptor_ISOCHRONOUS
 *  - \ref USBEndpointDescriptor_BULK
 *  - \ref USBEndpointDescriptor_INTERRUPT
 */
/**  Control endpoint type. */
#define USBEndpointDescriptor_CONTROL       0
/**  Isochronous endpoint type. */
#define USBEndpointDescriptor_ISOCHRONOUS   1
/**  Bulk endpoint type. */
#define USBEndpointDescriptor_BULK          2
/**  Interrupt endpoint type. */
#define USBEndpointDescriptor_INTERRUPT     3
/**     @}*/

/** \addtogroup usb_ep_size USB Endpoint maximun sizes
 *      @{
 *  This section lists definitions of USB endpoint maximun sizes.
 *  - \ref USBEndpointDescriptor_MAXCTRLSIZE_FS
 *  - \ref USBEndpointDescriptor_MAXCTRLSIZE_HS
 *  - \ref USBEndpointDescriptor_MAXBULKSIZE_FS
 *  - \ref USBEndpointDescriptor_MAXBULKSIZE_HS
 *  - \ref USBEndpointDescriptor_MAXINTERRUPTSIZE_FS
 *  - \ref USBEndpointDescriptor_MAXINTERRUPTSIZE_HS
 *  - \ref USBEndpointDescriptor_MAXISOCHRONOUSSIZE_FS
 *  - \ref USBEndpointDescriptor_MAXISOCHRONOUSSIZE_HS
 */
/**  Maximum size for a full-speed control endpoint. */
#define USBEndpointDescriptor_MAXCTRLSIZE_FS                64
/**  Maximum size for a high-speed control endpoint. */
#define USBEndpointDescriptor_MAXCTRLSIZE_HS                64
/**  Maximum size for a full-speed bulk endpoint. */
#define USBEndpointDescriptor_MAXBULKSIZE_FS                64
/**  Maximum size for a high-speed bulk endpoint. */
#define USBEndpointDescriptor_MAXBULKSIZE_HS                512
/**  Maximum size for a full-speed interrupt endpoint. */
#define USBEndpointDescriptor_MAXINTERRUPTSIZE_FS           64
/**  Maximum size for a high-speed interrupt endpoint. */
#define USBEndpointDescriptor_MAXINTERRUPTSIZE_HS           1024
/**  Maximum size for a full-speed isochronous endpoint. */
#define USBEndpointDescriptor_MAXISOCHRONOUSSIZE_FS         1023
/**  Maximum size for a high-speed isochronous endpoint. */
#define USBEndpointDescriptor_MAXISOCHRONOUSSIZE_HS         1024
/**     @}*/

/** \addtogroup usb_ep_addr USB Endpoint address define
 *      @{
 *  This section lists macro for USB endpoint address definition.
 *  - \ref USBEndpointDescriptor_ADDRESS
 */
/**
 *  Calculates the address of an endpoint given its number and direction
 *  \param direction USB endpoint direction definition
 *  \param number USB endpoint number
 *  \return The value used to set the endpoint descriptor based on input number
 *          and direction
 */
#define USBEndpointDescriptor_ADDRESS(direction, number) \
    (((direction & 0x01) << 7) | (number & 0xF))
/**     @}*/

/*----------------------------------------------------------------------------
 *         Types
 *----------------------------------------------------------------------------*/

#ifdef __ICCARM__          /* IAR */
#pragma pack(1)            /* IAR */
#define __attribute__(...) /* IAR */
#endif                     /* IAR */

/**
 * \typedef USBEndpointDescriptor
 * \brief USB standard endpoint descriptor structure.
 */
typedef struct _USBEndpointDescriptor {

   /**  Size of the descriptor in bytes. */
   unsigned char bLength;
   /**  Descriptor type (\ref USBGenericDescriptor_ENDPOINT). */
   unsigned char bDescriptorType;
   /**  Address and direction of the endpoint. */
   unsigned char bEndpointAddress;
   /**  Endpoint type and additional characteristics
        (for isochronous endpoints). */
   unsigned char bmAttributes;
   /**  Maximum packet size (in bytes) of the endpoint. */
   unsigned short wMaxPacketSize;
   /**  Polling rate of the endpoint. */
   unsigned char bInterval;

} __attribute__ ((packed)) USBEndpointDescriptor; /* GCC*/

#ifdef __ICCARM__          /* IAR */
#pragma pack()             /* IAR */
#endif                     /* IAR */

/*----------------------------------------------------------------------------
 *         Exported functions
 *----------------------------------------------------------------------------*/

extern unsigned char USBEndpointDescriptor_GetNumber(
    const USBEndpointDescriptor *endpoint);

extern unsigned char USBEndpointDescriptor_GetDirection(
    const USBEndpointDescriptor *endpoint);

extern unsigned char USBEndpointDescriptor_GetType(
    const USBEndpointDescriptor *endpoint);

extern unsigned short USBEndpointDescriptor_GetMaxPacketSize(
    const USBEndpointDescriptor *endpoint);

extern unsigned char USBEndpointDescriptor_GetInterval(
    const USBEndpointDescriptor *endpoint);

/**  @}*/
/** @}*/
/**@}*/
#endif /*#ifndef USBENDPOINTDESCRIPTOR_H*/

