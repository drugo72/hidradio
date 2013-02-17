/**
 * @file pal_board.c
 *
 * @brief PAL board specific functionality
 *
 * This file implements PAL board specific functionality for AVR32 AT32UC3L064.
 *
 * $Id: pal_board.c 25122 2011-01-25 08:15:57Z yogesh.bellan $
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
typedef union
{
  unsigned long                 cpusel;
  avr32_pm_cpusel_t             CPUSEL;
} u_avr32_pm_cpusel_t;

typedef union
{
  unsigned long                   fcmd;
  avr32_flashcdw_fcmd_t           FCMD;
} u_avr32_flashcdw_fcmd_t;

typedef union
{
  unsigned long                   fcr;
  avr32_flashcdw_fcr_t            FCR;
} u_avr32_flashcdw_fcr_t;

/**
 * Create a table for interrupt groups.
 * Each group contains a member pointer to corresponding interrupt handler.
 */
#if defined(__GNUC__)
struct
{
  volatile __int_handler int_handler;
}int_table[AVR32_INTC_NUM_INT_GRPS];
#endif


/* === Globals ============================================================= */
static unsigned int flashcdw_error_status = 0;


/* === Prototypes ========================================================== */

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

void gpio_enable_output(uint32_t pin)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
    gpio_port->gpers = 1 << (pin & 0x1F);
    gpio_port->oderc = 1 << (pin & 0x1F);
    gpio_port->oders = 1 << (pin & 0x1F); // The GPIO output driver is enabled for that pin.
}


void gpio_enable_input(uint32_t pin)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
    gpio_port->gpers = 1 << (pin & 0x1F);
    gpio_port->oderc = 1 << (pin & 0x1F);
    gpio_port->oderc = 1 << (pin & 0x1F); // The GPIO output driver is disabled for that pin.
}


void gpio_set_gpio_pin(uint32_t pin)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
    gpio_port->ovrs  = 1 << (pin & 0x1F); // Value to be driven on the I/O line: 1.
}


void gpio_clr_gpio_pin(uint32_t pin)
{
     volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
     gpio_port->ovrc  = 1 << (pin & 0x1F); // Value to be driven on the I/O line: 0.
     gpio_port->oders = 1 << (pin & 0x1F); // The GPIO output driver is enabled for that pin.
     gpio_port->gpers = 1 << (pin & 0x1F); // The GPIO module controls that pin.
}


void gpio_tgl_gpio_pin(uint32_t pin)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
    gpio_port->ovrt  = 1 << (pin & 0x1F); // Value to be driven on the I/O line: tgl.
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
        gpio_port->pmr0c |= 1 << (pin & 0x1F);
        gpio_port->pmr1c |= 1 << (pin & 0x1F);
        gpio_port->pmr2c |= 1 << (pin & 0x1F);
        break;

        case 1: // B function.
        gpio_port->pmr0s |= 1 << (pin & 0x1F);
        gpio_port->pmr1c |= 1 << (pin & 0x1F);
        gpio_port->pmr2c |= 1 << (pin & 0x1F);
        break;

        case 2: // C function.
        gpio_port->pmr0c |= 1 << (pin & 0x1F);
        gpio_port->pmr1s |= 1 << (pin & 0x1F);
        gpio_port->pmr2c |= 1 << (pin & 0x1F);
        break;

        case 3: // D function.
        gpio_port->pmr0s |= 1 << (pin & 0x1F);
        gpio_port->pmr1s |= 1 << (pin & 0x1F);
        gpio_port->pmr2c |= 1 << (pin & 0x1F);
        break;

        case 4: // E function.
        gpio_port->pmr0c |= 1 << (pin & 0x1F);
        gpio_port->pmr1c |= 1 << (pin & 0x1F);
        gpio_port->pmr2s |= 1 << (pin & 0x1F);
        break;


        case 5: // F function.
        gpio_port->pmr0s |= 1 << (pin & 0x1F);
        gpio_port->pmr1c |= 1 << (pin & 0x1F);
        gpio_port->pmr2s |= 1 << (pin & 0x1F);
        break;

        case 6: // G function.
        gpio_port->pmr0c |= 1 << (pin & 0x1F);
        gpio_port->pmr1s |= 1 << (pin & 0x1F);
        gpio_port->pmr2s |= 1 << (pin & 0x1F);
        break;

        case 7: // H function.
        gpio_port->pmr0s |= 1 << (pin & 0x1F);
        gpio_port->pmr1s |= 1 << (pin & 0x1F);
        gpio_port->pmr2s |= 1 << (pin & 0x1F);
        break;

        default: return;
        }

        // Disable GPIO control.
        gpio_port->gperc |= 1 << (pin & 0x1F);
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
     //gpio_enable_module_pin(AVR32_EIC_EXTINT_1_0_PIN,6);
     gpio_enable_input(EXT_INT);

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
}


