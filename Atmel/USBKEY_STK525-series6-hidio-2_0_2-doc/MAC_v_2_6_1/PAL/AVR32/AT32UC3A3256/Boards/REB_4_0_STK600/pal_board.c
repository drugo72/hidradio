/**
 * @file pal_board.c
 *
 * @brief PAL board specific functionality
 *
 * This file implements PAL board specific functionality for AVR32 AT32UC3A3256.
 *
 * $Id: pal_board.c 25120 2011-01-25 08:11:55Z yogesh.bellan $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */

#include <stdbool.h>
#include <stdlib.h>
#include "pal_types.h"
#include "pal.h"
#include "pal_boardtypes.h"
#include "pal_config.h"
#include "pal_internal.h"
#include "pal_timer.h"

#if (BOARD_TYPE == REB_4_0_STK600)

/* === Macros ============================================================== */

/* === Types =============================================================== */

/**
 * Encoding of the board family in the board_family configuration
 * record member.
 */
enum boardfamilycode
{
    CFG_BFAMILY_RADIO_EXTENDER, /* Radio Extender boards */
    CFG_BFAMILY_RCB             /* Radio Controller boards */
} SHORTENUM;

/**
 * Structure of the configuration record EEPROM data.  These data
 * reside at offset CFG_BASE_ADDR, and allocate the upper address
 * space of the EEPROM.  The lower address space is available for
 * further extensions, and/or customer use (the upper part can be
 * software write-protected on demand).
 *
 * The record is divided into 32 bytes of structured binary
 * configuration data, followed by up to 30 bytes of textual
 * configuration data (terminated with \0), and terminated by a CRC-16
 * checksum.  The CRC algorithm used is the same CCITT algorithm that
 * is also used to protect IEEE 802.15.4 frames.
 *
 * All multibyte integer values are stored in little-endia byte order.
 */
struct cfg_eeprom_data
{
#if 0
    union
    {
        /** Raw access to binary configuration data. */
        uint8_t raw_binary[CFG_BINARYLEN];

        /* Structured access to binary configuration data. */
        struct
        {
            /** factory-supplied EUI-64 address */
            uint64_t mac_address;

            /** factory-supplied board serial number */
            uint64_t serial_number;

            /** board ID: family */
            enum boardfamilycode board_family;

            /** board ID: major, minor, revision */
            uint8_t board_id[3];

            /** feature byte 1: RF frontend features */
            uint8_t feature1;

            /** 16 MHz crystal oscillator calibration value */
            uint8_t cal_16mhz;

            /** RC oscillator calibration value, Vcc = 3.6 V */
            uint8_t cal_rc_36;

            /** RC oscillator calibration value, Vcc = 2.0 V */
            uint8_t cal_rc_20;

            /** antenna gain, 1/10 dB */
            int8_t antenna_gain;

            /* 7 bytes reserved, leave as 0xFF */
        }
        struct_binary;
    }
    binary_data;

    /** Textual board name, ASCIZ string */
    char board_name[CFG_NAMELEN];
#endif
    /** CRC-16 over binary_data and board_name[] */
    uint16_t crc;
};

/* === Globals ============================================================= */


/* === Prototypes ========================================================== */

#ifdef EXTERN_EEPROM_AVAILABLE
static uint8_t at25010_read_byte(uint8_t addr);
#endif

/* === Implementation ======================================================= */

/**
 * @brief Provides timestamp of the last received frame
 *
 * This function provides the timestamp (in microseconds)
 * of the last received frame.
 *
 * @param[out] Timestamp in microseconds
 */
void pal_trx_read_timestamp(uint32_t *timestamp)
{
    /*
     * Everytime a transceiver interrupt is triggred, input capture register of
     * the AVR is latched. The 'sys_time' is concatenated to the ICR to
     * generate the time stamp of the received frambe.
     * 'ICR_Higher byte'   'ICR_Lower byte'
     *  ---------|--------- => 32 bit timestamp
     *   16 bits   16 bits
     */
    *timestamp  = (uint32_t)TIMER_HIGH_REGISTER << (uint32_t)16;
    *timestamp |= (uint32_t)TIMER_LOW_REGISTER;
}



