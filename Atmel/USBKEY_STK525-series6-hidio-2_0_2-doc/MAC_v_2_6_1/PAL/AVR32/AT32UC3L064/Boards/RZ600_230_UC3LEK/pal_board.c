/**
 * @file pal_board.c
 *
 * @brief PAL board specific functionality
 *
 * This file implements PAL board specific functionality for AT32UC3L064.
 *
 * $Id: pal_board.c 25915 2011-03-11 11:52:24Z yogesh.bellan $
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
#include "pal.h"
#include "pal_internal.h"
#include "pal_boardtypes.h"
#include "pal_config.h"
#include "pal_timer.h"
/*
 * Headerfile for AT32UC3L QTouch Libary API interface for QMatrix, QTouch and
 * Autonomous Touch method using the Capacitive Touch (CAT) module.
 */
#ifdef TOUCH_UC3LEK
#include "touch_api_at32uc3l.h"
#endif

#if (BOARD_TYPE == RZ600_230_UC3LEK)

/* === Macros ============================================================== */

/* === Types =============================================================== */

/**
 * UC3L PowerManager Writable Bit-Field Registers
 */
typedef union
{
    unsigned long                 cpusel;
    avr32_pm_cpusel_t             CPUSEL;
} u_avr32_pm_cpusel_t;

/**
 * FLASHCDW Writable Bit-Field Registers
 */
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
 * Table for interrupt groups.
 * Each group contains a member pointer to corresponding interrupt handler.
 */
#if defined(__GNUC__)
struct
{
    volatile __int_handler int_handler;
}int_table[AVR32_INTC_NUM_INT_GRPS];
#endif


/* === Globals ============================================================= */

/**
 * Error status variable of the FLASHCDW.
 * It is updated by funcitons that issue FLASHCDW commands.
 */
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
    *timestamp  = (uint32_t)TIMER_HIGH_REGISTER << 16;
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


/**
 * @brief Enables the GPIO for ouput
 *
 * @param pin GPIO pin to be configured for output
 */
void gpio_enable_output(uint32_t pin)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
    gpio_port->gpers = 1 << (pin & 0x1F);
    gpio_port->oderc = 1 << (pin & 0x1F);
    gpio_port->oders = 1 << (pin & 0x1F); // The GPIO output driver is enabled for that pin.
}


/**
 * @brief Enables the GPIO for input
 *
 * @param pin GPIO pin to be configured for input
 */
void gpio_enable_input(uint32_t pin)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
    gpio_port->gpers = 1 << (pin & 0x1F);
    gpio_port->oderc = 1 << (pin & 0x1F);
    gpio_port->oderc = 1 << (pin & 0x1F); // The GPIO output driver is disabled for that pin.
}


/**
 * @brief Sets the GPIO pin
 *
 * @param pin GPIO pin to be set
 */
void gpio_set_gpio_pin(uint32_t pin)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
    gpio_port->ovrs  = 1 << (pin & 0x1F); // Value to be driven on the I/O line: 1.
}


/**
 * @brief Clears the GPIO pin
 *
 * @param pin GPIO pin to be cleared
 */
void gpio_clr_gpio_pin(uint32_t pin)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
    gpio_port->ovrc  = 1 << (pin & 0x1F); // Value to be driven on the I/O line: 0.
    gpio_port->oders = 1 << (pin & 0x1F); // The GPIO output driver is enabled for that pin.
    gpio_port->gpers = 1 << (pin & 0x1F); // The GPIO module controls that pin.
}


/**
 * @brief Toggles the GPIO pin
 *
 * @param pin GPIO pin to be toggled
 */
void gpio_tgl_gpio_pin(uint32_t pin)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];
    gpio_port->ovrt  = 1 << (pin & 0x1F); // Value to be driven on the I/O line: tgl.
}


/**
 * @brief Sets the GPIO pin state
 *
 * @param pin GPIO pin to be made HIGH or LOW
 * @param state New pin state
 */
