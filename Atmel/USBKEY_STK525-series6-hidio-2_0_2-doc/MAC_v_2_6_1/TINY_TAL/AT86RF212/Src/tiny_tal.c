/**
 * @file tiny_tal.c
 *
 * @brief This file implements the TAL state machine and provides general
 * functionality used by the TAL.
 *
 * $Id: tiny_tal.c 25353 2011-02-03 12:18:04Z sschneid $
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
#include <stdlib.h>
#include "pal.h"
#include "return_val.h"
#include "tiny_tal.h"
#include "ieee_const.h"
#include "tiny_tal_pib.h"
#include "tiny_tal_irq_handler.h"
#include "at86rf212.h"
#include "stack_config.h"
#include "tiny_tal_rx.h"
#include "tiny_tal_tx.h"
#include "tiny_tal_constants.h"
#include "tiny_tal_internal.h"

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */

/*
 * Value used for checking proper locking of PLL during switch from
 * TRX_PFF to PLL_ON.
 */
#define PLL_LOCK_ATTEMPTS           (3)

/* === GLOBALS ============================================================= */

/*
 * TAL PIBs
 */
/**
 * The maximum number of back-offs the CSMA-CA algorithm will attempt
 * before declaring a CSMA_CA failure.
 */
uint8_t tal_pib_MaxCSMABackoffs;

/**
 * The minimum value of the backoff exponent BE in the CSMA-CA algorithm.
 */
uint8_t tal_pib_MinBE;

/**
 * 16-bit PAN ID.
 */
uint16_t tal_pib_PANId;

/**
 * Node's 16-bit short address.
 */
uint16_t tal_pib_ShortAddress;

/**
 * Node's 64-bit (IEEE) address.
 */
uint64_t tal_pib_IeeeAddress;

/**
 * Current RF channel to be used for all transmissions and receptions.
 */
uint8_t tal_pib_CurrentChannel;

/**
 * Current channel page; supported: page 0, 2, and 5; high-data rate mode: 16, 17, 18, 19
 */
uint8_t tal_pib_CurrentPage;

/**
 * Maximum number of symbols in a frame:
 * = phySHRDuration + ceiling([aMaxPHYPacketSize + 1] x phySymbolsPerOctet)
 */
uint16_t tal_pib_MaxFrameDuration;

/**
 * Duration of the synchronization header (SHR) in symbols for the current PHY.
 */
uint8_t tal_pib_SHRDuration;

/**
 * Number of symbols per octet for the current PHY.
 */
uint8_t tal_pib_SymbolsPerOctet;

/**
 * The maximum value of the backoff exponent BE in the CSMA-CA algorithm.
 */
uint8_t tal_pib_MaxBE;

/**
 * The maximum number of retries allowed after a transmission failure.
 */
uint8_t tal_pib_MaxFrameRetries;

/**
 * Default value of transmit power of transceiver
 * using IEEE defined format of phyTransmitPower.
 */
uint8_t tal_pib_TransmitPower;

/**
 * CCA Mode
 */
uint8_t tal_pib_CCAMode;

/**
 * Indicates if the node is a PAN coordinator or not.
 */
bool tal_pib_PrivatePanCoordinator;


/*
 * Global TAL variables
 * These variables are only to be used by the TAL internally.
 */

/**
 * Current state of the TAL state machine.
 */
tal_state_t tal_state;

/**
 * Current state of the transceiver.
 */
tal_trx_status_t tal_trx_status;

/* Flag indicating awake end irq at successful wake-up from sleep. */
volatile bool tal_awake_end_flag;

/* === PROTOTYPES ========================================================== */

static void switch_pll_on(void);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief TAL task handling
 *
 * This function
 * - Checks and allocates the receive buffer.
 * - Processes the TAL incoming frame queue.
 * - Implements the TAL state machine.
 */
void tal_task(void)
{
    /*
     * If the transceiver has received a frame and it has been placed
     * into the queue of the TAL, the frame needs to be processed further.
     */
    if (tiny_tal_frame_rx)
    {
        process_incoming_frame();
        tiny_tal_frame_rx = false;
    }

    /* Handle the TAL state machines */
    switch (tal_state)
    {
        case TAL_IDLE:
        /* Do nothing, but fall through... */
        case TAL_TX_AUTO:
            /* Wait until state is changed to TAL_TX_DONE inside tx end ISR */
            break;

        case TAL_TX_DONE:
            tx_done_handling();    // see tal_tx.c
            break;

        default:
            ASSERT("tal_state is not handled" == 0);
            break;
    }
} /* tal_task() */



/**
 * @brief Sets transceiver state
 *
 * @param trx_cmd needs to be one of the trx commands
 *
 * @return current trx state
 */
