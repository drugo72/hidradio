/**
 * @file pal_board.c
 *
 * @brief PAL board specific functionality
 *
 * This file implements PAL board specific functionality.
 *
 * $Id: pal_board.c 25917 2011-03-14 05:21:39Z jignesh.vaghela $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ============================================================ */

#include <stdbool.h>
#include <stdlib.h>
#include "pal.h"
#include "pal_boardtypes.h"
#include "pal_config.h"

#if (BOARD_TYPE == EK1)

/* === Macros ============================================================== */


/* === Types =============================================================== */


/* === Globals ============================================================= */


/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

/**
 * @brief Calibrates the internal RC oscillator
 *
 * This function calibrates the internal RC oscillator, based
 * on the external 32.768 Hz crystal clock source.
 *
 * @return True if calibration is successful, false otherwise.
 */
bool pal_calibrate_rc_osc(void)
{
#ifdef ENABLE_RC_OSC_CALIBRATION
    /*
     * Use the 32.768 kHz external crystal oscillator as reference clock source.
     */
    /*
     * This is the actual value of the timer to be calibrated
     * after each calibration run.
     */
    uint16_t curr_value_cal_timer = 0;

    /* This is the best OSCCAL value. */
    uint8_t best_value_osccal = 0;
    /*
     * This is the smallest difference between the target value and the actual value
     * of the timer the timer to be calibrated.
     */

    uint16_t best_dif_timers = ~(uint16_t)0;
    /*
     * This is the difference between  between the target value and the actual value
     * of the timer the timer to be calibrated of the current calibration attempt.
     */
    uint16_t curr_dif_timers = 0;

    uint16_t counter;
    uint8_t tccr2b;
    uint8_t tccr1b;
    uint8_t tccr1a;
    bool cal_result = true;

    ENTER_CRITICAL_REGION();

    /*
     * Save current values of timer status.
     */
    tccr2b = TCCR2B;
    tccr1b = TCCR1B;
    tccr1a = TCCR1A;

    /*
     * Stop timers 1 and 2.
     * Set timer 1 to normal mode (no CTC, no PWM, just count).
     */
    TCCR2B = 0;
    TCCR1B = 0;
    TCCR1A = 0;

    for (counter = 0; counter < MAX_CAL_LOOP_CNT;  counter++)
    {
        /*
         * Delete pending timer 1 and 2 interrupts, and clear the
         * counters.
         */
        TIFR1 = 0xFF;
        TIFR2 = 0xFF;
        TCNT2 = 0;
        TCNT1 = 0;

        /* Kick timer1 with internal clock source and no prescaler */
        TCCR1B = (1 << CS10);

        /*
         * Kick timer2 with external 32.768 Hz asynchronous clock
         * and no prescaler
         */
        TCCR2B = _BV(CS20);
        ASSR = _BV(AS2);

        /*
         * Wait for timer 2 to overflow.
         */
        while (!(TIFR2 & _BV(TOV2)))
        {
            /* Wait */
        }


        /*
         * Stop timer 1.  Now, TCNT1 contains the number of CPU cycles
         * counted during F_CPU / (32 * 256) cycles.
         */
        TCCR1B = 0;
        TCCR2B = 0;

        curr_value_cal_timer = TCNT1;

        /*
         * Check if reference timer is running at all,
         * i.e. the reference clock is available.
         */
        if (0 == curr_value_cal_timer)
        {
            /* Reference timer is not running, return error. */
            cal_result = false;
            break;
        }

        if (curr_value_cal_timer <= (uint16_t)(TARGETVAL_CALIBRATION))
        {
            curr_dif_timers = (uint16_t)(TARGETVAL_CALIBRATION) - curr_value_cal_timer;
        }
        else
        {
            curr_dif_timers = curr_value_cal_timer - (uint16_t)(TARGETVAL_CALIBRATION);
        }

        if (curr_dif_timers < best_dif_timers)
        {
            best_dif_timers = curr_dif_timers;
            best_value_osccal = OSCCAL;
        }

        if (curr_value_cal_timer <= (uint16_t)(TARGETVAL_CALIBRATION))
        {
            /* Too slow, increase speed */
            OSCCAL++;
        }
        else
        {
            /* Too fast, lower speed */
            OSCCAL--;
        }
    }

    TCCR2B = tccr2b;
    TCCR1B = tccr1b;
    TCCR1A = tccr1a;
    LEAVE_CRITICAL_REGION();

    OSCCAL = best_value_osccal;

    return (cal_result);
#else
    /* RC oscillator calibration is not enabled for this board. */
    return (true);
#endif  /* ENABLE_RC_OSC_CALIBRATION */
}



