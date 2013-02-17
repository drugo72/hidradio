/**
 * @file pal_config.h
 *
 * @brief PAL configuration for SAM3SEK
 *
 * This header file contains configuration parameters for AT91SAM3S4C.
 *
 * $Id: pal_config.h 25440 2011-02-08 13:52:04Z mahendran.p $
 *
 */
/**
 * @author
 * Atmel Corporation: http://www.atmel.com
 * Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */


#ifndef PAL_CONFIG_H
#define PAL_CONFIG_H

/* === Includes ==============================================================*/
#include "AT91SAM3S4.h"
#include "pal_types.h"
#include "pal_boardtypes.h"
#include "armtypes.h"
#include "pal_internal.h"
#include "return_val.h"

#if (BOARD_TYPE == RZ600_230_SAM3SEK)
/* === Types =================================================================*/

/*
 * Indentifiers for PIO's in AT91SAM3S4B
 */
typedef enum pio_type_tag
{
    PIO_A,
    PIO_B,
    PIO_C
} pio_type_t;

/* Enumerations used to identify LEDs */
typedef enum led_id_tag
{
    LED_0,
    LED_1,
    LED_2
} led_id_t;

#define NO_OF_LEDS                      (3)

/* Enumerations used to identify buttons */
typedef enum button_id_tag
{
    BUTTON_0,
    BUTTON_1
} button_id_t;

#define NO_OF_BUTTONS                   (2)

/* === Externals =============================================================*/


/* === Macros ================================================================*/
/* The CPU clock */
/** Frequency of the board main oscillator */
#define BOARD_MAINOSC                  (12000000)

#define F_CPU                          (32000000)

#define TRX_INT_PIN  {PIO_PA17, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP}

/** Base address of SPI peripheral connected to the RF TRX. */
#define BOARD_TRX_SPI_BASE         SPI
/** Identifier of SPI peripheral connected to the transceiver. */
#define BOARD_TRX_SPI_ID           ID_SPI
/** Pins of the SPI peripheral connected to the transceiver. */
#define BOARD_TRX_SPI_PINS         PINS_SPI
/** Chip select connected to the transceiver. */
#define BOARD_TRX_NPCS             2
/** Chip select pin connected to the touchscreen controller. */
#define BOARD_TRX_NPCS_PIN         PIN_SPI_NPCS0_PA11

/** SPI MISO pin definition. */
#define PIN_SPI_MISO  {1 << 12, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI MOSI pin definition. */
#define PIN_SPI_MOSI  {1 << 13, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI SPCK pin definition. */
#define PIN_SPI_SPCK  {1 << 14, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI chip select pin definition. */
#define PIN_SPI_NPCS0_PA11  {1 << 11, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** List of SPI pin definitions (MISO, MOSI & SPCK). */
//#define PINS_SPI      PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SPCK
/** Push button #0 definition */
#define PIN_PUSHBUTTON_1    {1 << 3, PIOB, ID_PIOB, PIO_INPUT, \
                       PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
/** Push button #1 definition. */
#define PIN_PUSHBUTTON_2    {1 << 12, PIOC, ID_PIOC, PIO_INPUT, \
                    PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE}
/** List of all push button definitions. */
#define PINS_PUSHBUTTONS    PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2
/** USB attributes configuration descriptor */
#define BOARD_USB_BMATTRIBUTES USBConfigurationDescriptor_SELFPOWERED_NORWAKEUP