/**
 * @brief This function registers the corresponding interrupt for the group.
 *
 * The function stores pointer to the interrupt handler in int_table[group],
 * so that _get_interrupt can retrieve it when vectored.
 * And sets the corresponding interrupt group's priority level.
 *
 * @param handler Function pointer to the corresponding interrupt subroutine.
 * @param group Interrupt group.
 * @param int_level Priority level for the group.
 */
#if defined(__GNUC__)
void register_interrupt(__int_handler handler, unsigned int group, unsigned int int_level)
{
    int_table[group].int_handler = handler;
    AVR32_INTC.ipr[group] = ipr_val[int_level & (AVR32_INTC_IPR_INTLEVEL_MASK >> AVR32_INTC_IPR_INTLEVEL_OFFSET)];
}
#endif


/**
 * @brief This function gets the interrupt handler of the current event.
 *
 * @param int_level Interrupt priority level to handle.
 * @return Interrupt handler to execute.
 */
#if defined(__GNUC__)
__int_handler _get_interrupt_handler(unsigned int int_level)
{
    unsigned int int_grp = AVR32_INTC.icr[AVR32_INTC_INT3 - int_level];
    unsigned int int_req = AVR32_INTC.irr[int_grp];

    return (int_req) ? int_table[int_grp].int_handler : NULL ;
}
#endif


/**
 * @brief Initialize the interrupt system of the ATUC3A3256
 */
void interrupt_system_init(void)
{
    /* Enable high priority interrupts */
#if defined(__GNUC__)

    // Disable all interrupts.
    pal_global_irq_disable();

    // Initialize EVBA address.
    Set_system_register(AVR32_EVBA, (int)&_evba );

    register_interrupt(&ext_int_isr, EXT_INT_ISR_GROUP, EXT_INT_ISR_PRIORITY);

#if (TOTAL_NUMBER_OF_TIMERS > 0)
    register_interrupt(&TC_isr, TC_CH1_ISR_GROUP, TC_CH1_ISR_PRIORITY);
    register_interrupt(&TC2_isr, TC_CH2_ISR_GROUP, TC_CH2_ISR_PRIORITY);
#endif

#ifdef SIO_HUB
    register_interrupt(&usart0_isr, USART_ISR_GROUP, USART_ISR_PRIORITY);
#endif

#ifdef TOUCH_UC3LEK
    register_interrupt(&touch_acq_done_irq, 29, AVR32_INTC_INT3); //AVR32_CAT_IRQ_GROUP
#endif

    //Enable all interrupts.
    pal_global_irq_enable();
#endif
}

long pm_set_clk_domain_div(pm_clk_domain_t clock_domain, pm_divratio_t divratio)
{
  u_avr32_pm_cpusel_t u_avr32_pm_cpusel = {AVR32_PM.cpusel};

  // ckSEL must not be written while SR.CKRDY is 0.
  while(!(AVR32_PM.sr & AVR32_PM_SR_CKRDY_MASK));

  // Modify
  u_avr32_pm_cpusel.CPUSEL.cpudiv= 1;
  u_avr32_pm_cpusel.CPUSEL.cpusel = divratio;
  AVR32_ENTER_CRITICAL_REGION( );
  // Unlock the write-protected ckSEL register
  PM_UNLOCK(AVR32_PM_CPUSEL + clock_domain*sizeof(avr32_pm_cpusel_t));
  // Update
  *(&(AVR32_PM.cpusel) + clock_domain)= u_avr32_pm_cpusel.cpusel;
  AVR32_LEAVE_CRITICAL_REGION( );

  return PASS;
}

/**
 ** Clock Functions
 **/

long pm_set_mclk_source(pm_clk_src_t src)
{
  AVR32_ENTER_CRITICAL_REGION( );
  PM_UNLOCK(AVR32_PM_MCCTRL); /* Unlock the write-protected MCCTRL register */
  AVR32_PM.mcctrl = src;
  AVR32_LEAVE_CRITICAL_REGION( );
  return PASS;
}


