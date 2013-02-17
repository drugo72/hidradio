/**
 * @file pal_config.h
 *
 * @brief PAL configuration for AVR32 AT32UC3A3256
 *
 * This header file contains configuration parameters for AVR32 AT32UC3A3256.
 *
 * $Id: pal_config.h 25120 2011-01-25 08:11:55Z yogesh.bellan $
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
#ifndef PAL_CONFIG_H
#define PAL_CONFIG_H

/* === Includes =============================================================*/
#include "avr32types.h"
#include "pal_boardtypes.h"


#if (BOARD_TYPE == REB_2_3_STK600)

/*
 * This header file is required since a function with
 * return type retval_t is declared
 */
#include "return_val.h"

/* === Types ================================================================*/

/* Enumerations used to idenfify LEDs */
typedef enum led_id_tag
{
    LED_0,
    LED_1,
    LED_2
} led_id_t;

/* Enumerations used to idenfify buttons */
typedef enum button_id_tag
{
    BUTTON_0
} button_id_t;

/* === Externals ============================================================*/


/* === Macros ===============================================================*/
#define F_CPU           (30000000UL)                             /* CPU clock frequency.*/
#define FOSC0           (12000000UL)                               /* Osc0 frequency: Hz.*/
#define OSC0_STARTUP    AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC     /* Osc0 startup time: RCOsc periods.*/

/*
 * LEDs on STK600
 */
#define LED0_PIN                        AVR32_PIN_PA00
#define LED1_PIN                        AVR32_PIN_PA01
#define LED2_PIN                        AVR32_PIN_PA02


/*
 * PINs where buttons are connected
 */
#define BUTTON_PIN_0                    (AVR32_PIN_PA21)   /* Button 0 pin */

/*
 * RESET pin of transceiver
 */
#define TRX_RST                         (AVR32_PIN_PA17)

/*
 * Bus Selection for peripherals
 */
#define SPI_PBA_BUS_SEL                 (0x20)
#define USART_PBA_BUS_SEL               (0x01)
#define TIMER_PBA_BUS_SEL               (0x08)

/*
 * Reset pin low
 */
#define RST_LOW()                       do {                                        \
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[TRX_RST >> 5];     \
        gpio_port->ovrc  = 1 << (TRX_RST & 0x1F);                                   \
        gpio_port->oders = 1 << (TRX_RST & 0x1F);                                   \
        gpio_port->gpers = 1 << (TRX_RST & 0x1F);                                   \
} while (0);

/*
 * Reset pin high
 */
#define RST_HIGH()                      do {                                        \
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[TRX_RST >> 5];     \
        gpio_port->ovrs  = 1 << (TRX_RST & 0x1F);                                   \
        gpio_port->oders = 1 << (TRX_RST & 0x1F);                                   \
        gpio_port->gpers = 1 << (TRX_RST & 0x1F);                                   \
} while (0);


/*
 * Sleep Transceiver pin
 */
#define SLP_TR                          (AVR32_PIN_PA19)


/*
 * Reset pin low
 */
#define SLP_TR_LOW()                       do {                                    \
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[SLP_TR >> 5];     \
        gpio_port->ovrc  = 1 << (SLP_TR & 0x1F);                                   \
        gpio_port->oders = 1 << (SLP_TR & 0x1F);                                   \
        gpio_port->gpers = 1 << (SLP_TR & 0x1F);                                   \
} while (0);

/*
 * Reset pin high
 */
#define SLP_TR_HIGH()                      do {                                        \
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[SLP_TR >> 5];     \
        gpio_port->ovrs  = 1 << (SLP_TR & 0x1F);                                   \
        gpio_port->oders = 1 << (SLP_TR & 0x1F);                                   \
        gpio_port->gpers = 1 << (SLP_TR & 0x1F);                                   \
} while (0);



/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()           DISABLE_TRX_IRQ()


/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()           ENABLE_TRX_IRQ()


/*
 * SPI Pins settings for ATAVR32UC3A3256 & AT86RF231
 */