/**
 * @brief Calibrates the internal RC oscillator
 *
 * This function calibrates the internal RC oscillator.
 *
 * @return True since the RC oscillator is always calibrated
 *         automatically at startup by hardware itself
 */
bool pal_calibrate_rc_osc(void)
{
    return (true);
}

void gpio_enable_output(unsigned int pin)
{
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
        gpio_port->gpers = 1 << (pin & 0x1F);
        gpio_port->oderc = 1 << (pin & 0x1F);
        gpio_port->oders = 1 << (pin & 0x1F); // The GPIO output driver is enabled for that pin.
}


void gpio_enable_input(unsigned int pin)
{
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
        gpio_port->gpers = 1 << (pin & 0x1F);
        gpio_port->oderc = 1 << (pin & 0x1F);
        gpio_port->oderc = 1 << (pin & 0x1F); // The GPIO output driver is disabled for that pin.
}


void gpio_set_gpio_pin(unsigned int pin)
{
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];

        gpio_port->ovrs  = 1 << (pin & 0x1F); // Value to be driven on the I/O line: 1.
}


void gpio_clr_gpio_pin(unsigned int pin)
{
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];

          gpio_port->ovrc  = 1 << (pin & 0x1F); // Value to be driven on the I/O line: 0.
          gpio_port->oders = 1 << (pin & 0x1F); // The GPIO output driver is enabled for that pin.
          gpio_port->gpers = 1 << (pin & 0x1F); // The GPIO module controls that pin.
}


void gpio_tgl_gpio_pin(unsigned int pin)
{
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];

        gpio_port->ovrt  = 1 << (pin & 0x1F); // Value to be driven on the I/O line: tgl.
}


pin_state_t gpio_get(uint32_t pin)
{
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];

        /* Check whether the pin is HIGH */
        if ((gpio_port->pvr >> (pin & 0x1F)) & HIGH)
        {
                return(HIGH);
        }
        else
        {
                return(LOW);
        }
}


/**
 * @brief Sets the GPIO pin state
 *
 * This functions sets the state of a GPIO pin.
 *
 * @param gpio_pin GPIO pin to be made HIGH or LOW
 * @param state New pin state
 */
void pal_gpio_set(gpio_pin_type_t gpio_pin, pin_state_t state)
{
        if (SLP_TR_PIN == gpio_pin)
        {
                if (HIGH == state)
                {
                        gpio_set_gpio_pin(SLP_TR);
                }
                else
                {
                        gpio_clr_gpio_pin(SLP_TR);
                }
        }
        else if (RST_PIN == gpio_pin)
        {
                if (HIGH == state)
                {
                        gpio_set_gpio_pin(TRX_RST);
                }
                else
                {
                        gpio_clr_gpio_pin(TRX_RST);
                }
        }
}


/**
 * @brief Gets the current GPIO pin state
 *
 * This function gets the current state of a GPIO pin.
 *
 * @param gpio_pin GPIO pin to be read
 *
 * @return Current GPIO pin state
 */
pin_state_t pal_gpio_get(gpio_pin_type_t gpio_pin)
{
    pin_state_t pin_state = LOW;

    if (RST_PIN == gpio_pin)
    {
        pin_state = gpio_get(TRX_RST);
    }
    else if (SLP_TR_PIN == gpio_pin)
    {
        pin_state = gpio_get(SLP_TR);
    }

    return pin_state;
}

/**
 * @brief Initializes the GPIO pins
 *
 * This function is used to initialize the port pins used to connect
 * the microcontroller to transceiver.
 */
