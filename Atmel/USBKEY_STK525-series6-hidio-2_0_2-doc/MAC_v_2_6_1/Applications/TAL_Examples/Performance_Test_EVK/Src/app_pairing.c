/**
 * @file app_pairing.c
 *
 * @brief  pairing functinality for TAL Example - Performance_Test_EVK
 *
 * $Id: app_pairing.c 24877 2011-01-13 06:13:13Z felix.varghese $
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
#include "app_pairing.h"
#include "app_frame_format.h"
#include "app_op_mode_1.h"
#include "app_op_mode_2.h"

/* === TYPES =============================================================== */
/* Different states in pairing */
typedef enum
{
    UNPAIRED,
    PAIRING_REQ,
    PAIRING_RSP,
    PAIRING_RSP_REC,
    WAIT_FOR_PAIR_CONF,
    PAIRING_SUCCESS,
    PAIRING_ABORT
}app_state_pairing_t;

/* === MACROS ============================================================== */
/* Commands involved in pairing */
#define PAIRING_REQUEST      0x01
#define PAIRING_RESPONSE     0x02
#define PAIRING_CONFIRM      0x03

/* Timer values for the timer used in pairing */
#define PAIR_RESPONSE_TIMEOUT_IN_MICRO_SEC           200000
#define PAIR_REQUEST_SEND_INTERVAL_IN_MICRO_SEC      50000

#define MAX_PAIR_REQ_COUNT  0xFF
/* === PROTOTYPES ========================================================== */
/* Functions for Pairing */
static void app_task_pairing(void);
static void app_tx_frame_done_cb_for_pairing(retval_t , frame_info_t *);
static void app_rx_frame_cb_for_pairing(frame_info_t *frame);

static void send_pairing_req(void);
static void send_pairing_rsp(uint16_t SrcAddr, uint8_t SeqNum);
static void send_pairing_conf(void);

/* Timer handlers */
static void pair_timeout_timer_handler_cb(void *parameter);
static void tx_app_pairing_timer_handler_cb(void *parameter);

/* === GLOBALS ============================================================= */
static uint8_t seq_num;
static bool transmitting = false;
static startup_mode_e mode_startup;
static app_state_pairing_t app_state_pairing;

uint16_t src_addr_pairing;
uint16_t dest_addr_pairing;
/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Application task to start pairing
 *
 * @param mode starts the pairing in this particular mode
 */
void app_task_pairing_init(startup_mode_e mode)
{
    /* Startup init mode as per the startup event ocurrance */
    mode_startup = mode;

    /* random number initialsed for the sequence number */
    seq_num = rand();

    /* Startup modes for this application */
    switch(mode_startup)
    {
        case OP_MODE_1_KEY_PRESS:
        {
            app_set_led_value(LED_MODE_1); /* LED D1*/
            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf("\r\n Key press detected : OP MODE 1");
                }
            break;
        }

        case OP_MODE_UNKNOWN:
        {
            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf("\r\n Frames Rec on air: ");
                }
            break;
        }

        case OP_MODE_2_CHAR_ON_UART:
        {
            app_set_led_value(LED_MODE_2); /* LED D3*/
            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf("\r\n Character received : OP MODE 2");
                    printf("\r\n Press ENTER to abort pairing");
                }
            break;
        }
        default:
        {
            return;
        }
    }
    /* Init the call backs and app_task */
    app_task = &app_task_pairing;
    app_tx_frame_done_cb = &app_tx_frame_done_cb_for_pairing;
    app_rx_frame_cb = &app_rx_frame_cb_for_pairing;
}

/**
 * @brief Application task handling pairing
 *
 * This function
 * - Implements the pairing state machine.
 */
