/**
 * @file pal_irq.c
 *
 * @brief PAL IRQ functionality
 *
 * This file contains functions to initialize, enable, disable and install
 * handler for the transceiver interrupts. It also contains functions to enable,
 * disable and get the status of global interrupt
 *
 * $Id: pal_irq.c 24716 2011-01-05 14:08:04Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */

#include <stdint.h>
#include "pal_types.h"
#include "return_val.h"
#include "pal.h"
#include "pal_irq.h"
#include "pal_boardtypes.h"

#if (BOARD_TYPE == REB_5_0_STK600)
/* === Types ============================================================== */

/**
 * This is a typedef of the function which is called from the transceiver ISR
 */
typedef void (*irq_handler_t)(void);

/* === Globals ============================================================= */

/*
 * Function pointers to store the callback function of
 * the transceiver interrupt
 */
static irq_handler_t irq_hdl_trx;


/*
 * Function pointers to store the callback function of
 * the timestamp interrupt
 */
#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
static irq_handler_t irq_hdl_trx_tstamp;
#endif

/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

/**
 * @brief Initializes the transceiver interrupts
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver interrupts
 *
 * @param trx_irq_num Transceiver interrupt line to be initialized
 * @param trx_irq_cb Callback function for the given transceiver
 * interrupt
 */
void pal_trx_irq_init(FUNC_PTR trx_irq_cb)
{
     irq_hdl_trx = (irq_handler_t)trx_irq_cb;
    /*
     * Set the handler function.
     * The handler is set before enabling the interrupt to prepare for spurious
     * interrupts, that can pop up the moment they are enabled
     */
     volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[EXT_INT >> 5];
     uint32_t mask = 1 << (EXT_INT & 0x1F);
     gpio_port->gfers = mask;
     gpio_port->imr0s = mask;
     gpio_port->imr1c = mask;
     gpio_port->ifrc = mask;

}

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
/**
 * @brief Initializes the transceiver timestamp interrupt
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver timestamp interrupt
 *
 * @param trx_irq_cb Callback function for the transceiver timestamp
 * interrupt
 */
void pal_trx_irq_init_tstamp(FUNC_PTR trx_irq_cb)
{
        irq_hdl_trx_tstamp = (irq_handler_t)trx_irq_cb;
        /* Rising edge on DIG2 pin used to trigger IRQ */
        TC_CH2.ier = AVR32_TC_IER0_LDRAS_MASK ;
}
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */


#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
/**
 * @brief Function to read the timestamp
 */
void read_timestamp()
{
    /*Clearing the timestamp interrupt*/
    pal_trx_irq_flag_clr_tstamp();
    irq_hdl_trx_tstamp();
}
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */


/**
 * @brief ISR for transceiver's main interrupt
 */
#pragma handler=EXT_INT_ISR_GROUP,EXT_INT_ISR_PRIORITY
__interrupt void ext_int_isr(void)
{
    /*Clearing the RF interrupt*/
    pal_trx_irq_flag_clr();
    /*Calling the interrupt routines*/
    irq_hdl_trx();
}

#endif
/* EOF */
