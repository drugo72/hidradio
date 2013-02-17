/**
 * @file app_pairing.h
 *
 * @brief These are application-specific resources which are used
 *        in the application for pairing.
 *
 * $Id: app_pairing.h 23328 2010-09-06 13:30:41Z prajosh.premdas $
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
#ifndef APP_FRAME_PAIRING_H
#define APP_FRAME_PAIRING_H

/* === Includes ============================================================= */

/* === Macros =============================================================== */

/* === Types ================================================================ */

/* === Externals ============================================================ */
extern uint16_t dest_addr_pairing;
extern uint16_t src_addr_pairing;
/* === Prototypes =========================================================== */

/* Functions exposed by pairing module */
extern void app_task_pairing_init(startup_mode_e mode_startup);
extern void app_pairing_reset(void);


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_FRAME_PAIRING_H */
/* EOF */