static void gpio_enable_module_pin(unsigned int pin, unsigned int function)
{
        volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];

        // Enable the correct function.
        switch (function)
        {
        case 0: // A function.
        gpio_port->pmr0c = 1 << (pin & 0x1F);
        gpio_port->pmr1c = 1 << (pin & 0x1F);
        break;

        case 1: // B function.
        gpio_port->pmr0s = 1 << (pin & 0x1F);
        gpio_port->pmr1c = 1 << (pin & 0x1F);
        break;

        case 2: // C function.
        gpio_port->pmr0c = 1 << (pin & 0x1F);
        gpio_port->pmr1s = 1 << (pin & 0x1F);
        break;

        case 3: // D function.
        gpio_port->pmr0s = 1 << (pin & 0x1F);
        gpio_port->pmr1s = 1 << (pin & 0x1F);
        break;

        default: return;
        }

        // Disable GPIO control.
        gpio_port->gperc = 1 << (pin & 0x1F);
}


/**
 * @brief Initializes the GPIO pins
 *
 * This function is used to initialize the port pins used to connect
 * the microcontroller to transceiver.
 */
void gpio_init(void)
{
#if( (defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))
		 /* Set-up the UART0 pins to function A. */
		 gpio_enable_module_pin(USART_RX, USART_RX_FUNCTION);
		 gpio_enable_module_pin(USART_TX, USART_TX_FUNCTION);
#endif

        /* Initialize peripheral interrupt pin. */
        gpio_enable_input(EXT_INT);
        gpio_enable_module_pin(AVR32_EIC_EXTINT_8_PIN,AVR32_EIC_EXTINT_8_FUNCTION);

        /* Initialize SLP_TR, RST and SS as GPIO. */
        gpio_enable_output(TRX_RST);
        gpio_enable_output(SLP_TR);
        gpio_set_gpio_pin(SLP_TR);
        gpio_enable_output(SEL);
        gpio_set_gpio_pin(SEL);

        /* Initialize pins for SPI0 (11 - 13) to function A. */
        gpio_enable_module_pin(MOSI, MOSI_FUNCTION);
        gpio_enable_module_pin(MISO, MISO_FUNCTION);
        gpio_enable_module_pin(SCK, SCK_FUNCTION);
        gpio_enable_input(AVR32_PIN_PX18);
        gpio_enable_module_pin( AVR32_TC0_A2_0_PIN,AVR32_TC0_A2_0_FUNCTION);
}


/**
 * @brief Initialize the interrupt system of the ATUC3A3256
 */
void interrupt_system_init(void)
{
    /* Enable high priority interrupts */

}


/**
 * @brief Initialize the clock of the ATUC3A3256
 */