static void app_task_pairing(void)
{
    if(sio_getchar_nowait() == '\r')
    {
        /* Abort pairing if Enter is received on UART and in MODE2 where pairing
         * was initated by char received on UART
         */
        if(((app_state_pairing == UNPAIRED)||(app_state_pairing == PAIRING_REQ))
           &&(mode_startup == OP_MODE_2_CHAR_ON_UART))
        {
            app_state_pairing = PAIRING_ABORT;
        }
    }

    switch(app_state_pairing)
    {
        case UNPAIRED:
        {
            do
            {
                /* assign a random address */
                dest_addr_pairing = rand();
            }while((dest_addr_pairing == 0) ||
                   (dest_addr_pairing == BROADCAST));

            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf("\r\n Pairing Initiated");
                }

            /* Send pairing request */
            send_pairing_req();

            /* Timer to send the next pairing request */
            pal_timer_start(APP_TIMER_TO_TX,
                        PAIR_REQUEST_SEND_INTERVAL_IN_MICRO_SEC,
                        TIMEOUT_RELATIVE,
                        (FUNC_PTR)tx_app_pairing_timer_handler_cb,
                        NULL);
            app_state_pairing = PAIRING_REQ;
            break;
        }

        case PAIRING_SUCCESS:
        {
            /* pairing completed so stop the timer */
            pal_timer_stop(APP_TIMER_TO_TX);
            switch(mode_startup)
            {
                case OP_MODE_1_KEY_PRESS:
                case OP_MODE_1_RX_FRAME:
                {
                    /* Start app mode 1 */
                    app_task_op_mode_1_init(mode_startup);
                    break;
                }

                case OP_MODE_2_RX_FRAME:
                  app_set_led_value(LED_MODE_2); /* LED D3*/
                  /* intentional fall through */
                case OP_MODE_2_CHAR_ON_UART:
                {
                    /* Start app mode 2 */
                    app_task_op_mode_2_init(mode_startup, true);
                    break;
                }
                default:
                    break;
            }
            break;
        }

        case PAIRING_ABORT:
        {
            /* pairing aborted so stop the timer */
            pal_timer_stop(APP_TIMER_TO_TX);
            app_set_led_value(LED_MODE_2); /* LED D3*/
            app_task_op_mode_2_init(mode_startup, false);
            break;
        }
        default:
            break;
    }
}

/**
 * @brief Timer handler for supporting pairing
 *
 * @param parameter pass parameters to timer handler
 */
static void tx_app_pairing_timer_handler_cb(void *parameter)
{
    static uint8_t pairing_req_count;

    /* Stop Compiler from crying */
    parameter = parameter;

    switch(app_state_pairing)
    {
        case PAIRING_REQ:
        {
            if(pairing_req_count == MAX_PAIR_REQ_COUNT)
            {
                pairing_req_count = 0;
                #if (PAL_GENERIC_TYPE == MEGA_RF)
                    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                    if(board == SENSOR_TERM_BOARD)
                    #endif
                #endif
                    {
                        printf("\r\n Pairing Failed");
                    }
                app_task_startup_init();
            }
            else
            {
                #if (PAL_GENERIC_TYPE == MEGA_RF)
                    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                    if(board == SENSOR_TERM_BOARD)
                    #endif
                #endif
                    {
                        printf(".");
                    }
                send_pairing_req();
                pairing_req_count++;
                pal_timer_start(APP_TIMER_TO_TX,
                                PAIR_REQUEST_SEND_INTERVAL_IN_MICRO_SEC,
                                TIMEOUT_RELATIVE,
                                (FUNC_PTR)tx_app_pairing_timer_handler_cb,
                                NULL);
                app_state_pairing = PAIRING_REQ;
            }
            break;
        }
        default:
            break;
    }
}

/**
 * @brief Timer handler to handle pairing timeouts
 *
 * @param parameter pass parameters to timer handler
 */
static void pair_timeout_timer_handler_cb(void *parameter)
{
    /* Stop Compiler from crying */
    parameter = parameter;

    switch(app_state_pairing)
    {
        case UNPAIRED:
        case PAIRING_RSP:
        case WAIT_FOR_PAIR_CONF:
        {
            pal_timer_stop(APP_TIMER_TO_TX);
            app_task_startup_init();
            break;
        }
        default:
            break;
    }
}

/**
 * @brief Reset pairing application
 */
void app_pairing_reset(void)
{
    app_state_pairing = UNPAIRED;
}

