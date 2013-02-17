/**
 * @file pal_config.h
 *
 * @brief PAL configuration for AT32UC3L064
 *
 * This header file contains configuration parameters for AT32UC3L064.
 *
 * $Id: pal_config.h 25880 2011-03-08 05:56:34Z yogesh.bellan $
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

/* Includes for Touch Library. */
#ifdef TOUCH_UC3LEK
#include "touch_config_at32uc3l.h"
#endif

#if (BOARD_TYPE == RZ600_212_UC3LEK)

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
    LED_2,
    LED_3
} led_id_t;

/* Enumerations used to idenfify buttons */
typedef enum button_id_tag
{
    BUTTON_0
} button_id_t;

/*The clock domains of the power manager*/
typedef enum
{
    PM_CLK_DOMAIN_0 = AVR32_PM_CLK_GRP_CPU,
    PM_CLK_DOMAIN_1 = AVR32_PM_CLK_GRP_HSB,
    PM_CLK_DOMAIN_2 = AVR32_PM_CLK_GRP_PBA,
    PM_CLK_DOMAIN_3 = AVR32_PM_CLK_GRP_PBB,
    PM_CLK_DOMAIN_INVALID
} pm_clk_domain_t;

/* The possible synchronous clock division ratio. */
typedef enum
{
    PM_CKSEL_DIVRATIO_2 = 0,  // Divide the main clock by 2^1
    PM_CKSEL_DIVRATIO_4,      // Divide the main clock by 2^2
    PM_CKSEL_DIVRATIO_8,      // Divide the main clock by 2^3
    PM_CKSEL_DIVRATIO_16,     // Divide the main clock by 2^4
    PM_CKSEL_DIVRATIO_32,     // Divide the main clock by 2^5
    PM_CKSEL_DIVRATIO_64,     // Divide the main clock by 2^6
    PM_CKSEL_DIVRATIO_128,    // Divide the main clock by 2^7
    PM_CKSEL_DIVRATIO_256,    // Divide the main clock by 2^8
    PM_CKSEL_DIVRATIO_ERROR
} pm_divratio_t;

/* The clock sources of the power manager. */
typedef enum
{
    PM_CLK_SRC_SLOW = AVR32_PM_MCSEL_SLOW,
    PM_CLK_SRC_OSC0 = AVR32_PM_MCSEL_OSC0,
    PM_CLK_SRC_DFLL0 = AVR32_PM_MCSEL_DFLL0,
    PM_CLK_SRC_RC120M = AVR32_PM_MCSEL_RC120M,
    PM_CLK_SRC_INVALID
} pm_clk_src_t;

/**
 * GPIO pins
 */
typedef enum gpio_pin_type_tag
{
    RST_PIN,
    SLP_TR_PIN
} gpio_pin_type_t;

/* Pointer to interrupt handler. */
#if (defined __GNUC__)
    typedef void (*__int_handler)(void);
#elif (defined __ICCAVR32__)
    typedef void (__interrupt *__int_handler)(void);
#endif

/* === Externals ============================================================*/

#if defined(__GNUC__)
    // define _evba from exception.S
    extern void _evba;

    // Values to store in the interrupt priority registers for the various interrupt priority levels.
    extern const uint32_t ipr_val[1 << AVR32_INTC_IPR_INTLEVEL_SIZE];
#endif

/* === Macros ===============================================================*/
#define F_CPU               (32000000UL)                             /* CPU clock frequency.*/
#define F_CPU_COUNTER       (32000000UL)                             /* In order to get the exact cpu clock delays*/
#define CPUCLK_HZ           (32000000UL)
#define FOSC0               (12000000UL)                             /* Osc0 frequency: Hz.*/
#define OSC0_STARTUP        (AVR32_SCIF_OSCCTRL0_STARTUP_2048_RCOSC)   /* Osc0 startup time: RCOsc periods.*/
#define ONE_MHZ             (1000000UL)
#define CPU_COUNTER         (F_CPU_COUNTER/ONE_MHZ)

