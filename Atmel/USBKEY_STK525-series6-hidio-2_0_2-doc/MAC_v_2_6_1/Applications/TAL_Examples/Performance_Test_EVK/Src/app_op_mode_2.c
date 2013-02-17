/**
 * @file app_op_mode_2.c
 *
 * @brief  application for opmode 2 of TAL Example - Performance_Test_EVK
 *
 * $Id: app_op_mode_2.c 25495 2011-02-10 09:03:27Z felix.varghese $
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
#include "app_op_mode_2.h"
#include "app_frame_format.h"

/* === TYPES =============================================================== */
/* Enumeration used for transceiver operation mode */
typedef enum op_mode_tag
{
    TX_OP_MODE,
    TEST_FRAMES_SENT,
    WAIT_FOR_TEST_RES,
    SET_PARAMETER,
#ifdef ANTENNA_DIVERSITY
    DIVERSITY_STATUS_REQ,
    DIVERSITY_SET_REQ,
#endif /* #ifdef ANTENNA_DIVERSITY */
    CONTINOUS_TX_MODE,
    IDENTIFY_PAIR
}op_mode_t;

typedef struct
{
    parameters_t parm;
    uint8_t value;
}set_parm_cb_t;

/* === MACROS ============================================================== */
#define SET_PARAM        0x01
#define RESULT_REQ       0x02
#define RESULT_RSP       0x03
#ifdef ANTENNA_DIVERSITY
#define DIV_STAT_REQ     0x04
#define DIV_STAT_RSP     0x05
#define DIV_SET_REQ      0x06
#endif /* #ifdef ANTENNA_DIVERSITY */
#define IDENTIFY_NODE    0x07
#define PER_TEST_PKT     0xA5

#define TIMEOUT_FOR_RESPONSE_IN_MICRO_SEC  200000
#define LED_BLINK_RATE_IN_MICRO_SEC        50000

/* At very LED_TOGGLE_COUNT_FOR_PER count the Led toggles when PER test
   is in progress */
#define LED_TOGGLE_COUNT_FOR_PER            20

/* === PROTOTYPES ========================================================== */
static void print_main_menu(void);
static void get_number_test_frames(void);
static void get_frame_length(void);
static void get_channel(void);
static void get_tx_pwr(void);
static void get_page(void);
static void start_test(void);
static void print_result(void);
static void toggle_ack_request(void);
static void toggle_csma_enabled(void);
static void toggle_retry_enabled(void);
static void start_ed_scan(void);
static void get_sensor_data(void);
static void configure_frame_sending(void);

#if ((TAL_TYPE != AT86RF230B) || ((TAL_TYPE == AT86RF230B) && (defined CW_SUPPORTED)))
static void start_cw_transmission(void);
static void pulse_cw_transmission(void);
static void stop_pulse_cb(void *callback_parameter);
#endif

#ifdef ANTENNA_DIVERSITY
static void toogle_antenna_diversity(void);
static uint8_t choose_antenna(void);
#endif

static void send_parameters_changed(parameters_t parm, uint8_t val);
static bool send_result_req(void);
static void send_result_rsp(void);

#ifdef ANTENNA_DIVERSITY
static bool send_diversity_status_req(void);
static void send_diversity_status_rsp(void);
static bool send_diversity_set_req(div_set_req_t div_msg);
static void diversity_settings_remote_node(void);
#endif /* #ifdef ANTENNA_DIVERSITY */

static void send_identify_command(void);

static void wait_for_reply_timer_handler_cb(void *parameter);
static void identify_timer_handler_cb(void *parameter);

/* Functions for Operation Mode 2 Transmitter*/
static void app_task_op_mode_2_transmitter(void);
static void app_tx_frame_done_cb_for_op_mode_2_transmitter(retval_t status, frame_info_t *frame);
static void app_rx_frame_cb_for_op_mode_2_transmitter(frame_info_t *frame);

/* Functions for Operation Mode 2 Reflector*/
static void app_task_op_mode_2_reflector(void);
static void app_rx_frame_cb_for_op_mode_2_reflector(frame_info_t *frame);
static void app_tx_frame_done_cb_for_op_mode_2_reflector(retval_t status, frame_info_t *frame);

/* === GLOBALS ============================================================= */
static uint32_t number_test_frames = 100;
static uint8_t phy_frame_length = 20;
static op_mode_t op_mode = TX_OP_MODE;
static uint32_t number_rx_frames;
static uint32_t frame_successful;
static uint32_t frame_no_ack;
static uint32_t frame_access_failure;
static uint32_t frame_failure;
static uint32_t frames_to_transmit;
static bool ack_request = true;
static bool csma_enabled = true;
static bool retry_enabled = true;
static uint32_t start_time;
static uint32_t end_time;
static uint32_t aver_lqi;
static uint8_t channel_before_scan;
static uint32_t scan_channel_mask;
static bool scanning = false;
static uint8_t scan_duration;
#ifdef ANTENNA_DIVERSITY
static bool antenna_diversity = true;
#endif

static uint8_t seq_num;
static bool pairing = false;
static bool transmitting = false;
static bool TrxSleepStatus = false;
static set_parm_cb_t set_parm_cb;

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Initialise the application in operation mode 2
 *        Operation Mode 2 is used for PER measurement
 *
 * @param mode starts Operation Mode 2 in this particular mode
 *        Operation Mode 2 can be started as OP_MODE_2_CHAR_ON_UART and
 *        OP_MODE_2_RX_FRAME
 * @param pairing_status can be true or false
 *        true signifies pairing was success
 *        false signifies pairing was aborted
 */
void app_task_op_mode_2_init(startup_mode_e mode, bool pairing_status)
{
    pairing = pairing_status;

    #if (PAL_GENERIC_TYPE == MEGA_RF)
        #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
        if(board == SENSOR_TERM_BOARD)
        #endif
    #endif
        {
            if(pairing_status == true)
            {
                printf("\r\n Pairing Success");
            }
            else
            {
                printf("\r\n Pairing Aborted");
            }
        }

    switch(mode)
    {
        case OP_MODE_2_CHAR_ON_UART:
        {
            printf("\r\n Starting OP MODE 2 as Transmitter");
            /* Init the call backs and app_task */
            /* In case of PER measurement app 2 sets of application is executed
               for Transmitter and reflector*/
            app_task = &app_task_op_mode_2_transmitter;
            app_tx_frame_done_cb = &app_tx_frame_done_cb_for_op_mode_2_transmitter;
            app_rx_frame_cb = &app_rx_frame_cb_for_op_mode_2_transmitter;
            break;
        }

        case OP_MODE_2_RX_FRAME:
        {
            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf("\r\n Starting OP MODE 2 as Reflector");
                }
            /* Init the call backs and app_task */
            /* In case of PER measurement app 2 sets of application is executed
               for Transmitter and reflector*/
            app_task = &app_task_op_mode_2_reflector;
            app_tx_frame_done_cb = &app_tx_frame_done_cb_for_op_mode_2_reflector;
            app_rx_frame_cb = &app_rx_frame_cb_for_op_mode_2_reflector;
            break;
        }
        default:
            break;
    }
}

/**
 * @brief Application task for Operation Mode 2 transmitter
 *
 * This function
 * - Implements the Operation Mode 2 state machine.
 * - On start up as transmitter application will print menu on screen
 * - On user inputs through menu app executes various tests
 */
static void app_task_op_mode_2_transmitter(void)
{
    if (frames_to_transmit > 0)
    {
        /* PER measurement test is on as frames_to_transmit more frames has to
           be transmitted. On completion of the test result also has to be
           queried from the remote node */
        if (!transmitting)
        {
            transmitting = true;
            tx_frame_info->mpdu[PL_POS_SEQ_NUM]++;
            if (csma_enabled)
            {
                tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, retry_enabled);
            }
            else
            {
                tal_tx_frame(tx_frame_info, NO_CSMA_NO_IFS, retry_enabled);
            }
        }
    }
    else if (op_mode == CONTINOUS_TX_MODE)
    {
        /* While CW transmission wait for any key to stop transmitting. */
    }
    else if (op_mode == SET_PARAMETER)
    {
        /* While setting parameter on remote node wait for call back
           from the TAL specifing the result */
    }
#ifdef ANTENNA_DIVERSITY
    else if (op_mode == DIVERSITY_STATUS_REQ)
    {
        /* While quering the diversity seetting on remote node wait for result */
    }
    else if (op_mode == DIVERSITY_SET_REQ)
    {
        /* While changing the diversity setting on remote node wait for call back
           from the TAL specifing the result */
    }
