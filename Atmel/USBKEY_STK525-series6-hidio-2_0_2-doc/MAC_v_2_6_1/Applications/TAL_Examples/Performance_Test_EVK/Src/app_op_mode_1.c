/**
 * @file app_op_mode_1.c
 *
 * @brief  application for opmode 1 of TAL Example - Performance_Test_EVK
 *
 * $Id: app_op_mode_1.c 24877 2011-01-13 06:13:13Z felix.varghese $
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
#include "bmm.h"
#include "ieee_const.h"
#include "app_config.h"
#include "sio_handler.h"
#include "app_pairing.h"
#include "app_frame_format.h"
#include "app_op_mode_1.h"

/* === TYPES =============================================================== */
/* Enumeration used for application in operation mode 1 */
typedef enum
{
    APP_INIT,
    TX_OFF,
    TX_ON,
    STOP_TX,
    START_TX
}app_state_op_mode_1_e;

/* === MACROS ============================================================== */
#define DATA_PKT             0x00

#define APP_SEND_TEST_PKT_INTERVAL_IN_MICRO_SEC 250000
/* === PROTOTYPES ========================================================== */
/* Functions for Operation Mode 1 */
static void app_task_op_mode_1(void);
static void app_tx_frame_done_cb_for_op_mode_1(retval_t status, frame_info_t *frame);
static void app_rx_frame_cb_for_op_mode_1(frame_info_t *frame);
static void app_sent_frames_on_air_for_mode_1(void);

static void tx_app_op_mode_1_timer_handler_cb(void *parameter);
static void app_trail_led(void);
static void app_print_statistics(void);
/* === GLOBALS ============================================================= */
static app_state_op_mode_1_e app_state_op_mode_1;
static uint8_t seq_num;
static bool transmitting = false;

static uint32_t num_of_frames_send;
static uint32_t num_of_frames_rec;
/* === IMPLEMENTATION ====================================================== */
/**
 * @brief Initialise the application in operation mode 1
 *        Operation Mode 1 is used for range test
 *
 * @param mode_startup starts Operation Mode 1 in this particular mode
 *        Operation Mode 1 can be started as OP_MODE_1_KEY_PRESS and
 *        OP_MODE_1_RX_FRAME
 */
void app_task_op_mode_1_init(startup_mode_e mode_startup)
{
    /* Init the call backs and app_task */
    app_task = &app_task_op_mode_1;
    app_tx_frame_done_cb = &app_tx_frame_done_cb_for_op_mode_1;
    app_rx_frame_cb = &app_rx_frame_cb_for_op_mode_1;

    /* Switch off all the LEDs */
    app_set_led_value(LED_ALL_OFF);
#if (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
    if(board == SENSOR_TERM_BOARD)
    #endif
#endif
    {
        printf("\r\n Pairing Success");
    }

    switch(mode_startup)
    {
        case OP_MODE_1_KEY_PRESS:
        {
            /* Start the node as Transmitter */
            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf("\r\n I am TX");
                }
            app_state_op_mode_1 = APP_INIT;
            break;
        }

        case OP_MODE_1_RX_FRAME:
        {
            /* Start the node as Receiver */
            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf("\r\n I am RX");
                }
            app_state_op_mode_1 = TX_OFF;
            break;
        }
        default:
            break;
    }
}

/**
 * @brief Application task for Operation Mode 1
 *
 * This function
 * - Implements the Operation Mode 1 state machine.
 * - On start up as transmitter application will start the transmission
 * - On user inputs through the key press Starts or Stops transmission
 */
