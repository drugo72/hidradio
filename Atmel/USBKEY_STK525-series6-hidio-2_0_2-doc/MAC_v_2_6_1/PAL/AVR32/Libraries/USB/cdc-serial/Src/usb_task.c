/* This source file is part of the ATMEL AVR32-SoftwareFramework-AT32UC3A3-0.7.0 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief Management of the USB task either device/host or both.
 *
 * The USB task selects the correct USB task (USB device task or USB host
 * task) to be executed depending on the current mode available.
 *
 * According to the values of USB_DEVICE_FEATURE and USB_HOST_FEATURE
 * (located in the conf_usb.h file), the USB task can be configured to
 * support USB device mode or USB host mode or both for a dual-role device
 * application.
 *
 * This module also contains the general USB interrupt subroutine. This
 * subroutine is used to detect asynchronous USB events.
 *
 * Note:
 *   - The USB task belongs to main; the USB device and host tasks do not.
 *     They are called from the general USB task.
 *   - See the conf_usb.h file for more details about the configuration of
 *     this module.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a USB module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ***************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. ATMEL grants developer a non-exclusive, limited license to use the Software
 * as a development platform solely in connection with an Atmel AVR product
 * ("Atmel Product").
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */
/**
 * $Id: usb_task.c 24663 2011-01-03 12:11:34Z yogesh.bellan $
 */
//_____  I N C L U D E S ___________________________________________________

#include "avr32types.h"
#include "compiler.h"
//#include "intc.h"

#include "conf_usb.h"
#include "usb_drv.h"
#include "usb_task.h"

#if USB_DEVICE_FEATURE == ENABLED
#include "usb_descriptors.h"
#include "usb_device_task.h"
#endif

#include "pal_internal.h"


//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________

//!
//! Public: U16 g_usb_event
//! usb_connected is used to store USB events detected upon
//! USB general interrupt subroutine
//! Its value is managed by the following macros (See \ref usb_task.h file)
//! Usb_send_event(x)
//! Usb_ack_event(x)
//! Is_usb_event(x)
//! Usb_clear_all_event()
volatile U16 g_usb_event = 0;


#if USB_DEVICE_FEATURE == ENABLED

//!
//! Public: Bool usb_connected
//! usb_connected is set to TRUE when VBus has been detected
//! usb_connected is set to FALSE otherwise
//! Used with USB_DEVICE_FEATURE == ENABLED only
extern volatile Bool usb_connected;

  #ifdef FREERTOS_USED
//! Handle to the USB Device task
extern xTaskHandle usb_device_tsk;
  #endif

#endif


#if USB_HOST_FEATURE == ENABLED

static const char log_device_disconnected[] = "Device disconnected\n";

//!
//! Private: U8 private_sof_counter
//! Incremented  by host SOF interrupt subroutime
//! This counter is used to detect time-out in host requests.
//! It must not be modified by the user applicative tasks.
volatile U32 private_sof_counter;

  #if USB_HOST_PIPE_INTERRUPT_TRANSFER == ENABLE
extern volatile Bool g_sav_int_sof_enable;
extern volatile S_pipe_int it_pipe_str[MAX_PEP_NB];
  #endif

  #ifdef FREERTOS_USED
//! Handle to the USB Host task
extern xTaskHandle usb_host_tsk;
  #endif

#endif


#if USB_DEVICE_FEATURE == ENABLED && USB_HOST_FEATURE == ENABLED

static const char log_pin_id_changed[]      = "Pin Id changed\n";

//!
//! Public: U8 g_usb_mode
//! Used in dual-role application (both device/host) to store
//! the current mode the USB controller is operating
volatile U8 g_usb_mode = USB_MODE_UNDEFINED;
static volatile U8 g_old_usb_mode;

#endif


//_____ D E C L A R A T I O N S ____________________________________________




//! @brief This function initializes the USB process.
//!
//! Depending on the mode supported (HOST/DEVICE/DUAL_ROLE) the function
//! calls the coresponding USB mode initialization function
void usb_task_init(void)
{
    Usb_force_device_mode();
    usb_device_task_init();
}