#endif /* #ifdef ANTENNA_DIVERSITY */
    else if (op_mode == IDENTIFY_PAIR)
    {
        /* Wait for result while identify pair command is sent */
    }
    else if (op_mode == TEST_FRAMES_SENT)
    {
        /* Test frames has been sent, now ask for the result */
        if(send_result_req())
        {
            pal_timer_start(APP_TIMER_TO_TX,
                            TIMEOUT_FOR_RESPONSE_IN_MICRO_SEC,
                            TIMEOUT_RELATIVE,
                            (FUNC_PTR)wait_for_reply_timer_handler_cb,
                            NULL);
            op_mode = WAIT_FOR_TEST_RES;
        }
        else
        {
            op_mode = TX_OP_MODE;
        }
    }
    else if (op_mode == WAIT_FOR_TEST_RES)
    {
        /* Wait for PER test result */
    }
    else
    {
        if (scanning == false)
        {
            print_main_menu();
        }
    }
}

/**
 * @brief Callback that is called if data has been received by trx.
 *        This call back is used when the node is operating as
 *        op mode 2 transmitter
 *
 * @param mac_frame_info    Pointer to received data structure
 */
static void app_rx_frame_cb_for_op_mode_2_transmitter(frame_info_t *mac_frame_info)
{
    app_payload_t *msg;
#ifdef ANTENNA_DIVERSITY
    uint8_t input;
    div_set_req_t div_msg;
#endif /* #ifdef ANTENNA_DIVERSITY */

    /* Point to the message : 1 =>size is first byte and 2=>FCS*/
    msg = (app_payload_t *)(mac_frame_info->mpdu + 1 + FRAME_OVERHEAD - 2);

    switch((msg->u8CmdID))
    {
        case RESULT_RSP:
        {
            if(op_mode == WAIT_FOR_TEST_RES)
            {
                if(*(mac_frame_info->mpdu) == ( FRAME_OVERHEAD +
                                                ((sizeof(app_payload_t) -
                                                  sizeof(general_pkt_u)) +
                                                  sizeof(result_rsp_t))))
                {
                    pal_timer_stop(APP_TIMER_TO_TX);
                    number_rx_frames = msg->payload[0].test_result_rsp_data.num_of_frames_rx;
                    aver_lqi = msg->payload[0].test_result_rsp_data.lqi_avrg_rx;
                    /* Print PER test Result */
                    printf("\r\n\r\nResults from RX node:");
                    printf("\r\nNumber of received frames = %" PRIu32 "; average LQI = %d", number_rx_frames, (uint8_t)aver_lqi);
                    printf("\r\nPress any key to continue");
                    sio_getchar();
                    op_mode = TX_OP_MODE;
                }
            }
            break;
        }
#ifdef ANTENNA_DIVERSITY
        case DIV_STAT_RSP:
        {
            if(op_mode == DIVERSITY_STATUS_REQ)
            {
                if(*(mac_frame_info->mpdu) == ( FRAME_OVERHEAD +
                                                ((sizeof(app_payload_t) -
                                                  sizeof(general_pkt_u)) +
                                                  sizeof(div_stat_rsp_t))))
                {
                    pal_timer_stop(APP_TIMER_TO_TX);
                    number_rx_frames = msg->payload[0].test_result_rsp_data.num_of_frames_rx;
                    aver_lqi = msg->payload[0].test_result_rsp_data.lqi_avrg_rx;
                    printf("\r\n\r\nResults from RX node:");
                    printf("\r\nDiversity is");

                    if(msg->payload[0].div_stat_rsp_data.status)
                    {
                        printf(" enabled");
                    }
                    else
                    {
                        printf(" disabled and antenna ");
                        if(msg->payload[0].div_stat_rsp_data.ant_sel == ENABLE_ANTENNA_1)
                        {
                            printf(" A1/X2 is enabled");
                        }
                        else if(msg->payload[0].div_stat_rsp_data.ant_sel == ENABLE_ANTENNA_0)
                        {
                            printf(" A2/X3 is enabled");
                        }
                    }

                    printf("\r\nPress 'C' to change or any key to accept setting:");
                    input = sio_getchar();
                    input = toupper(input);
                    if(input == 'C')
                    {
                        printf("\r\nTo change configuration press:");
                        printf("\r\n'1'=> enable diversity" );
                        printf("\r\n'2'=> disable diversity and use A1/X2");
                        printf("\r\n'3'=> disable diversity and use A2/X3");
                        printf("\r\n'Any key to exit...");

                        input = sio_getchar();
                        switch(input)
                        {
                            case '1':
                            {
                                div_msg.status = 1;
                                div_msg.ant_sel = 0;
                                break;
                            }
                            case '2':
                            {
                                div_msg.status = 0;
                                div_msg.ant_sel = ENABLE_ANTENNA_1;
                                break;
                            }
                            case '3':
                            {
                                div_msg.status = 0;
                                div_msg.ant_sel = ENABLE_ANTENNA_0;
                                break;
                            }
                            default:
                            {
                                op_mode = TX_OP_MODE;
                                return;
                            }
                        }

                        if(send_diversity_set_req(div_msg))
                        {
                            op_mode = DIVERSITY_SET_REQ;
                        }
                        else
                        {
                            printf("\r\nDiversity set failed");
                            printf("\r\nPress any key to continue");
                            sio_getchar();
                            op_mode = TX_OP_MODE;
                        }
                    }
                    else
                    {
                        op_mode = TX_OP_MODE;
                    }
                }
            }
            break;
        }
#endif /* #ifdef ANTENNA_DIVERSITY */
    }
}


/**
 * @brief Callback that is called once tx is done.
 *        This call back is used when the node is operating as
 *        op mode 2 transmitter
 *
 * @param status    Status of the transmission procedure
 * @param frame     Pointer to the transmitted frame structure
 */
static void app_tx_frame_done_cb_for_op_mode_2_transmitter(retval_t status, frame_info_t *frame)
{
    bool valid_cb = false;
    uint8_t temp_var;
    static bool toggle_led = false;
    static uint8_t tx_count;

    if(op_mode == SET_PARAMETER)
    {
        /* set the parameter on this node */
        switch(set_parm_cb.parm)
        {
            case CHANNEL:
            {
                tal_pib_set(phyCurrentChannel, (pib_value_t *)&set_parm_cb.value);
                printf("\r\n Channel set on source node");
                valid_cb = true;
                break;
            }

            case CHANNEL_PAGE:
            {
                tal_pib_set(phyCurrentPage, (pib_value_t *)&set_parm_cb.value);
                printf("\r\n Channel page set on source node");
                valid_cb = true;
                break;
            }

            case TX_POWER:
            {
                temp_var = CONV_DBM_TO_phyTransmitPower((int8_t) set_parm_cb.value);
                tal_pib_set(phyTransmitPower, (pib_value_t *)&temp_var);
                printf("\r\n Tx Power set to %d dBm on source node", (int8_t)set_parm_cb.value);
                valid_cb = true;
                break;
            }
        }
        /* prints the result of set parameter on the remote node */
        if(valid_cb)
        {
            op_mode = TX_OP_MODE;
            if (status == MAC_SUCCESS)
            {
                printf(" and remote node sucessfully");
            }
            else
            {
                /* if the parameter set fails it is annunciated to
                   the user and waits for inputs from user */
                printf(" sucessfully but failed on remote node");
                printf("\r\n All further tests may fail please check the remote node");
                printf("\r\n Press any key to return to main menu.\r\n");
                sio_getchar();
            }
        }
        transmitting = false;
        return;
    }
    else if(op_mode == WAIT_FOR_TEST_RES)
    {
        print_result();
        if (status == MAC_SUCCESS)
        {
            op_mode = WAIT_FOR_TEST_RES;
        }
        else
        {
            /* if PER test result request fails it is annunciated to
               the user and waits for inputs from user */
            pal_timer_stop(APP_TIMER_TO_TX);
            printf("\r\nUnable to contact Remote node");
            printf("\r\nPress any key to continue");
            sio_getchar();
            op_mode = TX_OP_MODE;
        }
        transmitting = false;
        return;
    }
#ifdef ANTENNA_DIVERSITY
    else if(op_mode == DIVERSITY_SET_REQ)
    {
        op_mode = TX_OP_MODE;
        if (status == MAC_SUCCESS)
        {
            printf("\r\nDiversity paramters set successfully on remote node");
        }
        else
        {
            /* if Antenna diversity set request fails it is annunciated to
               the user and waits for inputs from user */
            pal_timer_stop(APP_TIMER_TO_TX);
            printf("\r\n Diversity paramters could not be set on remote node");
            printf("\r\n All further tests may fail please check the remote node");
            printf("\r\n Press any key to return to main menu.\r\n");
            sio_getchar();
        }
    }
#endif /* #ifdef ANTENNA_DIVERSITY */
    else if(op_mode == IDENTIFY_PAIR)
    {
        op_mode = TX_OP_MODE;
        if (status == MAC_SUCCESS)
        {
            printf("\r\nIdentify sent successfully...LED blinking on node");
        }
        else
        {
            /* if Identify pair request fails it is annunciated to
               the user and waits for inputs from user */
            printf("\r\n Identify sent fail");
            printf("\r\n All further tests may fail please check the remote node");
            printf("\r\n Press any key to return to main menu.\r\n");
            sio_getchar();
        }
    }
    else if(op_mode == TX_OP_MODE)
    {
        if (status == MAC_SUCCESS)
        {
            frame_successful++;
        }
        else
        {
            if (status == MAC_NO_ACK)
            {
                frame_no_ack++;
            }
            else if (status == MAC_CHANNEL_ACCESS_FAILURE)
            {
                frame_access_failure++;
            }
            else
            {
                frame_failure++;
            }
        }

        /* PER test is ON after sending each frame the number of
           frames to be sent is decremented */
        frames_to_transmit--;

        /* Led is toggled indicating the test in progress at the count of
           LED_TOGGLE_COUNT_FOR_PER */
        if(tx_count > LED_TOGGLE_COUNT_FOR_PER)
        {
            if(toggle_led == true)
            {
                /* OFF the LED */
                app_set_led_value(0);
                toggle_led = false;
                tx_count = 0;
            }
            else
            {
                /* ON the LED */
                app_set_led_value(LED_MODE_2);
                toggle_led = true;
                tx_count = 0;
            }
        }
        else
        {
            tx_count++;
        }

        if (frames_to_transmit == 0)
        {
            pal_get_current_time(&end_time);
            op_mode = TEST_FRAMES_SENT;

            /* Test Over ON the LED */
            app_set_led_value(LED_MODE_2);
            toggle_led = false;
            tx_count = 0;
        }
    }
    /* After transmission is completed, allow next transmission. */
    transmitting = false;

    frame = frame;  /* Keep compiler happy. */
}