static void app_task_op_mode_1(void)
{
    uint16_t char_received;
    uint8_t key_press;

    /* Check for any key press */
    key_press = app_debounce_button();

    /* See if any character received on the Uart */
    char_received = sio_getchar_nowait();

    if(char_received != 0xFFFF)
    {
        /* if any character is received then print the test statistics */
        app_print_statistics();
    }

    switch(app_state_op_mode_1)
    {
       case APP_INIT:
       {
            /* Initiates transmission */
            /* This state is added for flexibility and for future pre test enhancements */
            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf("\r\n Starting TX in OP MODE 1");
                }
            app_state_op_mode_1 = START_TX;
            break;
       }

       case START_TX:
       {
            /* Start Transmission of test packets */
            app_sent_frames_on_air_for_mode_1();
            pal_timer_start(APP_TIMER_TO_TX,
                            APP_SEND_TEST_PKT_INTERVAL_IN_MICRO_SEC,
                            TIMEOUT_RELATIVE,
                            (FUNC_PTR)tx_app_op_mode_1_timer_handler_cb,
                            NULL);
            app_state_op_mode_1 = TX_ON;
            break;
        }

        case TX_ON:
        {
            if(key_press != 0)
            {
                /* Stop Transmission of test packets as key press detected */
                #if (PAL_GENERIC_TYPE == MEGA_RF)
                    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                    if(board == SENSOR_TERM_BOARD)
                    #endif
                #endif
                    {
                        printf("\r\n Stoping TX in OP MODE 1");
                    }
                pal_timer_stop(APP_TIMER_TO_TX);
                app_state_op_mode_1 = TX_OFF;
            }
            break;
        }

        case TX_OFF:
        {
            if(key_press != 0)
            {
                /* Start Transmission of test packets as key press detected */
                #if (PAL_GENERIC_TYPE == MEGA_RF)
                    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                    if(board == SENSOR_TERM_BOARD)
                    #endif
                #endif
                    {
                        printf("\r\n Starting TX in OP MODE 1");
                    }
                pal_timer_start(APP_TIMER_TO_TX,
                                APP_SEND_TEST_PKT_INTERVAL_IN_MICRO_SEC,
                                TIMEOUT_RELATIVE,
                                (FUNC_PTR)tx_app_op_mode_1_timer_handler_cb,
                                NULL);
                app_state_op_mode_1 = START_TX;
            }
            break;
        }
        default:
            break;
    }

}

/**
 * @brief Callback that is called if data has been received by trx.
 *
 * @param mac_frame_info Pointer to received data structure
 */
static void app_rx_frame_cb_for_op_mode_1(frame_info_t *mac_frame_info)
{
    app_payload_t *msg;
    if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                      sizeof(general_pkt_u)) +
                                                      sizeof(data_pkt_op_mode_1_t))))
    {
        /* Point to the message : 1 =>size is first byte and 2=>FCS*/
        msg = (app_payload_t *)(mac_frame_info->mpdu + 1 + FRAME_OVERHEAD - 2);
        if((msg->u8CmdID) == DATA_PKT)
        {
            /* Correct packet received so trail the LED on board */
            app_trail_led();
            /* Increment the frames received count */
            num_of_frames_rec++;
        }
    }

}

/**
 * @brief Callback that is called once tx is done.
 *
 * @param status    Status of the transmission procedure
 * @param frame     Pointer to the transmitted frame structure
 */
static void app_tx_frame_done_cb_for_op_mode_1(retval_t status, frame_info_t *frame)
{
    status = status;
    frame = frame;
    /* After transmission is completed, allow next transmission. */
    transmitting = false;
}

/**
 * @brief Timer handler for supporting Operation Mode 1.
 *
 * @param parameter pass parameters to timer handler
 */
static void tx_app_op_mode_1_timer_handler_cb(void *parameter)
{
    /* Make compiler happy */
    parameter = parameter;

    switch(app_state_op_mode_1)
    {
        case STOP_TX:
        {
            /* If the state is to stop the transmission then stop the timer */
            pal_timer_stop(APP_TIMER_TO_TX);
            app_state_op_mode_1 = STOP_TX;
            break;
        }

        case TX_ON:
        {
            /* Sent the test frame on Air */
            app_sent_frames_on_air_for_mode_1();
            /* Restart the timer */
            pal_timer_start(APP_TIMER_TO_TX,
                            APP_SEND_TEST_PKT_INTERVAL_IN_MICRO_SEC,
                            TIMEOUT_RELATIVE,
                            (FUNC_PTR)tx_app_op_mode_1_timer_handler_cb,
                            NULL);
            app_state_op_mode_1 = TX_ON;
            break;
        }
        default:
            break;
    }
}

/**
 * @brief Send Operation Mode 1 test frames to the paired device. This is a unicast
 *        with source address as paired address
 */