void clock_init(void)
{
    /*Pll to get maximum output*/
    avr32_pm_pll_t pll_conf;

    /* To set the oscillator mode */
    AVR32_PM.OSCCTRL0.mode = AVR32_PM_OSCCTRL0_MODE_CRYSTAL_G3;

	/*Start up time for the oscillator is set*/
    AVR32_PM.OSCCTRL0.startup = AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC;

    /* Enable the Osc0. */
    AVR32_PM.mcctrl |= AVR32_PM_MCCTRL_OSC0EN_MASK;

	/*Waiting till the internal oscillator gets stable*/
    while (!(AVR32_PM.poscsr & AVR32_PM_POSCSR_OSC0RDY_MASK));

    /* Switch main clock to Osc0. */
    AVR32_PM.MCCTRL.mcsel =AVR32_PM_MCSEL_OSC0;

    /* Set-up the PLL, enable it and wait for lock. */
	/*To make the oscillator output to 60Mhz*/
    pll_conf.pllmul   = AVR32_PM_PLL0_PLLMUL_SIZE,
	/*Assigning the division factor for PLL*/
    pll_conf.plldiv   = AVR32_PM_PLL0_PLLDIV_SIZE_USER,
	/*Selecting the type of oscillator*/
    pll_conf.pllosc   = AVR32_PM_PLLOSC_USER,
	/*To make the oscillator output divided by 2 so that it gives us 30Mhz*/
    pll_conf.pllopt   = AVR32_PM_PLL0_PLLOPT_SIZE,
	/*Count of the pll is configured*/
    pll_conf.pllcount = AVR32_PM_PLL0_PLLCOUNT_USER,
	/*Enabling the pll*/
    pll_conf.pllen    = AVR32_PM_PLL0_PLLEN_USER,

    /*Assigning the pll value to get a maximum of 30Mhz output*/
    AVR32_PM.PLL[PLL0_SELECT] = pll_conf;

    /*Waiting till the oscillator gets stable*/
    while (!(AVR32_PM.poscsr & AVR32_PM_POSCSR_LOCK0_MASK));

    /* Set-up the bus speed: 12MHz with the HSB at 48MHz. */
    avr32_pm_cksel_t bus_conf;

    /*  To set the HSB bus for the usb features*/
    bus_conf.hsbsel = ZERO_INIT;
    bus_conf.hsbdiv = ZERO_INIT;
    bus_conf.pbasel = ZERO_INIT;
    bus_conf.pbadiv = ZERO_INIT;
    bus_conf.pbbsel = ZERO_INIT;
    bus_conf.pbbdiv = ZERO_INIT;
    AVR32_PM.CKSEL = bus_conf;

    /*Waiting till the clock gets stable*/
    while (!(AVR32_PM.poscsr & AVR32_PM_POSCSR_CKRDY_MASK));

    /* Set one wait state. */
    AVR32_FLASHC.FCR.fws = ENABLE_FCR;

    /* Enable clock. */
    AVR32_PM.MCCTRL.mcsel = AVR32_PM_MCCTRL_MCSEL_PLL0;

    /* Set up GCLK for USB. */
    avr32_pm_gcctrl_t gc_conf;

    /*Enabling the clock to the usb*/
    gc_conf.cen = USB_CLOCK_ENABLE;

    /*Selecting the clock control for the usb peripheral*/
    AVR32_PM.GCCTRL[AVR32_PM_GCLK_USBB] = gc_conf;

    /*Selecting pba clock to spi peripheral */
    AVR32_PM.clkmask[SPI_BUS] 	|= SPI_PBA_BUS_SEL;

    /*Selecting pba clock to timer peripheral */
    AVR32_PM.clkmask[TIMER_BUS] |= TIMER_PBA_BUS_SEL;

    /*Selecting pba clock to uart peripheral */
    AVR32_PM.clkmask[UART_BUS] 	|= USART_PBA_BUS_SEL;
}

/**
 * @brief Initialize the timer of the ATUC3A3256
 */