/**
  * @brief Application task for Operation Mode 2 reflector
 */
static void app_task_op_mode_2_reflector(void)
{

}

/**
 * @brief Callback that is called if data has been received by trx.
 *        This call back is used when the node is operating as
 *        op mode 2 reflector
 *
 * @param mac_frame_info    Pointer to received data structure
 */
static void app_rx_frame_cb_for_op_mode_2_reflector(frame_info_t *mac_frame_info)
{
    app_payload_t *msg;
    static bool toggle_led = false;
    static uint8_t rx_count;

    /* Point to the message : 1 =>size is first byte and 2=>FCS*/
    msg = (app_payload_t *)(mac_frame_info->mpdu + 1 + FRAME_OVERHEAD - 2);

    switch((msg->u8CmdID))
    {
        case SET_PARAM:
        {
            /* if set parameter command received then extract the parameter
               and value and set the same. Size of the packet is also verified */
            switch(msg->payload[0].set_parm_req_data.parmameter)
            {
                case CHANNEL:
                {
                    if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                                      sizeof(general_pkt_u)) +
                                                                      sizeof(set_parm_req_t))))
                    {
                        uint8_t val = msg->payload[0].set_parm_req_data.value;
                        tal_pib_set(phyCurrentChannel, (pib_value_t *)&val);
                        #if (PAL_GENERIC_TYPE == MEGA_RF)
                            #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                            if(board == SENSOR_TERM_BOARD)
                            #endif
                        #endif
                            {
                                printf("\r\n Channel changed to %d", val);
                            }
                    }
                    break;
                }

                case CHANNEL_PAGE:
                {
                    if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                                      sizeof(general_pkt_u)) +
                                                                      sizeof(set_parm_req_t))))
                    {
                        uint8_t val = msg->payload[0].set_parm_req_data.value;
                        tal_pib_set(phyCurrentPage, (pib_value_t *)&val);
                        #if (PAL_GENERIC_TYPE == MEGA_RF)
                            #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                            if(board == SENSOR_TERM_BOARD)
                            #endif
                        #endif
                            {
                                printf("\r\n Channel page changed to %d", val);
                            }
                    }
                    break;
                }

                case TX_POWER:
                {
                    if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                                      sizeof(general_pkt_u)) +
                                                                      sizeof(set_parm_req_t))))
                    {
                        uint8_t val = msg->payload[0].set_parm_req_data.value;
                        uint8_t  temp_var = CONV_DBM_TO_phyTransmitPower((int8_t) val);
                        tal_pib_set(phyTransmitPower, (pib_value_t *)&temp_var);
                        #if (PAL_GENERIC_TYPE == MEGA_RF)
                            #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                            if(board == SENSOR_TERM_BOARD)
                            #endif
                        #endif
                            {
                                printf("\r\n Tx Power set to %d dBm on the node", (int8_t)val);
                            }
                    }
                    break;
                }
            }
            break;
        } /* case SET_PARAM */

        case PER_TEST_PKT:
        {
            /* if PER test frames received then increment number_rx_frames */
            if(number_rx_frames == 0)
            {
            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf("\r\n Receiving test packets.....");
                }
            }
            number_rx_frames++;
            /* Extract LQI and  */
            aver_lqi += mac_frame_info->mpdu[mac_frame_info->mpdu[0] + LQI_LEN];

            /* Led is toggled indicating the test in progress at the count of
               LED_TOGGLE_COUNT_FOR_PER */
            if(rx_count > LED_TOGGLE_COUNT_FOR_PER)
            {
                if(toggle_led == true)
                {
                    /* OFF the LED */
                    app_set_led_value(0);
                    toggle_led = false;
                    rx_count = 0;
                }
                else
                {
                    /* ON the LED */
                    app_set_led_value(LED_MODE_2);
                    toggle_led = true;
                    rx_count = 0;
                }
            }
            else
            {
                rx_count++;
            }

            break;
        } /* case PER_TEST_PKT */

        case RESULT_REQ:
        {
            if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                              sizeof(general_pkt_u)) +
                                                              sizeof(result_req_t))))
            {
                if (number_rx_frames != 0)
                {
                    aver_lqi = aver_lqi / number_rx_frames;
                }
                send_result_rsp();
                #if (PAL_GENERIC_TYPE == MEGA_RF)
                    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                    if(board == SENSOR_TERM_BOARD)
                    #endif
                #endif
                    {
                        printf("\r\n Number of received frames = %" PRIu32 "; average LQI = %d", number_rx_frames, (uint8_t)aver_lqi);
                    }
                number_rx_frames = 0;
                aver_lqi = 0;

                /* Test Over ON the LED */
                app_set_led_value(LED_MODE_2);
                toggle_led = false;
                rx_count = 0;
            }
            break;
        } /* RESULT_REQ */

#ifdef ANTENNA_DIVERSITY
        case DIV_STAT_REQ:
        {
            if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                              sizeof(general_pkt_u)) +
                                                              sizeof(div_stat_req_t))))
            {
                send_diversity_status_rsp();
            }
            break;
        } /* DIV_STAT_REQ */

        case DIV_SET_REQ:
        {
            if(*(mac_frame_info->mpdu) == (FRAME_OVERHEAD + ((sizeof(app_payload_t) -
                                                              sizeof(general_pkt_u)) +
                                                              sizeof(div_set_req_t))))
            {
                if (msg->payload[0].div_set_req_data.status)
                {
                    pal_trx_bit_write(SR_ANT_CTRL, ANTENNA_DEFAULT);
                    pal_trx_bit_write(SR_PDT_THRES, THRES_ANT_DIV_ENABLE);
                    pal_trx_bit_write(SR_ANT_DIV_EN, ANT_DIV_ENABLE);
                    pal_trx_bit_write(SR_ANT_EXT_SW_EN, ANT_EXT_SW_SWITCH_ENABLE);
                    printf("\r\n Antenna diversity enabled");
                }
                else
                {
                    pal_trx_bit_write(SR_ANT_CTRL, msg->payload[0].div_set_req_data.ant_sel);
                    pal_trx_bit_write(SR_ANT_DIV_EN, ANT_DIV_DISABLE);
                    pal_trx_bit_write(SR_PDT_THRES, THRES_ANT_DIV_DISABLED);
                    printf("\r\n Antenna diversity disabled and antenna");
                    if(msg->payload[0].div_set_req_data.ant_sel == ENABLE_ANTENNA_1)
                    {
                        printf(" selected is A1/X2");
                    }
                    else if(msg->payload[0].div_set_req_data.ant_sel == ENABLE_ANTENNA_0)
                    {
                        printf(" selected is A2/X3");
                    }
                    else
                    {
                         printf(" selected is invalid");
                    }
                }
            }
            break;
        } /* DIV_SET_REQ */
#endif /* #ifdef ANTENNA_DIVERSITY */

        case IDENTIFY_NODE:
        {
            #if (PAL_GENERIC_TYPE == MEGA_RF)
                #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
                if(board == SENSOR_TERM_BOARD)
                #endif
            #endif
                {
                    printf(" \r\n Blinking LED: identify frame received");
                }
            pal_timer_start(APP_TIMER_TO_TX,
                            LED_BLINK_RATE_IN_MICRO_SEC,
                            TIMEOUT_RELATIVE,
                            (FUNC_PTR)identify_timer_handler_cb,
                            NULL);
            break;
        }
    }
}