/**
 * @brief Initialize LEDs
 */
void pal_led_init(void)
{
    LED_PORT |= (1 << LED_PIN_0);
    LED_PORT_DIR |= (1 << LED_PIN_0);
    LED_PORT |= (1 << LED_PIN_1);
    LED_PORT_DIR |= (1 << LED_PIN_1);
    LED_PORT |= (1 << LED_PIN_2);
    LED_PORT_DIR |= (1 << LED_PIN_2);
}



/**
 * @brief Control LED status
 *
 * @param[in]  led_no LED ID
 * @param[in]  led_setting LED_ON, LED_OFF, LED_TOGGLE
 */
void pal_led(led_id_t led_no, led_action_t led_setting)
{
    switch (led_no)
    {
        default:
        case 0:
            switch (led_setting)
            {
                case LED_ON:    LED_PORT &= ~(1 << LED_PIN_0); break;
                case LED_OFF:   LED_PORT |= (1 << LED_PIN_0); break;
                case LED_TOGGLE: LED_PORT ^= (1 << LED_PIN_0); break;
            }
            break;

        case 1:
            switch (led_setting)
            {
                case LED_ON:    LED_PORT &= ~(1 << LED_PIN_1); break;
                case LED_OFF:   LED_PORT |= (1 << LED_PIN_1); break;
                case LED_TOGGLE: LED_PORT ^= (1 << LED_PIN_1); break;
            }
            break;

        case 2:
            switch (led_setting)
            {
                case LED_ON:    LED_PORT &= ~(1 << LED_PIN_2); break;
                case LED_OFF:   LED_PORT |= (1 << LED_PIN_2); break;
                case LED_TOGGLE: LED_PORT ^= (1 << LED_PIN_2); break;
            }
            break;
    }
}


/**
 * @brief Initialize the button
 */
void pal_button_init(void)
{
    BUTTON_PORT |= (1 << BUTTON_PIN_0);
    BUTTON_PORT_DIR &= ~(1 << BUTTON_PIN_0);
}



/**
 * @brief Read button
 *
 * @param button_no Button ID
 */
button_state_t pal_button_read(button_id_t button_no)
{
    /* Keep compiler happy. */
    button_no = button_no;

    if ((BUTTON_INPUT_PINS & (1 << BUTTON_PIN_0)) == 0x00)
    {
        return BUTTON_PRESSED;
    }
    else
    {
        return BUTTON_OFF;
    }
}

#endif /* EK1 */


/**
 * @brief Sleep/Power handling
 *
 * Handles Sleep/power modes for entire system
 *
 * @param sleep_mode desired sleep/power mode
 *
 * @ingroup apiPalApi
 */
void pal_sleep_mode(uint8_t sleep_mode)
{
    // Clear interrupt flag
    EIFR = (1 << INTF5);
    // Enable external interrupt
    EIMSK |= (1 << INT5);
    SET_SLEEP_MODE(sleep_mode);
    SLEEP_MODE();
    /* Continue after wakeup */
}


/**
 * @brief ISR for external interrupt at pin PE5
 */
ISR(INT5_vect)
{
    /* MCU wakeup */
    // Disable external interrupt
    EIMSK &= ~(1 << INT5);
}

/* EOF */
