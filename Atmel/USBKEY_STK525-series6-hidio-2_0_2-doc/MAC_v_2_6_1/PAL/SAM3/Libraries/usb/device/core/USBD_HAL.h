/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
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
 * $Id: USBD_HAL.h 23837 2010-10-13 13:45:05Z mahendran.p $
 */

#ifndef USBD_HAL_H
#define USBD_HAL_H

/**
 *  \file
 *
 *  This file defines functions for USB Device Hardware Access Level.
 */

/** \addtogroup usbd_hal
 *@{*/

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

/* Introduced in C99 by working group ISO/IEC JTC1/SC22/WG14. */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "USBD.h"
#include <usb/common/core/USBGenericDescriptor.h>
#include <usb/common/core/USBDeviceDescriptor.h>
#include <usb/common/core/USBEndpointDescriptor.h>
#include <usb/common/core/USBGenericRequest.h>

/*----------------------------------------------------------------------------
 *        Consts
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *        Macros
 *----------------------------------------------------------------------------*/

/** Get bitmap for an endpoint */
#define bmEP(bEP)   (1 << (bEP))

/*----------------------------------------------------------------------------
 *        Types
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *        Exported functoins
 *----------------------------------------------------------------------------*/

extern void USBD_HAL_Init(void);
extern void USBD_HAL_Connect(void);
extern void USBD_HAL_Disconnect(void);

extern void USBD_HAL_RemoteWakeUp(void);
extern void USBD_HAL_SetConfiguration(uint8_t cfgnum);
extern void USBD_HAL_SetAddress(uint8_t address);
extern uint8_t USBD_HAL_IsHighSpeed(void);

extern void USBD_HAL_Suspend(void);
extern void USBD_HAL_Activate(void);

extern void USBD_HAL_ResetEPs(uint32_t bmEPs,uint8_t bStatus);
extern uint8_t USBD_HAL_ConfigureEP(const USBEndpointDescriptor * pDescriptor);

extern uint8_t USBD_HAL_SetTransferCallback(uint8_t bEP,
                                            TransferCallback fCallback,
                                            void * pCbData);
extern uint8_t USBD_HAL_Write(uint8_t bEndpoint,
                              const void * pData,
                              uint32_t dLength);
extern uint8_t USBD_HAL_Read(uint8_t bEndpoint,
                             void * pData,
                             uint32_t dLength);
extern uint8_t USBD_HAL_Stall(uint8_t bEP);
extern uint8_t USBD_HAL_Halt(uint8_t bEndpoint,uint8_t ctl);
/**@}*/

#endif // #define USBD_HAL_H