/** UART0 pin RX */
#define PIN_UART0_RXD    {0x1 << 9, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** UART0 pin TX */
#define PIN_UART0_TXD    {0x1 << 10, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/** USART0 pin RX */
#define PIN_USART0_RXD    {0x1 << 19, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** USART0 pin TX */
#define PIN_USART0_TXD    {0x1 << 18, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** USART0 pin CTS */
#define PIN_USART0_CTS    {0x1 << 8, PIOB, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/** USART0 pin RTS */
#define PIN_USART0_RTS    {0x1 << 7, PIOB, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/** USART0 pin SCK */
#define PIN_USART0_SCK    {0x1 << 17, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/** USART1 pin RX */
#define PIN_USART1_RXD    {0x1 << 21, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** USART1 pin TX */
#define PIN_USART1_TXD    {0x1 << 22, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** USART1 pin CTS */
#define PIN_USART1_CTS    {0x1 << 25, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** USART1 pin RTS */
#define PIN_USART1_RTS    {0x1 << 24, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** USART1 pin ENABLE */
#define PIN_USART1_EN     {0x1 << 23, PIOA, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

/** USB VBus monitoring pin definition. */
#define PIN_USB_VBUS    {1 << 21, PIOC, ID_PIOC, PIO_INPUT, PIO_PULLUP}


/** Number of USB endpoints */
#define CHIP_USB_NUMENDPOINTS 8

#define CHIP_USB_ENDPOINTS_MAXPACKETSIZE(i) \
   ((i == 0) ? 64 : \
   ((i == 1) ? 64 : \
   ((i == 2) ? 64 : \
   ((i == 3) ? 64 : \
   ((i == 4) ? 512 : \
   ((i == 5) ? 512 : \
   ((i == 6) ? 64 : \
   ((i == 7) ? 64 : 0 ))))))))

   /** Endpoints Number of Bank */
#define CHIP_USB_ENDPOINTS_BANKS(i) \
   ((i == 0) ? 1 : \
   ((i == 1) ? 2 : \
   ((i == 2) ? 2 : \
   ((i == 3) ? 1 : \
   ((i == 4) ? 2 : \
   ((i == 5) ? 2 : \
   ((i == 6) ? 2 : \
   ((i == 7) ? 2 : 0 ))))))))


#if (F_CPU == 32000000UL)

/* SPI baud rate divider to generate 4MHz SPI clock, when F_CPU is 32MHz. */
#define SPI_BR_DIVIDER                  (8)

#elif (F_CPU == 48000000UL)

/* SPI baud rate divider to generate 4MHz SPI clock, when F_CPU is 48MHz. */
#define SPI_BR_DIVIDER                  (12)

#else
#error "unsupported CPU clock"
#endif

/*
 * This board does NOT have an external eeprom available.
 */
#ifndef EXTERN_EEPROM_AVAILABLE
#define EXTERN_EEPROM_AVAILABLE            (0)
#endif

/*
 * Macros dealing with the PMC of AT91SAM3S4B
 */

/*
 * Enables the clock to the given peripheral id
 */
#define PERIPHERAL_CLOCK_ENABLE(id)     (pal_peripheral_clock_enable(id))

/*
 * Disables the clock to the given peripheral id
 */
#define PERIPHERAL_CLOCK_DISABLE(id)    (pal_peripheral_clock_disable(id))



/*
 * SPI Base Register:
 * SPI0 is used by SAM3SEK
 */
#define SPI_USED             (SPI)


/*
 * Macros dealing with the PIO of AT91SAM3S4C
 */

/*
 * Enables the PIO control for the requested pin and sets it as output
 */

#define PIN_SET_AS_PIO_OUTPUT(pin, pio) do {                            \
    Pio *pio_ptr;                                                       \
    if (PIO_A == pio)                                                   \
    {                                                                   \
        pio_ptr = PIOA;                                                 \
    }                                                                   \
    else if(PIO_B == pio)                                                               \
    {                                                                   \
        pio_ptr = PIOB;                                      \
    }                                                                  \
    else                                                               \
    {                                                                  \
        pio_ptr = PIOC;                                                \
    }                                                                   \
    /* Bit corresponding to pin is set in output enable register. */    \
    pio_ptr->PIO_OER = pin;                                             \
    /* PIO control is enabled on the specified pin. */                  \
    pio_ptr->PIO_PER = pin;                                             \
} while (0);