/**
 * @brief Callback that is called once tx is done.
 *        This call back is used when the node is operating as
 *        op mode 2 reflector
 *
 * @param status    Status of the transmission procedure
 * @param frame     Pointer to the transmitted frame structure
 */
static void app_tx_frame_done_cb_for_op_mode_2_reflector(retval_t status, frame_info_t *frame)
{
    status = status;
    frame = frame;
    transmitting = false;
}

/**
 * @brief the value is input in UART term as hex
 *
 * @param value Pointer to data received in UART
 *
 * @return true if correct data received, false otherwise
 */
uint8_t read_value_in_hex(uint8_t *value)
{
    char input_char[3]= {0, 0, 0};
    uint8_t i;
    uint8_t input;
    uint8_t temp;

    for (i = 0; i < 2; i++)
    {
        input = sio_getchar();
        input = toupper(input);
        if (((input < '0') || (input > '9')) &&
            ((input < 'A') || (input > 'F')))
        {
            printf("\r\n Wrong value.. Press any key....");
            sio_getchar();
            return(false);
        }
        else
        {
            temp = input - 0x30;
            if(temp > 9)
            {
                temp = temp - 7;
            }
            input_char[i] = temp;
        }
    }
    temp = input_char[0];
    temp = temp<<4;
    temp |= input_char[1];

    *(value) = temp;
    return(true);
}

#if (PAL_GENERIC_TYPE != MEGA_RF)

/**
 * @brief used to read - write radio transceiver registers
 */
static void read_write_reg(void)
{
    uint8_t i;
    uint8_t input;
    uint8_t reg;
    uint8_t reg_val;
    uint8_t reg_start;
    uint8_t reg_end;
    int8_t num_of_reg_to_read;

    printf("\r\n R - to Read Reg");
    printf("\r\n W - to Write Reg");
    printf("\r\n D - to get register Dump");
    printf("\r\n >");

    /* Get input from terminal program / user. */
    input = sio_getchar();
    input = toupper(input);

    /* Handle input from terminal program. */
    switch (input)
    {
        case 'R':
        {
            printf("\r\n Read Register: 0x");
            if(read_value_in_hex(&reg) == false)
            {
                return;
            }

            if(reg > 0x3F)
            {
                printf("\r\n Out of Range register value.. Press any key....");
                sio_getchar();
                return;
            }

            reg_val = pal_trx_reg_read(reg);

            printf("\r\n Value of Reg 0x%x is 0x%x",reg,reg_val);
            printf("\r\n Press any key....");
            sio_getchar();
        }
        break;

        case 'W':
        {
            printf("\r\n Write Register: 0x");
            if(read_value_in_hex(&reg) == false)
            {
                return;
            }

            if(reg > 0x3F)
            {
                printf("\r\n Out of Range register value.. Press any key....");
                sio_getchar();
                return;
            }

            printf("\r\n Write Value: 0x");
            if(read_value_in_hex(&reg_val) == false)
            {
                return;
            }

            pal_trx_reg_write(reg, reg_val);

            printf("\r\n Value written in Reg 0x%x is 0x%x",reg,reg_val);
            printf("\r\n Press any key....");
            sio_getchar();
        }
        break;

        case 'D':
        {
            printf("\r\n Start Register: 0x");
            if(read_value_in_hex(&reg_start) == false)
            {
                return;
            }

            if(reg_start > 0x3F)
            {
                printf("\r\n Out of Range register value.. Press any key....");
                sio_getchar();
                return;
            }

            printf("\r\n End Register: 0x");
            if(read_value_in_hex(&reg_end) == false)
            {
                return;
            }

            if(reg_end > 0x3F)
            {
                printf("\r\n Out of Range register value.. Press any key....");
                sio_getchar();
                return;
            }

            num_of_reg_to_read = ((reg_end - reg_start));

            if(num_of_reg_to_read < 0)
            {
                printf("\r\n Registers order incorrect.. Press any key....");
                sio_getchar();
                return;
            }
            else
            {
                num_of_reg_to_read += 1;
            }

            reg = reg_start;

            for(i = 0 ; i < num_of_reg_to_read ; i++)
            {
                reg_val = pal_trx_reg_read(reg + i);
                printf("\r\n Value of Reg 0x%x is 0x%x",(reg + i),reg_val);
            }
            printf("\r\n Press any key....");
            sio_getchar();
        }
        break;
    }
}

#endif /* (PAL_GENERIC_TYPE != MEGA_RF) */

/**
 * @brief Put radio transreciver in sleep
 */
static void toggle_trx_sleep(void)
{
    if(TrxSleepStatus == false)
    {
        if(tal_trx_sleep(SLEEP_MODE_1) == MAC_SUCCESS)
        {
            printf("\r\n TRX in sleep mode 1 : Options disabled");
            TrxSleepStatus = true;
        }
        else
        {
            printf("\r\n TRX cannot be set in sleep mode");
            printf("\r\n Press any key....");
            sio_getchar();
        }
    }
    else
    {
        if(tal_trx_wakeup()== MAC_SUCCESS)
        {
            printf("\r\n TRX woke up");
            TrxSleepStatus = false;
        }
        else
        {
            printf("\r\n TRX cannot be woken up");
            printf("\r\n Press any key....");
            sio_getchar();
        }
    }
}

/**
 * @brief Print main menu to terminal program
 */