/*
 * LEDs on AT32UC3L-EK
 */
#define NO_OF_LEDS                      (4)
#define LED0_PIN                        (AVR32_PIN_PA21)
#define LED1_PIN                        (AVR32_PIN_PB10)
#define LED2_PIN                        (AVR32_PIN_PB02)
#define LED3_PIN                        (AVR32_PIN_PB03)


/*
 * PINs where buttons are connected
 */
#define BUTTON_PIN_0                   (AVR32_PIN_PA11)

/*
 * RESET pin of transceiver
 */
#define TRX_RST                        (AVR32_PIN_PA00)

/*
 * Sleep Transceiver pin
 */
#define SLP_TR                         (AVR32_PIN_PA08)

/*
 * Spi Chip Select pin
 */
#define SEL                            (AVR32_PIN_PA03)

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
#define SLP_TR_HIGH()                      do {                                    \
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[SLP_TR >> 5];     \
        gpio_port->ovrs  = 1 << (SLP_TR & 0x1F);                                   \
        gpio_port->oders = 1 << (SLP_TR & 0x1F);                                   \
        gpio_port->gpers = 1 << (SLP_TR & 0x1F);                                   \
} while (0);



/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION()               DISABLE_TRX_IRQ()


/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()               ENABLE_TRX_IRQ()



/*
 * SPI Pins settings for AT32UC3L064 & AT86RF231
 */

#define RF_SPI_DIV                      (5)
#define RF_SPI                          (AVR32_SPI)
#define RF_SPI_CS_REG                   (AVR32_SPI.csr0)
#define RF_SPI_NPCS                     (0)
#define RF_SPI_NCPS_MASK                (0x0E << 16)
#define MOSI                            (AVR32_SPI_MOSI_0_0_PIN)
#define MOSI_FUNCTION                   (AVR32_SPI_MOSI_0_0_FUNCTION)
#define MISO                            (AVR32_SPI_MISO_0_0_PIN)
#define MISO_FUNCTION                   (AVR32_SPI_MISO_0_0_FUNCTION)
#define SCK                             (AVR32_SPI_SCK_0_0_PIN)
#define SCK_FUNCTION                    (AVR32_SPI_SCK_0_0_FUNCTION)


/*
 * SPI wait function for Security
 */
#define SPI_WAIT()                      do                                              \
                                        {                                               \
                                            uint32_t status;                            \
                                            do {                                        \
                                                    status = RF_SPI.sr;                 \
                                                    status &= AVR32_SPI_SR_RDRF_MASK;   \
                                            } while (status != AVR32_SPI_SR_RDRF_MASK); \
                                        } while (0)

/*
 * SPI read function for Security
 */
#define SPI_READ()                      (uint8_t)RF_SPI.rdr

/*
 * SPI write function for Security
 */
#define SPI_WRITE(x)                    RF_SPI.tdr = x

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
 * UART related settings for AT32UC3L064
 */
#define SYS_F_UART                           (32000000UL)
#define USART_BR                             (0x0D)
#define USART0                               (AVR32_USART0)
#define USART1                               (AVR32_USART1)
#define USART2                               (AVR32_USART2)
#define USART3                               (AVR32_USART3)

/* Selection of Uart */

/* Selection of Uart 0 */
#if(defined UART0)
#define USART                                USART0
#define USART_ISR_GROUP                      USART0_ISR_GROUP
#define USART_ISR_PRIORITY                   (1)
#define USART_RX                             (AVR32_USART0_RXD_0_1_PIN)
#define USART_RX_FUNCTION                    (AVR32_USART0_RXD_0_1_FUNCTION)
#define USART_TX                             (AVR32_USART0_TXD_0_1_PIN)
#define USART_TX_FUNCTION                    (AVR32_USART0_TXD_0_1_FUNCTION)
#endif