/**
 * @brief Send pairing request. This is broadcasted with source address
 *        as DEFAULT_ADDR
 */
static void send_pairing_req(void)
{
    uint8_t i;
    uint8_t payload_length;
    uint8_t *data_tmp;
    app_payload_t msg;
    pairing_req_t *data;

    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        msg.u8CmdID = PAIRING_REQUEST;
        seq_num++;
        msg.u8SeqNum = seq_num;
        data = (pairing_req_t *)msg.payload;

        if(mode_startup == OP_MODE_1_KEY_PRESS)
        {
            data->u8OpMode = OPERATION_MODE_1;
        }
        else if(mode_startup == OP_MODE_2_CHAR_ON_UART)
        {
            data->u8OpMode = OPERATION_MODE_2;
        }

        /* The nodes taking part in pairing will set its address to this value
         * and sent back the pairing rsp
         */
        data->u16nwk_addr = dest_addr_pairing;


        payload_length = ((sizeof(app_payload_t) -
                           sizeof(general_pkt_u)) +
                           sizeof(pairing_req_t));

        /* Get length of current frame. */
        frame_length = (FRAME_OVERHEAD + payload_length);

        /* Set payload pointer. */
        frame_ptr = temp_frame_ptr =
                    (uint8_t *)tx_frame_info +
                    LARGE_BUFFER_SIZE -
                    payload_length - 2; /* Add 2 octets for FCS. */

        /*
         * Payload is stored to the end of the buffer avoiding payload
         * copying by TAL.
         */
        data_tmp = (uint8_t *) &msg;
        for (i = 0; i < payload_length; i++)
        {
            *temp_frame_ptr++ = *(data_tmp + i);
        }

        /* Source Address */
        temp_value_16 = DEFAULT_ADDR;
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
        temp_value_16 = DST_SHORT_ADDR;
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
        frame_ptr -= 2;
        convert_16_bit_to_byte_array(fcf, frame_ptr);

        /* First element shall be length of PHY frame. */
        frame_ptr--;
        *frame_ptr = frame_length;

        /* Finished building of frame. */
        tx_frame_info->mpdu = frame_ptr;

        /* transmit the frame */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);
    }
}

/**
 * @brief Send pairing response. This is a unicast send to the node which
 *        had earlier send the pairing request
 *
 * @param SrcAddr The address which had been sent as payload of pairing request
 *                it becomes the source address of node which sent pairing rsp
 * @param SeqNum Sequence number of the Pairing request frame
 */
