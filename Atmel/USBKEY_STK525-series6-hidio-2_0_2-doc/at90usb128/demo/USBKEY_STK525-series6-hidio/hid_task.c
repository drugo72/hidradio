/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file manages the generic HID IN/OUT task.
//!
//! - Compiler:           IAR EWAVR and GNU GCC for AVR
//! - Supported devices:  AT90USB1287, AT90USB1286, AT90USB647, AT90USB646
//!
//! \author               Atmel Corporation: http://www.atmel.com \n
//!                       Support and FAQ: http://support.atmel.no/
//!
//! ***************************************************************************

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//_____  I N C L U D E S ___________________________________________________

#include <stdlib.h>
#include "config.h"
#include "conf_usb.h"
#include "hid_task.h"
#include "lib_mcu/usb/usb_drv.h"
#include "usb_descriptors.h"
#include "modules/usb/device_chap9/usb_standard_request.h"
#include "usb_specific_request.h"
#include "lib_mcu/util/start_boot.h"

#include "tal.h"
#include "bmm.h"
#include "qmm.h"
#include "ieee_const.h"

#include "hidradio.h"

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________


//_____ D E C L A R A T I O N S ____________________________________________


/* Define a max len report */
static union {
  hr_report_t report;
  uint8_t buf[MAX_REPORT_LEN];
} in_report_buf;

static hr_report_t * const in_reportp = &in_report_buf.report;

static union {
  hr_report_t report;
  uint8_t buf[MAX_REPORT_LEN];
} out_report_buf;

static hr_report_t * const out_reportp = &out_report_buf.report;

// Buffer for response
static union {
  hr_report_t report;
  uint8_t buf[MAX_REPORT_LEN];
} resp_report_buf;

static hr_report_t *resp_reportp;

static queue_t rx_queue;

static union {
  frame_info_t frame_info;
  uint8_t buf[sizeof(frame_info_t) + 1 /* PSDU length */ + 127 - 2 /* FCS */ + 1 /* LQI */ + 1 /* ED level */];
} frame_info_buf;

static frame_info_t * const frame_infop = &frame_info_buf.frame_info;

static uint16_t in_nleft = 0;
static uint16_t out_nleft = 0;

static uint8_t rx_on_when_idle = true;

static volatile uint8_t cpt_sof=0;

static uint8_t find_min_length_report_id(uint16_t min_required);
static void start_send_in_report(void);
static void recv_out_report(void);
static void send_in_report(void);
static void send_response(void);
static void send_queued_frame(void);
static uint8_t is_sending_in_report(void);
static void do_handle_report(void);


//! @brief This function initializes the target board ressources.
//!
void hid_task_init(void)
{
  uint8_t channel = 26;
  uint8_t promiscuous = true;

  Leds_init();
  Joy_init();
  
  qmm_queue_init(&rx_queue);

  frame_infop->mpdu = (uint8_t*)frame_infop + sizeof(frame_info_t);
  
  tal_pib_set(phyCurrentChannel, (pib_value_t *)&channel);
  tal_pib_set(macPromiscuousMode, (pib_value_t *)&promiscuous);
  tal_rx_enable(PHY_RX_ON);
}

//! @brief Entry point of the HID generic communication task
//! This function manages IN/OUT repport management.
//!
void hid_task(void)
{
   if(!Is_device_enumerated())          // Check USB HID is enumerated
      return;

   recv_out_report();
   send_response();
   send_queued_frame();
   send_in_report(); 
}

void do_rx_on_when_idle(void)
{
   if (rx_on_when_idle) {
     tal_rx_enable(PHY_RX_ON);
   } else {
     tal_rx_enable(PHY_TRX_OFF);
   }
}

void send_response(void)
{
   if (!is_sending_in_report() && resp_reportp != NULL) {
     memcpy(in_reportp, resp_reportp, REPORT_LEN(resp_reportp->id));
     resp_reportp = NULL;
     start_send_in_report();
     Usb_disable_stall_handshake();
   }
}

//! @brief  This function increments the cpt_sof counter each times
//! the USB Start Of Frame interrupt subroutine is executed (1ms)
//! Usefull to manage time delays
//!
void sof_action()
{
   cpt_sof++;
}

void tal_rx_frame_cb(frame_info_t *rx_frame)
{
  qmm_queue_append(&rx_queue, rx_frame->buffer_header);
  do_rx_on_when_idle();
}

void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
{
  hr_data_cnf_t *cnfp;

  resp_reportp = &resp_report_buf.report;

  resp_reportp->id = 1;
  resp_reportp->msg.type = HR_TYPE_DATA_CNF;
  cnfp = &resp_reportp->msg.data.data_cnf;
  cnfp->status = status;

  do_rx_on_when_idle();
}

