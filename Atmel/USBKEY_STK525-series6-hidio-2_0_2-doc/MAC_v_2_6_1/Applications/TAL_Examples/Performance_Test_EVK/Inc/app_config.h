/**
 * @file app_config.h
 *
 * @brief These are application-specific resources which are used
 *        in the application in addition to the underlaying stack.
 *
 * $Id: app_config.h 24880 2011-01-13 07:11:54Z felix.varghese $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* === Includes ============================================================= */

#include "stack_config.h"
#include "tal.h"

/* === Macros =============================================================== */

/* Version of the software */
#define MAJOR_NUMBER        1
#define MINOR_NUMBER        0

#if (NUMBER_OF_TOTAL_STACK_TIMERS == 0)
#define APP_FIRST_TIMER_ID          (0)
#else
#define APP_FIRST_TIMER_ID          (LAST_STACK_TIMER_ID + 1)
#endif

#if (TAL_TYPE == AT86RF212)
#define DEFAULT_CHANNEL         (1)
#else
#define DEFAULT_CHANNEL         (20)
#endif
#define DEFAULT_PAN_ID          (0xCAFE)
#define DST_PAN_ID              (DEFAULT_PAN_ID)
#define SRC_PAN_ID              (DEFAULT_PAN_ID)
#define DEFAULT_ADDR            (0x0001)
#define DST_SHORT_ADDR          (0xFFFF)

/* Frame overhead due to selected address scheme incl. FCS */
#if (DST_PAN_ID == SRC_PAN_ID)
#define FRAME_OVERHEAD          (11)
#else
#define FRAME_OVERHEAD          (13)
#endif

#define DEFAULT_SCAN_DURATION   8

#define OPERATION_MODE_1        0x01
#define OPERATION_MODE_2        0x02

#define LED_ALL_OFF     0
#define LED_STARTUP     2
#define LED_MODE_1      1
#define LED_MODE_2      4

/* === Types ================================================================ */

typedef enum
{
  OP_MODE_UNKNOWN,
  OP_MODE_1_KEY_PRESS,
  OP_MODE_2_CHAR_ON_UART,
  OP_MODE_1_RX_FRAME,
  OP_MODE_2_RX_FRAME
}startup_mode_e;

/* Timer ID's used by the Application */
typedef enum
{
    /* App Timers start from APP_FIRST_TIMER_ID */

    /* App timer 1 */
    T_APP_TIMER = (APP_FIRST_TIMER_ID),
    APP_TIMER_TO_TX
} app_timer_t;

#define NUMBER_OF_APP_TIMERS        (2)

#define TOTAL_NUMBER_OF_TIMERS      (NUMBER_OF_APP_TIMERS + NUMBER_OF_TOTAL_STACK_TIMERS)

/* Additional buffers used by the application */
#define NUMBER_OF_LARGE_APP_BUFS    (10)
#define NUMBER_OF_SMALL_APP_BUFS    (0)

#define TOTAL_NUMBER_OF_LARGE_BUFS  (NUMBER_OF_LARGE_APP_BUFS + NUMBER_OF_LARGE_STACK_BUFS)
#define TOTAL_NUMBER_OF_SMALL_BUFS  (NUMBER_OF_SMALL_APP_BUFS + NUMBER_OF_SMALL_STACK_BUFS)

#define TOTAL_NUMBER_OF_BUFS        (TOTAL_NUMBER_OF_LARGE_BUFS + TOTAL_NUMBER_OF_SMALL_BUFS)

/*
 * UART transmit buffer size
 */
#define UART_MAX_TX_BUF_LENGTH      (10)

/*
 * UART receive buffer size
 */
#define UART_MAX_RX_BUF_LENGTH      (10)

/*
 * USB transmit buffer size
 */
#define USB_TX_BUF_SIZE             (10)

/*
 * USB receive buffer size
 */
#define USB_RX_BUF_SIZE             (10)

/* Offset of IEEE address storage location within EEPROM */
#define EE_IEEE_ADDR                (0)

/* === Externals ============================================================ */
extern void (*app_task)(void);
extern void (*app_tx_frame_done_cb)(retval_t status, frame_info_t *frame);
extern void (*app_rx_frame_cb)(frame_info_t *frame);

extern frame_info_t *tx_frame_info;

#if (PAL_GENERIC_TYPE == MEGA_RF)
    #if (BOARD_TYPE == RCB_6_3_PLAIN_OR_STB)
    extern board_t board ;
    #endif
#endif

/* === Prototypes =========================================================== */
extern uint8_t app_debounce_button(void);
extern void app_set_led_value(uint8_t v);
extern void app_task_startup_init();

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_CONFIG_H */
/* EOF */
