/**
 * @file app_frame_format.h
 *
 * @brief These are application-specific resources which are used
 *        in the application in addition to the underlaying stack.
 *
 * $Id: app_frame_format.h 23759 2010-10-08 05:56:31Z prajosh.premdas $
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
#ifndef APP_FRAME_FORMAT_H
#define APP_FRAME_FORMAT_H

/* === Includes ============================================================= */
#include "app_op_mode_2.h"

/* === Macros =============================================================== */

/* === Types ================================================================ */

typedef struct
{
    uint8_t u8OpMode;
    uint16_t u16nwk_addr;
}pairing_req_t;

typedef struct
{
    uint8_t u8OpMode;
    uint16_t u16nwk_addr;
}pairing_rsp_t;

typedef struct
{
    uint16_t u16nwk_addr;
}pairing_conf_t;

typedef struct
{
    uint32_t pkt_count;
}data_pkt_op_mode_1_t;

typedef struct
{
    parameters_t parmameter;
    uint8_t value;
}set_parm_req_t;

typedef struct
{
    uint8_t Cmd;
}result_req_t;

typedef struct
{
    uint32_t num_of_frames_rx;
    uint32_t lqi_avrg_rx;
}result_rsp_t;

typedef struct
{
    uint8_t status;
}div_stat_req_t;

typedef struct
{
    uint8_t status    :1;
    uint8_t ant_sel   :2;
}div_stat_rsp_t;

typedef struct
{
    uint8_t status    :1;
    uint8_t ant_sel   :2;
}div_set_req_t;

typedef struct
{
    uint8_t status;
}div_set_rsp_t;

typedef union
{
    pairing_req_t           pairing_req_data;
    pairing_rsp_t           pairing_rsp_data;
    pairing_conf_t          pairing_conf_data;
    data_pkt_op_mode_1_t    data_payload;
    set_parm_req_t          set_parm_req_data;
    result_rsp_t            test_result_rsp_data;
    div_stat_rsp_t          div_stat_rsp_data;
    div_set_req_t           div_set_req_data;
}general_pkt_u;

typedef struct
{
    uint8_t u8CmdID;
    uint8_t u8SeqNum;
    general_pkt_u payload[1];
}app_payload_t;

/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_FRAME_FORMAT_H */
/* EOF */