void send_queued_frame(void)
{
  if (!is_sending_in_report()) {
    buffer_t *buf;
    frame_info_t *rx_framep;

    buf = qmm_queue_remove(&rx_queue, NULL);
    if (buf == NULL) {
      return;
    }

    rx_framep = (frame_info_t*)BMM_BUFFER_POINTER(buf);

    if (rx_framep != NULL) {
      uint8_t data_len;
      uint8_t report_id;
      uint8_t psdu_len;
      uint16_t min_required;
      hr_data_ind_t *indp = &in_reportp->msg.data.data_ind;
      uint8_t lqi;
      uint8_t ed_level;

      psdu_len = rx_framep->mpdu[0];
      lqi = rx_framep->mpdu[psdu_len + 1];
      ed_level = rx_framep->mpdu[psdu_len + 2];
  
      data_len = offsetof(hr_data_ind_t, psdu) + psdu_len;

      //// FIXME: for now we truncate data to fit a single transaction
      // data_len = MIN(data_len, MAX_MSG_DATA_LEN);

      min_required = offsetof(hr_report_t, msg) + offsetof(hr_msg_t, data) + data_len;

      report_id = find_min_length_report_id(min_required);
      if (report_id == 0) {
	goto cleanup;
      }

      in_reportp->id = report_id;

      in_reportp->msg.type = HR_TYPE_DATA_IND;

      indp->lqi = lqi; // or ed_level?
      U32TONA(rx_framep->time_stamp, indp->timestamp);
      indp->psdu_len = psdu_len;
      memcpy(indp->psdu, &rx_framep->mpdu[1], psdu_len);
  
      start_send_in_report();

      Led2_toggle();

    cleanup:
      /* free buffer that was used for frame reception */
      bmm_buffer_free(rx_framep->buffer_header);
    }
  } 
}

uint8_t find_min_length_report_id(uint16_t min_required)
{
  uint8_t i;
  uint16_t report_len;

  if (min_required >= MAX_REPORT_LEN) {
    return 0;
  }

  for (i = 1; i < MAX_REPORT_ID; i++) {
    report_len = REPORT_LEN(i);
    if (report_len >= min_required) {
      break;
    }
  }

  return i;
}

void start_send_in_report(void)
{
  in_nleft = REPORT_LEN(in_reportp->id);
}

uint8_t is_sending_in_report(void)
{
  return in_nleft != 0;
}

void send_in_report(void)
{
  if (in_nleft != 0) {
    uint16_t nwritten;
    uint8_t ntowrite;
    uint8_t i;
    uint8_t *ptr;

    Usb_select_endpoint(EP_HID_IN);
    if(!Is_usb_write_enabled())
      return;                                // Not ready to send report

    nwritten = REPORT_LEN(in_reportp->id) - in_nleft;
    ntowrite = (uint8_t) MIN(in_nleft, EP_IN_LENGTH);
    ptr = (uint8_t*)in_reportp + nwritten;

    for (i = 0; i < ntowrite; i++) {
      Usb_write_byte(ptr[i]);
    }

    in_nleft -= ntowrite; 

    Usb_ack_in_ready();                       // Send data over the USB
  }
}

void recv_out_report(void)
{
  uint16_t nread;
  uint8_t ntoread;
  uint8_t i;
  uint8_t *ptr;

  Usb_select_endpoint(EP_HID_OUT);
  if(Is_usb_receive_out()) {

    if (out_nleft == 0) {
      // Read the report ID
      out_reportp->id = Usb_read_byte();
      if (out_reportp->id < 1 || out_reportp->id > MAX_REPORT_ID) {
        Usb_enable_stall_handshake();
	Usb_ack_receive_out();
	return;
      }
      out_nleft = REPORT_LEN(out_reportp->id) - 1;
    }

    ntoread = MIN(out_nleft, Usb_byte_counter_8());
    nread = REPORT_LEN(out_reportp->id) - out_nleft; 
    ptr = (uint8_t*)out_reportp + nread;

    for (i = 0; i < ntoread; i++) {
      ptr[i] = Usb_read_byte();
    }

    out_nleft -= ntoread;

    Usb_ack_receive_out();

    if (out_nleft == 0) {
      do_handle_report();
    }
  }
}

bool is_supported_attr_id(uint8_t attr_id)
{
  if (attr_id == HR_ATTR_PHY_CURRENT_CHANNEL ||
      attr_id == HR_ATTR_PHY_CHANNELS_SUPPORTED ||
      attr_id == HR_ATTR_PHY_TRANSMIT_POWER ||
      attr_id == HR_ATTR_PHY_CCA_MODE ||
      attr_id == HR_ATTR_MAC_MAX_CSMA_BACKOFFS ||
      attr_id == HR_ATTR_MAC_MIN_BE ||
      attr_id == HR_ATTR_MAC_MAX_FRAME_RETRIES ||
      attr_id == HR_ATTR_MAC_PAN_ID ||
      attr_id == HR_ATTR_MAC_PROMISCUOUS_MODE ||
      attr_id == HR_ATTR_MAC_RX_ON_WHEN_IDLE ||
      attr_id == HR_ATTR_MAC_SHORT_ADDRESS ||
      attr_id == HR_ATTR_I_AM_COORD) {
    return true;
  } else {
    return false;
  }
}