void pal_gpio_set(gpio_pin_type_t pin, pin_state_t state)
{
    if (SLP_TR_PIN == pin)
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
    else if (RST_PIN == pin)
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
 * This function is used to initialize the port pins to behave for particular
 * function.
 * @param pin GPIO pin for the initialization
 * @param function  Funciton number for the pin
 */
static void gpio_enable_module_pin(unsigned int pin, unsigned int function)
{
    volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[pin >> 5];

    /* Enable the correct function. */
    switch (function)
    {
        case 0: // function A.
        gpio_port->pmr0c = 1 << (pin & 0x1F);
        gpio_port->pmr1c = 1 << (pin & 0x1F);
        #if defined(AVR32_GPIO_210_H_INCLUDED) || defined(AVR32_GPIO_211_H_INCLUDED)
            gpio_port->pmr2c = 1 << (pin & 0x1F);
        #endif
        /* Disable GPIO control. */
        gpio_port->gperc = 1 << (pin & 0x1F);
        break;

        case 1: // function B.
        gpio_port->pmr0s |= 1 << (pin & 0x1F);
        gpio_port->pmr1c |= 1 << (pin & 0x1F);
        gpio_port->pmr2c |= 1 << (pin & 0x1F);
        /* Disable GPIO control. */
        gpio_port->gperc |= 1 << (pin & 0x1F);
        break;

        default:return;
    }

}


/**
 * @brief Initializes the GPIO pins
 *
 * This function is used to initialize the transceiver and peripheral
 * port pins from AT32UC3A3235.
 */
void gpio_init(void)
{
    #if ((defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))
        /* Set-up the UART pins to respective functions. */
        gpio_enable_module_pin(USART_RX, USART_RX_FUNCTION);
        gpio_enable_module_pin(USART_TX, USART_TX_FUNCTION);
    #endif

    /* Initialize peripheral interrupt pin. */
    gpio_enable_input(EXT_INT);

    /* Initialize TRX_RST, SLP_TR and SEL as GPIO. */
    gpio_enable_output(TRX_RST);
    gpio_enable_output(SLP_TR);
    gpio_set_gpio_pin(SLP_TR);
    gpio_enable_output(SEL);
    gpio_set_gpio_pin(SEL);

    /* Initialize pins for SPI pins(11 - 13) to respective functions. */
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
void register_interrupt(__int_handler handler, uint32_t group, uint32_t int_level)
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
__int_handler _get_interrupt_handler(uint32_t int_level)
{
    uint32_t int_grp = AVR32_INTC.icr[AVR32_INTC_INT3 - int_level];
    uint32_t int_req = AVR32_INTC.irr[int_grp];
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
    /* Disable all interrupts. */
    pal_global_irq_disable();

    /* Initialize Exception Vector Base Address (EVBA). */
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
    register_interrupt(&touch_acq_done_irq, CAT_IRQ_GROUP, CAT_ISR_PRIORITY);
    #endif

    /*Enable all interrupts. */
    pal_global_irq_enable();

#endif
}


/**
 * @brief This function is used to divide the clock and set the domain of the clock.
 *
 * The CPUSEL and PBASEL and PBBSEL registers all have the same structure
 * The ckSEL registers are contiguous and memory-mapped in
 * that order: CPUSEL, HSBSEL, PBASEL, PBBSEL.
 * @param clock_domain clock domain
 * @param divratio division ratio of the clock
 */
long pm_set_clk_domain_div(pm_clk_domain_t clock_domain, pm_divratio_t divratio)
{
    u_avr32_pm_cpusel_t u_avr32_pm_cpusel = {AVR32_PM.cpusel};

    /* wait untill SR.CKRDY becomes 1.*/
    while(!(AVR32_PM.sr & AVR32_PM_SR_CKRDY_MASK));

    /* Division ratio and cpu division is updated */
    u_avr32_pm_cpusel.CPUSEL.cpudiv= 1;
    u_avr32_pm_cpusel.CPUSEL.cpusel = divratio;

    /* Entering critical region */
    AVR32_ENTER_CRITICAL_REGION( );

    /* Unlock the write-protected ckSEL register */
    PM_UNLOCK(AVR32_PM_CPUSEL + clock_domain*sizeof(avr32_pm_cpusel_t));

    /* Cpu select register is updated */
    *(&(AVR32_PM.cpusel) + clock_domain)= u_avr32_pm_cpusel.cpusel;

    /* Leave critical region */
    AVR32_LEAVE_CRITICAL_REGION( );

    /* Return success */
    return SUCCESS;
}


/**
 * @brief This function sets the clock source for the system
 * @param src Clock Source
 */
long pm_set_mclk_source(pm_clk_src_t src)
{
    /* Entering critical region */
    AVR32_ENTER_CRITICAL_REGION( );
    /* Unlock the write-protected MCCTRL register */
    PM_UNLOCK(AVR32_PM_MCCTRL);
    /* Select the source for MCCTRL */
    AVR32_PM.mcctrl = src;
    /* Leave critical region */
    AVR32_LEAVE_CRITICAL_REGION( );
    /* Return success */
    return SUCCESS;
}


/**
 * @brief This function used to get the error status of the Flash
 */
static unsigned int flashcdw_get_error_status(void)
{
    return AVR32_FLASHCDW.fsr & (AVR32_FLASHCDW_FSR_LOCKE_MASK |
                             AVR32_FLASHCDW_FSR_PROGE_MASK);
}


/**
 * @brief This function used to check whether the flash is ready
 */
bool flashcdw_is_ready(void)
{
    return ((AVR32_FLASHCDW.fsr & AVR32_FLASHCDW_FSR_FRDY_MASK) != 0);
}


/**
 * @brief This function used to wait untill the flash is ready for the operation(default state)
 */
void flashcdw_default_wait_until_ready(void)
{
    while (!flashcdw_is_ready());
}

/**
 * @brief This function used to wait untill the flash is ready for the operation(Normal state)
 */
void (*volatile flashcdw_wait_until_ready)(void) = flashcdw_default_wait_until_ready;


/**
 * @brief This function used to issue flash commands
 * @param Flash command, Flash page
 */
void flashcdw_issue_command(unsigned int command, int page_number)
{
    u_avr32_flashcdw_fcmd_t u_avr32_flashcdw_fcmd;
    /* Wait untill the Flash is ready */
    flashcdw_wait_until_ready();
    /* Update the flash command */
    u_avr32_flashcdw_fcmd.fcmd = AVR32_FLASHCDW.fcmd;
    u_avr32_flashcdw_fcmd.FCMD.cmd = command;
    /* To define the Flash page */
    if (page_number >= 0) u_avr32_flashcdw_fcmd.FCMD.pagen = page_number;
    u_avr32_flashcdw_fcmd.FCMD.key = AVR32_FLASHCDW_FCMD_KEY_KEY;
    AVR32_FLASHCDW.fcmd = u_avr32_flashcdw_fcmd.fcmd;
    /* To get the Flash error status if any */
    flashcdw_error_status = flashcdw_get_error_status();
    {
        /*Keep Compiler Smiling Always*/
        flashcdw_error_status = flashcdw_error_status;
    }
    /* Wait untill the Flash is ready */
    flashcdw_wait_until_ready();
}

/**
 * @brief This function used to issue flash wait states
 * @param wait_state Flash wait state
 */
void flashcdw_set_wait_state(unsigned int wait_state)
{
    u_avr32_flashcdw_fcr_t u_avr32_flashcdw_fcr = {AVR32_FLASHCDW.fcr};
    u_avr32_flashcdw_fcr.FCR.fws = wait_state;
    AVR32_FLASHCDW.fcr = u_avr32_flashcdw_fcr.fcr;
}

/**
 * @brief This function used to issue flash wait states in the read mode
 * @param cpu_f_hz Cpu wait state in hertz
 */
void flashcdw_set_flash_waitstate_and_readmode(unsigned long cpu_f_hz)
{
    /* Check if the cpu runs greater than 15Mhz */
    if( cpu_f_hz > AVR32_FLASHCDW_FWS_0_MAX_FREQ )
    {
        /* Check if the cpu runs less than or equal to 30 Mhz */
        if( cpu_f_hz <= AVR32_FLASHCDW_FWS_1_MAX_FREQ )
        {
            /* Set a wait-state, disable the high-speed read mode.*/
            flashcdw_set_wait_state(1);
            flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_HSDIS, -1);
        }
        else
        {
            /* Set a wait-state, enable the high-speed read mode.*/
            flashcdw_set_wait_state(1);
            flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_HSEN, -1);
        }
    }

    /* Check if the cpu runs less than or equal to 15Mhz */
    else
    {
        /* No wait-state, disable the high-speed read mode */
        flashcdw_set_wait_state(0);
        flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_HSDIS, -1);
    }
}