void timer_init_non_generic(void)
{
    /* Timer channel 0 configuration */
    /* Set up the timer channel0 to make a 1 millisecond output to feed channel 1. */
    avr32_tc_cmr_t cmr_conf = {.waveform = {
        .acpc = AVR32_TC_CMR0_ACPC_CLEAR,             /* 0x02 - CLear- RC Compare Effect on TIOA */
        .acpa = AVR32_TC_CMR0_ACPA_SET,               /* 0x01 - Set - RA Compare Effect on TIOA */
        .wave = SET,                                  /* 0x01 - Capture Mode is Disabled */
        .wavsel = AVR32_TC_CMR0_WAVSEL_UP_AUTO,       /* 0x02 - UP mode with automatic trigger on RC Compare */
        .tcclks = AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK4,  /* 0x01 - F_pba/32 = 0.9 Microsecond */
    }};

    /*AVR32_TC0.channel[0].CMR*/
    TC_CH0.CMR = cmr_conf;
    /*All interrupts are disabled for timers*/
    TC_CH0.idr = DISABLE_ALL_TIMER_INTERRUPTS;
    /*Register A value to Comapre*/
    TC_CH0.ra = ONE_MILLISECOND_DELAY;
    /*Register C value to Comapre*/
    TC_CH0.rc = ONE_MILLISECOND_OVERFLOW;
    /*Clock is Enabled for channel 0*/
    TC_CH0.CCR.clken = AVR32_TC_CCR0_CLKEN_SIZE;

   /* Timer channel 1 configuration */
    avr32_tc_cmr_t cmr_conf2 = {.waveform = {
        .bswtrg=SET,	    			      /* Software trigger enabled*/
        .beevt=NOT_SET,                               /* External Event Effect on TIOB*/
        .eevt =NOT_SET,                               /* TIOB is chosen as the external event signal, it is configured as an input*/
        .eevtedg=SET,                                 /* Event Selection Rising Edge*/
        .wave = SET,                                  /* 0x01 - Capture Mode is Disabled*/
        .wavsel = AVR32_TC_CMR0_WAVSEL_UP_AUTO,       /* 0x02 - UP mode with automatic trigger on RC Compare*/
        .tcclks = AVR32_TC_CMR0_TCCLKS_XC1,           /* Clock is chained from channel 0 */
    }};

    /*AVR32_TC0.channel[1].CMR*/
    TC_CH1.CMR = cmr_conf2 ;
    /* All interrupts are disabled for timers*/
    TC_CH1.idr = DISABLE_ALL_TIMER_INTERRUPTS;
    /*Clock is Enabled for channel 1*/
    TC_CH1.CCR.clken = AVR32_TC_CCR1_CLKEN_SIZE;


   /* Timer channel 2 configuration */
    avr32_tc_cmr_t cmr_conf3 = {.capture = {
        .ldrb=NOT_SET,
        .ldra=SET,		                      /*Loading Each Edge*/
        .wave =NOT_SET,                               /*TIOB is chosen as the external event & configured as an input*/
        .cpctrg=NOT_SET,
        .abetrg=SET,                                  /*TIOA as External Trigger*/
        .etrgedg=SET,		                      /*Event Selection Rising Edge*/
        .ldbdis=NOT_SET,
        .ldbstop=NOT_SET,
        .burst=NOT_SET,
        .tcclks = AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK4,   /*External timer clock is selected & it gives a clock of 1 Mhz*/
    }};
    /*Configuring AVR32_TC1.channel[0].CMR*/
    TC_CH2.CMR = cmr_conf3 ;

    /*All interrupts are disabled for timers*/
    TC_CH2.idr = DISABLE_ALL_TIMER_INTERRUPTS;

    /*Enabling the interrupt for the capture*/
    TC_CH2.ier = AVR32_TC_IER0_LDRAS_MASK ;
    TC_CH2.CCR.clken = AVR32_TC_CCR0_CLKEN_SIZE;

    /*Chaining timer channels & External Clock Signal 1 Selection, TIOA0 is Given as i/p*/
    TC.BMR.tc1xc1s = AVR32_TC_BMR_TC1XC1S_TIOA0;

    /*Software trigger simultaneously for each of the channels*/
    TC.BCR.sync = AVR32_TC_SYNC_SIZE;

     /* Timer/Counter 1 configuration */
    avr32_tc_cmr_t cmr_conf4 = {.waveform = {
        .bswtrg=SET,					/*Software trigger enabled*/
        .beevt=NOT_SET,                                 /*External Event Effect on TIOB*/
        .eevt =NOT_SET,                                 /*TIOB is chosen as the external event signal, it is configured as an input*/
        .eevtedg=SET,                                   /*Event Selection Rising Edge*/
        .wave = SET,                                    /*Capture Mode is Disabled*/
        .wavsel = AVR32_TC_CMR0_WAVSEL_UP_AUTO,       	/*UP mode with automatic trigger on RC Compare*/
        .tcclks = AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK4,  	/*External timer clock is selected & it gives a clock of 1 Mhz*/
     }};
    /*Configuring AVR32_TC1.channel[1].CMR*/
    TC2_CH0.CMR = cmr_conf4 ;
    /*All interrupts are disabled for timers*/
    TC2_CH0.idr = DISABLE_ALL_TIMER_INTERRUPTS;
    /*Test value to test the output compare effectively*/
    TC2_CH0.ra = TEST_OUTPUT_VALUE;

#ifdef ENABLE_HIGH_PRIO_TMR
    avr32_tc_cmr_t cmr_conf5 = {.waveform = {
        .bswtrg=SET,					/*Software trigger enabled*/
        .beevt=NOT_SET,                                 /*External Event Effect on TIOB*/
        .eevt =NOT_SET,                                 /*TIOB is chosen as the external event signal, it is configured as an input*/
        .eevtedg=SET,                                   /*Event Selection Rising Edge*/
        .wave = SET,                                    /*Capture Mode is Disabled*/
        .wavsel = AVR32_TC_CMR0_WAVSEL_UP_AUTO,       	/*UP mode with automatic trigger on RC Compare*/
        .tcclks = AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK4,    /*External timer clock is selected & it gives a clock of 1 Mhz*/
    }};
    /*Configuring AVR32_TC1.channel[1].CMR*/
    TC2_CH1.CMR = cmr_conf5 ;
    /*All interrupts are disabled for timers*/
    TC2_CH1.idr = DISABLE_ALL_TIMER_INTERRUPTS;
    /*Test value to test the output compare effectively*/
    TC2_CH1.CCR.clken = AVR32_TC_CCR0_CLKEN_SIZE;
#endif

    /*Enable overflow interrupt*/
    TC2_CH0.ier   =  AVR32_TC_IER0_COVFS_MASK;

    /*Enable output compare match interrupt*/
    TC2_CH0.ier  |=  AVR32_TC_IER0_CPAS_MASK;

    /*Enabling the clock*/
    TC2_CH0.CCR.clken = AVR32_TC_CCR0_CLKEN_SIZE;

    /*Chaining timer channel 0 & synching them */
    TC.BCR.sync = AVR32_TC_SYNC_SIZE;

    /*Chaining timer channel 0 & synching them */
    TC2.BCR.sync = AVR32_TC_SYNC_SIZE;
}



