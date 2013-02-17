/**
 * @file pal_boardtypes.h
 *
 * @brief PAL board types for AT91SAM3S4B
 *
 * This header file contains board types based on AT91SAM3S4B
 *
 * $Id: pal_boardtypes.h 24055 2010-11-01 11:27:04Z mahendran.p $
 */
/**
 * @author
 * Atmel Corporation: http://www.atmel.com
 * Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef PAL_BOARDTYPES_H
#define PAL_BOARDTYPES_H


/*
 * Boards for AT86RF212
 */

//usb board w/ sub ghz trx rev 0
#define DERF_USB_13E00              (0x01)

//usb board with micro sd card rev 0
#define DERF_USB_13E04              (0x02)

/*
 * Boards for AT86RF231
 */

//usb board w/ 2.4Ghz trx rev 0
#define DERF_USB_23E00              (0x03)
//usb board w/ 2.4Ghz trx rev 1
#define DERF_USB_23E01             (0x04)
//usb board w/ 2.4Ghz trx and micro sd card rev 0
#define DERF_USB_23E04              (0x05)

#endif  /* PAL_BOARDTYPES_H */
/* EOF */