#define RF_SPI_DIV                      (4)
#define RF_SPI                          (AVR32_SPI0)
#define RF_SPI_CS_REG                   (AVR32_SPI0.csr0)
#define RF_SPI_NPCS                     (0)
#define RF_SPI_NCPS_MASK                (0x0E << 16)
#define SEL                             (AVR32_PIN_PA09)
#define MOSI                            (AVR32_SPI0_MOSI_0_0_PIN)
#define MOSI_FUNCTION                   (AVR32_SPI0_MOSI_0_0_FUNCTION)
#define MISO                            (AVR32_SPI0_MISO_0_0_PIN)
#define MISO_FUNCTION                   (AVR32_SPI0_MISO_0_0_FUNCTION)
#define SCK                             (AVR32_SPI0_SCK_0_0_PIN)
#define SCK_FUNCTION                    (AVR32_SPI0_SCK_0_0_FUNCTION)


/*
 * Slave select made low
 */
#define SS_LOW()                        do {                                    \
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[SEL >> 5];     \
        gpio_port->ovrc  = 1 << (SEL & 0x1F);                                   \
        gpio_port->oders = 1 << (SEL & 0x1F);                                   \
        gpio_port->gpers = 1 << (SEL & 0x1F);                                   \
} while (0);

/*
 * Slave select made high
 */
#define SS_HIGH()                       do {                                    \
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[SEL >> 5];     \
        gpio_port->ovrs  = 1 << (SEL & 0x1F);                                   \
        gpio_port->oders = 1 << (SEL & 0x1F);                                   \
        gpio_port->gpers = 1 << (SEL & 0x1F);                                   \
} while (0);


/*
 * UART related settings for ATAVR32UC3A3256
 */
#define SYS_F_UART                           (30000000UL)
#define USART_BR                             (0x0D)
#define USART0                               (AVR32_USART0)
#define USART1                               (AVR32_USART1)
#define USART2                               (AVR32_USART2)
#define USART3                               (AVR32_USART3)

/*Selection of Uart*/
#if(defined UART0)
#define USART                                USART0
#define USART_ISR_GROUP                      (5)
#define USART_ISR_PRIORITY                   (1)
#define USART_RX                             (AVR32_USART0_RXD_0_1_PIN)
#define USART_RX_FUNCTION                    (AVR32_USART0_RXD_0_1_FUNCTION)
#define USART_TX                             (AVR32_USART0_TXD_0_1_PIN)
#define USART_TX_FUNCTION                    (AVR32_USART0_TXD_0_1_FUNCTION)

#elif (defined UART1)
#define USART                                USART1
#define USART_ISR_GROUP                      (6)
#define USART_ISR_PRIORITY                   (1)
#define USART_RX                             (AVR32_USART1_RXD_0_1_PIN)
#define USART_RX_FUNCTION                    (AVR32_USART1_RXD_0_1_FUNCTION)
#define USART_TX                             (AVR32_USART1_TXD_0_1_PIN)
#define USART_TX_FUNCTION                    (AVR32_USART1_TXD_0_1_FUNCTION)

#elif (defined UART2)
#define USART                                USART2
#define USART_ISR_GROUP                      (7)
#define USART_ISR_PRIORITY                   (1)
#define USART_RX                             (AVR32_USART2_RXD_0_1_PIN)
#define USART_RX_FUNCTION                    (AVR32_USART2_RXD_0_1_FUNCTION)
#define USART_TX                             (AVR32_USART2_TXD_0_1_PIN)
#define USART_TX_FUNCTION                    (AVR32_USART2_TXD_0_1_FUNCTION)

#elif (defined UART3)
#define USART                                USART3
#define USART_ISR_GROUP                      (8)
#define USART_ISR_PRIORITY                   (1)
#define USART_RX                             (AVR32_USART3_RXD_0_1_PIN)
#define USART_RX_FUNCTION                    (AVR32_USART3_RXD_0_1_FUNCTION)
#define USART_TX                             (AVR32_USART3_TXD_0_1_PIN)
#define USART_TX_FUNCTION                    (AVR32_USART3_TXD_0_1_FUNCTION)
#endif


/*BaudRate settings of the uart*/
#define USART_CH_MODE                        .chmode
#define USART_STOP_BIT                       .nbstop
#define USART_PARITY                         .par
#define USART_CHR1                           .chrl
#define USART_MODE                           .mode


