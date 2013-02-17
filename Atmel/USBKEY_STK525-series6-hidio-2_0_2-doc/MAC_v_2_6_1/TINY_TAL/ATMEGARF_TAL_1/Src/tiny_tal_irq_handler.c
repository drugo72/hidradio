/**
 * @file tiny_tal_irq_handler.c
 *
 * @brief This file handles the interrupts generated by the transceiver.
 *
 * $Id: tiny_tal_irq_handler.c 24722 2011-01-05 15:35:53Z sschneid $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "pal.h"
#include "return_val.h"
#include "tiny_tal.h"
#include "ieee_const.h"
#include "stack_config.h"
#include "tiny_tal_irq_handler.h"
#include "tiny_tal_rx.h"
#include "atmega128rfa1.h"
#include "tiny_tal_internal.h"
#include "tiny_tal_constants.h"
#include "tiny_tal_tx.h"

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */


/* === GLOBALS ============================================================= */


/* === PROTOTYPES ========================================================== */


/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Transceiver interrupt handler
 *
 * This function handles the transceiver generated interrupts for RX end.
 */
void trx_rx_end_handler_cb(void)
{
    ENTER_CRITICAL_REGION();

    if (!tiny_tal_frame_rx)
    {
        /* Handle rx interrupt. */
        handle_received_frame_irq();    // see tal_rx.c
    }

    LEAVE_CRITICAL_REGION();
}/* trx_rx_end_handler_cb() */


/**
 * @brief Transceiver interrupt handler
 *
 * This function handles the transceiver generated interrupts for TX end.
 */
void trx_tx_end_handler_cb(void)
{
    ENTER_CRITICAL_REGION();

    /* Get the result and push it to the queue. */
    handle_tx_end_irq();            // see tal_tx.c

    LEAVE_CRITICAL_REGION();
}/* trx_tx_end_handler_cb() */


/**
 * @brief Transceiver interrupt handler for awake end IRQ
 *
 * This function handles the transceiver awake end interrupt.
 */
void trx_awake_handler_cb(void)
{
    /* Set the wake-up flag. */
    tal_awake_end_flag = true;
}
/* EOF */
