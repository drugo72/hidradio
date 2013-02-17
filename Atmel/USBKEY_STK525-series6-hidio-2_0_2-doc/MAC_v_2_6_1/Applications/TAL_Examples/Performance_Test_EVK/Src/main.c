/**
 * @file main.c
 *
 * @brief  Main of TAL Example - Performance_Test
 *
 * $Id: main.c 24877 2011-01-13 06:13:13Z felix.varghese $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include "pal.h"
#include "tal.h"
#include "tfa.h"
#include "app_config.h"
#include "ieee_const.h"
#include "bmm.h"
#include "sio_handler.h"
#include "app_frame_format.h"
#include "app_pairing.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */
#define COUNT_FOR_VALID_KEY_PRESS 20
/* === PROTOTYPES ========================================================== */
static void configure_pibs(void);

/* Functions for Startup */
static void app_task_startup(void);
static void app_rx_frame_cb_for_startup(frame_info_t *frame);

/* === GLOBALS ============================================================= */
frame_info_t *tx_frame_info;

void (*app_task)(void);
void (*app_tx_frame_done_cb)(retval_t status, frame_info_t *frame);
void (*app_rx_frame_cb)(frame_info_t *frame);

uint8_t storage_buffer[LARGE_BUFFER_SIZE];

#if (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
        board_t board ;
    #endif
#endif

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Main function of the Performance_Test_EVK application
 */
int main(void)
{

#if (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
    board = board_identify();
    #endif
#endif

    /* Initialize the TAL layer */
    if (tal_init() != MAC_SUCCESS)
    {
        /* something went wrong during initialization */
        pal_alert();
    }

    /* Calibrate MCU's RC oscillator */
    pal_calibrate_rc_osc();

    /* Initialize LEDs */
    pal_led_init();

    /* Initialize switches */
    pal_button_init();

    /*
     * The stack is initialized above, hence the global interrupts are enabled
     * here.
     */
    pal_global_irq_enable();

    /* Initialize the serial interface used for communication with terminal program */
    if (pal_sio_init(SIO_CHANNEL) != MAC_SUCCESS)
    {
        /* something went wrong during initialization */
        pal_alert();
    }

#if ((!defined __ICCAVR__) && (!defined __ICCARM__))
    fdevopen(_sio_putchar, _sio_getchar);
#endif

    /* Init tx frame info structure value that do not change during program execution */
    tx_frame_info = (frame_info_t *)storage_buffer;

    /* Initialize startup application */
    app_task_startup_init();

    /* Endless while loop */
    while (1)
    {
        pal_task(); /* Handle platform specific tasks, like serial interface */
        tal_task(); /* Handle transceiver specific tasks */
        app_task(); /* Application task */
    }
}

/**
 * @brief Callback that is called if data has been received by trx.
 *
 * @param frame    Pointer to received data structure
 */
void tal_rx_frame_cb(frame_info_t *frame)
{
    if(NULL != app_rx_frame_cb)
    {
        /* Call application specific callbacks */
        app_rx_frame_cb(frame);
    }

    /* free buffer that was used for frame reception */
    bmm_buffer_free((buffer_t *)(frame->buffer_header));
}

/**
 * @brief Callback that is called once tx is done.
 *
 * @param status    Status of the transmission procedure
 * @param frame     Pointer to the transmitted frame structure
 */
void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
{
    if(NULL != app_tx_frame_done_cb)
    {
        /* Call application specific callbacks */
        app_tx_frame_done_cb(status, frame);
    }
}

/**
 * @brief Application task
 */
void app_task_startup_init()
{
    /* Initialise the application with app_task_startup */
    app_task = &app_task_startup;
    app_rx_frame_cb = &app_rx_frame_cb_for_startup;

    /* Configure the TAL PIBs; e.g. set short address */
    configure_pibs();

    /* Leave the radio in RX mode in default channel */
    tal_rx_enable(PHY_RX_ON);

    /* reset the pairing status to UNPAIRED */
    app_pairing_reset();

    /* indicating application is started */
    app_set_led_value(LED_STARTUP);
}