/*
 * Enables the PIO control for the requested pin and sets it as input
 */
#define PIN_SET_AS_PIO_INPUT(pin, pio) do {                             \
    Pio *pio_ptr;                                                       \
    if (PIO_A == pio)                                                   \
    {                                                                   \
        pio_ptr = PIOA;                                                 \
    }                                                                   \
    else if(PIO_B == pio)                                               \
    {                                                                   \
        pio_ptr = PIOB;                                                 \
    }                                                                   \
    else                                                               \
    {                                                                  \
        pio_ptr = PIOC;                                                \
    }                                                                   \
    /* Bit corresponding to pin is set in output disable register. */   \
    pio_ptr->PIO_ODR = pin;                                             \
    /* PIO control is enabled on the specified pin. */                  \
    pio_ptr->PIO_PER = pin;                                             \
} while (0);

/*
 * Sets the pin of requested PIO
 */
#define PIN_SET(pin, pio)   do {    \
    Pio *pio_ptr;                   \
    if (PIO_A == pio)               \
    {                               \
        pio_ptr = PIOA;             \
    }                               \
    else if(PIO_B == pio)           \
    {                               \
        pio_ptr = PIOB;             \
    }                               \
    else                                                               \
    {                                                                  \
        pio_ptr = PIOC;                                                \
    }                                                                   \
    pio_ptr->PIO_SODR = pin;        \
} while (0);

/*
 * Clears the pin of requested PIO
 */
#define PIN_CLEAR(pin, pio) do {    \
    Pio *pio_ptr;                   \
    if (PIO_A == pio)               \
    {                               \
        pio_ptr = PIOA;             \
    }                               \
    else if(PIO_B == pio)                            \
    {                               \
        pio_ptr = PIOB;  \
    }                               \
    else                                                               \
    {                                                                  \
        pio_ptr = PIOC;                                                \
    }                                                                   \
    pio_ptr->PIO_CODR = pin;        \
} while (0);

/*
 * Gets the status of the requested pin of PIO
 */
#define PIN_GET_STATUS(status, pin, pio) do {               \
    Pio *pio_ptr;                                           \
    if (PIO_A == pio)                                       \
    {                                                       \
        pio_ptr = PIOA;                                     \
    }                                                       \
    else if(PIO_B == pio)                                   \
    {                                                       \
        pio_ptr = PIOB;                                     \
    }                                                       \
    else                                                               \
    {                                                                  \
        pio_ptr = PIOC;                                                \
    }                                                                   \
    status = (pio_ptr->PIO_ODSR & pin ? true : false);      \
} while (0);

/* Enables the transceiver interrupts. */
#define ENABLE_TRX_IRQ() do	{                              \
        (pal_pio_enable_interrupt(&(const pin_t)TRX_INT_PIN));           \
} while(0);

/* Disables the transceiver interrupts. */
#define DISABLE_TRX_IRQ() do {                             \
        (pal_pio_disable_interrupt(&(const pin_t)TRX_INT_PIN));          \
}while(0);

/* Clears the transceiver interrupts. */
#define	CLEAR_TRX_IRQ()	do{\
         (NVIC_ClearPendingIRQ((IRQn_Type)ID_PIOA));       \
}while(0);

/* Enables the transceiver time stamp interrupt */
#define ENABLE_TRX_IRQ_TSTAMP()
/* Disables the transceiver time stamp interrupt */
#define DISABLE_TRX_IRQ_TSTAMP()
/* Clears the transceiver time stamp interrupt */
#define CLEAR_TRX_IRQ_TSTAMP()


/*
 * Macros dealing with the global interrupt (IRQ bit) of AT91SAM7S256.
 */

/* Enables the global interrupt. */
#define ENABLE_GLOBAL_IRQ()             sei()

/* Disables the global interrupt. */
#define DISABLE_GLOBAL_IRQ()            cli()