static void print_main_menu(void)
{
    uint8_t temp_var;
    uint8_t input;
    int8_t tx_pwr_dbm;

    printf("\r\n******************************************************\r\n");
    printf("Software Version:%d.%d\r\n", MAJOR_NUMBER,MINOR_NUMBER);
    printf("Performance test application (");

    /* Transceiver version */
#if (TAL_TYPE == AT86RF212)
    printf("AT86RF212");
#elif (TAL_TYPE == AT86RF230A)
    printf("AT86RF230A");
#elif (TAL_TYPE == AT86RF230B)
    printf("AT86RF230B");
#elif (TAL_TYPE == AT86RF231)
    printf("AT86RF231");
#elif (TAL_TYPE == AT86RF232)
    printf("AT86RF232");
#elif (TAL_TYPE == ATMEGARF_TAL_1)
    // no output
#else
#error "unknown TAL type ";
#endif

#if (TAL_TYPE != ATMEGARF_TAL_1)
    printf(" / ");
#endif

    /* Print MCU version */
#if (PAL_GENERIC_TYPE == AVR)
    #if (PAL_TYPE == ATMEGA1281)
        printf("ATmega1281");
    #elif (PAL_TYPE == ATMEGA2561)
        printf("ATmega2561");
    #elif (PAL_TYPE == ATMEGA644P)
        printf("ATmega644P");
    #elif (PAL_TYPE == ATMEGA1284P)
        printf("ATmega1284P");
    #elif (PAL_TYPE == AT90USB1287)
        printf("AT90USB1287");
    #else
    #error "unknown PAL_TYPE";
    #endif
#elif (PAL_GENERIC_TYPE == XMEGA)
    #if (PAL_TYPE == ATXMEGA128A1)
        printf("ATxmega128A1");
    #elif (PAL_TYPE == ATXMEGA128A3)
        printf("ATxmega128A3");
    #elif (PAL_TYPE == ATXMEGA256A3)
        printf("ATxmega256A3");
    #elif (PAL_TYPE == ATXMEGA256D3)
        printf("ATxmega256D3");
    #else
    #error "unknown PAL_TYPE";
    #endif
#elif (PAL_GENERIC_TYPE == MEGA_RF)
    #if (PAL_TYPE == ATMEGA128RFA1)
        printf("ATmega128RFA1");
    #else
    #error "unknown PAL_TYPE";
    #endif
#elif (PAL_GENERIC_TYPE == ARM7)
    #if (PAL_TYPE == AT91SAM7X256)
        printf("AT91SAM7X256");
    #else
    #error "unknown PAL_TYPE";
    #endif
#elif (PAL_GENERIC_TYPE == MEGA_RF)
    /* no putput */
#else
#error "unknown PAL_GENERIC_TYPE";
#endif

    printf(")\r\nSettings:\r\n");

    if(TrxSleepStatus == true)
    {
        printf("(T) : Wake up Transceiver\r\n");
        printf(">");

        /* Get input from terminal program / user. */
        input = sio_getchar();
        input = toupper(input);
        switch (input)
        {
          case 'T': toggle_trx_sleep(); break;
        }
        return;
    }

    if(pairing == true)
    {
        /* incase channel changed in reg read write*/
        tal_pib_set(phyCurrentChannel, (pib_value_t *)&tal_pib_CurrentChannel);
    }

    /* Print channel settings */
    tal_pib_get(phyCurrentChannel, &temp_var);
    printf("(C) : Channel = %d\r\n", temp_var);

    /* Print channel page settings */
    printf("(P) : Channel page ");

    if(pairing == true)
    {
        /* incase channel page changed in reg read write*/
        tal_pib_set(phyCurrentPage, (pib_value_t *)&tal_pib_CurrentPage);
    }

    if (tal_pib_get(phyCurrentPage, &temp_var) == MAC_SUCCESS)
    {
        printf("= %d\r\n", temp_var);
    }
    else
    {
        printf("not supported\r\n");
    }

    if(pairing == true)
    {
        /* incase TX power changed in reg read write*/
        tal_pib_set(phyTransmitPower, (pib_value_t *)&tal_pib_TransmitPower);
    }

    /* Print tx power settings */
    tal_pib_get(phyTransmitPower, &temp_var);
    tx_pwr_dbm = CONV_phyTransmitPower_TO_DBM(temp_var);
    printf("(W) : Tx power = %d dBm\r\n", tx_pwr_dbm);

    if(pairing == true)
    {

        printf("(N) : Number of test frames = %" PRIu32 "\r\n", number_test_frames);
        printf("(L) : Frame length (PSDU) = %d\r\n", phy_frame_length);

        /* Print ACK settings */
        printf("(A) : ACK request = ");
        if (ack_request)
        {
            printf("ACK requested\r\n");
        }
        else
        {
            printf("no ACK requested\r\n");
        }

        /* Print frame retry settings */
        printf("(F) : Frame retry enabled = ");
        if (retry_enabled == true)
        {
            printf("true\r\n");
        }
        else
        {
            printf("false\r\n");
        }

        /* Print CSMA settings */
        printf("(M) : CSMA enabled = ");
        if (csma_enabled == true)
        {
            printf("true\r\n");
        }
        else
        {
            printf("false\r\n");
        }
#ifdef ANTENNA_DIVERSITY
        printf("(Q) : Diversity settings on remote node\r\n");
#endif /* #ifdef ANTENNA_DIVERSITY */

        printf("(I) : Identify paired device by blinking\r\n");
    }

    printf("(E) : Energy scan on all channels\r\n");
#if ((TAL_TYPE != AT86RF230B) || ((TAL_TYPE == AT86RF230B) && (defined CW_SUPPORTED)))
    printf("(U) : Continuous transmission on current channel\r\n");
    printf("(D) : Transmit a continuous wave pulse on current channel\r\n");
#endif
#ifdef ANTENNA_DIVERSITY
    printf("(Y) : Toggle antenna diversity - ");

    antenna_diversity = pal_trx_bit_read(SR_ANT_DIV_EN) ? true : false;
    if (antenna_diversity)
    {
        printf("enabled\r\n");
    }
    else
    {
        uint8_t antenna_temp;
        printf("disabled");
        antenna_temp = pal_trx_bit_read(SR_ANT_CTRL);
        if(antenna_temp == ENABLE_ANTENNA_1)
        {
            printf(" and antenna selected is A1/X2\r\n");
        }
        else if(antenna_temp == ENABLE_ANTENNA_0)
        {
            printf(" and antenna selected is A2/X3\r\n");
        }
        else
        {
            printf(" Invalid antenna selection\r\n");
        }
    }
#endif /* #ifdef ANTENNA_DIVERSITY */

    /* Register read wrte not supported for this MCU */
#if (PAL_GENERIC_TYPE != MEGA_RF)
    printf("(H) : Read - Write Transceiver Registers\r\n");
#endif

    printf("(T) : Put Transceiver in sleep\r\n");

#if (PAL_GENERIC_TYPE == MEGA_RF)
    printf("(V) : Get sensor data, i.e. supply voltage and temperature\r\n");
#else
    printf("(V) : Get sensor data, i.e. supply voltage\r\n");
#endif

    if(pairing == true)
    {
        printf("(S) : Start test\r\n");
    }
    printf(">");

    /* Get input from terminal program / user. */
    input = sio_getchar();
    input = toupper(input);

    if(pairing == true)
    {
        /* Handle input from terminal program. */
        switch (input)
        {
            case 'N': get_number_test_frames(); break;
            case 'L': get_frame_length(); break;
            case 'A': toggle_ack_request(); break;
            case 'M': toggle_csma_enabled(); break;
            case 'F': toggle_retry_enabled(); break;

#ifdef ANTENNA_DIVERSITY
            case 'Q':
            {
                diversity_settings_remote_node();
                break;
            }
#endif /* #ifdef ANTENNA_DIVERSITY */

            case 'I':
            {
                op_mode = IDENTIFY_PAIR;
                send_identify_command();
                break;
            }

            case 'S': start_test(); break;
        }
        //return;
    }

    /* Handle input from terminal program. */
    switch (input)
    {
        case 'C': get_channel(); break;
        case 'P': get_page(); break;
        case 'W': get_tx_pwr(); break;

        case 'E': start_ed_scan(); break;

#if ((TAL_TYPE != AT86RF230B) || ((TAL_TYPE == AT86RF230B) && (defined CW_SUPPORTED)))
        case 'U':
            start_cw_transmission();
            break;

        case 'D':
            pulse_cw_transmission();
            break;
#endif

#ifdef ANTENNA_DIVERSITY
        case 'Y': toogle_antenna_diversity();
            break;
#endif

   /* Register read wrte not supported for this MCU */
#if (PAL_GENERIC_TYPE != MEGA_RF)
        case 'H':
            read_write_reg();
            break;
#endif
        case 'T':
            toggle_trx_sleep();
            break;

        case 'V':
            get_sensor_data();
            break;
    }
}

/**
 * @brief Start energy scan on current channel page
 */
static void start_ed_scan(void)
{
    uint8_t first_channel;
    uint8_t i;
    bool character_entered = false; // no character is entered
    char input_char[3]= {0, 0, 0};
    uint8_t input;
#if (TAL_TYPE == AT86RF212)
    uint8_t page;
    uint8_t channel;
#endif

    uint8_t formfeed = 0x0C;
    pal_sio_tx(SIO_CHANNEL, &formfeed, 1);
    printf("Energy scan\r\n");
    printf("Scan duration = aBaseSuperframeDuration * (2^n + 1) symbols\r\n");

#if (TAL_TYPE == AT86RF212)
    tal_pib_get(phyCurrentPage, &page);
    tal_pib_get(phyCurrentChannel, &channel);
    switch (page)
    {
        case 0: printf("E.g. n=0 -> 96/48ms, n=6 -> 3/1.6s, n=14 -> 13/6.6min; ch0/1-10\r\n"); break;
        case 2: printf("E.g. n=0 -> 77/30ms, n=6 -> 2.5/1s, n=14 -> 10.5/4min; ch0/1-10\r\n"); break;
        case 5: printf("E.g. n=0 -> 30ms, n=6 -> 1s, n=14 -> 4min; ch0/0-3\r\n"); break;
        case 16:
        case 17: printf("E.g. n=0 -> 30ms, n=6 -> 1s, n=14 -> 4min\r\n"); break;
        default: return;
    }
#else   /* 2.4 GHz */
    printf("E.g. n=0 -> 30ms, n=6 -> 1s, n=14 -> 4min\r\n");
#endif
    printf("Enter scan duration (n = 0..14) and press Enter: ");
    for (i = 0; i < 3; i++)
    {
        input = sio_getchar();
        if ((input < '0') || (input > '9'))
        {
            break;
        }
        character_entered = true;
        input_char[i] = input;
    }
    scan_duration = atol(input_char);

    if ((character_entered == false) || (scan_duration > 15))
    {
        printf("\r\ninvalid scan duration\r\n");
        printf("\r\nPress any key to return to main menu.");
        sio_getchar();
        return;
    }

    scanning = true;
    printf("\r\nEnergy scaning (duration n=%d)... Wait until test is completed.\r\n", scan_duration);
    printf("Channel\tEnergy (dBm)\r\n");
    tal_pib_get(phyCurrentChannel, &channel_before_scan);
    tal_pib_get(phyChannelsSupported, (uint8_t *)&scan_channel_mask);
    /* Identify first channel */
    for (i = 0; i <= MAX_CHANNEL; i++)
    {
        if ((scan_channel_mask & ((uint32_t)1 << i)) > 0)
        {
            first_channel = i;
            scan_channel_mask &= ~((uint32_t)1 << i);
            break;
        }
    }
    tal_pib_set(phyCurrentChannel, (pib_value_t *)&first_channel);
    tal_ed_start(scan_duration);
}


/**
 * @brief Start CW transmission on current channel page
 */