static unsigned int flashcdw_get_error_status(void)
{
  return AVR32_FLASHCDW.fsr & (AVR32_FLASHCDW_FSR_LOCKE_MASK |
                             AVR32_FLASHCDW_FSR_PROGE_MASK);
}

bool flashcdw_is_ready(void)
{
  return ((AVR32_FLASHCDW.fsr & AVR32_FLASHCDW_FSR_FRDY_MASK) != 0);
}


void flashcdw_default_wait_until_ready(void)
{
  while (!flashcdw_is_ready());
}


void (*volatile flashcdw_wait_until_ready)(void) = flashcdw_default_wait_until_ready;


void flashcdw_issue_command(unsigned int command, int page_number)
{
  u_avr32_flashcdw_fcmd_t u_avr32_flashcdw_fcmd;
  flashcdw_wait_until_ready();
  u_avr32_flashcdw_fcmd.fcmd = AVR32_FLASHCDW.fcmd;
  u_avr32_flashcdw_fcmd.FCMD.cmd = command;
  if (page_number >= 0) u_avr32_flashcdw_fcmd.FCMD.pagen = page_number;
  u_avr32_flashcdw_fcmd.FCMD.key = AVR32_FLASHCDW_FCMD_KEY_KEY;
  AVR32_FLASHCDW.fcmd = u_avr32_flashcdw_fcmd.fcmd;
  flashcdw_error_status = flashcdw_get_error_status();
  {
    /*Keep Compiler Smiling Always*/
    flashcdw_error_status = flashcdw_error_status;
  }
  flashcdw_wait_until_ready();
}


void flashcdw_set_wait_state(unsigned int wait_state)
{
  u_avr32_flashcdw_fcr_t u_avr32_flashcdw_fcr = {AVR32_FLASHCDW.fcr};
  u_avr32_flashcdw_fcr.FCR.fws = wait_state;
  AVR32_FLASHCDW.fcr = u_avr32_flashcdw_fcr.fcr;
}

void flashcdw_set_flash_waitstate_and_readmode(unsigned long cpu_f_hz)
{
  if(cpu_f_hz > AVR32_FLASHCDW_FWS_0_MAX_FREQ)    // > 15MHz
  {
    if(cpu_f_hz <= AVR32_FLASHCDW_FWS_1_MAX_FREQ) // <= 30MHz
    {
      // Set a wait-state, disable the high-speed read mode.
      flashcdw_set_wait_state(1);
      flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_HSDIS, -1);
    }
    else
    {
      // Set a wait-state, enable the high-speed read mode.
      flashcdw_set_wait_state(1);
      flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_HSEN, -1);
    }
  }
  else  // <= 15MHz
  {
    // No wait-state, disable the high-speed read mode
    flashcdw_set_wait_state(0);
    flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_HSDIS, -1);
  }
}

/**
 ** 120MHz RCosc Functions
 **/

void scif_start_rc120M(void)
{
  AVR32_ENTER_CRITICAL_REGION( );
  // Unlock the write-protected RC120MCR register
  SCIF_UNLOCK(AVR32_SCIF_RC120MCR);
  AVR32_SCIF.rc120mcr = AVR32_SCIF_RC120MCR_EN_MASK;
  AVR32_LEAVE_CRITICAL_REGION( );
}


/* \brief Start RC120M, switch main clock to RC120M.
 */
static void local_set_main_clock_to_rc120m(void)
{
  // Start the 120MHz internal RCosc (RC120M) clock
  scif_start_rc120M();

  // Since our target is to set the CPU&HSB frequency domains to 30MHz, set the
  // appropriate wait-state and speed read mode on the flash controller.
  flashcdw_set_flash_waitstate_and_readmode(CPUCLK_HZ);

  // Set the CPU clock domain to 30MHz (by applying a division ratio = 4).
  pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_CPU, PM_CKSEL_DIVRATIO_4);

  // Set the PBA clock domain to 30MHz (by applying a division ratio = 4).
  pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_PBA, PM_CKSEL_DIVRATIO_4);

  // Set the PBB clock domain to 30MHz (by applying a division ratio = 4).
  pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_PBB, PM_CKSEL_DIVRATIO_4);

  // Set the main clock source to be the RC120M.
  pm_set_mclk_source(PM_CLK_SRC_RC120M);
}

/**
 * @brief Initialize the clock of the ATUC3A3256
 */
void clock_init(void)
{
  local_set_main_clock_to_rc120m();
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
 * @brief Reads and returns the GPIO pin status value
 *
 * @param pin GPIO pin whose value is to be read
 * @return HIGH
 * @return LOW
 */
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