/*
 * This macro is used to mark the start of a critical region. It saves the
 * current status register and then disables the interrupt.
 */
#define ENTER_CRITICAL_REGION()         \
    {uint32_t sreg; GET_CPSR(sreg); DISABLE_GLOBAL_IRQ()

/*
 * This macro is used to mark the end of a critical region. It restores the
 * current status register.
 */
#define LEAVE_CRITICAL_REGION()         SET_CPSR(sreg);}

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION() {uint32_t irq_mask=__get_PRIMASK(); DISABLE_TRX_IRQ()

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()  __set_PRIMASK(irq_mask); }


/*
 * GPIO macros for AT91SAM3S4C
 */

/*
 * This board uses an SPI-attached transceiver.
 */
#define PAL_USE_SPI_TRX                 (1)

/* RESET pin is pin 18 of PIOA. */
#define RST                             (PIO_PA18)

/* Sleep Transceiver pin is pin 15 of PIOA. */
#define SLP_TR                          (PIO_PA15)

/*
 * Slave select pin is pin 28 PA11
 */
#define SEL                             (PIO_PB2B_NPCS2)

/*
 * SPI Bus Master Output/Slave Input pin is pin 22 is PA13
 */
#define MOSI                            (PIO_PA13A_MOSI)

/*
 * SPI Bus Master Input/Slave Output pin is pin 27 is PA12
 */
#define MISO                            (PIO_PA12A_MISO)

/*
 * SPI serial clock pin is pin 21 is PA14
 */
#define SCK                             (PIO_PA14A_SPCK)

/* Left Button*/
#define LEFT_BUTTON                     (PIO_PB3)
/* Right Button*/
#define RIGHT_BUTTON                    (PIO_PC12)

/*
 * Set TRX GPIO pins.
 */
#define RST_HIGH()                      {PIN_SET(RST, PIO_A)}
#define RST_LOW()                       {PIN_CLEAR(RST, PIO_A)}
#define SLP_TR_HIGH()                   {PIN_SET(SLP_TR, PIO_A)}
#define SLP_TR_LOW()                    {PIN_CLEAR(SLP_TR, PIO_A)}


/*
 * Timer macros for AT91SAM3S4
 */

/*
 * These macros are placeholders for delay functions for high speed processors.
 *
 * The following delay are not reasonbly implemented via delay functions,
 * but rather via a certain number of NOP operations.
 * The actual number of NOPs for each delay is fully MCU and frequency
 * dependent, so it needs to be updated for each board configuration.
 *
 * AT91SAM7S256 @ FPU
 */
#if (F_CPU == 32000000UL)
/* 32 MHz */
/*
 * Wait for 65 ns.
 * time t7: SLP_TR time (see data sheet or SWPM).
 * In case the system clock is 32 MHz we need to have this value filled,
 * otherwise frame transmission may not be initiated properly.
 */
#define PAL_WAIT_65_NS()                {nop(); nop();}

/* Wait for 500 ns. */
#define PAL_WAIT_500_NS()               {nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop();}
/* Wait for 1 us. */
#define PAL_WAIT_1_US()                 {nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop();}
#elif (F_CPU == 48000000UL)
/* 48 MHz */
/*
 * Wait for 65 ns.
 * time t7: SLP_TR time (see data sheet or SWPM).
 * In case the system clock is 48 MHz we need to have this value filled,
 * otherwise frame transmission may not be initiated properly.
 */
#define PAL_WAIT_65_NS()                {nop(); nop();nop(); nop();}

/* Wait for 500 ns. */
#define PAL_WAIT_500_NS()               {nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop();}
/* Wait for 1 us. */
#define PAL_WAIT_1_US()                 {nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop();}
#endif

/*
 * The smallest timeout in microseconds
 */
#define MIN_TIMEOUT                     (0x80)

/*
 * The largest timeout in microseconds
 */
#define MAX_TIMEOUT                     (0x7FFFFFFF)

/*
 * Settings to give clocking to timer when radio is awake
 *
 */
#define TIMER_SRC_DURING_TRX_AWAKE()

/*
 * Settings to give clocking to timer when radio is sleeping
 *
 */
#define TIMER_SRC_DURING_TRX_SLEEP()

/*
 * Maximum numbers of software timers running at a time
 */
#define MAX_NO_OF_TIMERS                (25)
#if (MAX_NO_OF_TIMERS > 255)
#error "MAX_NO_OF_TIMERS must not be larger than 255"
#endif

/* If this macro is defined then the upper 16-bit of the timestamp value
is taken from sys_time which is a SW count incremented whenever lower 16bit
overflow happens */
#define TIMESTAMP_UPPER_16BIT_SW     (1)

/*
 * Hardware register that holds Rx timestamp
 */
#ifdef TIMESTAMP_UPPER_16BIT_SW
#define TIME_STAMP_LOW_REGISTER             (PAL_TIMER_CH.TC_CV)
#define TIMER_LOW_REGISTER                  (PAL_TIMER_CH.TC_CV)
#else
#define TIME_STAMP_LOW_REGISTER             (TC0->TC_CHANNEL[0].TC_CV)
#define TIMER_LOW_REGISTER                  (TC0->TC_CHANNEL[0].TC_CV)
#define TIME_STAMP_HIGH_REGISTER            (TC0->TC_CHANNEL[1].TC_CV)
#endif

/* Macros for PAL Normal Timer Register */
#define PAL_TIMER_CH                 (TC0->TC_CHANNEL[2])
#define PAL_TIMER_IRQ_ID             (TC2_IRQn)
#define PAL_TIMER_PERIPH_ID          (ID_TC2)
#define PAL_TIMER_CCR_EN             (TC_CCR0_CLKEN)
#define PAL_TIMER_IER_FLAG           (TC_IER0_CPCS)
#define PAL_TIMER_IDR_FLAG           (TC_IDR0_CPCS)
#define PAL_TIMER_SR_FLAG            (TC_SR0_CPCS)
#define PAL_TIMER_SR_OVERFLOW_FLAG   (TC_SR0_COVFS)
#define PAL_TIMER_STATUS_REG         (PAL_TIMER_CH.TC_SR)
#define PAL_TIMER_IER                (PAL_TIMER_CH.TC_IER)
#define PAL_TIMER_IDR                (PAL_TIMER_CH.TC_IDR)
#define PAL_TIMER_COMP_REG           (PAL_TIMER_CH.TC_RC)
#define PAL_TIMER_CCR                (PAL_TIMER_CH.TC_CCR)
#define PAL_TIMER_IRQ                 void TC2_IrqHandler(void)


/* Macros for PAL High Priority Timer Register */
#define PAL_HIGHPRI_TMR_CH            (TC1->TC_CHANNEL[1])
#define PAL_HIGHPRIO_TIMER_IRQ_ID     (TC4_IRQn)
#define PAL_HIGHPRIO_TIMER_PERIPH_ID  (ID_TC4)
#define PAL_HIGHPRIO_TIMER_CCR_EN     (TC_CCR1_CLKEN)
#define PAL_HIGHPRIO_TIMER_IER_FLAG   (TC_IER1_CPCS)
#define PAL_HIGHPRIO_TIMER_IDR_FLAG   (TC_IDR1_CPCS)
#define PAL_HIGHPRIO_TIMER_STATUS_REG (PAL_HIGHPRI_TMR_CH.TC_SR)
#define PAL_HIGHPRIO_TIMER_IER        (PAL_HIGHPRI_TMR_CH.TC_IER)
#define PAL_HIGHPRIO_TIMER_IDR        (PAL_HIGHPRI_TMR_CH.TC_IDR)
#define PAL_HIGHPRIO_TIMER_COMP_REG   (PAL_HIGHPRI_TMR_CH.TC_RC)
#define PAL_HIGHPRIO_TIMER_CCR        (PAL_HIGHPRI_TMR_CH.TC_CCR)
#define PAL_HIGHPRIO_TIMER_IRQ         void TC4_IrqHandler(void)

/*
 * TRX Access macros for AT91SAM3S4
 */

/*
 * Position of the PCS (peripheral chip select) field in the SPI_MR register.
 */
#define PCS_FIELD_IN_MR                 (16)

/*
 * Value that needs to be written to in the PCS field of the SPI_MR to
 * activate the line to which the trx select line is connected.
 */
#define PCS_FIELD_VALUE                 (3)

/*
 * Value of PCS field in SPI_MR (mode register) which indicates the contoller
 * about the line to which the slave is connected.
 */
#define SS_ENABLE                       (PCS_FIELD_VALUE << PCS_FIELD_IN_MR)

/*
 * Slave select made low
 */
#define SS_LOW()                        PIN_CLEAR(SEL, PIO_B)

/*
 * Slave select made high
 */
#define SS_HIGH()                       PIN_SET(SEL, PIO_B)

/*
 * Dummy value to be written in SPI transmit register to retrieve data form it
 */
#define SPI_DUMMY_VALUE                 (0x00)

/* Reads the data from the SPI receive register. */
#define SPI_READ(register_value)    do {                        \
    while ((SPI->SPI_SR & SPI_SR_RDRF) == 0);    \
    register_value = (SPI->SPI_RDR & 0xFFFF);       \
} while (0);