/**
 * @brief Read one byte from an AT25010 EEPROM.
 *
 * Due to the connection between MCU, TRX and EEPROM, an EEPROM access
 * causes a transceiver reset. Therefore an entire transceiver configuration
 * is necessary after EEPROM access.
 *
 * @param addr Byte address to read from
 *
 * @return Data read from EEPROM
 */
#ifdef EXTERN_EEPROM_AVAILABLE
static uint8_t at25010_read_byte(uint8_t addr)
{

    uint8_t read_value;
#if 0
    RST_LOW();
    ENTER_CRITICAL_REGION();

    SS_LOW();

    PAL_WAIT_1_US();

    SPI_DATA_REG = AT25010_CMD_READ;
    SPI_WAIT();
    SPI_DATA_REG = addr & 0x7F; // mask out Bit 7 for 128x8 EEPROM
    SPI_WAIT();

    SPI_DATA_REG = 0;   // dummy value to start SPI transfer
    SPI_WAIT();
    read_value = SPI_DATA_REG;

    SS_HIGH();
    LEAVE_CRITICAL_REGION();
    RST_HIGH();
#endif
    return read_value;
}
#endif



/**
 * @brief Get data from external EEPROM
 *
 * @param[out] *value storage location for result
 *
 * @return SUCCESS if external EERPOM is available and contains valid contents
 *         FAILURE else
 */
