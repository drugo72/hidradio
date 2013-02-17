/**
 * @file pal_uart.h
 *
 * @brief PAL uart internal functions prototypes
 *
 * This header contains the function prototypes for transmit,
 * receive functions and macros used in UART module.
 *
 * $Id: pal_uart.h 24759 2011-01-06 13:50:26Z mahendran.p $
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
#ifndef PAL_UART_H
#define PAL_UART_H

/* === Includes ============================================================= */

#include "app_config.h"

/* === Types ================================================================ */

#if ((defined UART0) || (defined UART1))

/*
 * Structure containing the transmit and receive buffer
 * and also the buffer head, tail and count
 */
typedef struct uart_communication_buffer_tag
{
    /* Transmit buffer */
    uint8_t tx_buf[UART_MAX_TX_BUF_LENGTH];

    /* Receive buffer */
    uint8_t rx_buf[UART_MAX_RX_BUF_LENGTH];

    /* Transmit buffer head */
    uint8_t tx_buf_head;

    /* Transmit buffer tail */
    uint8_t tx_buf_tail;

    /* Receive buffer head */
    uint8_t rx_buf_head;

    /* Receive buffer tail */
    uint8_t rx_buf_tail;

    /* Number of bytes in transmit buffer */
    uint8_t tx_count;

    /* Number of bytes in receive buffer */
    uint8_t rx_count;

} uart_communication_buffer_t;

/* Irq handlers of UART */
typedef void (*uart_irq_handler_t) (void);

/* === Externals ============================================================ */


/* === Macros =============================================================== */

/* UART baud rate of in bits per second. */
#define UART_BAUD_115k2               (115200)

#define DISABLE_ALL_INTERRUPTS       (0xFFFFFFFF)
/*
 * Value to be loaded in the SCBR register of UART to obtain a baud rate of
 * 9600 bps.
 * As given in datasheet Baud Rate = Selected Clock / 16 /CD
 * hence CD = Baud Rate / Selected Clock / 16
 */
#define UART_BAUD_DIVISOR           (uint16_t) ((F_CPU / UART_BAUD_115k2) / 16)

/* UART0 */
#ifdef UART0

/* UART Base */
#define UART0_BASE                 (UART_0)

/* Enables the RX interrupt of UART0 */
#define ENABLE_UART_0_RX_INT()      (UART0_BASE->UART_IER = UART_IER_RXRDY)

/* Enables the TX interrupt of UART0 */
#define ENABLE_UART_0_TX_INT()      (UART0_BASE->UART_IER = UART_IER_TXRDY)

/* Disables the RX interrupt of UART0 */
#define DISABLE_UART_0_RX_INT()     (UART0_BASE->UART_IDR = UART_IDR_RXRDY)

/* Disables the TX interrupt of UART0 */
#define DISABLE_UART_0_TX_INT()     (UART0_BASE->UART_IDR = UART_IDR_TXRDY)

/* Writes data in the UART data register */
#define WRITE_TO_UART_0(tx_data)   do {                             \
    /*                                                              \
     * Data is written to tranmsit register only if there are is    \
     * no ongoing data transmission.                                \
     */                                                             \
    while ((UART0_BASE->UART_SR & UART_SR_TXEMPTY) == 0);       \
    UART0_BASE->UART_THR = tx_data;                               \
} while (0);

/* Writes data in the UART data register */
#define READ_FROM_UART_0(rx_data)   do {                            \
    /*                                                              \
     * Reading  from receive register is held until there is some   \
     * data received in the receive register.                       \
     */                                                             \
    while ((UART0_BASE->UART_SR & UART_SR_RXRDY) == 0);         \
    rx_data = UART0_BASE->UART_RHR;                               \
} while (0);

/** Pins used for UART transfer */
#define PINS_UART0      PIN_UART0_TXD, PIN_UART0_RXD

#endif  /* UART0 */

/* UART1 */
#ifdef UART1

/* UART Base */
#define UART1_BASE                 (UART_1)

/* Enables the RX interrupt of UART0 */
#define ENABLE_UART_1_RX_INT()      (UART1_BASE->UART_IER = UART_IER_RXRDY)

/* Enables the TX interrupt of UART0 */
#define ENABLE_UART_1_TX_INT()      (UART1_BASE->UART_IER = UART_IER_TXRDY)

/* Disables the RX interrupt of UART0 */
#define DISABLE_UART_1_RX_INT()     (UART1_BASE->UART_IDR = UART_IDR_RXRDY)

/* Disables the TX interrupt of UART0 */
#define DISABLE_UART_1_TX_INT()     (UART1_BASE->UART_IDR = UART_IDR_TXRDY)

/* Writes data in the UART data register */
#define WRITE_TO_UART_1(tx_data)   do {                             \
    /*                                                              \
     * Data is written to tranmsit register only if there are is    \
     * no ongoing data transmission.                                \
     */                                                             \
    while ((UART1_BASE->UART_SR & UART_SR_TXEMPTY) == 0);       \
    UART1_BASE->UART_THR = tx_data;                               \
} while (0);

/* Writes data in the UART data register */
#define READ_FROM_UART_1(rx_data)   do {                            \
    /*                                                              \
     * Reading  from receive register is held until there is some   \
     * data received in the receive register.                       \
     */                                                             \
    while ((UART1_BASE->UART_SR & UART_SR_RXRDY) == 0);         \
    rx_data = UART1_BASE->UART_RHR;                               \
} while (0);

/** Pins used for UART transfer */
#define PINS_UART1      PIN_UART1_RXD, PIN_UART1_TXD

#endif  /* UART1 */

/* === Prototypes =========================================================== */
#ifdef __cplusplus
extern "C" {
#endif

void sio_uart_0_init(uint32_t baud_rate);
void sio_uart_1_init(uint32_t baud_rate);
uint8_t sio_uart_0_rx(uint8_t *data, uint8_t max_length);
uint8_t sio_uart_1_rx(uint8_t *data, uint8_t max_length);
uint8_t sio_uart_0_tx(uint8_t *data, uint8_t length);
uint8_t sio_uart_1_tx(uint8_t *data, uint8_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #if ((defined UART0) || (defined UART1)) */

#endif /* PAL_UART_H */
/* EOF */