/**
 * @brief This function used to start the 120 Mhz RC oscillator
 */

void scif_start_rc120M(void)
{
    /* Entering critical region */
    AVR32_ENTER_CRITICAL_REGION( );
    /*Unlock RC120MCR write protected register */
    SCIF_UNLOCK(AVR32_SCIF_RC120MCR);
    AVR32_SCIF.rc120mcr = AVR32_SCIF_RC120MCR_EN_MASK;
    /* Leave critical region */
    AVR32_LEAVE_CRITICAL_REGION( );
}

/**
 * @brief This function used to switch from internal RC to 120 Mhz RC oscillator
 */
static void local_set_main_clock_to_rc120m(void)
{
    /* Start the 120MHz internal RCosc (RC120M) clock */
    scif_start_rc120M();

    /* Since our target is to set the CPU&HSB frequency domains to 30MHz, set the
    appropriate wait-state and speed read mode on the flash controller.*/
    flashcdw_set_flash_waitstate_and_readmode(CPUCLK_HZ);

    /* Set the CPU clock domain to 30MHz (by applying a division ratio = 4).*/
    pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_CPU, PM_CKSEL_DIVRATIO_4);

    /* Set the PBA clock domain to 30MHz (by applying a division ratio = 4). */
    pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_PBA, PM_CKSEL_DIVRATIO_4);

    /* Set the PBB clock domain to 30MHz (by applying a division ratio = 4).*/
    pm_set_clk_domain_div((pm_clk_domain_t)AVR32_PM_CLK_GRP_PBB, PM_CKSEL_DIVRATIO_4);

    /* Set the main clock source to be the RC120M.*/
    pm_set_mclk_source(PM_CLK_SRC_RC120M);
}