/* Writes the data into the SPI transmit register. */
#define SPI_WRITE(data)     do {                                    \
    while ((SPI->SPI_SR & SPI_SR_TDRE) == 0);        \
    SPI->SPI_TDR = data & 0xFFFF;                       \
    while ((SPI->SPI_SR & SPI_SR_TXEMPTY) == 0);     \
} while (0);


/*
 * Value of an external PA gain
 * If no external PA is available, value is 0.
 */
#define EXTERN_PA_GAIN                  (0)

/*
 * Alert initialization
 */
#define ALERT_INIT() {PIN_SET_AS_PIO_OUTPUT((PIO_PC20), PIO_C); \
                      PIN_SET_AS_PIO_OUTPUT((PIO_PA19 | PIO_PA20), PIO_A); \
                      pal_led(LED_0, LED_OFF);}

/*
 * Alert indication
 */
#define ALERT_INDICATE()    do                      \
{                                                   \
    bool pin_status;                                \
    PIN_GET_STATUS(pin_status, (PIO_PA19 | PIO_PA20), PIO_A);    \
    if (pin_status)                                 \
    {                                               \
        PIN_CLEAR(PIO_PC20, PIO_C);                 \
        PIN_CLEAR((PIO_PA19 | PIO_PA20), PIO_A);    \
    }                                               \
    else                                            \
    {                                               \
        PIN_SET(PIO_PC20, PIO_C);                   \
        PIN_SET((PIO_PA19 | PIO_PA20), PIO_A);      \
    }                                               \
} while (0);