static void app_sent_frames_on_air_for_mode_1(void)
{
    uint8_t i;

    uint8_t payload_length;
    uint8_t *data_tmp;

    app_payload_t msg;
    data_pkt_op_mode_1_t *data;

    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        /* Trail the LED indicating transmission */
        app_trail_led();

        /* Increment the frames transmitted counter */
        num_of_frames_send++;

        /* Form the payload for the test frames */
        msg.u8CmdID = DATA_PKT;
        seq_num++;
        msg.u8SeqNum = seq_num;
        data = (data_pkt_op_mode_1_t *)msg.payload;
        data->pkt_count = num_of_frames_send;

        payload_length = ((sizeof(app_payload_t) -
                          sizeof(general_pkt_u)) +
                          sizeof(data_pkt_op_mode_1_t));

        /* Get length of current frame. */
        frame_length = (FRAME_OVERHEAD + payload_length);

        /* Set payload pointer. */
        frame_ptr = temp_frame_ptr =
                    (uint8_t *)tx_frame_info +
                    LARGE_BUFFER_SIZE -
                    payload_length - 2; /* Add 2 octets for FCS. */

        /*
         * Payload is stored to the end of the buffer avoiding payload copying by TAL.
         */
        data_tmp = (uint8_t *) &msg;
        for (i = 0; i < payload_length; i++)
        {
            /* Load the payload */
            *temp_frame_ptr++ = *(data_tmp + i);
        }

        /* Source Address */
        temp_value_16 = src_addr_pairing;
        frame_ptr -= 2;
        convert_16_bit_to_byte_array(temp_value_16, frame_ptr);

        /* Source PAN-Id */
#if (DST_PAN_ID == SRC_PAN_ID)
        /* No source PAN-Id included, but FCF updated. */
        fcf |= FCF_PAN_ID_COMPRESSION;
#else
        frame_ptr -= 2;
        temp_value_16 = SRC_PAN_ID;
        convert_16_bit_to_byte_array(temp_value_16, frame_ptr);
#endif

        /* Destination Address */
        temp_value_16 = dest_addr_pairing;
        frame_ptr -= 2;
        convert_16_bit_to_byte_array(temp_value_16, frame_ptr);

        /* Destination PAN-Id */
        temp_value_16 = DST_PAN_ID;
        frame_ptr -= 2;
        convert_16_bit_to_byte_array(temp_value_16, frame_ptr);

        /* Set DSN. */
        frame_ptr--;
        *frame_ptr = seq_num;

        /* Set the FCF. */
        fcf |= FCF_FRAMETYPE_DATA | FCF_SET_SOURCE_ADDR_MODE(FCF_SHORT_ADDR) |
                                    FCF_SET_DEST_ADDR_MODE(FCF_SHORT_ADDR);
        /* Unicast with ACK_REQUEST */
        fcf |= FCF_ACK_REQUEST;
        frame_ptr -= 2;
        convert_16_bit_to_byte_array(fcf, frame_ptr);

        /* First element shall be length of PHY frame. */
        frame_ptr--;
        *frame_ptr = frame_length;

        /* Finished building of frame. */
        tx_frame_info->mpdu = frame_ptr;

        /* Transmit the frame */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);
    }
}

/**
 * @brief Trail LED for Operation Mode 1 on the available LEDs.
 */
static void app_trail_led(void)
{
    static uint8_t led_set;

    /* Set LED accordingly */
    app_set_led_value(1<<led_set);
    led_set++;
    led_set > 3 ? led_set = 0: led_set;
}

/**
 * @brief Print the statistics for Operation Mode 1 on the UART Terminal.
 */
static void app_print_statistics(void)
{
    #if (PAL_GENERIC_TYPE == MEGA_RF)
        #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
        if(board == SENSOR_TERM_BOARD)
        #endif
    #endif
        {
            /* Print number of frames send */
            printf("\r\n No. of Frames TX: %" PRIu32, num_of_frames_send );
            /* Print number of frames received */
            printf("\r\n No. of Frames RX: %" PRIu32, num_of_frames_rec);
        }
}

/* EOF */
