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
 * $Id: USBDeviceDescriptor.h 24055 2010-11-01 11:27:04Z mahendran.p $
 */

/**
 * \file
 * \section Purpose
 *
 *    Class for manipulating USB device descriptors.
 *
 * \section Usage
 *
 *    - Declare a USBDeviceDescriptor instance as the device descriptor of a
 *      USB device.
 */

#ifndef USBDEVICEDESCRIPTOR_H
#define USBDEVICEDESCRIPTOR_H
/** \addtogroup usb_descriptor
 *@{
 */
/** \addtogroup usb_device_descriptor USB Device Descriptor
 *  @{
 */

/*---------------------------------------------------------------------------
 *      Headers
 *---------------------------------------------------------------------------*/

/* These headers were introduced in C99 by working group
 * ISO/IEC JTC1/SC22/WG14.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "pal_config.h"

/*---------------------------------------------------------------------------
 *         Definitions
 *---------------------------------------------------------------------------*/

/** \addtogroup usb_release_number USB release numbers
 *      @{
 *  This section lists the codes of USB release numbers.
 *  - \ref USBDeviceDescriptor_USB2_00
 */

/**  The device supports USB 2.00. */
#define USBDeviceDescriptor_USB1_10         0x0110
#define USBDeviceDescriptor_USB2_00         0x0200
/**     @}*/

/*---------------------------------------------------------------------------
 *         Types
 *---------------------------------------------------------------------------*/

#ifdef __ICCARM__          // IAR
#pragma pack(1)            // IAR
#define __attribute__(...) // IAR
#endif                     // IAR

/**
 * \typedef USBDeviceDescriptor
 * \brief USB standard device descriptor structure.
 */
typedef struct _USBDeviceDescriptor {

   /**  Size of this descriptor in bytes. */
   uint8_t bLength;
   /**  Descriptor type (USBGenericDescriptor_DEVICE). */
   uint8_t bDescriptorType;
   /**  USB specification release number in BCD format. */
   uint16_t bcdUSB;
   /**  Device class code. */
   uint8_t bDeviceClass;
   /** Device subclass code. */
   uint8_t bDeviceSubClass;
   /** Device protocol code. */
   uint8_t bDeviceProtocol;
   /**  Maximum packet size of endpoint 0 (in bytes). */
   uint8_t bMaxPacketSize0;
   /** Vendor ID. */
   uint16_t idVendor;
   /** Product ID. */
   uint16_t idProduct;
   /** Device release number in BCD format. */
   uint16_t bcdDevice;
   /** Index of the manufacturer string descriptor. */
   uint8_t iManufacturer;
   /** Index of the product string descriptor. */
   uint8_t iProduct;
   /** Index of the serial number string descriptor. */
   uint8_t iSerialNumber;
   /** Number of possible configurations for the device. */
   uint8_t bNumConfigurations;

} __attribute__ ((packed)) USBDeviceDescriptor; // GCC

#if defined(CHIP_USB_OTGHS)
/**
 * \typedef USBOtgDescriptor
 * \brief USB On-The-Go descriptor struct.
 */
typedef struct _USBOtgDescriptor {

   /** Size of this descriptor in bytes. */
   uint8_t bLength;
   /** Descriptor type (USBGenericDescriptor_OTG). */
   uint8_t bDescriptorType;
   /** Attribute Fields D7…2: Reserved D1: HNP support D0: SRP support */
   uint8_t bmAttributes;

} __attribute__ ((packed)) USBOtgDescriptor; // GCC
#endif

#ifdef __ICCARM__          // IAR
#pragma pack()             // IAR
#endif                     // IAR

/** @}*/
/**@}*/
#endif //#ifndef USBDEVICEDESCRIPTOR_H

