/**
 * @file pal_timer.h
 *
 * @brief PAL timer internal functions prototypes for AVR 8-Bit Mega RF Single Chips
 *
 * This header has the timer specific stuctures, macros and
 * internal functions for AVR 8-Bit Mega RF Single Chips.
 *
 * $Id: pal_timer.h 25524 2011-02-11 06:02:45Z uwalter $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */


/* Prevent double inclusion */
#ifndef PAL_TIMER_H
#define PAL_TIMER_H

/* === Includes ============================================================= */


/* === Types ================================================================ */

/*
 * This defines the structure of the time type.
 */
typedef struct timer_info_tag
{
    /* Timeout in microseconds */
    uint32_t abs_exp_timer;

    /* Callback function to be executed on expiry of the timer */
    FUNC_PTR timer_cb;

    /* Parameter to be passed to the callback function of the expired timer */
    void *param_cb;

    /* Next timer which was started or has expired */
    uint_fast8_t next_timer_in_queue;
} timer_info_t;

/*
 * Type definition for callbacks for timer functions
 */
typedef void (*timer_expiry_cb_t)(void *);

/* === Externals ============================================================ */

#ifdef NO_32KHZ_CRYSTAL
extern volatile uint16_t sys_time;
#endif

/* === Macros ================================================================ */

/*
 * Value to indicate end of timer in the array or queue
 */
#define NO_TIMER                (0xFF)

#ifdef NO_32KHZ_CRYSTAL
/*
 * Shift mask to obtain the 16-bit system time out of a 32-bit timeout
 */
#define SYS_TIME_SHIFT_MASK     (16)
#endif

#ifdef NO_32KHZ_CRYSTAL
/*
 * Mask to obtain the 16-bit H/W time out of a 32-bit timeout
 */
#define HW_TIME_MASK            (0xFFFF)
#endif

/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

void timer_init(void);
void timer_init_non_generic(void);
void internal_timer_handler(void);
void timer_service(void);

#ifdef NO_32KHZ_CRYSTAL
void add_sleeptime_to_systime(void);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PAL_TIMER_H */
/* EOF */