#ifdef TOUCH_UC3LEK
long int scif_stop_gclk(unsigned int gclk)
{
    unsigned int  timeout = SCIF_POLL_TIMEOUT;

    /* Stop the generic clock. */
    AVR32_SCIF.gcctrl[gclk] &= ~AVR32_SCIF_GCCTRL_CEN_MASK;

    /* Wait until the generic clock is actually stopped. */
    while(AVR32_SCIF.gcctrl[gclk] & AVR32_SCIF_GCCTRL_CEN_MASK)
    {
        if(--timeout == 0)
            return ERROR;
    }

    /* Return success */
    return SUCCESS;
}
#endif


#ifdef TOUCH_UC3LEK
long int scif_gc_setup(unsigned int gclk, scif_gcctrl_oscsel_t clk_src, unsigned int diven, unsigned int divfactor)
{
    /*Initializing the variable to default state*/
    int restart_gc = false;

    /* Change the division factor to conform to the equation:
     * fgclk = fsrc/divfactor = fsrc/(2*(div+1))
     */
    divfactor = (divfactor>>1) -1;

    /* If the generic clock is already enabled, disable it before changing its setup.*/
    if(AVR32_SCIF.gcctrl[gclk] & AVR32_SCIF_GCCTRL_CEN_MASK)
    {
        restart_gc = true;
        if(scif_stop_gclk(gclk) < 0)
        /* Could not stop the generic clock*/
        return ERROR;
    }

    /* Setup the generic clock. */
    AVR32_SCIF.gcctrl[gclk] = ((divfactor << AVR32_SCIF_GCCTRL_DIV_OFFSET)&AVR32_SCIF_GCCTRL_DIV_MASK)
                            |((diven << AVR32_SCIF_GCCTRL_DIVEN_OFFSET)&AVR32_SCIF_GCCTRL_DIVEN_MASK)
                            |((clk_src << AVR32_SCIF_GCCTRL_OSCSEL_OFFSET)&AVR32_SCIF_GCCTRL_OSCSEL_MASK);

    /* Restart the gc if it previously was enabled.*/
    if(true == restart_gc)  AVR32_SCIF.gcctrl[gclk] |= (AVR32_SCIF_GCCTRL_CEN_MASK);

    /* Return success */
    return SUCCESS;
}
#endif


