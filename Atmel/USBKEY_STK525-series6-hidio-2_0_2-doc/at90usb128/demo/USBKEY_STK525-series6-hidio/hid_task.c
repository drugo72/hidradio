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

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________


//_____ D E C L A R A T I O N S ____________________________________________

         volatile U8    cpt_sof=0;
extern            U8    jump_bootloader;
                  U8    g_last_joy       =0;
				  U8 jitter_elapsed;
				  U16 jitter, t0_jitter;



typedef struct {
  uint8_t id;
  uint8_t type;
  uint8_t length; /* Length of meaningful data in the report */
  uint8_t data[1];
} hidradio_report_t;

typedef struct {
  uint8_t timestamp[4];
  uint8_t lqi;
  uint8_t psdu_length;
  uint8_t psdu[];
} hidradio_phy_data_ind_t;

static uint16_t nleft = 0;

#define U32TONA(s, d) do {(d)[0] = (s) & 0xff; (d)[1] = ((s) >> 8) & 0xff; (d)[2] = ((s) >> 16) & 0xff; (d)[3] = ((s) >> 24) & 0xff;} while(0)

#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* Report length = (report ID * 64) - 1 */
#define REPORT_LEN(id) (((uint16_t)(id) << 6) - 1)
#define MAX_REPORT_ID 1

#define MAX_REPORT_LEN REPORT_LEN(MAX_REPORT_ID)
#define MAX_DATA_LEN (MAX_REPORT_LEN - offsetof(hidradio_report_t, data))

/* Define a max len report */
static union {
  hidradio_report_t report;
  uint8_t buf[MAX_REPORT_LEN];
} report_buf;

static hidradio_report_t *reportp = &report_buf.report;

queue_t in_queue;

static uint8_t find_min_length_report_id(uint16_t min_required);
static void start_send_in_report(void);
static void send_in_report_task(void);
static void in_queue_task(void);


void hid_report_out  (void);
void hid_report_in   (void);
void hid_loopback    (void);

//! @brief This function initializes the target board ressources.
//!
void hid_task_init(void)
{
   Leds_init();
   Joy_init();
   Timer_init();
   qmm_queue_init(&in_queue);
}

//! @brief Entry point of the HID generic communication task
//! This function manages IN/OUT repport management.
//!
void hid_task(void)
{
   if(!Is_device_enumerated())          // Check USB HID is enumerated
      return;

//   hid_report_out();
//   hid_report_in();
//   hid_loopback();
   in_queue_task();
   send_in_report_task();
}


//! @brief Get data report from Host
//!
void hid_report_out(void)
{
   Usb_select_endpoint(EP_HID_OUT);
   if(Is_usb_receive_out())
   {
      //* Read report from HID Generic Demo
      U8 led_state;
      U8 led_number;
      led_state      = Usb_read_byte()&0x0F; // RepportOUT[0] is LEDS value
      led_number     = Usb_read_byte()&0x0F;
      switch (led_number)
      {
         case 1:
            if(led_state)
            {  Led0_on();   }
            else {Led0_off();}
            break;
         case 2:
            if(led_state)
            {  Led1_on();   }
            else {Led1_off();}
            break;
         case 3:
            if(led_state)
            {  Led2_on();   }
            else {Led2_off();}
            break;
         case 4:
            if(led_state)
            {  Led3_on();   }
            else {Led3_off();}
            break;
      }  
      Usb_ack_receive_out();
   }

   //** Check if we received DFU mode command from host
   if(jump_bootloader)
   {
      U32 volatile tempo;
      Leds_off();
      Usb_detach();                          // Detach actual generic HID application
      for(tempo=0;tempo<70000;tempo++);      // Wait some time before
      start_boot();                          // Jumping to booltoader
   }
}


//! @brief Send data report to Host
//!
void hid_report_in(void)
{
   U8 joy   =0;

   Usb_select_endpoint(EP_HID_IN);
   if(!Is_usb_write_enabled())
      return;                                // Not ready to send report
   
   // Build the Joytick report
   if(Is_joy_up()|| Is_joy_down() || Is_joy_right() || Is_joy_left() )                     //! Check for UP event
   {
     joy=0x01;
   }
   if(joy==g_last_joy)
      return;                                // Same report then no send report
   g_last_joy=joy;
  
   // Send report
   Usb_write_byte(g_last_joy);               // Joystick
   Usb_write_byte(GPIOR1);                   // Dummy (not used)
   Usb_write_byte(GPIOR1);                   // Dummy (not used)
   Usb_write_byte(GPIOR1);                   // Dummy (not used)
   Usb_write_byte(GPIOR1);                   // Dummy (not used)
   Usb_write_byte(GPIOR1);                   // Dummy (not used)
   Usb_write_byte(GPIOR1);                   // Dummy (not used)
   Usb_write_byte(GPIOR1);                   // Dummy (not used)
   Usb_ack_in_ready();                       // Send data over the USB
}

