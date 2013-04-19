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

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________


//_____ D E C L A R A T I O N S ____________________________________________

#define HR_TYPE_GET_REQ 0x00
#define HR_TYPE_GET_CNF 0x01

#define HR_TYPE_SET_REQ 0x02
#define HR_TYPE_SET_CNF 0x03

#define HR_TYPE_DATA_REQ 0x08
#define HR_TYPE_DATA_CNF 0x09

#define HR_TYPE_DATA_IND 0x0a

#define HR_TYPE_ECHO_REQ 0x20
#define HR_TYPE_ECHO_RESP 0x21

#define HR_TYPE_UNSUPPORTED_MSG 0xff



#define HR_STATUS_SUCCESS 0x00
#define HR_STATUS_CHANNEL_ACCESS_FAILURE 0xe1
#define HR_STATUS_NO_ACK 0xe9
#define HR_STATUS_INVALID_PARAMETER 0xe8
#define HR_STATUS_UNSUPPORTED_ATTRIBUTE 0xf4

#define HR_ATTR_PHY_CURRENT_CHANNEL 0x00
#define HR_ATTR_PHY_CHANNELS_SUPPORTED 0x01
#define HR_ATTR_PHY_TRANSMIT_POWER 0x02
#define HR_ATTR_PHY_CCA_MODE 0x03

#define HR_ATTR_MAC_MAX_CSMA_BACKOFFS 0x4e
#define HR_ATTR_MAC_MIN_BE 0x4f
#define HR_ATTR_MAC_MAX_FRAME_RETRIES 0x59
#define HR_ATTR_MAC_PAN_ID 0x50
#define HR_ATTR_MAC_PROMISCUOUS_MODE 0x51
#define HR_ATTR_MAC_RX_ON_WHEN_IDLE 0x52
#define HR_ATTR_MAC_SHORT_ADDRESS 0x53

/* TODO: In order to operate properly, the transceiver needs to know if we are the coordinator */


typedef struct {
  uint8_t attr_id;
  uint8_t attr_val[1];
} hr_set_req_t;

typedef struct {
uint8_t status;
uint8_t attr_id;
} hr_set_cnf_t;

typedef struct {
uint8_t attr_id;
} hr_get_req_t;

typedef struct {
uint8_t status;
uint8_t attr_id;
uint8_t attr_val[1];
} hr_get_cnf_t;


#define HR_TX_OPTS_CSMA_CA 0x01
#define HR_TX_OPTS_ACK_REQ 0x02
typedef struct {
  uint8_t tx_opts; /* bitmap of HR_TX_OPTS_CSMA_CA, HR_TX_OPTS_ACK_REQ */
  uint8_t psdu_len;
  uint8_t psdu[1]; // Placeholder for PSDU. MPDU's FCS is computed by hw and MUST NOT be included.
} hr_data_req_t;

typedef struct {
  // The values are non-standard.
  // The enhanced tranceiver RX state is controlled by the MIB attribute macRxOnWhenIdle.
  // so it's OK to issue a data request at any time. The transceiver takes care of turning the
  // receiver off and, if the case, turning it on at the end of transmission
  // (in other words, TRX_OFF or RX_ON never happen).
  // If the data request was issued with no tx_opts the only status possible is SUCCESS.
  // Otherwise, other than SUCCESS, the status can be either CHANNEL_ACCESS_FAILURE (HR_TX_OPTS_CSMA_CA set) or NO_ACK (HR_TX_OPTS_ACK_REQ set).
  uint8_t status;
} hr_data_cnf_t;

typedef struct {
  uint8_t timestamp[4]; // Non-standard
  uint8_t lqi;
  uint8_t psdu_len;
  uint8_t psdu[1]; // Placeholder for PSDU. MPDU's FCS is checked by hw and is not included.
} hr_data_ind_t;

typedef struct {
  uint8_t type;
  union {
    hr_data_req_t data_req;
    hr_data_cnf_t data_cnf;
    hr_data_ind_t data_ind;
    hr_set_req_t set_req;
    hr_set_cnf_t set_cnf;
    hr_get_req_t get_req;
    hr_get_cnf_t get_cnf;
  } data;
} hr_msg_t;

typedef struct {
  uint8_t id;
  hr_msg_t msg;
} hr_report_t;

static uint16_t in_nleft = 0;
static uint16_t out_nleft = 0;

#define U32TONA(s, d) do {(d)[0] = (s) & 0xff; (d)[1] = ((s) >> 8) & 0xff; (d)[2] = ((s) >> 16) & 0xff; (d)[3] = ((s) >> 24) & 0xff;} while(0)

#define MIN(x, y) ((x) < (y) ? (x) : (y))

// TODO? The longest report length can be made a multiple of the EP size (does not require an extra ZLP)

// TODO? According to this:
// http://www.microchip.com/forums/m485841.aspx
// The host does not send ZLP for reports whose length is a multiple of the EP size.
// Thus the OUT report lengths might be 64, 128 etc. 