/*
 * ADC Initialization values
 */
/*
 * Number of ADC conversions to be done during generation of random number.
 * Since a 16-bit value is needed and 4 ADC channels are used
 * to get 1 single bit (always the LSB), 4 subsequent conversions are required.
 */
#define BOARD_ADC_FREQ                  (6000000)

#define NO_OF_CONVERSIONS               (16)

/* Value of the ADC clock in Hz */
#define USED_ADC_CLOCK_FREQ             (6000000)
/* Value of MCK in Hz */
#define USED_MCK_CLOCK_FREQ             (F_CPU)

#define ADC_STARTUP_TIME_NS             (10)        /* In us */
#define ADC_SAMPLE_AND_HOLD_TIME_US     (1200)      /* In ns */

/* Defines for the ADC Mode register */
#define ADC_PRESCAL ((USED_MCK_CLOCK_FREQ / (2 * USED_ADC_CLOCK_FREQ)) - 1)
#define ADC_STARTUP (((USED_ADC_CLOCK_FREQ / 1000000) * \
                               ADC_STARTUP_TIME_NS / 8) - 1)
#define ADC_SHTIM   ((((USED_ADC_CLOCK_FREQ / 1000000) *  \
                          ADC_SAMPLE_AND_HOLD_TIME_US) / 1000) - 1)