void hid_loopback(void)
{
	static U8 pending = 0;
	static U8 led_state;
    static U8 led_number;
	static U16 t0, t1;
	static U16 before;
	static U16 cnt;

	U16 now = Timer_read_unsafe();

	if((now - before) > 1000) {
		before = now;
		cnt++;
	}
	
	if (cnt > 1000) {
		cnt = 0;
		Led3_toggle();

		jitter = (U16)(random() & 0x03ff); // Assuming RAND_MAX a multiple of 1024
		t0_jitter = Timer_read_unsafe();
		jitter_elapsed = 1;
	}

	if(jitter_elapsed && (now - t0_jitter) > jitter) {
		jitter_elapsed = 0;
		pending = 1;
	}


   Usb_select_endpoint(EP_HID_OUT);
   if(Is_usb_receive_out())
   {
      t1 = Timer_read_unsafe();

      //* Read report from HID Generic Demo
      led_state      = Usb_read_byte()&0x0F; // RepportOUT[0] is LEDS value
      led_number     = Usb_read_byte()&0x0F;
      switch (led_number)
      {
         case 1:
            if(led_state)
            {  Led0_on();   }
            else {Led0_off();}
            break;
         case 2:
            if(led_state)
            {  Led1_on();   }
            else {Led1_off();}
            break;
         case 3:
            if(led_state)
            {  Led2_on();   }
            else {Led2_off();}
            break;
         case 4:
            if(led_state)
            {  Led3_on();   }
            else {Led3_off();}
            break;
      }  
      Usb_ack_receive_out();

	  pending = 1;
   }

   if (pending) {
       U16 dt = t1 - t0;
       t0 = Timer_read_unsafe();

	   Usb_select_endpoint(EP_HID_IN);
	   if(!Is_usb_write_enabled())
	      return;                                // Not ready to send report
   
	   // Send report
	   Usb_write_byte(led_state);
	   Usb_write_byte(led_number);
#if 1 /* OK to shorten the report size on Linux hidraw & libusb */
	   Usb_write_byte(GPIOR1);                   // Dummy (not used)
	   Usb_write_byte(GPIOR1);                   // Dummy (not used)
	   Usb_write_byte(GPIOR1);                   // Dummy (not used)
	   Usb_write_byte(GPIOR1);                   // Dummy (not used)
	   Usb_write_byte((dt >> 8) & 0x00ff);
	   Usb_write_byte(dt & 0x00ff);
#endif
	   Usb_ack_in_ready();                       // Send data over the USB

	   pending = 0;
   }
}

//! @brief  This function increments the cpt_sof counter each times
//! the USB Start Of Frame interrupt subroutine is executed (1ms)
//! Usefull to manage time delays
//!
void sof_action()
{
   cpt_sof++;

#if 0
   if ((cpt_sof & 0x03ff) == 0) { // once every 1024ms
    Led3_toggle();
	jitter = (U16)(random() & 0x03ff); // Assuming RAND_MAX a multiple of 1024
	t0_jitter = Timer_read_unsafe();
	jitter_elapsed = 1;
   }
#endif
}

void tal_rx_frame_cb(frame_info_t *rx_frame)
{
  qmm_queue_append(&in_queue, rx_frame->buffer_header);
}

void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
{
  // TODO
}

void in_queue_task(void)
{
  if (nleft == 0) {
    buffer_t *buf;
    frame_info_t *rx_frame;

    buf = qmm_queue_remove(&in_queue, NULL);
    if (buf == NULL) {
      return;
    }

    rx_frame = (frame_info_t*)BMM_BUFFER_POINTER(buf);

    if (rx_frame != NULL) {
      uint8_t data_len;
      uint8_t report_id;
      uint8_t psdu_len;
      uint16_t min_required;
      hidradio_phy_data_ind_t *indp = (hidradio_phy_data_ind_t*)reportp->data;

      psdu_len = rx_frame->mpdu[0];

      indp->psdu_length = psdu_len;
      memcpy(indp->psdu, &rx_frame->mpdu[1], psdu_len);
  
      U32TONA(rx_frame->time_stamp, indp->timestamp);
      indp->lqi = 0xAA; // FIXME
  
      data_len = offsetof(hidradio_phy_data_ind_t, psdu) + indp->psdu_length;

      // FIXME: for now we truncate data to fit a single transaction
      data_len = MIN(data_len, MAX_DATA_LEN);

      min_required = offsetof(hidradio_report_t, data) + data_len;

      report_id = find_min_length_report_id(min_required);
      if (report_id == 0) {
	goto cleanup;
      }

      reportp->id = report_id;
      reportp->type = 0x55; // FIXME
      reportp->length = data_len;
  
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
  nleft = REPORT_LEN(reportp->id);
}

void send_in_report_task(void)
{
  if (nleft != 0) {
    uint16_t nwritten;
    uint8_t ntowrite;
    uint8_t i;
    uint8_t *ptr;

    Usb_select_endpoint(EP_HID_IN);
    if(!Is_usb_write_enabled())
      return;                                // Not ready to send report

    nwritten = REPORT_LEN(reportp->id) - nleft;
    ntowrite = (uint8_t) MIN(nleft, EP_IN_LENGTH);
    ptr = (uint8_t*)reportp + nwritten;

    for (i = 0; i < ntowrite; i++) {
      Usb_write_byte(ptr[i]);
    }

    Usb_ack_in_ready();                       // Send data over the USB

    nleft -= ntowrite; 
  }
}