#if ((TAL_TYPE != AT86RF230B) || ((TAL_TYPE == AT86RF230B) && (defined CW_SUPPORTED)))
static void start_cw_transmission(void)
{
    uint8_t channel;
    uint8_t input;

    tal_pib_get(phyCurrentChannel, &channel);

    printf("\r\nEnter continuous transmission mode; C = CW mode, P = PRBS mode: ");
    input = sio_getchar();
    input = toupper(input);
    switch (input)
    {
        case 'C':
#if (TAL_TYPE == AT86RF212)
            printf("\r\nContinuous wave (CW) transmission on channel %d (-0.1MHz)\r\n", channel);
#else
            printf("\r\nContinuous wave (CW) transmission on channel %d (-0.5MHz)\r\n", channel);
#endif
            tfa_continuous_tx_start(CW_MODE);
            break;

        case 'P':
            printf("\r\nContinuous wave (PBRS) transmission on channel %d\r\n", channel);
            tfa_continuous_tx_start(PRBS_MODE);
            break;

        default:
            printf("\r\nWrong character entered\r\n");
            return;
    }

    op_mode = CONTINOUS_TX_MODE;
    printf("Press any key to cancel continuous transmission.");
    sio_getchar();

    /* Stop CW transmission again */
    tfa_continuous_tx_stop();
    op_mode = TX_OP_MODE;

    print_main_menu();
}
#endif


/**
 * @brief Send an energy pulse on current channel page
 */
#if ((TAL_TYPE != AT86RF230B) || ((TAL_TYPE == AT86RF230B) && (defined CW_SUPPORTED)))
static void pulse_cw_transmission(void)
{
    uint8_t channel;

    op_mode = CONTINOUS_TX_MODE;
    tal_pib_get(phyCurrentChannel, &channel);
#if (TAL_TYPE == AT86RF212)
    printf("\r\nContinuous wave (CW) transmission on channel %d (-0.1MHz)\r\n", channel);
#else
    printf("\r\nContinuous wave (CW) transmission on channel %d (-0.5MHz)\r\n", channel);
#endif
    tfa_continuous_tx_start(CW_MODE);
    pal_timer_start(T_APP_TIMER,
                    50000,
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)stop_pulse_cb,
                    NULL);
}
#endif


/**
 * @brief Stop sending a CW signal on current channel page
 */
#if ((TAL_TYPE != AT86RF230B) || ((TAL_TYPE == AT86RF230B) && (defined CW_SUPPORTED)))
static void stop_pulse_cb(void *callback_parameter)
{
    /* Stop CW transmission again */
    tfa_continuous_tx_stop();
    op_mode = TX_OP_MODE;

    print_main_menu();

    /* Keep compiler happy. */
    callback_parameter = callback_parameter;
}
#endif


/**
 * User call back function for finished ED Scan
 */
void tal_ed_end_cb(uint8_t energy_level)
{
    uint8_t i;
    uint8_t channel;
#if (TAL_TYPE == AT86RF212)
    uint8_t page;
#endif

    /* Print result */
    tal_pib_get(phyCurrentChannel, &channel);

    /* Re-scale ED value to Pin(dBm) */
#if (TAL_TYPE == AT86RF212)
    float reg_val = (float)62/255 * energy_level;
    uint8_t p_in = (uint8_t)(99 - (reg_val * 1.03));
    printf("%d\t-%d  ", channel, p_in);
#elif (TAL_TYPE == ATMEGARF_TAL_1)
    float reg_val = (float)55/255 * energy_level;
    uint8_t p_in = (uint8_t)(90 - reg_val);
    printf("%d\t-%d  ", channel, p_in);
#elif (TAL_TYPE == AT86RF230B) || (TAL_TYPE == AT86RF231) || (TAL_TYPE == AT86RF232)
    float reg_val = (float)56/255 * energy_level;
    uint8_t p_in = (uint8_t)(91 - reg_val);
    printf("%d\t-%d  ", channel, p_in);
#endif

    for (i = 0; i < energy_level / 4; i++)
    {
        printf("X");
    }
    printf("\r\n");

    /* Check for next channel */
    channel = 0xFF;
    for (i = 0; i <= MAX_CHANNEL; i++)
    {
        if ((scan_channel_mask & ((uint32_t)1 << i)) > 0)
        {
            channel = i;
            scan_channel_mask &= ~((uint32_t)1 << i);
            break;
        }
#if (TAL_TYPE == AT86RF212)
        tal_pib_get(phyCurrentPage, &page);
        if (page == 5)
        {
            if (i == 4)
            {
                channel = 0xFF;
            }
        }
#endif
    }

    /* Check if all channels were scanned. */
    if (channel == 0xFF)
    {
        /* Set original channel. */
        tal_pib_set(phyCurrentChannel, (pib_value_t *)&channel_before_scan);
        scanning = false;
        printf("\r\nPress any key to return to main menu.");
        sio_getchar();
    }
    else
    {
        /* Scan next channel */
        tal_pib_set(phyCurrentChannel, (pib_value_t *)&channel);
        tal_ed_start(scan_duration);
    }
}


/**
 * @brief Sub-menu to get channel setting
 */
static void get_channel(void)
{
    char input_char[3]= {0, 0, 0};
    uint8_t i;
    uint8_t input;
    uint8_t channel;
#if (TAL_TYPE == AT86RF212)
    uint8_t ch_page;
#endif

#if (TAL_TYPE == AT86RF212)
    tal_pib_get(phyCurrentPage, &ch_page);
    if (ch_page == 5)
    {
        printf("\r\nEnter channel (0..3) and press 'Enter': ");
    }
    else
    {
        printf("\r\nEnter channel (0..10) and press 'Enter': ");
    }
#else
    printf("\r\nEnter channel (11..26) and press 'Enter': ");
#endif
    for (i = 0; i < 3; i++)
    {
        input = sio_getchar();
        if ((input < '0') || (input > '9'))
        {
            break;
        }
        input_char[i] = input;
    }

    channel = atol(input_char);
    if((uint32_t)TRX_SUPPORTED_CHANNELS & ((uint32_t)0x01 << channel))
    {
        if(pairing == true)
        {
            send_parameters_changed(CHANNEL, channel);
        }
        else
        {
            tal_pib_set(phyCurrentChannel, (pib_value_t *)&channel);
        }
    }
    else
    {
        printf("\r\nInvalid Channel... Press any key to return to main menu.");
        sio_getchar();
    }
}


/**
 * @brief Sub-menu to get channel page setting
 */
static void get_page(void)
{
#if (TAL_TYPE == AT86RF232)
    printf("\r\nOnly channel page 0 supported by AT86RF232.\r\n");
    printf("\r\nPress any key to return to main menu.\r\n");
    sio_getchar();
    return;
#else
    char input_char[3]= {0, 0, 0};
    uint8_t i;
    uint8_t input;
    uint8_t ch_page;

#if (TAL_TYPE == AT86RF212)
    printf("\r\nchannel page\tbrutto data rate (kbit/s)\r\n");
    printf("page\tch 0 or \tch 1-10\r\n");
    printf("0\t20 or \t\t40 (BPSK)\r\n");
    printf("2\t100 or \t\t250 (O-QPSK)\r\n");
    printf("5\t250 (Chinese band, ch 0-3)\r\n");
    printf("16 *)\t200 or \t\t500\r\n");
    printf("17 *)\t400 or \t\t1000\r\n");
    printf("18 *)\t500 (Chinese band, ch 0-3)\r\n");
    printf("19 *)\t1000 (Chinese band, ch 0-3)\r\n");
    printf("*) proprietary channel page\r\n");
    printf("\r\nEnter channel page (0, 2, 5, 16, 17, 18 or 19) and press 'Enter': ");
#endif
#if ((TAL_TYPE == AT86RF231) || (TAL_TYPE == ATMEGARF_TAL_1))
    printf("\r\nchannel page\tbrutto data rate (kbit/s)\r\n");
    printf("0\t\t250\r\n");
    printf("2 *)\t\t500\r\n");
    printf("16 *)\t\t1000\r\n");
    printf("17 *)\t\t2000\r\n");
    printf("*) proprietary channel page\r\n");
    printf("\r\nEnter channel page (0, 2, 16, or 17) and press 'Enter': ");
#endif

    for (i = 0; i < 3; i++)
    {
        input = sio_getchar();
        if ((input < '0') || (input > '9'))
        {
            break;
        }
        input_char[i] = input;
    }

    ch_page = atoi(input_char);
    switch (ch_page)
    {
        case 0:
        case 2:
        case 16:
        case 17:
        {
            if(pairing == true)
            {
                send_parameters_changed(CHANNEL_PAGE, ch_page);
            }
            else
            {
                tal_pib_set(phyCurrentPage, (pib_value_t *)&ch_page);
            }
            break;
        }
        default:
        {
            printf("\r\nInvalid Channel page... Press any key to return to main menu.");
            sio_getchar();
            break;
        }
    }
#endif  /* #if (TAL_TYPE == AT86RF232) */
}


/**
 * @brief Sub-menu to get transmit power value
 */