#ifdef EXTERN_EEPROM_AVAILABLE
retval_t extern_eeprom_get(ps_id_t attribute, void *value)
{
#if 0
    uint8_t i;
    struct cfg_eeprom_data cfg;
    uint8_t *up;
    uint16_t crc;

    // Read data from external EEPROM.
    for (i = 0, up = (uint8_t *)&cfg; i < sizeof(cfg); i++, up++)
    {
        *up = at25010_read_byte(i);
    }

    // Calcute CRC to validate data correctness
    for (i = 0, crc = 0, up = (uint8_t *)&cfg; i < sizeof(cfg); i++, up++)
    {
        crc = CRC_CCITT_UPDATE(crc, *up);
    }
    if (crc != 0)
    {
        return FAILURE;
    }

    // Get the requested value out of the EEPROM data structure
    switch (attribute)
    {
        case PS_IEEE_ADDR:
            *(uint64_t *)value = cfg.binary_data.struct_binary.mac_address;
            break;

        case PS_XTAL_TRIM:
            *(uint8_t *)value = cfg.binary_data.struct_binary.cal_16mhz;
            break;

        default:
            return UNSUPPORTED_ATTRIBUTE;
    }
#endif
    return SUCCESS;
}
#endif


#if defined(__ICCAVR__) || defined(DOXYGEN)
/* This function is available in WINAVR library */
/**
 * @brief Computes the CCITT-CRC16 on a byte by byte basis
 *
 * This function computes the CCITT-CRC16 on a byte by byte basis.
 * It updates the CRC for transmitted and received data using the CCITT 16bit
 * algorithm (X^16 + X^12 + X^5 + 1).
 *
 * @param crc Current crc value
 * @param data Next byte that should be included into the CRC16
 *
 * @return updated CRC16
 */
#ifdef EXTERN_EEPROM_AVAILABLE
uint16_t crc_ccitt_update(uint16_t crc, uint8_t data)
{
    data ^= crc & 0xFF;
    data ^= data << 4;

    return ((((uint16_t)data << 8) | ((crc & 0xFF00) >> 8)) ^ \
            (uint8_t)(data >> 4) ^ \
            ((uint16_t)data << 3));
}
#endif /* EXTERN_EEPROM_AVAILABLE */
#endif /* __ICCAVR__ || defined(DOXYGEN) */


/**
 * @brief Initialize LEDs
 */
void pal_led_init(void)
{
        gpio_enable_output(LED0_PIN);
        gpio_enable_output(LED1_PIN);
        gpio_enable_output(LED2_PIN);
}


/**
 * @brief Control LED status
 *
 * param[in]  led_no LED ID
 * param[in]  led_setting LED_ON, LED_OFF, LED_TOGGLE
 */
void pal_led(led_id_t led_no, led_action_t led_setting)
{
    uint8_t pin;

    switch (led_no)
    {
        case LED_0: pin = LED0_PIN; break;
        case LED_1: pin = LED1_PIN; break;
        case LED_2: pin = LED2_PIN; break;
        default: pin = LED2_PIN; break;
    }

    switch (led_setting)
    {
        case LED_ON: gpio_clr_gpio_pin(pin); break;

        case LED_OFF: gpio_set_gpio_pin(pin); break;

        case LED_TOGGLE: gpio_tgl_gpio_pin(pin); break;
        default: /* do nothing */ break;
    }
}

/**
 * @brief Button initialization
 */
void pal_button_init(void)
{
    gpio_enable_input(BUTTON_PIN_0);
}


/**
 * @brief Reading the Button value
 * @param button_no Button Pin Number
 *
 * @return BUTTON_PRESSED if button is pressed
 *         BUTTON_OFF if button is not pressed
 */

button_state_t pal_button_read(button_id_t button_no)
{
    /* Keep compiler happy. */
    button_no = button_no;

    if (LOW == gpio_get(BUTTON_PIN_0))
    {
        return BUTTON_PRESSED;
    }
    else
    {
        return BUTTON_OFF;
    }
}

#endif /* REB_4_0_STK600 */

/* EOF */