/* Report length = (report ID * 64) - 1 */
#define REPORT_LEN(id) (((uint16_t)(id) << 6) - 1)
#define MSG_DATA_LEN(id) (REPORT_LEN((id)) - (offsetof(hr_report_t, msg) - offsetof(hr_msg_t, data)))

#define MAX_REPORT_ID 3

#define MAX_REPORT_LEN REPORT_LEN(MAX_REPORT_ID)
#define MAX_MSG_DATA_LEN MSG_DATA_LEN(MAX_REPORT_ID)

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
  uint8_t buf[sizeof(frame_info_t) + 1 /* PSDU length */ + 127 - 2 /* FCS */];
} frame_info_buf;

static frame_info_t * const frame_infop = &frame_info_buf.frame_info;

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
}

void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
{
  hr_data_cnf_t *cnfp;

  resp_reportp = &resp_report_buf.report;

  resp_reportp->id = 0;
  resp_reportp->msg.type = HR_TYPE_DATA_CNF;
  cnfp = &resp_reportp->msg.data.data_cnf;
  cnfp->status = status;
}

void send_queued_frame(void)
{
  if (!is_sending_in_report()) {
    buffer_t *buf;
    frame_info_t *rx_frame;

    buf = qmm_queue_remove(&rx_queue, NULL);
    if (buf == NULL) {
      return;
    }

    rx_frame = (frame_info_t*)BMM_BUFFER_POINTER(buf);

    if (rx_frame != NULL) {
      uint8_t data_len;
      uint8_t report_id;
      uint8_t psdu_len;
      uint16_t min_required;
      hr_data_ind_t *indp = &in_reportp->msg.data.data_ind;

      psdu_len = rx_frame->mpdu[0];
  
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

      indp->lqi = 0xAA; // FIXME  
      U32TONA(rx_frame->time_stamp, indp->timestamp);
      indp->psdu_len = psdu_len;
      memcpy(indp->psdu, &rx_frame->mpdu[1], psdu_len);
  
      start_send_in_report();

      Led2_toggle();

    cleanup:
      /* free buffer that was used for frame reception */
      bmm_buffer_free(rx_frame->buffer_header);
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

void do_set_req(void)
{
  retval_t retval = HR_STATUS_UNSUPPORTED_ATTRIBUTE;
  hr_set_req_t *reqp;
  hr_set_cnf_t *cnfp;

  reqp = &out_reportp->msg.data.set_req;

  resp_reportp = &resp_report_buf.report;
  resp_reportp->id = 1;
  resp_reportp->msg.type = HR_TYPE_SET_CNF;

  cnfp = &resp_reportp->msg.data.set_cnf;

  switch(reqp->attr_id) {
  case HR_ATTR_PHY_CURRENT_CHANNEL:
    retval = tal_pib_set(phyCurrentChannel, (pib_value_t*)&reqp->attr_val[0]);
    break;
  case HR_ATTR_PHY_CHANNELS_SUPPORTED:
    break;
  case HR_ATTR_PHY_TRANSMIT_POWER:
    break;
  case HR_ATTR_PHY_CCA_MODE:
    break;
    // MAC PIB attributes
  case HR_ATTR_MAC_MAX_CSMA_BACKOFFS:
    break;
  case HR_ATTR_MAC_MIN_BE:
    break;
  case HR_ATTR_MAC_MAX_FRAME_RETRIES:
    break;
  case HR_ATTR_MAC_PAN_ID:
    break;
  case HR_ATTR_MAC_PROMISCUOUS_MODE:
    retval = tal_pib_set(macPromiscuousMode, (pib_value_t*)&reqp->attr_val[0]);
    break;
  case HR_ATTR_MAC_RX_ON_WHEN_IDLE:
    break;
  case HR_ATTR_MAC_SHORT_ADDRESS:
    break;
  default:
    retval = HR_STATUS_UNSUPPORTED_ATTRIBUTE;
    break;
  }

  if (retval == TAL_BUSY) {
    /* TODO: this should never happen (internal error) */
  }

  cnfp->status = retval;
  cnfp->attr_id = reqp->attr_id;
}

void do_data_req(void)
{
  hr_data_req_t *reqp = &out_reportp->msg.data.data_req;
  bool perform_frame_retry = false;
  csma_mode_t csma_mode = NO_CSMA_NO_IFS; // USB latency should be enough
  retval_t retval;

  if ((reqp->tx_opts & HR_TX_OPTS_CSMA_CA) != 0) {
    csma_mode = CSMA_UNSLOTTED;
  }

  if ((reqp->tx_opts & HR_TX_OPTS_ACK_REQ) != 0) {
    perform_frame_retry = true;
  }

  /* First byte is the PSDU length */
  frame_infop->mpdu[0] = reqp->psdu_len;
  memcpy(&frame_infop->mpdu[1], reqp->psdu, reqp->psdu_len);

  retval = tal_tx_frame(frame_infop,
                        csma_mode,
                        perform_frame_retry);

  if (retval == TAL_BUSY) {
    /* TODO: this should never happen (internal error) */
    tal_tx_frame_done_cb(retval, NULL);
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
    do_handle_default();
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