static void send_pairing_rsp(uint16_t SrcAddr, uint8_t SeqNum)
{
    uint8_t i;
    uint8_t payload_length;
    uint8_t *data_tmp;
    app_payload_t msg;
    pairing_rsp_t *data;

    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        msg.u8CmdID = PAIRING_RESPONSE;
        msg.u8SeqNum = SeqNum;
        data = (pairing_rsp_t *)msg.payload;

        if(mode_startup == OP_MODE_1_RX_FRAME)
        {
            data->u8OpMode = OPERATION_MODE_1;
        }
        else if(mode_startup == OP_MODE_2_RX_FRAME)
        {
            data->u8OpMode = OPERATION_MODE_2;
        }

        /* Issues an address for the receptor. If this node gets paired then
         * receptor changes its short address to this value
         */
        data->u16nwk_addr = dest_addr_pairing;

        payload_length = ((sizeof(app_payload_t) -
                           sizeof(general_pkt_u)) +
                           sizeof(pairing_rsp_t));

        /* Get length of current frame. */
        frame_length = (FRAME_OVERHEAD + payload_length);

        /* Set payload pointer. */
        frame_ptr = temp_frame_ptr =
                    (uint8_t *)tx_frame_info +
                    LARGE_BUFFER_SIZE -
                    payload_length - 2; /* Add 2 octets for FCS. */

        /*
         * Payload is stored to the end of the buffer avoiding
         * payload copying by TAL.
         */
        data_tmp = (uint8_t *) &msg;
        for (i = 0; i < payload_length; i++)
        {
            *temp_frame_ptr++ = *(data_tmp + i);
        }

        /* Source Address */
        temp_value_16 = SrcAddr;
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
        temp_value_16 = DEFAULT_ADDR;
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
 * @brief Send the pairing confirmation confirming node which has been paired
 *        This frame is sent as a unicast to node paired. All other nodes which
 *        took part in the pairing times out and resets the status as UNPAIRED
 */
static void send_pairing_conf(void)
{
    uint8_t i;
    uint8_t payload_length;
    uint8_t *data_tmp;
    app_payload_t msg;
    pairing_conf_t *data;

    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        msg.u8CmdID = PAIRING_CONFIRM;
        seq_num++;
        msg.u8SeqNum = seq_num;
        data = (pairing_conf_t *)msg.payload;

        /* The payload it nodes address itself. The receptor of frame compares
           the address to the address it has issued and if it matches then
           declares itself as paired */
        data->u16nwk_addr = src_addr_pairing;

        payload_length = ((sizeof(app_payload_t) -
                           sizeof(general_pkt_u)) +
                           sizeof(pairing_conf_t));

        /* Get length of current frame. */
        frame_length = (FRAME_OVERHEAD + payload_length);

        /* Set payload pointer. */
        frame_ptr = temp_frame_ptr =
                    (uint8_t *)tx_frame_info +
                    LARGE_BUFFER_SIZE -
                    payload_length - 2; /* Add 2 octets for FCS. */

        /*
         * Payload is stored to the end of the buffer avoiding
         * payload copying by TAL.
         */
        data_tmp = (uint8_t *) &msg;
        for (i = 0; i < payload_length; i++)
        {
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
        fcf |= FCF_ACK_REQUEST;

        frame_ptr -= 2;
        convert_16_bit_to_byte_array(fcf, frame_ptr);

        /* First element shall be length of PHY frame. */
        frame_ptr--;
        *frame_ptr = frame_length;

        /* Finished building of frame. */
        tx_frame_info->mpdu = frame_ptr;

        /* Transmits the frame */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);
    }
}

/**
 * @brief Callback that is called if data has been received by trx.
 *
 * @param mac_frame_info    Pointer to received data structure
 */