/* Selection of Uart 1 */
#if (defined UART1)
#define USART                                USART1
#define USART_ISR_GROUP                      USART1_ISR_GROUP
#define USART_ISR_PRIORITY                   (1)
#define USART_RX                             (AVR32_USART1_RXD_0_1_PIN)
#define USART_RX_FUNCTION                    (AVR32_USART1_RXD_0_1_FUNCTION)
#define USART_TX                             (AVR32_USART1_TXD_0_1_PIN)
#define USART_TX_FUNCTION                    (AVR32_USART1_TXD_0_1_FUNCTION)
#endif

/* Selection of Uart 2 */
#if (defined UART2)
#define USART                                USART2
#define USART_ISR_GROUP                      USART2_ISR_GROUP
#define USART_ISR_PRIORITY                   (1)
#define USART_RX                             (AVR32_USART2_RXD_0_1_PIN)
#define USART_RX_FUNCTION                    (AVR32_USART2_RXD_0_1_FUNCTION)
#define USART_TX                             (AVR32_USART2_TXD_0_1_PIN)
#define USART_TX_FUNCTION                    (AVR32_USART2_TXD_0_1_FUNCTION)
#endif

/* Selection of Uart 3 */
#if (defined UART3)
#define USART                                USART3
#define USART_ISR_GROUP                      (18)
#define USART_ISR_PRIORITY                   (1)
#define USART_RX                             (AVR32_USART3_RXD_0_1_PIN)
#define USART_RX_FUNCTION                    (AVR32_USART3_RXD_0_1_FUNCTION)
#define USART_TX                             (AVR32_USART3_TXD_0_1_PIN)
#define USART_TX_FUNCTION                    (AVR32_USART3_TXD_0_1_FUNCTION)
#endif


/*BaudRate settings of the uart*/
#define USART_CH_MODE                        .usart_mode.chmode
#define USART_STOP_BIT                       .usart_mode.nbstop
#define USART_PARITY                         .usart_mode.par
#define USART_CHR1                           .usart_mode.chrl
#define USART_MODE                           .usart_mode.mode


/*
 * PLL related settings for AT32UC3L064
 */
#define AVR32_PM_PLL0_PLLCOUNT_USER          (16)
#define AVR32_PM_PLLOSC_USER                 (0)
#define AVR32_PM_PLL0_PLLDIV_SIZE_USER       (1)
#define AVR32_PM_PLL0_PLLEN_USER             (1)
#define ZERO_INIT                            (0)
#define ENABLE_FCR                           (1)
#define USB_CLOCK_ENABLE                     (1)
#define SPI_BUS                              (0)
#define TIMER_BUS                            (1)
#define UART_BUS                             (2)
#define PLL0_SELECT                          (0)


/*
 * Timer related settings for AT32UC3L064
 */
#define TC_CH1_ISR_GROUP        (25)
#define TC_CH2_ISR_GROUP        (26)
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
 * IRQ macros for AT32UCL064
 */

/*
 * AT86RF231:
 *
 * TRX_MAIN_IRQ_HDLR_IDX        TRX interrupt
 * TRX_TSTAMP_IRQ_HDLR_IDX      Time stamping interrupt (only if Antenna Diversity is not used)
 */

/*
 * External Interrupt related settings for AT32UC3L064
 */
#define EXT_INT_ISR_GROUP               (14)
#define EXT_INT_ISR_PRIORITY            (1)
#define EXT_INT                         (AVR32_PIN_PA09)


#define CAT_IRQ_GROUP                   (29)
#define CAT_ISR_PRIORITY                (AVR32_INTC_INT3)

/* Enables the transceiver interrupts */
#define ENABLE_TRX_IRQ() do {\
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[EXT_INT >> 5];\
        gpio_port->iers = 1 << (EXT_INT & 0x1F);\
}while(0)

/* Enables the timestamp interrupts */
#define ENABLE_TRX_IRQ_TSTAMP()     do {\
      TC_CH2.ier = AVR32_TC_IER0_LDRAS_MASK ;\
      }while(0)

/* Disables the transceiver interrupts */
#define DISABLE_TRX_IRQ() do{                                                   \
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[EXT_INT >> 5]; \
        gpio_port->ierc = 1 << (EXT_INT & 0xFF);                                \
    }while(0)