#define ADC_NUM_1                       (4)

/** Indicates board has an ILI9325 external component to manage LCD. */
#define BOARD_LCD_ILI9325

/** LCD pins definition. */
#define BOARD_LCD_PINS        PIN_EBI_DATA_BUS, PIN_EBI_NRD, PIN_EBI_NWE, \
                                PIN_EBI_NCS1, PIN_EBI_LCD_RS
/** Backlight pin definition. */
#define BOARD_BACKLIGHT_PIN  {1 << 13, PIOC, ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}
/** Define ILI9325 base address. */
#define BOARD_LCD_BASE              0x61000000
/** Define ILI9325 register select signal. */
#define BOARD_LCD_RS                (1 << 1)
/** Display width in pixels. */
#define BOARD_LCD_WIDTH             240
/** Display height in pixels. */
#define BOARD_LCD_HEIGHT            320

/** EBI Data Bus pins */
#define PIN_EBI_DATA_BUS     {0xFF,    PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}
/** EBI NRD pin */
#define PIN_EBI_NRD          {1 << 11, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}
/** EBI NWE pin */
#define PIN_EBI_NWE          {1 << 8,  PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}
/** EBI NCS0 pin */
#define PIN_EBI_NCS0         {1 << 20, PIOB, ID_PIOB, PIO_PERIPH_A, PIO_PULLUP}
/** EBI pins for PSRAM address bus */
#define PIN_EBI_PSRAM_ADDR_BUS {0x3f00fff, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}
/** EBI pins for PSRAM NBS pins */
#define PIN_EBI_PSRAM_NBS  {1 << 7, PIOB, ID_PIOB, PIO_PERIPH_B, PIO_PULLUP}, \
                           {1 << 15, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}

/** EBI A1 pin */
#define PIN_EBI_A1      {1 << 19, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}
/** EBI NCS1 pin */
#define PIN_EBI_NCS1    {1 << 15, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}
/** EBI pin for LCD RS */
#define PIN_EBI_LCD_RS  {1 << 19, PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP}

/** Internal SRAM address */
#define AT91C_ISRAM                   AT91C_IRAM
/** Internal SRAM size */
#define AT91C_ISRAM_SIZE              0x00008000

/** Internal Flash size */
#define AT91C_IFLASH_SIZE               (0x40000)
/** Internal Flash page size */
#define AT91C_IFLASH_PAGE_SIZE              (256) /* Internal FLASH 0 Page Size: 256 bytes */
/** Internal Flash number of pages */
#define AT91C_IFLASH_NB_OF_PAGES           (1024) /* Internal FLASH 0 Number of Pages: 512 */
/** Internal Flash lock region size */
#define AT91C_IFLASH_LOCK_REGION_SIZE     (16384) /* Internal FLASH 0 Lock Region Size: 16 Kbytes */
/** Internal Flash number of lock bits */
#define AT91C_IFLASH_NB_OF_LOCK_BITS         (16) /* Internal FLASH 0 Number of Lock Bits: 16 */

#define FLASH_PAL_ADDRESS   AT91C_IFLASH + AT91C_IFLASH_SIZE - AT91C_IFLASH_PAGE_SIZE

#define FLASH_PAL_PAGENUM    1023

/* === Prototypes ===========================================================*/
void LowLevelInit(void);
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #if (BOARD_TYPE == RZ600_230_SAM3SEK)*/

#endif  /* PAL_CONFIG_H */
/* EOF */