/*
 * Timer related settings for AT32UC3A3256
 */
#define AVR32_PM_PLL0_PLLCOUNT_USER	  (16)
#define AVR32_PM_PLLOSC_USER		  (0)
#define AVR32_PM_PLL0_PLLDIV_SIZE_USER	  (1)
#define AVR32_PM_PLL0_PLLEN_USER    	  (1)
#define ZERO_INIT      			  (0)
#define ENABLE_FCR			  (1)
#define USB_CLOCK_ENABLE		  (1)
#define SPI_BUS				  (0)
#define TIMER_BUS			  (1)
#define UART_BUS			  (2)
#define PLL0_SELECT			  (0)


/*
 * Timer related settings for ATAVR32UC3A3256
 */
#define TC_CH1_ISR_GROUP        (14)
#define TC_CH2_ISR_GROUP        (16)
#define TC_CH1_ISR_PRIORITY     (1)
#define TC_CH2_ISR_PRIORITY     (1)
#define TC                      (AVR32_TC0)
#define TC2                     (AVR32_TC1)
#define TC_CH0                  (AVR32_TC0.channel[0])
#define TC_CH1                  (AVR32_TC0.channel[1])
#define TC_CH2                  (AVR32_TC0.channel[2])
#define TC2_CH0                 (AVR32_TC1.channel[0])
#define TC2_CH1                 (AVR32_TC1.channel[1])
#define TIMER_COMP_REG          (TC2_CH0.ra)
#define TIMER_COMP_ENABLE_REG   (TC2_CH0.ier)
#define TIMER_COMP_DISABLE_REG  (TC2_CH0.idr)
#define TIMER_COMP_FLAG         (AVR32_TC_IER0_CPAS_MASK)
#define TIMER_CHANNEL_COUNT     (TC2_CH0.cv)

/*
 * IRQ macros for ATAVR32UC3A3512
 */

/*
 * AT86RF231:
 *
 * TRX_MAIN_IRQ_HDLR_IDX        TRX interrupt (PORTC Pin2)
 * TRX_TSTAMP_IRQ_HDLR_IDX      Time stamping interrupt (PORTC Pin1,
 *                              only if Antenna Diversity is not used)
 */

/*
 * Timer related settings for ATAVR32UC3A0512
 */
#define EXT_INT_ISR_GROUP               (2)
#define EXT_INT_ISR_PRIORITY            (1)
#define EXT_INT                         (AVR32_PIN_PA20)

/**
 * GPIO pins
 */
typedef enum gpio_pin_type_tag
{
    RST_PIN,
    SLP_TR_PIN
} gpio_pin_type_t;



#define ENABLE_TRX_IRQ() do {\
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[EXT_INT >> 5];\
        gpio_port->iers = 1 << (EXT_INT & 0x1F);\
}while(0)

/* Enables the transceiver interrupts */
#define ENABLE_TRX_IRQ_TSTAMP()     do {\
      TC_CH2.ier = AVR32_TC_IER0_LDRAS_MASK ;\
      }while(0)


#define DISABLE_TRX_IRQ() do{                                                   \
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[EXT_INT >> 5]; \
        gpio_port->ierc = 1 << (EXT_INT & 0xFF);                                \
    }while(0)

#define DISABLE_TRX_IRQ_TSTAMP() do{                                            \
        TC_CH2.idr &= ~AVR32_TC_IER0_LDRAS_MASK ;                               \
    }while(0)

/* Disables the transceiver interrupts */
/* Clear the Timestamp interrupts */
#define CLEAR_TRX_IRQ_TSTAMP() do {\
        uint32_t status1 = TC_CH2.sr;                                           \
        status1 &= TC_CH2.imr;                                                  \
        }while(0)

/* Clear the transceiver interrupts */
#define CLEAR_TRX_IRQ() do {\
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[EXT_INT >> 5];     \
    gpio_port->ifrc = 1 << (EXT_INT & 0xFF);                                    \
}while(0)



/* Enables the global interrupt */
#define ENABLE_GLOBAL_IRQ()             sei()

/* Disables the global interrupt */
#define DISABLE_GLOBAL_IRQ()            cli()

