/**
 * @file app_op_mode_2.h
 *
 * @brief These are application-specific resources which are used
 *        in the application in addition to the underlaying stack.
 *
 * $Id: app_op_mode_2.h 23760 2010-10-08 05:56:54Z prajosh.premdas $
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
#ifndef APP_OP_MODE_2_H
#define APP_OP_MODE_2_H

/* === Includes ============================================================= */

/* === Macros =============================================================== */

/* === Types ================================================================ */
typedef enum
{
    CHANNEL,
    CHANNEL_PAGE,
    TX_POWER
} parameters_t;

/* === Externals ============================================================ */

/* === Prototypes =========================================================== */

void app_task_op_mode_2_init(startup_mode_e mode,  bool pairing_status);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_OP_MODE_2_H */
/* EOF */