/* Disables the timestamp interrupts */
#define DISABLE_TRX_IRQ_TSTAMP() do{                                            \
        TC_CH2.idr &= ~AVR32_TC_IER0_LDRAS_MASK ;                               \
    }while(0)

/* Clear the Timestamp interrupts */
#define CLEAR_TRX_IRQ_TSTAMP() do {\
        uint32_t status1 = TC_CH2.sr;                                           \
        status1 &= TC_CH2.imr;                                                  \
        }while(0)

/* Clear the transceiver interrupts */
#define CLEAR_TRX_IRQ() do {\
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[EXT_INT >> 5];     \
    gpio_port->ifrc = 1 << (EXT_INT & 0x1F);                                    \
}while(0)

/* Unlock PM register macro*/
#define PM_UNLOCK(reg)  (AVR32_PM.unlock = (unsigned long)(AVR32_PM_UNLOCK_KEY_VALUE << AVR32_PM_UNLOCK_KEY_OFFSET)|(reg))

#define EXT_UNLOCK(reg)  do{\
      avr32_gpio_port_t *gpio_port; \
      gpio_port->unlock = (unsigned long)(AVR32_PM_UNLOCK_KEY_VALUE << AVR32_GPIO_UNLOCK_KEY_OFFSET)|(reg);\
      }while(0)


/* Enables the global interrupt */
#define ENABLE_GLOBAL_IRQ()                  Enable_global_interrupt()

/* Disables the global interrupt */
#define DISABLE_GLOBAL_IRQ()                 Disable_global_interrupt()

/* This macro saves the global interrupt status */
#define ENTER_CRITICAL_REGION()              AVR32_ENTER_CRITICAL_REGION( )

/* This macro restores the global interrupt status */
#define LEAVE_CRITICAL_REGION()              AVR32_LEAVE_CRITICAL_REGION( )

/*
 *  This macro restores the global interrupt status
 */
#define SUCCESS         (0)
#define FAIL            (1)
#define ERROR           (-1)

#if DEF_TOUCH_QMATRIX == 1
/* The different clock source for the generic clocks */
typedef enum
{
    SCIF_GCCTRL_SLOWCLOCK  = AVR32_SCIF_GC_USES_CLK_SLOW,
    SCIF_GCCTRL_OSC32K     = AVR32_SCIF_GC_USES_CLK_32,
    SCIF_GCCTRL_DFLL0      = AVR32_SCIF_GC_USES_DFLL0,
    SCIF_GCCTRL_OSC0       = AVR32_SCIF_GC_USES_OSC0,
    SCIF_GCCTRL_RC120M     = AVR32_SCIF_GC_USES_RC120M,
    SCIF_GCCTRL_CPUCLOCK   = AVR32_SCIF_GC_USES_CLK_CPU,
    SCIF_GCCTRL_HSBCLOCK   = AVR32_SCIF_GC_USES_CLK_HSB,
    SCIF_GCCTRL_PBACLOCK   = AVR32_SCIF_GC_USES_CLK_PBA,
    SCIF_GCCTRL_PBBCLOCK   = AVR32_SCIF_GC_USES_CLK_PBB,
    SCIF_GCCTRL_RC32K      = AVR32_SCIF_GC_USES_RC32K,
    SCIF_GCCTRL_CRIPOSC    = AVR32_SCIF_GC_USES_CRIPOSC,
    SCIF_GCCTRL_CLK1K      = AVR32_SCIF_GC_USES_CLK_1K,
    SCIF_GCCTRL_OSCSEL_INVALID
} scif_gcctrl_oscsel_t;

/*The timeguard used for polling in ticks. */
#define SCIF_POLL_TIMEOUT             100000
#endif // #if DEF_TOUCH_QMATRIX == 1

/*if interrupts are globally enabled, else \c 0*/
#define Is_global_interrupt_enabled()         (!Tst_bits(Get_system_register(AVR32_SR), AVR32_SR_GM_MASK))