void usb_task(void)
{
  usb_device_task();
}


//! @brief USB interrupt routine
//!
//! This function is called each time a USB interrupt occurs.
//! The following USB DEVICE events are taken in charge:
//! - VBus On / Off
//! - Start-of-Frame
//! - Suspend
//! - Wake-Up
//! - Resume
//! - Reset
//!
//! The following USB HOST events are taken in charge:
//! - Device connection
//! - Device Disconnection
//! - Start-of-Frame
//! - ID pin change
//! - SOF (or Keep alive in low-speed) sent
//! - Wake-up on USB line detected
//! - Pipe events
//!
//! For each event, the user can launch an action by completing the associated
//! \#define (see the conf_usb.h file to add actions on events).
//!
//! Note: Only interrupt events that are enabled are processed.
//!
//! Warning: If device and host tasks are not tasks in an RTOS, rough events
//! like ID transition, VBus transition, device disconnection, etc. that need to
//! kill then restart these tasks may lead to an undefined state if they occur
//! just before something is activated in the USB macro (e.g. pipe/endpoint
//! transfer...).
//!
//! @return Nothing in the standalone configuration; a boolean indicating
//!         whether a task switch is required in the FreeRTOS configuration
#if (defined __GNUC__)
  __attribute__((__interrupt__))
#elif (defined __ICCAVR32__)
  __interrupt static
#endif
void usb_general_interrupt(void)
{
  {
    // VBus state detection
    if (Is_usb_vbus_transition() && Is_usb_vbus_interrupt_enabled())
    {
      Usb_ack_vbus_transition();
      if (Is_usb_vbus_high())
      {
        usb_start_device();
        Usb_send_event(EVT_USB_POWERED);
        Usb_vbus_on_action();
      }
      else
      {
        Usb_unfreeze_clock();
        Usb_detach();
        usb_connected = FALSE;
        usb_configuration_nb = 0;
        Usb_send_event(EVT_USB_UNPOWERED);
        Usb_vbus_off_action();
      }
    }
    // Device Start-of-Frame received
    if (Is_usb_sof() && Is_usb_sof_interrupt_enabled())
    {
      Usb_ack_sof();
      Usb_sof_action();
    }
    // Device Suspend event (no more USB activity detected)
    if (Is_usb_suspend() && Is_usb_suspend_interrupt_enabled())
    {
      Usb_ack_suspend();
      Usb_enable_wake_up_interrupt();
      (void)Is_usb_wake_up_interrupt_enabled();
      Usb_freeze_clock();
      Usb_send_event(EVT_USB_SUSPEND);
      Usb_suspend_action();
    }
    // Wake-up event (USB activity detected): Used to resume
    if (Is_usb_wake_up() && Is_usb_wake_up_interrupt_enabled())
    {
      Usb_unfreeze_clock();
      (void)Is_usb_clock_frozen();
      Usb_ack_wake_up();
      Usb_disable_wake_up_interrupt();
      Usb_wake_up_action();
      Usb_send_event(EVT_USB_WAKE_UP);
    }
    // Resume state bus detection
    if (Is_usb_resume() && Is_usb_resume_interrupt_enabled())
    {
      Usb_disable_wake_up_interrupt();
      Usb_ack_resume();
      Usb_disable_resume_interrupt();
      Usb_resume_action();
      Usb_send_event(EVT_USB_RESUME);
    }
    // USB bus reset detection
    if (Is_usb_reset() && Is_usb_reset_interrupt_enabled())
    {
      Usb_ack_reset();
      usb_init_device();
      Usb_reset_action();
      Usb_send_event(EVT_USB_RESET);
    }
  }
}


void usb_suspend_action(void)
{
  Enable_global_interrupt();
  //! @todo Implement this on the silicon version
  //Enter_power_down_mode();  // For example...
}
/* EOF */