void do_get_req(void)
{
  retval_t retval;
  hr_get_req_t *reqp;
  hr_get_cnf_t *cnfp;

  reqp = &out_reportp->msg.data.get_req;

  resp_reportp = &resp_report_buf.report;
  resp_reportp->id = 1;
  resp_reportp->msg.type = HR_TYPE_GET_CNF;

  cnfp = &resp_reportp->msg.data.get_cnf;
  
  if (is_supported_attr_id(reqp->attr_id)) {
    if (reqp->attr_id == HR_ATTR_MAC_RX_ON_WHEN_IDLE) {
      cnfp->attr_val[0] = rx_on_when_idle;
      retval = HR_STATUS_SUCCESS;
    } else {
      retval = tal_pib_get(reqp->attr_id, cnfp->attr_val);
    }
    if (retval == TAL_BUSY) {
      /* TODO: this should never happen (internal error) */
    }
  } else {
    retval = HR_STATUS_UNSUPPORTED_ATTRIBUTE;
  }

  cnfp->status = retval;
  cnfp->attr_id = reqp->attr_id;
}

void do_set_req(void)
{
  retval_t retval;
  hr_set_req_t *reqp;
  hr_set_cnf_t *cnfp;

  reqp = &out_reportp->msg.data.set_req;

  resp_reportp = &resp_report_buf.report;
  resp_reportp->id = 1;
  resp_reportp->msg.type = HR_TYPE_SET_CNF;

  cnfp = &resp_reportp->msg.data.set_cnf;

  if (is_supported_attr_id(reqp->attr_id)) {
    if (reqp->attr_id == HR_ATTR_MAC_RX_ON_WHEN_IDLE) {
      rx_on_when_idle = reqp->attr_val[0];
      do_rx_on_when_idle(); // Should be done also here or only at the end of a TRX task? (2003 spec)
      retval = HR_STATUS_SUCCESS;
    } else {
      retval = tal_pib_set(reqp->attr_id, (pib_value_t*)reqp->attr_val);
      if (reqp->attr_id == HR_ATTR_MAC_PROMISCUOUS_MODE) {
	// TODO: this request affects/is affected by the value of rxOnWhenIdle.
	// Anyway the behavior differs between 2003 and 2006 spec.

#if 1
	// 2003 spec.
	rx_on_when_idle = reqp->attr_val[0];
#else
	// 2006 spec.
	if (reqp->attr_val[0] == 0) {
	  // Set the TRX state accordingly to the value of rxOnWhenIdle
	  
	  // We only need to reenable RX (tal_pib_set set TRX off)
	  if (rx_on_when_idle) {
	    tal_rx_enable(PHY_RX_ON);
	  }
	}
#endif 
      }
    }
    
    if (retval == TAL_BUSY) {
      /* TODO: this should never happen (internal error) */
    }
  } else {
    retval = HR_STATUS_UNSUPPORTED_ATTRIBUTE;
  }

  cnfp->status = retval;
  cnfp->attr_id = reqp->attr_id;
}

void do_data_req(void)
{
  hr_data_req_t *reqp;
  retval_t retval;

  reqp = &out_reportp->msg.data.data_req;

  if ((reqp->tx_opts & HR_TX_OPTS_WITHOUT_CSMA_CA) != 0) {
    /* TODO */
    tal_tx_frame_done_cb(HR_STATUS_INVALID_PARAMETER, NULL);
  } else {
    /* First byte is the PSDU length */
    frame_infop->mpdu[0] = reqp->psdu_len;
    memcpy(&frame_infop->mpdu[1], reqp->psdu, reqp->psdu_len);

    retval = tal_tx_frame(frame_infop,
			  CSMA_UNSLOTTED,
			  true);

    if (retval == TAL_BUSY) {
      /* TODO: this should never happen (internal error) */
      tal_tx_frame_done_cb(retval, NULL);
    }
  }
}

void do_handle_default(void)
{
  resp_reportp = &resp_report_buf.report;
  resp_reportp->id = 1;
  resp_reportp->msg.type = HR_TYPE_UNSUPPORTED_MSG;
}

void do_echo_req(void)
{
  void *dst;
  void *src;
  size_t len;

  resp_reportp = &resp_report_buf.report;
  resp_reportp->id = out_reportp->id;
  resp_reportp->msg.type = HR_TYPE_ECHO_RESP;

  dst = (void*)&resp_reportp->msg.data;
  src = (void*)&out_reportp->msg.data;
  len = MIN(MSG_DATA_LEN(out_reportp->id), MAX_MSG_DATA_LEN);

  memcpy(dst, src, len);
}

void do_handle_report(void)
{
  // One request at a time
  Usb_enable_stall_handshake();

  switch (out_reportp->msg.type) {
  case HR_TYPE_GET_REQ:
    do_get_req();
    break;
  case HR_TYPE_SET_REQ:
    do_set_req();
    break;
  case HR_TYPE_DATA_REQ:
    do_data_req();
    break;
  case HR_TYPE_ECHO_REQ:
    do_echo_req();
    break;
  default:
    do_handle_default();
    break;
  }
}