#ifdef TOUCH_UC3LEK
long int scif_gc_enable(unsigned int gclk)
{
    /* If the generic clock is already enabled, do nothing.*/
    if(!(AVR32_SCIF.gcctrl[gclk] & AVR32_SCIF_GCCTRL_CEN_MASK))
    {
        AVR32_SCIF.gcctrl[gclk] |= (AVR32_SCIF_GCCTRL_CEN_MASK);
    }

    /* Return success */
    return SUCCESS;
}
#endif


/**
 * @brief Initialize the clock of the ATUC3L064
 */
void clock_init(void)
{

    /* Initialize the clock of the ATUC3L064 */
    local_set_main_clock_to_rc120m();

#ifdef TOUCH_UC3LEK
    int32_t ret_val = 0u;
    /* Set up the GCLK_CAT for proper QMatrix operation.  The GCLK_CAT is not
       required to be setup for Autonomous Touch and QTouch Group A/B operation.
       Note: for UC3L devices, the generic clock configurations are handled by the
       SCIF module. Setup gc to use the DFLL0 as source clock, divisor enabled, apply
       a division factor. */
    ret_val |= scif_gc_setup(AVR32_SCIF_GCLK_CAT, SCIF_GCCTRL_RC120M, AVR32_GC_DIV_CLOCK, QM_GCLK_CAT_DIV);

    /* Now enable the generic clock */
    ret_val |= scif_gc_enable(AVR32_SCIF_GCLK_CAT);
#endif
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
        .bswtrg=SET,                                  /* Software trigger enabled*/
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
        .ldra=SET,                                    /*Loading Each Edge*/
        .wave =NOT_SET,                               /*TIOB is chosen as the external event & configured as an input*/
        .cpctrg=NOT_SET,
        .abetrg=SET,                                  /*TIOA as External Trigger*/
        .etrgedg=SET,                                 /*Event Selection Rising Edge*/
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
        .bswtrg=SET,                                    /*Software trigger enabled*/
        .beevt=NOT_SET,                                 /*External Event Effect on TIOB*/
        .eevt =NOT_SET,                                 /*TIOB is chosen as the external event signal, it is configured as an input*/
        .eevtedg=SET,                                   /*Event Selection Rising Edge*/
        .wave = SET,                                    /*Capture Mode is Disabled*/
        .wavsel = AVR32_TC_CMR0_WAVSEL_UP_AUTO,         /*UP mode with automatic trigger on RC Compare*/
        .tcclks = AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK4,    /*External timer clock is selected & it gives a clock of 1 Mhz*/
     }};
    /*Configuring AVR32_TC1.channel[1].CMR*/
    TC2_CH0.CMR = cmr_conf4 ;
    /*All interrupts are disabled for timers*/
    TC2_CH0.idr = DISABLE_ALL_TIMER_INTERRUPTS;

#ifdef ENABLE_HIGH_PRIO_TMR
    avr32_tc_cmr_t cmr_conf5 = {.waveform = {
        .bswtrg=SET,                                    /*Software trigger enabled*/
        .beevt=NOT_SET,                                 /*External Event Effect on TIOB*/
        .eevt =NOT_SET,                                 /*TIOB is chosen as the external event signal, it is configured as an input*/
        .eevtedg=SET,                                   /*Event Selection Rising Edge*/
        .wave = SET,                                    /*Capture Mode is Disabled*/
        .wavsel = AVR32_TC_CMR0_WAVSEL_UP_AUTO,         /*UP mode with automatic trigger on RC Compare*/
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
    /* To enable all the leds*/
    gpio_enable_output(LED0_PIN);
    gpio_enable_output(LED1_PIN);
    gpio_enable_output(LED2_PIN);
    gpio_enable_output(LED3_PIN);
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
        case LED_3: pin = LED3_PIN; break;
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

/**
 * @brief Generation of random seed for function srand() in case this
 *        is not supported by the transceiver (e.g. AT86RF230)
 *
 * @return uint16_t Random number to be used as seed for function srand()
 *
 */
uint16_t pal_generate_rand_seed()
{
    /* Todo */
    return (1);
}

#endif /* RZ600_230_UC3LEK */

/* EOF */