/*
 * This macro saves the global interrupt status
 */
#define ENTER_CRITICAL_REGION()         {__istate_t sreg = __get_interrupt_state(); cli()

/*
 *  This macro restores the global interrupt status
 */
#define LEAVE_CRITICAL_REGION()         __set_interrupt_state(sreg);}

/*
 * Mask used to obtain the gloabl interrupt status bit in the status register
 * of the AT32UC3A3256
 */
#ifndef __ICCAVR__
#define GLOBAL_IRQ_MASK                 (CPU_I_bm)
#else
#define GLOBAL_IRQ_MASK                 (I_bm)
#endif


/*
 * Value of an external PA gain
 * If no external PA is available, value is 0.
 */
#define EXTERN_PA_GAIN                  (0)

/*
 * Timer macros for AT32UC3A3256
 */

/*
 * These macros are placeholders for delay functions for high speed processors.
 *
 * The following delay are not reasonbly implemented via delay functions,
 * but rather via a certain number of NOP operations.
 * The actual number of NOPs for each delay is fully MCU and frequency
 * dependent, so it needs to be updated for each board configuration.
 *
 * AT32UC3A3256 @ 12MHz
 */
/* Wait for 500 ns. */
#define PAL_WAIT_500_NS()               do {            \
        uint32_t ticks = ((F_CPU / 1000000U) / 2);      \
        ticks += get_sys_reg(AVR32_COUNT);              \
        while (get_sys_reg(AVR32_COUNT) < ticks) {;}    \
} while (0)


/* Wait for 1 us. */
#define PAL_WAIT_1_US()               do {              \
        uint32_t ticks = ((F_CPU / 1000000U));          \
        ticks += get_sys_reg(AVR32_COUNT);              \
        while (get_sys_reg(AVR32_COUNT) < ticks) {;}    \
} while (0)

/* Wait for 65 ns. */
#define  PAL_WAIT_65_NS()  {nop(); nop();}

#define FUNC_PTR    void *
/*
 * The smallest timeout in microseconds
 */
#define MIN_TIMEOUT                     (0x80)

/*
 * The largest timeout in microseconds
 */
#define MAX_TIMEOUT                     (0x7FFFFFFF)

/**
 * Minimum time in microseconds, accepted as a delay request
 */
#define MIN_DELAY_VAL                   (5)


/*
 * Maximum numbers of software timers running at a time
 */
#define MAX_NO_OF_TIMERS                (25)

/*
 * The maximum time count in microseconds for a 32 bit timer
 */
#define TIMER_MAX_COUNT_IN_US           (0xFFFFFFFF)

/*
 * Hardware register that holds Rx timestamp
 */
#define TIMER_HIGH_REGISTER             (TC_CH1.cv)
#define TIMER_LOW_REGISTER              (TC_CH2.ra)
#define TIMER_LOW_VALUE_REGISTER        (TC_CH2.cv)

/*
 * Hardware register that holds High Priority timers
 */
#define HIGH_PRIORITY_TIMER_COUNT         (TC2_CH1.cv)
#define HIGH_PRIORITY_TIMER_COMP          (TC2_CH1.ra)
#define HIGH_PRIORITY_TIMER_DISABLE_INT   (TC2_CH1.idr)
#define HIGH_PRIORITY_TIMER_ENABLE_INT    (TC2_CH1.ier)

/*
 * Dummy value written in SPDR to retrieve data form it
 */
#define SPI_DUMMY_VALUE                 (0x00)


/*
 * IEEE address of board in EEPROM
 */
#define EE_IEEE_ADDR                    (0)


/*
 * Alert initialization
 */
#define ALERT_INIT()                    do {    \
} while (0)

/*
 * Alert indication
 */
#define ALERT_INDICATE()


/* === Prototypes ===========================================================*/
#ifdef __cplusplus
extern "C" {
#endif
void gpio_init(void);
void gpio_clr_gpio_pin(unsigned int pin);
void gpio_set_gpio_pin(unsigned int pin);
void gpio_tgl_gpio_pin(unsigned int pin);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

#endif  /* PAL_CONFIG_H */
/* EOF */