static void get_tx_pwr(void)
{
    char input_char[4]= {0, 0, 0, 0};
    uint8_t i;
    char input;
    uint8_t temp_var;
    int8_t tx_pwr;

    printf("\r\nEnter transmit power value (in dBm, e.g. \"-5\") and press 'Enter': ");
    for (i = 0; i < 4; i++)
    {
        input = sio_getchar();
        if (((input < '0') || (input > '9')) && (input != '-') && (input != '+'))
        {
            break;
        }
        input_char[i] = input;
    }
    tx_pwr = atoi(input_char);

    if(((tx_pwr >= 0) && (tx_pwr <= 3))||
       ((tx_pwr < 0) && (tx_pwr >= -17)))
    {
        if(pairing == true)
        {
            send_parameters_changed(TX_POWER, (uint8_t) tx_pwr);
        }
        else
        {
            temp_var = CONV_DBM_TO_phyTransmitPower(tx_pwr);
            tal_pib_set(phyTransmitPower, (pib_value_t *)&temp_var);
        }
    }
    else
    {
        printf("\r\nInvalid Power value... Press any key to return to main menu.");
        sio_getchar();
    }
}


/**
 * @brief Sub-menu to get amount of test frames
 */
static void get_number_test_frames(void)
{
    char input_char[10]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t i;
    uint8_t input;

    printf("\r\nEnter number (<= 2^32) of test frame and press 'Enter': ");
    for (i = 0; i < 10; i++)
    {
        input = sio_getchar();
        if ((input < '0') || (input > '9'))
        {
            break;
        }
        input_char[i] = input;
    }

    number_test_frames = atol(input_char);
}


/**
 * @brief Sub-menu to get test frames length
 */
static void get_frame_length(void)
{
    char input_char[4]= {0, 0, 0, 0};
    uint8_t i;
    uint8_t input;
    uint16_t length;

    printf("\r\nEnter length (%d .. 127) of test frame (incl. FCS) and press 'Enter': ", (FRAME_OVERHEAD+1));/* 1=> cmdID*/
    for (i = 0; i < 4; i++)
    {
        input = sio_getchar();
        if ((input < '0') || (input > '9'))
        {
            break;
        }
        input_char[i] = input;
    }

    length = atoi(input_char);

    /* Check for maximum allowed IEEE 802.15.4 frame length. */
    if (length > aMaxPHYPacketSize)
    {
        phy_frame_length = aMaxPHYPacketSize;
    }
    else if(length < (FRAME_OVERHEAD+1)) /* 1=> cmdID*/
    {
        phy_frame_length = (FRAME_OVERHEAD+1);
    }
    else
    {
        phy_frame_length = (uint8_t)length;
    }

    /* The FCF has to be udpated. */
    configure_frame_sending();
}


/**
 * @brief Support function to toggle ACK request value
 */
static void toggle_ack_request(void)
{
    if (ack_request)
    {
        ack_request = false;
    }
    else
    {
        ack_request = true;
    }
}


/**
 * @brief Support function to toggle CSMA usage value
 */
static void toggle_csma_enabled(void)
{
    if (csma_enabled)
    {
        csma_enabled = false;
    }
    else
    {
        csma_enabled = true;
    }
}


/**
 * @brief Support function to toggle retry value
 */
static void toggle_retry_enabled(void)
{
    if (retry_enabled)
    {
        retry_enabled = false;
    }
    else
    {
        retry_enabled = true;;
    }
}


#ifdef ANTENNA_DIVERSITY
/**
 * @brief Support function toggling antenna diversity
 */
static void toogle_antenna_diversity(void)
{
    uint8_t antenna_sel;
    if (antenna_diversity)
    {
        antenna_diversity = false;
        do
        {
            antenna_sel = choose_antenna();
        }while(antenna_sel == 0xFF);

        if(antenna_sel == 1)
        {
            /* Enable A1/X2 */
            pal_trx_bit_write(SR_ANT_CTRL, ENABLE_ANTENNA_1);
        }
        else
        {
            /* Enable A2/X3 */
            pal_trx_bit_write(SR_ANT_CTRL, ENABLE_ANTENNA_0);
        }
        pal_trx_bit_write(SR_ANT_DIV_EN, ANT_DIV_DISABLE);
        pal_trx_bit_write(SR_PDT_THRES, THRES_ANT_DIV_DISABLED);
    }
    else
    {
        antenna_diversity = true;
        pal_trx_bit_write(SR_ANT_CTRL, ANTENNA_DEFAULT);
        pal_trx_bit_write(SR_PDT_THRES, THRES_ANT_DIV_ENABLE);
        pal_trx_bit_write(SR_ANT_DIV_EN, ANT_DIV_ENABLE);
        pal_trx_bit_write(SR_ANT_EXT_SW_EN, ANT_EXT_SW_SWITCH_ENABLE);
    }
}

static uint8_t choose_antenna(void)
{
    uint8_t input;
    printf("\r\nChoose antenna => Enter '1' for A1/X2 or '2' for A2/X3 : ");
    input = sio_getchar();

    switch(input)
    {
        case '1':
        case '2':
        {
            return(input - 0x30);
        }
        default:
        {
            return 0xFF;
        }
    }
}
#endif


/**
 * @brief Sub-menu to print test result
 */
static void print_result(void)
{
    uint32_t duration;
    float data_volume;
    float duration_s;
    float data_rate;

    duration = pal_sub_time_us(end_time, start_time);
    printf(" Done.\r\n\r\nTest result:\r\n");
    printf("Test duration = %0.6f s\r\n", (double)((float)duration/1000000));
    printf("Transmitted frames = %" PRIu32 ", invalid frames = %" PRIu32 "\r\n", number_test_frames, frame_failure);

    if (ack_request)
    {
        printf("Frames w/o ACK = %" PRIu32 "\r\n", frame_no_ack);
    }
    if (csma_enabled)
    {
        printf("Channel access failures = %" PRIu32 "\r\n", frame_access_failure);
    }

    data_volume = phy_frame_length * number_test_frames * 8;
    duration_s = (float)duration / 1000000;
    data_rate = data_volume / duration_s / 1000;

    printf("Net data rate = %.2f kbit/s", (double)data_rate);
}


/**
 * @brief Sub-menu to print sensor data
 */
static void get_sensor_data(void)
{
    uint16_t bat_mon;
#if (PAL_GENERIC_TYPE == MEGA_RF)
    double temperature;
#endif

    bat_mon = tfa_get_batmon_voltage();
    printf("\r\nBattery monitor: U = %" PRIu16 ".%" PRIu16 " V\r\n", bat_mon/1000, bat_mon%1000);

#if (PAL_GENERIC_TYPE == MEGA_RF)
    temperature = tfa_get_temperature();
    printf("Temperature: T = %2.1f C\r\n", temperature);
#endif
}

/**
 * @brief Start the test procedure
         */
static void start_test(void)
{
    if (op_mode == TX_OP_MODE)
    {
        printf("\r\nTransmitting... Wait until test is completed.");

        frames_to_transmit = number_test_frames;
        frame_no_ack = 0;
        frame_access_failure = 0;
        frame_failure = 0;
        configure_frame_sending();
        pal_get_current_time(&start_time);
    }
    else
    {
        printf("\r\nError... Press any key to return.\r\n");
        sio_getchar();
    }
}

/**
 * @brief Configure the frame sending
 */
static void configure_frame_sending(void)
{
    uint16_t temp_value_16;
    uint8_t mac_frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;
    app_payload_t *tmp;

    /*
     * Fill in PHY frame.
     *
     * Also set TAL PIBs:
     * Use: retval_t tal_pib_set(uint8_t attribute, uint8_t *value);
     */
    /* Get length of current frame. */
    mac_frame_length = (phy_frame_length  - FRAME_OVERHEAD);

    /* Set payload pointer. */
    frame_ptr = temp_frame_ptr =
                (uint8_t *)tx_frame_info +
                LARGE_BUFFER_SIZE -
                mac_frame_length - 2; /* Add 2 octets for FCS. */

    tmp = (app_payload_t*) temp_frame_ptr;

    (tmp->u8CmdID) = PER_TEST_PKT;

    temp_frame_ptr++;

    /*
     * Assign dummy payload values.
     * Payload is stored to the end of the buffer avoiding payload copying by TAL.
     */
    for (uint8_t i = 0; i < (mac_frame_length-1); i++)/* 1=> cmd ID */
    {
        *temp_frame_ptr++ = i; // dummy value
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
    *frame_ptr = (uint8_t)rand();
    /* Set the FCF. */
    fcf |= FCF_FRAMETYPE_DATA | FCF_SET_SOURCE_ADDR_MODE(FCF_SHORT_ADDR) |
                                FCF_SET_DEST_ADDR_MODE(FCF_SHORT_ADDR);
    if (ack_request)
    {
        fcf |= FCF_ACK_REQUEST;
    }
    frame_ptr -= 2;
    convert_16_bit_to_byte_array(fcf, frame_ptr);

    /* First element shall be length of PHY frame. */
    frame_ptr--;
    *frame_ptr = phy_frame_length;

    /* Finished building of frame. */
    tx_frame_info->mpdu = frame_ptr;
}

/**
 * @brief Function used to send the parameters which has been updated.
 *
 * @param parm Parameters being modified i.e. Channel, channel page, etc
 * @param val Value of the parameter being modified
 *
 */
static void send_parameters_changed(parameters_t parm, uint8_t val)
{
    uint8_t i;

    uint8_t payload_length;
    uint8_t *data_tmp;

    app_payload_t msg;
    set_parm_req_t *data;

    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        op_mode =SET_PARAMETER;

        /* maitain a copy of the parameter to be used to set on this node */
        set_parm_cb.parm = parm;
        set_parm_cb.value = val;

        /* Create the payload */
        msg.u8CmdID = SET_PARAM;
        seq_num++;
        msg.u8SeqNum = seq_num;
        data = (set_parm_req_t *)msg.payload;
        data->parmameter = parm;
        data->value = val;

        payload_length = ((sizeof(app_payload_t) -
                          sizeof(general_pkt_u)) +
                          sizeof(set_parm_req_t));

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
            /* load the payload */
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

        /* send the frame on air */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);
    }
}