tal_trx_status_t set_trx_state(trx_cmd_t trx_cmd)
{
    if (tal_trx_status == TRX_SLEEP)
    {
        PAL_SLP_TR_LOW();

        /* Poll wake-up interrupt flag until set within ISR. */
        while (!tal_awake_end_flag);

        /* Reset wake-up interrupt flag. */
        tal_awake_end_flag = false;
        /* Clear existing interrupts */
        pal_trx_reg_read(RG_IRQ_STATUS);
        /* Re-install default IRQ handler for main interrupt. */
        pal_trx_irq_init((FUNC_PTR)trx_irq_handler_cb);
        /* Re-enable TRX_END interrupt */
        pal_trx_reg_write(RG_IRQ_MASK, TRX_IRQ_DEFAULT);

#ifdef ANTENNA_DIVERSITY
        /* Enable antenna diversity. */
        pal_trx_bit_write(SR_ANT_EXT_SW_EN, ANT_EXT_SW_SWITCH_ENABLE);
#endif

        if ((trx_cmd == CMD_TRX_OFF) || (trx_cmd == CMD_FORCE_TRX_OFF))
        {
            tal_trx_status = TRX_OFF;
            return TRX_OFF;
        }
    }

    tal_trx_status = (tal_trx_status_t)pal_trx_bit_read(SR_TRX_STATUS);

    switch (trx_cmd)    /* requested state */
    {
        case CMD_SLEEP:
            pal_trx_reg_write(RG_TRX_STATE, CMD_FORCE_TRX_OFF);
#ifdef ANTENNA_DIVERSITY
            /* Disable antenna diversity: sets pulls */
            pal_trx_bit_write(SR_ANT_EXT_SW_EN, ANT_EXT_SW_SWITCH_DISABLE);
#endif
            {
                uint16_t rand_value;

                /*
                 * Init the SEED value of the CSMA backoff algorithm.
                 */
                rand_value = (uint16_t)rand();
                pal_trx_reg_write(RG_CSMA_SEED_0, (uint8_t)rand_value);
                pal_trx_bit_write(SR_CSMA_SEED_1, (uint8_t)(rand_value >> 8));
            }

            /* Clear existing interrupts */
            pal_trx_reg_read(RG_IRQ_STATUS);
            /*
             * Install new IRQ handler for main interrupt for wake-up from sleep later.
             * The original IQR handler will be re-installed once the radio is awake
             * again.
             */
            pal_trx_irq_init((FUNC_PTR)trx_irq_awake_handler_cb);
            /*
             * Enable Awake_end interrupt.
             * This is used for save wake-up from sleep later.
             */
            pal_trx_bit_write(SR_IRQ_MASK, TRX_IRQ_CCA_ED_DONE);

            PAL_WAIT_1_US();
            PAL_SLP_TR_HIGH();
            pal_timer_delay(TRX_OFF_TO_SLEEP_TIME_CLKM_CYCLES);
            tal_trx_status = TRX_SLEEP;
            return TRX_SLEEP;   /* transceiver register cannot be read during TRX_SLEEP */

        case CMD_TRX_OFF:
            switch (tal_trx_status)
            {
                case TRX_OFF:
                    break;

                default:
                    pal_trx_reg_write(RG_TRX_STATE, CMD_TRX_OFF);
                    PAL_WAIT_1_US();
                    break;
            }
            break;

        case CMD_FORCE_TRX_OFF:
            switch (tal_trx_status)
            {
                case TRX_OFF:
                    break;

                default:
                    pal_trx_reg_write(RG_TRX_STATE, CMD_FORCE_TRX_OFF);
                    PAL_WAIT_1_US();
                    break;
            }
            break;

        case CMD_PLL_ON:
            switch (tal_trx_status)
            {
                case PLL_ON:
                    break;

                case TRX_OFF:
                    switch_pll_on();
                    break;

                case RX_ON:
                case RX_AACK_ON:
                case TX_ARET_ON:
                    pal_trx_reg_write(RG_TRX_STATE, CMD_PLL_ON);
                    PAL_WAIT_1_US();
                    break;

                case BUSY_RX:
                case BUSY_TX:
                case BUSY_RX_AACK:
                case BUSY_TX_ARET:
                    /* do nothing if trx is busy */
                    break;

                default:
                    ASSERT("state transition not handled" == 0);
                    break;
            }
            break;

        case CMD_FORCE_PLL_ON:
            switch (tal_trx_status)
            {
                case TRX_OFF:
                    switch_pll_on();
                    break;

                case PLL_ON:
                    break;

                default:
                    pal_trx_reg_write(RG_TRX_STATE, CMD_FORCE_PLL_ON);
                    break;
            }
            break;

        case CMD_RX_ON:
            switch (tal_trx_status)
            {
                case RX_ON:
                    break;

                case PLL_ON:
                case RX_AACK_ON:
                case TX_ARET_ON:
                    pal_trx_reg_write(RG_TRX_STATE, CMD_RX_ON);
                    PAL_WAIT_1_US();
                    break;

                case TRX_OFF:
                    switch_pll_on();
                    pal_trx_reg_write(RG_TRX_STATE, CMD_RX_ON);
                    PAL_WAIT_1_US();
                    break;

                case BUSY_RX:
                case BUSY_TX:
                case BUSY_RX_AACK:
                case BUSY_TX_ARET:
                    /* do nothing if trx is busy */
                    break;

                default:
                    ASSERT("state transition not handled" == 0);
                    break;
            }
            break;

        case CMD_RX_AACK_ON:
            switch (tal_trx_status)
            {
                case RX_AACK_ON:
                    break;

                case TX_ARET_ON:
                case PLL_ON:
                    pal_trx_reg_write(RG_TRX_STATE, CMD_RX_AACK_ON);
                    PAL_WAIT_1_US();
                    break;

                case TRX_OFF:
                    switch_pll_on();// state change from TRX_OFF to RX_AACK_ON can be done directly, too
                    pal_trx_reg_write(RG_TRX_STATE, CMD_RX_AACK_ON);
                    PAL_WAIT_1_US();
                    break;

                case RX_ON:
                    pal_trx_reg_write(RG_TRX_STATE, CMD_PLL_ON);
                    PAL_WAIT_1_US();
                    // check if state change could be applied
                    tal_trx_status = (tal_trx_status_t)pal_trx_bit_read(SR_TRX_STATUS);
                    if (tal_trx_status != PLL_ON)
                    {
                        return tal_trx_status;
                    }
                    pal_trx_reg_write(RG_TRX_STATE, CMD_RX_AACK_ON);
                    PAL_WAIT_1_US();
                    break;

                case BUSY_RX:
                case BUSY_TX:
                case BUSY_RX_AACK:
                case BUSY_TX_ARET:
                    /* do nothing if trx is busy */
                    break;

                default:
                    ASSERT("state transition not handled" == 0);
                    break;
            }
            break;

        case CMD_TX_ARET_ON:
            switch (tal_trx_status)
            {
                case TX_ARET_ON:
                    break;

                case PLL_ON:
                    pal_trx_reg_write(RG_TRX_STATE, CMD_TX_ARET_ON);
                    PAL_WAIT_1_US();
                    break;

                case RX_ON:
                case RX_AACK_ON:
                    pal_trx_reg_write(RG_TRX_STATE, CMD_PLL_ON);
                    PAL_WAIT_1_US();
                    // check if state change could be applied
                    tal_trx_status = (tal_trx_status_t)pal_trx_bit_read(SR_TRX_STATUS);
                    if (tal_trx_status != PLL_ON)
                    {
                        return tal_trx_status;
                    }
                    pal_trx_reg_write(RG_TRX_STATE, CMD_TX_ARET_ON);
                    PAL_WAIT_1_US();
                    break;

                case TRX_OFF:
                    switch_pll_on();// state change from TRX_OFF to TX_ARET_ON can be done directly, too
                    pal_trx_reg_write(RG_TRX_STATE, CMD_TX_ARET_ON);
                    PAL_WAIT_1_US();
                    break;

                case BUSY_RX:
                case BUSY_TX:
                case BUSY_RX_AACK:
                case BUSY_TX_ARET:
                    /* do nothing if trx is busy */
                    break;

                default:
                    ASSERT("state transition not handled" == 0);
                    break;
            }
            break;

        default:
            /* CMD_NOP, CMD_TX_START */
            ASSERT("trx command not handled" == 0);
            break;
    }

    do
    {
        tal_trx_status = (tal_trx_status_t)pal_trx_bit_read(SR_TRX_STATUS);
    } while (tal_trx_status == STATE_TRANSITION_IN_PROGRESS);

    return tal_trx_status;
} /* set_trx_state() */



