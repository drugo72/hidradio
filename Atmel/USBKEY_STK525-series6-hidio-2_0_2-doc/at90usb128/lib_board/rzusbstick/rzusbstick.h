/*This file is prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file contains the low level macros and definition for USB KEY board
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

#ifndef RZUSBSTICK_H
#define RZUSBSTICK_H

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#if (TARGET_BOARD==RZUSBSTICK)

//_____ M A C R O S ________________________________________________________


//! @defgroup RZUSBSTICK_module RZUSBSTICK
//! This module contains low level hardware abstraction layer for RZUSBstick board
//! @image html rzusbstick.gif
//! @{

      //! @defgroup RZUSBSTICK_leds Leds Management
      //! Macros to manage Leds on RZUSBstick
      //! The led 0 correspond at led D1 BLUE
      //! The led 1 correspond at led D2 RED
      //! The led 2 correspond at led D3 GREEN
      //! The led 3 correspond at led D4 ORANGE
      //! @{
#define  LED0_PORT            PORTD
#define  LED0_DDR             DDRD
#define  LED0_PIN             PIND
#define  LED0_BIT             PIND7

#define  LED1_PORT            PORTD
#define  LED1_DDR             DDRD
#define  LED1_PIN             PIND
#define  LED1_BIT             PIND5

#define  LED2_PORT            PORTE
#define  LED2_DDR             DDRE
#define  LED2_PIN             PINE
#define  LED2_BIT             PINE7

#define  LED3_PORT            PORTE
#define  LED3_DDR             DDRE
#define  LED3_PIN             PINE
#define  LED3_BIT             PINE6

#define  Leds_init()          (                           \
                               LED0_DDR |= (1<<LED0_BIT), \
                               LED1_DDR |= (1<<LED1_BIT), \
							   LED2_DDR |= (1<<LED2_BIT), \
							   LED3_DDR |= (1<<LED3_BIT)  \
                              )

#define  Leds_on()            (                             \
                               LED0_PORT |= (1<<LED0_BIT),  \
							   LED1_PORT &= ~(1<<LED1_BIT), \
							   LED2_PORT &= ~(1<<LED2_BIT), \
							   LED3_PORT &= ~(1<<LED3_BIT)  \
							  )

#define  Leds_off()           (                             \
                               LED0_PORT &= ~(1<<LED0_BIT), \
							   LED1_PORT |= (1<<LED1_BIT),  \
							   LED2_PORT |= (1<<LED2_BIT),  \
							   LED3_PORT |= (1<<LED3_BIT)   \
                              )

#define  Led0_on()            (LED0_PORT |=  (1<<LED0_BIT))
#define  Led1_on()            (LED1_PORT &= ~(1<<LED1_BIT))
#define  Led2_on()            (LED2_PORT &= ~(1<<LED2_BIT))
#define  Led3_on()            (LED3_PORT &= ~(1<<LED3_BIT))
#define  Led0_off()           (LED0_PORT &= ~(1<<LED0_BIT))
#define  Led1_off()           (LED1_PORT |=  (1<<LED1_BIT))
#define  Led2_off()           (LED2_PORT |=  (1<<LED2_BIT))
#define  Led3_off()           (LED3_PORT |=  (1<<LED3_BIT))
#define  Led0_toggle()        (LED0_PIN  |=  (1<<LED0_BIT))
#define  Led1_toggle()        (LED1_PIN  |=  (1<<LED1_BIT))
#define  Led2_toggle()        (LED2_PIN  |=  (1<<LED2_BIT))
#define  Led3_toggle()        (LED3_PIN  |=  (1<<LED3_BIT))
#define  Is_led0_on()         (LED0_PIN  &   (1<<LED0_BIT) ? TRUE : FALSE)
#define  Is_led1_on()         (LED1_PIN  &   (1<<LED1_BIT) ? FALSE : TRUE)
#define  Is_led2_on()         (LED2_PIN  &   (1<<LED2_BIT) ? FALSE : TRUE)
#define  Is_led3_on()         (LED3_PIN  &   (1<<LED3_BIT) ? FALSE : TRUE)


#define  Leds_set_val(c)      (                                         \
                               (((c) & 0x01) ? Led0_on() : Led0_off()), \
							   (((c) & 0x02) ? Led1_on() : Led1_off()), \
							   (((c) & 0x04) ? Led2_on() : Led2_off()), \
							   (((c) & 0x08) ? Led3_on() : Led3_off())  \
							  )

#define  Leds_get_val()       (                                            \
                               (LED0_PORT & (1<<LED0_BIT) ? 0x01 : 0x00) | \
							   (LED1_PORT & (1<<LED1_BIT) ? 0x00 : 0x02) | \
							   (LED2_PORT & (1<<LED2_BIT) ? 0x00 : 0x04) | \
							   (LED3_PORT & (1<<LED3_BIT) ? 0x00 : 0x08)   \
                              )

                      
      //! @}

      //! @defgroup RZUSBSTICK_joy Joystick Management
      //! Macros to manage Joystick on RZUSBstick
      //! @{
#define  Joy_init()           

#define  Is_btn_left()        Is_hwb()
#define  Is_btn_middle()      FALSE
#define  Is_btn_right()       FALSE

#define  Is_joy_up()          FALSE
#define  Is_joy_left()        FALSE
#define  Is_joy_down()        FALSE
#define  Is_joy_right()       FALSE

#define  Is_btn_not_left()    Is_not_hwb()
#define  Is_btn_not_middle()  FALSE
#define  Is_btn_not_right()   TRUE

#define  Is_joy_not_up()      FALSE
#define  Is_joy_not_left()    TRUE
#define  Is_joy_not_down()    TRUE
#define  Is_joy_not_right()   TRUE

//! Enable interrupts for switches
//! - Set up interrupts for switches
//! - clear flag
//! - enable interrupt
#define  Switches_enable_it()
 
//! Disable interrupts for switches ()
//! - disable interrupt
//! - clear flag
//! - Clear mask
#define  Switches_disable_it()
      //! @}


      //! @defgroup RZUSBSTICK_HWB HWB button management
      //! @{
#define Hwb_button_init()
#define Is_hwb()               FALSE
#define Is_not_hwb()           TRUE
      //! @}


      //! @defgroup RZUSBSTICK_df USB KEY Hardware data flash configuration
      //! Macros to init the environnement for DF on RZUSBstick
      //! @{

// One AT45DB642D
#define DF_8_MB            
#define DF_NB_MEM          2

// PINB0 : SPI Slave Select pin, this pin must be disable (disable input mode) because it is used by joystick
//         Note: the joystick can work fine in Output mode, because the pull-up resistor is activated and the level pin can be read.
// PINB1 : the SCK signal direction is OUTPUT
// PINB2 : the SI signal direction is OUTPUT
// PINB3 : the SO signal direction is INPUT
// PINE0 : the CS0 signal direction is OUTPUT
// PINE1 : the CS1 signal direction is OUTPUT
#define  Df_init_spi()
#define  Df_select_0()
#define  Df_select_1()
#define  Df_desel_all()

      //! @}

//! @}

// Timer
#define Timer_init() (TCCR1B = 1 << CS11) // 8MHz/8
#define Timer_read_unsafe() (TCNT1) // Not safe if TCNT1 modified in an interrupt

// USB KEY ADC Channel Definition
#define ADC_TEMP_CH  0x00

#ifdef USE_ADC       //!< this define is set in config.h file



//! @brief This function performs an ADC conversion from the RZUSBSTICK TEMP channel
//! an returns the 10 bits result in an U16 value.
//!
//! @warning USE_ADC should be defined in config.h
//!
//! @param none
//!
//! @return U16 analog sensor temperature value.
//!
   U16 Get_adc_temp_val(void);


//! @brief This function performs an ADC conversion from the RZUSBSTICK TEMP channel
//! an returns the 10 bits result of the temperature (in °C) in an S16 value.
//!
//! @warning USE_ADC should be defined in config.h
//!
//! @param none
//!
//! @return S16 temperature value in °C.
//!
   S16  Read_temperature(void);

#endif   // USE_ADC

#endif   // TARGET_BOARD==RZUSBSTICK

#endif   // RZUSBSTICK_H

