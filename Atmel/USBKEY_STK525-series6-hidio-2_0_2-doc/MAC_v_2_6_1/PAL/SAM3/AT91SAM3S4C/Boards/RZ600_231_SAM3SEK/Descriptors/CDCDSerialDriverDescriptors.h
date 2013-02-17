 /* @file CDCDSerialDriverDescriptors.h
 *
 * @brief USB Descriptor related code
 *
 * This file implements USB Descriptor
 * and reception functions.
 *
 * $Id: CDCDSerialDriverDescriptors.h 24193 2010-11-15 07:31:12Z mahendran.p $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */


#ifndef CDCDSERIALDRIVERDESCRIPTORS_H
#define CDCDSERIALDRIVERDESCRIPTORS_H

/* === Includes =========================================================*/
#include <usb/device/core/USBDDriverDescriptors.h>

/* === Macros =========================================================*/
/* Data OUT endpoint number */
#define CDCDSerialDriverDescriptors_DATAOUT             1
/* Data IN endpoint number */
#define CDCDSerialDriverDescriptors_DATAIN              2
/* Notification endpoint number */
#define CDCDSerialDriverDescriptors_NOTIFICATION        3

/* === Globals =========================================================*/
/* List of descriptors for a CDC device serial driver */
extern USBDDriverDescriptors cdcdSerialDriverDescriptors;

#endif /*#ifndef CDCDDRIVERDESCRIPTORS_H */

