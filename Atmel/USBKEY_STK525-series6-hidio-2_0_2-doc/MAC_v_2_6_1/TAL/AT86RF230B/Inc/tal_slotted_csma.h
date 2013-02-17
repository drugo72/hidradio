/**
 * @file tal_slotted_csma.h
 *
 * @brief File provides CSMA-CA states.
 *
 * $Id: tal_slotted_csma.h 23383 2010-09-09 10:52:19Z sschneid $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#ifdef BEACON_SUPPORT

/* Prevent double inclusion */
#ifndef TAL_SLOTTED_CSMA_H
#define TAL_SLOTTED_CSMA_H

/* === INCLUDES ============================================================ */

/* === EXTERNALS =========================================================== */

/* === TYPES =============================================================== */

/*
 * CSMA-CA states
 */
typedef enum csma_state_tag
{
    CSMA_IDLE = 0,
    BACKOFF_WAITING_FOR_CCA_TIMER,
    BACKOFF_WAITING_FOR_BEACON,
    CSMA_ACCESS_FAILURE,
    FRAME_SENDING_WITH_ACK,
    FRAME_SENDING_NO_ACK,
    WAITING_FOR_ACK,
    TX_DONE_SUCCESS,
    TX_DONE_FRAME_PENDING,
    TX_DONE_NO_ACK,
    NO_BEACON_TRACKING,
    CSMA_HANDLE_BEACON
} csma_state_t;


/* === MACROS ============================================================== */

/* === PROTOTYPES ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif


bool slotted_csma_start(bool perform_frame_retry);
void slotted_csma_state_handling(void);
#if (MAC_START_REQUEST_CONFIRM == 1)
void calculate_transaction_duration(void);
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TAL_SLOTTED_CSMA_H */

#endif /* BEACON_SUPPORT */


/* EOF */