/**
 * @brief Switches the PLL on
 */
static void switch_pll_on(void)
{
    trx_irq_reason_t irq_status;
    uint8_t poll_counter = 0;

    /* Check if trx is in TRX_OFF; only from PLL_ON the following procedure is applicable */
    if (pal_trx_bit_read(SR_TRX_STATUS) != TRX_OFF)
    {
        ASSERT("Switch PLL_ON failed, because trx is not in TRX_OFF" == 0);
        return;
    }

    pal_trx_reg_read(RG_IRQ_STATUS);    /* clear PLL lock bit */
    /* Switch PLL on */
    pal_trx_reg_write(RG_TRX_STATE, CMD_PLL_ON);

    /* Check if PLL has been locked. */
    do
    {
        irq_status = (trx_irq_reason_t)pal_trx_reg_read(RG_IRQ_STATUS);

        if (irq_status & TRX_IRQ_PLL_LOCK)
        {
            return;  // PLL is locked now
        }

        /* Wait a time interval of typical value for timer TR4. */
        pal_timer_delay(TRX_OFF_TO_PLL_ON_TIME_US);

        poll_counter++;
    } while (poll_counter < PLL_LOCK_ATTEMPTS);

#if (DEBUG > 0)
    ASSERT("PLL switch failed" == 0);
    pal_alert();
#endif
}


/* EOF */