/* Disables interrupts globally*/
#if (defined __GNUC__)
  #define Disable_global_interrupt()          ({__asm__ __volatile__ ("ssrf\t%0" :  : "i" (AVR32_SR_GM_OFFSET));})
#elif (defined __ICCAVR32__)
  #define Disable_global_interrupt()          (__disable_interrupt())
#endif

/* Enables interrupts globally */
#if (defined __GNUC__)
  #define Enable_global_interrupt()           ({__asm__ __volatile__ ("csrf\t%0" :  : "i" (AVR32_SR_GM_OFFSET));})
#elif (defined __ICCAVR32__)
  #define Enable_global_interrupt()           (__enable_interrupt())
#endif

#define Tst_bits( value, mask)                (Rd_bits(value, mask) != 0)
#define Rd_bits( value, mask)                 ((value) & (mask))

#if (defined __GNUC__)
  #define Get_system_register(sysreg)         __builtin_mfsr(sysreg)
#elif (defined __ICCAVR32__)
  #define Get_system_register(sysreg)         __get_system_register(sysreg)
#endif

#if (defined __GNUC__)
  #define Set_system_register(sysreg, value)  __builtin_mtsr(sysreg, value)
#elif (defined __ICCAVR32__)
  #define Set_system_register(sysreg, value)  __set_system_register(sysreg, value)
#endif


/*brief Protects subsequent code from interrupts*/
#define AVR32_ENTER_CRITICAL_REGION( ) \
  { \
  bool global_interrupt_enabled = Is_global_interrupt_enabled(); \
  Disable_global_interrupt(); // Disable the appropriate interrupts.

/* This macro must always be used in conjunction with AVR32_ENTER_CRITICAL_REGION
 *         so that interrupts are enabled again.
 */
#define AVR32_LEAVE_CRITICAL_REGION( ) \
  if (global_interrupt_enabled) Enable_global_interrupt(); \
  }

/* Unlock SCIF register macro */
#define SCIF_UNLOCK(reg)  (AVR32_SCIF.unlock = (AVR32_SCIF_UNLOCK_KEY_VALUE << AVR32_SCIF_UNLOCK_KEY_OFFSET)|(reg))


/* Mask used to obtain the gloabl interrupt status bit in the status register of the AT32UC3L064s */
#ifndef __ICCAVR__
#define GLOBAL_IRQ_MASK                 (CPU_I_bm)
#else
#define GLOBAL_IRQ_MASK                 (I_bm)
#endif


/* Value of an external PA gain If no external PA is available, value is 0 */
#define EXTERN_PA_GAIN                  (0)

/*
 * Timer macros for AT32UC3L064
 */

/*
 * These macros are placeholders for delay functions for high speed processors.
 *
 * The following delay are not reasonbly implemented via delay functions,
 * but rather via a certain number of NOP operations.
 * The actual number of NOPs for each delay is fully MCU and frequency
 * dependent, so it needs to be updated for each board configuration.
 *
 * AT32UC3L064 @ 12MHz
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
#define TIMER_HIGH_REGISTER             sys_time /*(TC_CH1.cv)*/
#define TIMER_LOW_REGISTER              (TC_CH2.ra)
#define TIMER_LOW_VALUE_REGISTER        (TC_CH2.cv)


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
    pal_led(LED_0, LED_OFF);\
    pal_led(LED_1, LED_OFF);\
    pal_led(LED_2, LED_OFF);\
    pal_led(LED_3, LED_OFF);\
 } while (0)

/*
 * Alert indication
 */
#define ALERT_INDICATE()             do {    \
    pal_led(LED_0, LED_TOGGLE);\
    pal_led(LED_1, LED_TOGGLE);\
    pal_led(LED_2, LED_TOGGLE);\
    pal_led(LED_3, LED_TOGGLE);\
 } while (0)


/* === Prototypes ===========================================================*/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

#endif  /* PAL_CONFIG_H */
/* EOF */