static void app_rx_frame_cb_for_pairing(frame_info_t *mac_frame_info)
{
    app_payload_t *msg;

    mac_frame_info = mac_frame_info;

    switch(app_state_pairing)
    {
        case UNPAIRED:
        {
            if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD +
                                           ((sizeof(app_payload_t) -
                                           sizeof(general_pkt_u)) +
                                           sizeof(pairing_req_t))))
            {
                /* Point to the message : 1 =>size is first byte and 2=>FCS*/
                msg = (app_payload_t *)(mac_frame_info->mpdu + 1 +
                                        FRAME_OVERHEAD - 2);

                if((msg->u8CmdID) == PAIRING_REQUEST)
                {
                    if((msg->payload[0].pairing_req_data.u8OpMode == OPERATION_MODE_1))
                    {
                        #if (PAL_GENERIC_TYPE == MEGA_RF)
                            #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                            if(board == SENSOR_TERM_BOARD)
                            #endif
                        #endif
                            {
                                printf("OP MODE 1");
                            }
                        mode_startup = OP_MODE_1_RX_FRAME;
                    }
                    else
                    {
                        #if (PAL_GENERIC_TYPE == MEGA_RF)
                            #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                            if(board == SENSOR_TERM_BOARD)
                            #endif
                        #endif
                            {
                                printf("OP MODE 2");
                            }
                        mode_startup = OP_MODE_2_RX_FRAME;
                    }

                    src_addr_pairing = (msg->payload[0].pairing_req_data.u16nwk_addr);
                    do
                    {
                        dest_addr_pairing = rand();
                    }while((dest_addr_pairing == 0) || (dest_addr_pairing == 0xFFFF));

                    send_pairing_rsp(src_addr_pairing, msg->u8SeqNum);

                    pal_timer_start(APP_TIMER_TO_TX,
                                    PAIR_RESPONSE_TIMEOUT_IN_MICRO_SEC,
                                    TIMEOUT_RELATIVE,
                                    (FUNC_PTR)pair_timeout_timer_handler_cb,
                                    NULL);

                    app_state_pairing = PAIRING_RSP;
                }
                else
                {
                    #if (PAL_GENERIC_TYPE == MEGA_RF)
                        #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                        if(board == SENSOR_TERM_BOARD)
                        #endif
                    #endif
                        {
                            printf("Bad Frame");
                        }
                    app_task_startup_init();
                }
            }
            else
            {
                #if (PAL_GENERIC_TYPE == MEGA_RF)
                    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                    if(board == SENSOR_TERM_BOARD)
                    #endif
                #endif
                    {
                        printf("Bad Frame");
                    }
                app_task_startup_init();
            }
            break;
        }

        case PAIRING_REQ:
        {
            if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD +
                                           ((sizeof(app_payload_t) -
                                           sizeof(general_pkt_u)) +
                                           sizeof(pairing_rsp_t))))
            {
                /* Point to the message : 1 =>size is first byte and 2=>FCS*/
                msg = (app_payload_t *)(mac_frame_info->mpdu + 1 + FRAME_OVERHEAD - 2);
                if((msg->u8CmdID) == PAIRING_RESPONSE)
                {
                    if(((msg->payload[0].pairing_req_data.u8OpMode == OPERATION_MODE_1) && (mode_startup == OP_MODE_1_KEY_PRESS))
                    ||((msg->payload[0].pairing_req_data.u8OpMode == OPERATION_MODE_2) && (mode_startup == OP_MODE_2_CHAR_ON_UART)))
                    {

                        src_addr_pairing = (msg->payload[0].pairing_rsp_data.u16nwk_addr);

                        /* Set the newly assigned address */
                        tal_pib_set(macShortAddress, (pib_value_t *)&src_addr_pairing);
                        send_pairing_conf();
                        app_state_pairing = PAIRING_RSP_REC;
                    }
                }
            }
            break;
        }

        case WAIT_FOR_PAIR_CONF:
        {
            if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                 sizeof(general_pkt_u)) +
                                                 sizeof(pairing_conf_t))))
            {
                /* Point to the message : 1 =>size is first byte and 2=>FCS*/
                msg = (app_payload_t *)(mac_frame_info->mpdu + 1 + FRAME_OVERHEAD - 2);
                if((msg->u8CmdID) == PAIRING_CONFIRM)
                {
                    if(dest_addr_pairing == (msg->payload[0].pairing_conf_data.u16nwk_addr))
                    {
                        pal_timer_stop(APP_TIMER_TO_TX);
                        app_state_pairing = PAIRING_SUCCESS;
                    }
                    else
                    {
                        pal_timer_stop(APP_TIMER_TO_TX);
                        app_task_startup_init();
                    }
                }
            }
            break;
        }

        default:
            break;
    }
}

/**
 * @brief Callback that is called once tx is done.
 *
 * @param status    Status of the transmission procedure
 * @param frame     Pointer to the transmitted frame structure
 */
static void app_tx_frame_done_cb_for_pairing(retval_t status, frame_info_t *frame)
{
    status = status;
    frame = frame;
    /* After transmission is completed, allow next transmission. */
    transmitting = false;

    switch(app_state_pairing)
    {
        case PAIRING_RSP:
        {
            if(status == MAC_SUCCESS)
            {
                /* Set the newly assigned address */
                tal_pib_set(macShortAddress, (pib_value_t *)&src_addr_pairing);

                app_state_pairing = WAIT_FOR_PAIR_CONF;
            }
            else
            {
                app_task_startup_init();
            }
            break;
        }

        case PAIRING_RSP_REC:
        {
            if(status == MAC_SUCCESS)
            {
                /* Set the newly assigned address */
                tal_pib_set(macShortAddress, (pib_value_t *)&src_addr_pairing);
                app_state_pairing = PAIRING_SUCCESS;
            }
            else
            {
                app_task_startup_init();
            }
            break;
        }

        default:
            break;
    }
}