/**
 * @brief Function used to request PER test result.
 *
 */
static bool send_result_req(void)
{
    uint8_t i;

    uint8_t payload_length;
    uint8_t *data_tmp;

    app_payload_t msg;
    result_req_t *data;

    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        /* Create the payload */
        msg.u8CmdID = RESULT_REQ;
        seq_num++;
        msg.u8SeqNum = seq_num;
        data = (result_req_t *)msg.payload;
        /* Just a dummy value */
        data->Cmd = 0xAA;

        payload_length = ((sizeof(app_payload_t) -
                          sizeof(general_pkt_u)) +
                          sizeof(result_req_t));

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
            /* load the payload */
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

        /* send the frame on air */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);

        return(true);
    }
    return(false);
}

/**
 * @brief Function used to send PER test result.
 *
 */
static void send_result_rsp(void)
{
    uint8_t i;

    uint8_t payload_length;
    uint8_t *data_tmp;

    app_payload_t msg;
    result_rsp_t *data;

    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        /* Create the payload */
        msg.u8CmdID = RESULT_RSP;
        seq_num++;
        msg.u8SeqNum = seq_num;
        data = (result_rsp_t *)msg.payload;
        data->num_of_frames_rx = number_rx_frames;
        data->lqi_avrg_rx = aver_lqi;

        payload_length = ((sizeof(app_payload_t) -
                          sizeof(general_pkt_u)) +
                          sizeof(result_rsp_t));

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
            /* load the payload */
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

        /* send the frame on air */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);
    }
}

#ifdef ANTENNA_DIVERSITY
/**
 * @brief Function used to query diversity settings in remote node
 */
static void diversity_settings_remote_node(void)
{
    if(send_diversity_status_req())
    {
        pal_timer_start(APP_TIMER_TO_TX,
                            TIMEOUT_FOR_RESPONSE_IN_MICRO_SEC,
                            TIMEOUT_RELATIVE,
                            (FUNC_PTR)wait_for_reply_timer_handler_cb,
                            NULL);
        op_mode = DIVERSITY_STATUS_REQ;
    }
    else
    {
        printf("\r\nDiversity query failed");
        printf("\r\nPress any key to continue");
        sio_getchar();
        op_mode = TX_OP_MODE;
    }
}

/**
 * @brief Function used to request diversity settings in remote node
 */
static bool send_diversity_status_req(void)
{
    uint8_t i;

    uint8_t payload_length;
    uint8_t *data_tmp;

    app_payload_t msg;
    div_stat_req_t *data;

    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        /* Create the payload */
        msg.u8CmdID = DIV_STAT_REQ;
        seq_num++;
        msg.u8SeqNum = seq_num;
        data = (div_stat_req_t *)msg.payload;

        /* Dummy value */
        data->status = 0xAA;

        payload_length = ((sizeof(app_payload_t) -
                          sizeof(general_pkt_u)) +
                          sizeof(div_stat_req_t));

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
            /* load the payload */
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

        /* send the frame on air */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);

        return(true);
    }
    return(false);
}

/**
 * @brief Function used to send diversity settings on remote node to host node
 */
static void send_diversity_status_rsp(void)
{
    uint8_t i;

    uint8_t payload_length;
    uint8_t *data_tmp;

    app_payload_t msg;
    div_stat_rsp_t *data;

    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        /* Create the payload */
        msg.u8CmdID = DIV_STAT_RSP;
        seq_num++;
        msg.u8SeqNum = seq_num;
        data = (div_stat_rsp_t *)msg.payload;

        if(pal_trx_bit_read(SR_ANT_DIV_EN))
        {
            data->status = 1;
            data->ant_sel = 0;
        }
        else
        {
            data->status = 0;
            data->ant_sel = pal_trx_bit_read(SR_ANT_CTRL);
        }

        payload_length = ((sizeof(app_payload_t) -
                          sizeof(general_pkt_u)) +
                          sizeof(div_stat_rsp_t));

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
            /* load the payload */
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

        /* send the frame on air */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);
    }
}

/**
 * @brief Function used to change diversity settings on remote node
 *
 * @param div_msg Diversity settings for the remote node
 *
 */
static bool send_diversity_set_req(div_set_req_t div_msg)
{
    uint8_t i;
    uint8_t payload_length;
    uint8_t *data_tmp;
    app_payload_t msg;
    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        /* Create the payload */
        msg.u8CmdID = DIV_SET_REQ;
        seq_num++;
        msg.u8SeqNum = seq_num;
        msg.payload[0].div_set_req_data = div_msg;;

        payload_length = ((sizeof(app_payload_t) -
                          sizeof(general_pkt_u)) +
                          sizeof(div_set_req_t));

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
            /* load the payload */
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

        /* send the frame on air */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);

        return(true);
    }
    return(false);
}
#endif /* #ifdef ANTENNA_DIVERSITY */

/**
 * @brief Function used to send identify paired node command
 *
 */
static void send_identify_command(void)
{
    uint8_t i;
    uint8_t payload_length;
    uint8_t *data_tmp;
    app_payload_t msg;
    uint16_t temp_value_16;
    uint8_t frame_length;
    uint8_t *frame_ptr;
    uint8_t *temp_frame_ptr;
    uint16_t fcf = 0;

    if((transmitting == false))
    {
        transmitting = true;

        /* Create the payload */
        msg.u8CmdID = IDENTIFY_NODE;
        seq_num++;
        msg.u8SeqNum = seq_num;

        payload_length = ((sizeof(app_payload_t) -
                          sizeof(general_pkt_u)));

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
            /* load the payload */
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

        /* send the frame on air */
        tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, true);
        return;
    }
    printf("\r\nIdentify failed to send");
    printf("\r\nPress any key to continue");
    sio_getchar();
    op_mode = TX_OP_MODE;
    return;
}

/**
 * @brief Wait for reply timer handler is called if any command sent on air
 *        times out before any response message is received. This function
 *        handles all the scenarios if the remote node is not accessible
 *
 * @param parameter pass parameters to timer handler
 */
static void wait_for_reply_timer_handler_cb(void *parameter)
{
    printf("\r\nUnable to get");

    switch(op_mode)
    {
        case WAIT_FOR_TEST_RES:
        {
            printf(" test results");
            break;
        }
#ifdef ANTENNA_DIVERSITY
        case DIVERSITY_STATUS_REQ:
        {
            printf(" diversity status of remote node");
            break;
        }
#endif /* #ifdef ANTENNA_DIVERSITY */
        default:
            break;
    }
    /* notifies remote node has problems */
    printf("\r\nPlease check the remote node");
    printf("\r\nPress any key to continue");
    sio_getchar();
    op_mode = TX_OP_MODE;
    parameter = parameter;
}

/**
 * @brief This timer handler is called if identify command is received on air
 *        by a node in reflector configuration. This is used to blink the LED
 *        and thus identify itself
 *
 * @param parameter pass parameters to timer handler
 */
static void identify_timer_handler_cb(void *parameter)
{
    static uint8_t led_count;
    parameter = parameter;

    if(led_count > 20)
    {
        led_count = 0;
        app_set_led_value(LED_MODE_2);
    }
    else
    {
        if(led_count & 0x01)
        {
            led_count++;
            app_set_led_value(LED_MODE_2);
            pal_timer_stop(APP_TIMER_TO_TX);
        }
        else
        {
            led_count++;
            app_set_led_value(3);
        }
        pal_timer_start(APP_TIMER_TO_TX,
                        LED_BLINK_RATE_IN_MICRO_SEC,
                        TIMEOUT_RELATIVE,
                        (FUNC_PTR)identify_timer_handler_cb,
                        NULL);
    }
    return;
}

/* EOF */