/**
 * @brief Application task startup
 */
static void app_task_startup(void)
{
    uint16_t char_received;
    uint8_t key_press;

    /* See if any character received on the UART */
    char_received = sio_getchar_nowait();

    /* Check for any key press */
    key_press = app_debounce_button();

    if(key_press != 0)
    {
        app_task_pairing_init(OP_MODE_1_KEY_PRESS);
    }
    else if(char_received != 0xFFFF)
    {
        app_task_pairing_init(OP_MODE_2_CHAR_ON_UART);
    }
}

/**
 * @brief Callback that is called if data has been received by trx in startup mode.
 *
 * @param frame    Pointer to received data structure
 */
static void app_rx_frame_cb_for_startup(frame_info_t *frame)
{
    app_task_pairing_init(OP_MODE_UNKNOWN);
    if(app_rx_frame_cb != NULL)
    {
        app_rx_frame_cb(frame);
    }
}

/**
 * @brief Configure the TAL PIbs
 */
static void configure_pibs(void)
{
    uint16_t temp_value_16;
    uint8_t temp_value_8;

    /* Set Default address. */
    temp_value_16 = DEFAULT_ADDR;
    tal_pib_set(macShortAddress, (pib_value_t *)&temp_value_16);

    /* Set PAN ID. */
    temp_value_16 = SRC_PAN_ID;
    tal_pib_set(macPANId, (pib_value_t *)&temp_value_16);

    /* Set channel. */
    temp_value_8 = (uint8_t)DEFAULT_CHANNEL;
    tal_pib_set(phyCurrentChannel, (pib_value_t *)&temp_value_8);
}

/**
 * @brief Display of a decimal value on the available LEDs.
 *
 * @param v value to be displayed decimal on the LEDs.
 *
 * @ingroup apiPalAppDemo
 */
void app_set_led_value(uint8_t v)
{
#if (PAL_GENERIC_TYPE == XMEGA)
    uint8_t led_set = v;
#elif (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
    uint8_t led_set = 0;
    switch(board)
    {
        case SENSOR_TERM_BOARD:
        {
            led_set |= (v & LED_MODE_1)? 1:0;
            led_set |= (v & LED_MODE_2)? 2:0;
            break;
        }

        case PLAIN:
        {
           led_set |= (v & LED_MODE_1)? 1:0;
           led_set |= (v & LED_STARTUP)? 2:0;
           led_set |= (v & LED_MODE_2)? 4:0;
           break;
        }
    }
    #endif
#endif

    pal_led(LED_0, (led_set & 1) ? LED_ON : LED_OFF );
    pal_led(LED_1, (led_set & 2) ? LED_ON : LED_OFF );

#if (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
    if(board == PLAIN)
    #endif
#endif
    {
        pal_led(LED_2, (led_set & 4) ? LED_ON : LED_OFF );
    }
}

/**
 * @brief Button debounce routine.
 *
 * Helper function for debouncing the transmit button.
 * @return 1 if a button event is detected, 0 otherwise.
 *
 * @ingroup apiPalAppDemo
 */
uint8_t app_debounce_button(void)
{

/* To support any new boards add the start button configuration here */
#if (PAL_GENERIC_TYPE == XMEGA)
    button_id_t app_start_button = BUTTON_0;
#elif (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
        button_id_t app_start_button = BUTTON_0;
    #else
    	button_id_t app_start_button = BUTTON_0;
    #endif
#endif

    uint8_t ret = 0;
    static uint8_t keycnt;
    button_state_t temp = pal_button_read(app_start_button);

    if(BUTTON_PRESSED == temp)
    {
        if (keycnt != COUNT_FOR_VALID_KEY_PRESS)
        {
            keycnt++;
        }
    }
    else if(BUTTON_OFF == temp && (keycnt == COUNT_FOR_VALID_KEY_PRESS))
    {
        ret = 1;
        keycnt = 0;
    }
    else
    {
        keycnt = 0;
    }
    return ret;
}


/* EOF */
