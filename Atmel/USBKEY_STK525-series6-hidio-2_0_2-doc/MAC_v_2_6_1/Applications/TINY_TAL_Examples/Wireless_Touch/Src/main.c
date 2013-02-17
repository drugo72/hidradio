/**
 * @file main.c
 *
 * @brief  Main of Tiny_TAL Example - Wireless_Touch
 *
 * $Id: main.c 25134 2011-01-25 12:35:28Z jignesh.vaghela $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
  This source file is taken from the ATMEL QTouch Library Release 4.2. & modified
  in order to support Wireless Transmission
  */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes ================================================================================ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "pal.h"
#include "tiny_tal.h"
#include "app_config.h"
#include "pal_config.h"
#include "ieee_const.h"
#include "touch_api_at32uc3l.h"

#if defined(__ICCAVR32__)
#include <intrinsics.h>
#endif


/* === Macros =================================================================================== */
#if (TAL_TYPE == AT86RF212)
    #ifdef CHINESE_BAND
        #define DEFAULT_CHANNEL         (0)
        #define DEFAULT_CHANNEL_PAGE    (5)
    #else
        #define DEFAULT_CHANNEL         (1)
        #define DEFAULT_CHANNEL_PAGE    (0)
    #endif  /* #ifdef CHINESE_BAND */
#else
#define DEFAULT_CHANNEL                 (20)
#endif
#define DEFAULT_PAN_ID                  (0xCAFE)
#define DST_PAN_ID                      (DEFAULT_PAN_ID)
#define SRC_PAN_ID                      (DEFAULT_PAN_ID)
#define OWN_SHORT_ADDR                  (0x0001)
#define DST_SHORT_ADDR                  (0x0001)

/* Frame overhead due to selected address scheme incl. FCS */
#if (DST_PAN_ID == SRC_PAN_ID)
#define FRAME_OVERHEAD                  (11)
#else
#define FRAME_OVERHEAD                  (13)
#endif

/* Maximum Application Data Length to be sent */
#define MAX_APP_DATA_LENGTH             (10)

#if ((MAX_APP_DATA_LENGTH > USB_TX_BUF_SIZE) || (MAX_APP_DATA_LENGTH > USB_RX_BUF_SIZE))
#error MAX_DATA_LENGTH too large
#endif


/* USART Selection */
#if( (defined UART0) ||(defined UART1) ||(defined UART2) || (defined UART3))
#define SIO_CHANNEL                     (SIO_0)
#endif
#ifdef USB0
#define SIO_CHANNEL                     (SIO_1)
#endif

/* Number of LEDS on Board*/
#if (NO_OF_LEDS >= 3)
#define LED_START                       (LED_0)
#define LED_DATA_RX                     (LED_1)
#define LED_DATA_TX                     (LED_2)
#elif (NO_OF_LEDS == 2)
#define LED_START                       (LED_0)
#define LED_DATA_RX                     (LED_0)
#define LED_DATA_TX                     (LED_1)
#else
#define LED_START                       (LED_0)
#define LED_DATA_RX                     (LED_1)
#define LED_DATA_TX                     (LED_2)
#endif

/* Example Timer configuration settings. */
/* configure timer for 1ms tick. */
#define TOUCH_ONE_MILLI_SEC                (1000u)

/* configuring the timer for 25 msec expiry */
#define TOUCH_MEASUREMENT_PERIOD_MS        (25u)


/* === GLOBALS ========================================================================================== */

/* Touch Measurement period in milliseconds.  This variable determines how often a new touch
   measurement must be done.Default value: Every 25 milliseconds. */
uint16_t measurement_period_ms = TOUCH_MEASUREMENT_PERIOD_MS;

/* Flag set by timer ISR when it's time to measure touch */
volatile uint8_t time_to_measure_touch = 0u;

/* Current time, set by timer ISR */
volatile uint16_t current_time_ms_touch = 0u;

/* Flag set by touch_qm_measure_complete_callback() function when a fresh Touch
   status is available. */
volatile uint8_t qm_measurement_done_touch = 0u;

/* QMatrix measured data pointer. */
touch_measure_data_t *p_qm_measure_data = NULL;

/* Transmission of the Touch Packets variables and buffers*/
static bool transmitting = false;
static bool transmitting_touch = false;
static uint8_t tx_buffer[LARGE_BUFFER_SIZE];
static void configure_frame_sending(void);
uint8_t tx_buffer_led[2];

/* QMatrix Data block provided as input to Touch library. */
static uint8_t qm_data_blk[PRIV_QM_DATA_BLK_SIZE];

/* QMatrix burst length.*/
uint8_t qm_burst_length[QM_NUM_CHANNELS] =
{
    128, /* x6 , y2 */   // Rotor
    128, /* x7, y2 */    // Rotor
    128, /* x10, y2 */   // Rotor
    128, /* x11, y2 */   // Rotor
    128, /* x14, y2 */   // Rotor
    128, /* x15 , y2 */  // Rotor
    128, /* x6, y7 */    // Touch Key down (v)
    128, /* x7, y7 */    // Touch Key up (^)
    128, /* x10, y7 */   // Touch Key left (<)
      1, /* x11, y7 */   // No connections
    128, /* x14, y7 */   // Touch Key play/pause (>/||)
    128  /* x15, y7 */   // Touch Key right (>)
};

/*
 * QMatrix Configuration structure provided as input to Touch Library.
 *
 * Note: Use the touch_config_at32uc3l.h configuration header file to fill in
 * the elements of this structure.  DO NOT modify any of the input values
 * directly in this structure.
 */
static touch_qm_config_t qm_config =
{
    /* QMatrix number of channels. */
    QM_NUM_CHANNELS,
    /* QMatrix number of sensors. */
    QM_NUM_SENSORS,
    /* QMatrix number of rotors and sliders. */
    QM_NUM_ROTORS_SLIDERS,
    /* QMatrix number of X Lines. */
    QM_NUM_X_LINES,
    /* QMatrix number of Y-Yk pairs. */
    QM_NUM_Y_LINES,
    /* QMatrix number of X sense pairs. */
    PRIV_QM_NUM_X_SENSE_PAIRS,
    /* QMatrix burst length write count. */
    PRIV_QM_BURST_LENGTH_WRITE_COUNT,
    {
    /* X pins selected. */
    QM_X_PINS_SELECTED,
    /* Y-Yk pairs selected. */
    QM_Y_PAIRS_SELECTED,
    /* specify the smp or dis pin option, if included in the hardware design.refer general_pin_options_t. */
    QM_SMP_DIS_PIN_OPTION,
    /* specify the vdiv pin option, if included in the hardware design.refer general_pin_options_t. */
    QM_VDIV_PIN_OPTION
    },
    /* Refer CAT User Interface section in AT32UC3L datasheet. */
    /* TOUCH LIBRARY WILL SHIFT THE VALUE SPECIFIED BELOW TO THE REQUIRED
      REGISTER OFFSET */
    {
    /* uint16_t div; MGCFG0 Register. */
    QM_CAT_CLK_DIV,
    /* uint8_t  chlen; MGCFG0 Register. */
    QM_CHLEN,
    /* uint8_t  selen; MGCFG0 Register. */
    QM_SELEN,
    /* uint8_t  dishift; MGCFG1 Register. */
    QM_DISHIFT,
    /* uint8_t  sync; MGCFG1 Register. */
    QM_ENABLE_EXTERNAL_SYNC,
    /* uint8_t  spread; MGCFG1 Register. */
    QM_ENABLE_SPREAD_SPECTRUM,
    /* uint8_t  dilen; MGCFG1 Register. */
    QM_DILEN,
    /* uint16_t max; MGCFG1 Register. */
    QM_MAX_ACQ_COUNT,
    /* uint8_t  acctrl; MGCFG2 Register. */
    1u,
    /* uint8_t  consen; MGCFG2 Register. */
    QM_CONSEN,
    /* uint8_t  cxdilen; MGCFG2 Register. */
    QM_CXDILEN,
    /* uint16_t synctim; MGCFG2 Register. */
    QM_SYNC_TIM,
    /* uint8_t  fsources; DICS Register. When SMP mode is used, this option is set to 0u by the Touch Library. */
    0u,
    /* uint8_t  glen; DICS Register. When SMP mode is used, this option is set to 0u by the Touch Library.  */
    1u,
    QM_INTVREFSEL,
    /* uint8_t  intvrefsel; DICS Register. When SMP mode is used, this option is set to 0u by the Touch Library.  */
    QM_INTREFSEL,
    /* uint8_t  intrefsel; DICS Register. When SMP mode is used, this option is set to 0u by the Touch Library.  */
    0u,
    /* uint8_t  trim; DICS Register. When SMP mode is used, this option is set to 0u by the Touch Library.  */
    0xFFu,
    /* uint8_t  sources; DICS Register. When SMP mode is used, this option is set to 0u by the Touch Library.  */
    0u,
    /* uint16_t shival0; ACSHIx Register.*/
    0u,
    /* uint16_t shival1; ACSHIx Register. */
    0u,
    /* uint16_t shival1; ACSHIx Register. */
    0u,
    /* uint16_t shival3; ACSHIx Register. */
    0u,
    /* uint16_t shival4; ACSHIx Register. */
    0u,
    /* uint16_t shival5; ACSHIx Register. */
    0u,
    /* uint16_t shival6; ACSHIx Register. */
    0u,
    },
    /* QMATRIX GLOBAL SENSOR CONFIGURATION INFO. */
    {
    /* uint8_t  di; Sensor detect integration (DI) limit. */
    QM_DI,
    /* uint8_t  neg_drift_rate; Sensor negative drift rate. */
    QM_NEG_DRIFT_RATE,
    /* uint8_t  pos_drift_rate; Sensor positive drift rate. */
    QM_POS_DRIFT_RATE,
    /* uint8_t  max_on_duration; Sensor maximum on duration. */
    QM_MAX_ON_DURATION,
    /* uint8_t  drift_hold_time; Sensor drift hold time. */
    QM_DRIFT_HOLD_TIME,
    /* uint8_t  pos_recal_delay; Sensor positive recalibration delay. */
    QM_POS_RECAL_DELAY,
    /* recal_threshold_t recal_threshold; Sensor recalibration threshold. */
    QM_RECAL_THRESHOLD,
    },
    /* QMatrix data block index. */
    qm_data_blk,
    /* QMatrix data block size. */
    PRIV_QM_DATA_BLK_SIZE,
    /* QMatrix burst length array starting index. */
    qm_burst_length,
    /* QMatrix filter callback function pointer. */
    QM_FILTER_CALLBACK
};

/*
 * General configuration options common to QMatrix, QTouch and Autonomous Touch
 * provided as input to Touch library.
 *
 * Note: Use the touch_config_at32uc3l.h configuration header file to fill in
 * the elements of this structure.  DO NOT modify any of the input values
 * directly in this structure.
 */
static touch_general_config_t general_config =
{
    /* Specify the sync pin option, if included in the hardware design.refer general_pin_options_t. */
    TOUCH_SYNC_PIN_OPTION,
    /* uint8_t  maxdev; SSCFG Register. Max deviation of spread spectrum. */
    TOUCH_SPREAD_SPECTRUM_MAX_DEV,
    /* uint32_t csares; CSA Resistor control register. */
    TOUCH_CSARES,
    /* uint32_t csbres;  CSA Resistor control register. */
    TOUCH_CSBRES
};

/* Touch Library input configuration structure. */
touch_config_t touch_config =
{
    /* Pointer to QMatrix configuration structure. */
    &qm_config,
    /* Pointer to Autonomous Touch configuration structure. */
    NULL,
    /* Pointer to QTouch Group A configuration structure. */
    NULL,
    /* Pointer to QTouch Group B configuration structure. */
    NULL,
    /* Pointer to General configuration structure. */
    &general_config
};

/* === Prototypes ======================================================================================*/
/* configure touch library for keys, rotors and sliders. */
static touch_ret_t config_uc3lek_touch_sensors( void );

/* QMatrix measure complete callback function example prototype. */
static void touch_qm_measure_complete_callback ( touch_measure_data_t *p_measure_data );

/* Application task for touch */
static void app_task();

/* Timer Callback for 25 millisecond */
static void timer_compare();

/* === Implementation ==================================================================================*/

/**
 * Main function, initialization and main message loop
 *
 * @return error code
 */

int main( void )
{
    /*  Initialize t the touch status */
    touch_ret_t touch_ret = TOUCH_SUCCESS;

    /* Initialize the TAL layer */
    if (tal_init() != MAC_SUCCESS)
    {
        // something went wrong during initialization
        pal_alert();
    }

    /* Configure timer to fire ISR regularly. */
    pal_timer_start(TIMER_TRIGGER_POLLING,
                    TOUCH_ONE_MILLI_SEC,
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)timer_compare,
                    NULL);

    /* Enable interrupts. */
    Enable_global_interrupt();

    /* Initialize the serial interface used for communication with terminal program */
    if (pal_sio_init(SIO_CHANNEL) != MAC_SUCCESS)
    {
        // something went wrong during initialization
         pal_alert();
    }

    /* Initialize touch library and uc3l cat module for QMatrix operation.
     Note: Set up the GCLK_CAT for proper QMatrix operation.  Refer init_system(). */
    touch_ret = touch_qm_sensors_init( &touch_config );

    if(touch_ret != TOUCH_SUCCESS)
    {
       while(1u); /* Check API Error return code. */
    }

    /* configure the touch library sensors. */
    touch_ret = config_uc3lek_touch_sensors();

    if(touch_ret != TOUCH_SUCCESS)
    {
        while(1u); /* Check API Error return code. */
    }

    /* Initialize touch sensing. */
    touch_ret = touch_qm_sensors_calibrate();

    if(touch_ret != TOUCH_SUCCESS)
    {
        while(1u); /* Check API Error return code. */
    }

    /* Configure the frame sending; e.g. set short address */
    configure_frame_sending();

    /* Switch receiver on */
    tal_rx_enable(PHY_RX_ON);

    /* Reset the Transmission flag */
    transmitting = false;

    /* Loop forever */
    while(1)
    {
        pal_task();
        tal_task();
        app_task();
    }
}

/**
 * @brief Application task
 */
void app_task()
{
    if(transmitting == false)
    {

	touch_ret_t touch_ret = TOUCH_SUCCESS;
	touch_qm_dma_t qm_dma;

	/*
	Provide the dma channels to be used by the CAT module.  For each
	acquisition cycle, any different combination of dma channels from 0 to 11
	can be provided. The touch library can also handle a different combination
	of dma channels for each call of the touch_qm_sensors_start_acquisition API.
	*/
	qm_dma.dma_ch1 = QM_DMA_CHANNEL_0;
	qm_dma.dma_ch2 = QM_DMA_CHANNEL_1;

	/*
	Process touch library events. The touch_event_dispatcher API needs to
	be called as frequently as possible in order to have a good touch response.
	*/
	touch_event_dispatcher();

	if( time_to_measure_touch == 1u )
        {
	    /* Clear flag: it's time to measure touch */
	    time_to_measure_touch = 0u;

	    /* Start a touch sensors measurement process. */
	    touch_ret = touch_qm_sensors_start_acquisition( current_time_ms_touch,&qm_dma,NORMAL_ACQ_MODE,
  							    touch_qm_measure_complete_callback);
    	    if( (touch_ret != TOUCH_SUCCESS) && (touch_ret != TOUCH_ACQ_INCOMPLETE) )
	    {
		pal_led(LED_0, LED_ON);  // LED0
		pal_led(LED_1, LED_ON);  // LED1
		pal_led(LED_2, LED_ON);  // LED2
		pal_led(LED_3, LED_ON);  // LED3
		while(1);
		/* Reaching this point can be due to -
		 1. The api has retured an error due to a invalid input parameter.
		 2. The api has been called during a invalid Touch Library state.
		*/
	     }
        }

	/* Host application code goes here */
	/* Led demo application. */
	if(qm_measurement_done_touch == 1u)
	{
            /* Clear flag: QMatrix measurement complete. */
	    qm_measurement_done_touch = 0u;
      	    uint8_t position_angle = p_qm_measure_data->p_rotor_slider_values[0];
	    if(position_angle != 0u)
	    {
		if(position_angle <= 64u)       pal_led(LED_0, LED_ON);  // Set LED0
		else if(position_angle <= 128u) pal_led(LED_1, LED_ON);  // Set LED1
		else if(position_angle <= 192u) pal_led(LED_2, LED_ON);  // Set LED2
		else                            pal_led(LED_3, LED_ON);  // Set LED3

		// Reset the position angle information
		p_qm_measure_data->p_rotor_slider_values[0u] = 0u;
		tx_buffer_led[0] = 0x00;
		tx_buffer_led[1] =position_angle ;
		transmitting_touch = true;
    	    }
	    /* Clear all LEDs when the down key (v) is touched.*/
    	    if(p_qm_measure_data->p_sensor_states[0u] & 0x2)
	    {
    	        pal_led(LED_0, LED_OFF);
		pal_led(LED_1, LED_OFF);
		pal_led(LED_2, LED_OFF);
		pal_led(LED_3, LED_OFF);
		tx_buffer_led[0] = 0x01;
		tx_buffer_led[1] =p_qm_measure_data->p_sensor_states[0u] ;
		transmitting_touch = true;
	    }
    	    /* Clear LED0 when the up key (^) is touched */
    	    if(p_qm_measure_data->p_sensor_states[0u] & 0x4)
	    {
		tx_buffer_led[0] = 0x01;
		tx_buffer_led[1] =p_qm_measure_data->p_sensor_states[0u] ;
		pal_led(LED_0, LED_OFF);
		transmitting_touch = true;
    	    }
	    /* Clear LED1 when the left key (<) is touched.*/
	    if(p_qm_measure_data->p_sensor_states[0u] & 0x8)
	    {
		tx_buffer_led[0] = 0x01;
		tx_buffer_led[1] =p_qm_measure_data->p_sensor_states[0u] ;
		pal_led(LED_1, LED_OFF);
		transmitting_touch = true;
            }
	    /* Clear LED2 when the play/pause key (>/||) is touched.*/
	    if(p_qm_measure_data->p_sensor_states[0u] & 0x10)
	    {
		tx_buffer_led[0] = 0x01;
		tx_buffer_led[1] =p_qm_measure_data->p_sensor_states[0u] ;
		pal_led(LED_2, LED_OFF);
		transmitting_touch = true;
	    }
    	    /* Clear LED3 when the right key (>) is touched.*/
	    if(p_qm_measure_data->p_sensor_states[0u] & 0x20)
	    {
		tx_buffer_led[0] = 0x01;
		tx_buffer_led[1] =p_qm_measure_data->p_sensor_states[0u] ;
		pal_led(LED_3, LED_OFF);
		transmitting_touch = true;
            }

    	    /*Transmissitting the Touch packets here*/
	    if(transmitting_touch == true)
	    {
	        transmitting = true;
		tx_buffer[PL_POS_SEQ_NUM]++;
		tx_buffer[0] = 2 + FRAME_OVERHEAD;
		memcpy(&tx_buffer[LENGTH_FIELD_LEN + FRAME_OVERHEAD  - FCS_LEN],&tx_buffer_led[0],2);
		tal_tx_frame(tx_buffer, CSMA_UNSLOTTED, false);
		transmitting_touch = false;
            }
        }
    }
}

/**
 * @brief Example configuration for 64 touch keys.
 *
 * This function used to configure the 64 touch keys.
 *
 *
 * @return   TOUCH_SUCCESS
 */
touch_ret_t config_uc3lek_touch_sensors( void )
{
    touch_ret_t touch_ret = TOUCH_SUCCESS;
    sensor_id_t sensor_id;

    /* configure touch rotor. */
    touch_ret = touch_qm_sensor_config( SENSOR_TYPE_ROTOR, (channel_t)0u, (channel_t)5u,
                                       NO_AKS_GROUP, 25u, (hysteresis_t)HYST_6_25, RES_8_BIT, 0u,
                                       &sensor_id );
    if(touch_ret != TOUCH_SUCCESS)
    {
	while(1); /* Check API Error return code. */
    }

    /* configure touch key. */
    touch_ret = touch_qm_sensor_config( SENSOR_TYPE_KEY, (channel_t)6u, (channel_t)6u,
                                       NO_AKS_GROUP, 25u, HYST_6_25, RES_1_BIT, 0u,
                                       &sensor_id );
    if(touch_ret != TOUCH_SUCCESS)
    {
	while(1); /* Check API Error return code. */
    }

    /* configure touch key. */
    touch_ret = touch_qm_sensor_config( SENSOR_TYPE_KEY, (channel_t)7u, (channel_t)7u,
                                       NO_AKS_GROUP, 25u, HYST_6_25, RES_1_BIT, 0u,
                                       &sensor_id );
    if(touch_ret != TOUCH_SUCCESS)
    {
	while(1); /* Check API Error return code. */
    }

    /* configure touch key. */
    touch_ret = touch_qm_sensor_config( SENSOR_TYPE_KEY, (channel_t)8u, (channel_t)8u,
                                       NO_AKS_GROUP, 25u, HYST_6_25, RES_1_BIT, 0u,
                                       &sensor_id );
    if(touch_ret != TOUCH_SUCCESS)
    {
	while(1); /* Check API Error return code. */
    }

    /* configure touch key. */
    touch_ret = touch_qm_sensor_config( SENSOR_TYPE_KEY, (channel_t)10u, (channel_t)10u,
                                       NO_AKS_GROUP, 25u, HYST_6_25, RES_1_BIT, 0u,
                                       &sensor_id );
    if(touch_ret != TOUCH_SUCCESS)
    {
	while(1); /* Check API Error return code. */
    }

    /* configure touch key. */
    touch_ret = touch_qm_sensor_config( SENSOR_TYPE_KEY, (channel_t)11u, (channel_t)11u,
                                       NO_AKS_GROUP, 25u, HYST_6_25, RES_1_BIT, 0u,
                                       &sensor_id );
    if(touch_ret != TOUCH_SUCCESS)
    {
	while(1); /* Check API Error return code. */
    }

    return ( touch_ret );
}

/**
 * @brief QMatrix measure complete callback function
 *
 * This function signifies that fresh values of touch status, rotor/slider
 * position, measured signals,references and Sensor data is available
 *
 * @param touch_measure_data_t *p_measure_data
 *
 */
void touch_qm_measure_complete_callback ( touch_measure_data_t *p_measure_data )
{
    /* Copy the QMatrix measure data pointer. */
    p_qm_measure_data = p_measure_data;

    /* Set the QMatrix measurement done flag. */
    qm_measurement_done_touch = 1u;
}

/**
 * @brief timer compare ISR.
 *
 * This function  is used to set the flag to measure touch.
 *
 */
static void timer_compare()
{
    /* update the current time */
    current_time_ms_touch++;

    /* every 25th ms*/
    if((current_time_ms_touch % measurement_period_ms) == 0u )
    {
	/* set flag: it's time to measure touch */
	time_to_measure_touch = 1u;
    }
	/* Re-start led timer again. */
    pal_timer_start(TIMER_TRIGGER_POLLING,
                    TOUCH_ONE_MILLI_SEC,
                    TIMEOUT_RELATIVE,
                    (FUNC_PTR)timer_compare,
                    NULL);
}



/**
 * @brief Configure the frame sending
 */
static void configure_frame_sending(void)
{
    uint8_t temp_value_8;
    uint16_t temp_value_16;
    uint16_t fcf = 0;

    /* Set initial frame length to MHR length without additional MSDU. */
    tx_buffer[0] = FRAME_OVERHEAD;

    /* Calculate FCF. */
    fcf = FCF_FRAMETYPE_DATA | FCF_ACK_REQUEST;
    fcf |= FCF_SET_SOURCE_ADDR_MODE(FCF_SHORT_ADDR);
    fcf |= FCF_SET_DEST_ADDR_MODE(FCF_SHORT_ADDR);

#if (DST_PAN_ID == SRC_PAN_ID)
    fcf |= FCF_PAN_ID_COMPRESSION;
#endif

    /* Set FCF in frame. */
    convert_16_bit_to_byte_array(CPU_ENDIAN_TO_LE16(fcf), &tx_buffer[PL_POS_FCF_1]);

    /* Set initial sequence number. */
    tx_buffer[PL_POS_SEQ_NUM] = (uint8_t)rand();

    /* Set Destination PAN-Id in frame. */
    temp_value_16 = DEFAULT_PAN_ID;
    convert_16_bit_to_byte_array(CPU_ENDIAN_TO_LE16(temp_value_16), &tx_buffer[PL_POS_DST_PAN_ID_START]);

    /* Set Destination Address in frame. */
    temp_value_16 = DST_SHORT_ADDR;
    convert_16_bit_to_byte_array(CPU_ENDIAN_TO_LE16(temp_value_16), &tx_buffer[PL_POS_DST_ADDR_START]);

    /* Set Source PAN-Id in frame. */
    temp_value_16 = DEFAULT_PAN_ID;
    tal_pib_set(macPANId, (pib_value_t *)&temp_value_16);
#if (DST_PAN_ID != SRC_PAN_ID)
    convert_16_bit_to_byte_array(CPU_ENDIAN_TO_LE16(temp_value_16), &tx_buffer[PL_POS_DST_ADDR_START + 2]);
#endif

    /* Set Source Address in frame. */
    temp_value_16 = OWN_SHORT_ADDR;
    tal_pib_set(macShortAddress, (pib_value_t *)&temp_value_16);
#if (DST_PAN_ID != SRC_PAN_ID)
    convert_16_bit_to_byte_array(CPU_ENDIAN_TO_LE16(temp_value_16), &tx_buffer[PL_POS_DST_ADDR_START + 4]);
#else
    convert_16_bit_to_byte_array(CPU_ENDIAN_TO_LE16(temp_value_16), &tx_buffer[PL_POS_DST_ADDR_START + 2]);
#endif
    /* Set proper channel. */
    temp_value_8 = DEFAULT_CHANNEL;
    tal_pib_set(phyCurrentChannel, (pib_value_t *)&temp_value_8);
}

/**
 * @brief Callback that is called if data has been received by trx.
 *
 * @param rx_frame_array Pointer to data array containing received frame
 */
void tal_rx_frame_cb(uint8_t *rx_frame_array)
{
    uint8_t *rx_payload_ptr = rx_frame_array + FRAME_OVERHEAD + LENGTH_FIELD_LEN - FCS_LEN;
    /* Rotor has to be enabled */
    if(*rx_payload_ptr == 0)
    {
	rx_payload_ptr++;
	if(*rx_payload_ptr != 0u)
	{
	    if(*rx_payload_ptr <= 64u)        // Set LED0
	    {
	        pal_led(LED_0, LED_ON);
	    }
	    else if(*rx_payload_ptr <= 128u)  // Set LED1
	    {
	        pal_led(LED_1, LED_ON);
	    }
	    else if(*rx_payload_ptr <= 192u)  // Set LED2
	    {
	        pal_led(LED_2, LED_ON);
	    }
	    else                              // Set LED3
	    {
	        pal_led(LED_3, LED_ON);
	    }
	}
    }
    /* Touch Keys has to be enabled */
    else if(*rx_payload_ptr == 1)
    {
	rx_payload_ptr++;
	if(*rx_payload_ptr & 0x2)
	{
      	    pal_led(LED_3, LED_OFF);
	    pal_led(LED_0, LED_OFF);
	    pal_led(LED_1, LED_OFF);
	    pal_led(LED_2, LED_OFF);
	    pal_led(LED_3, LED_OFF);
	}
	/* Clear LED0 when the up key (^) is touched*/
	if(*rx_payload_ptr & 0x4)
	{
	    pal_led(LED_0, LED_OFF);
	}
	/* Clear LED1 when the left key (<) is touched.*/
	if(*rx_payload_ptr & 0x8)
	{
	    pal_led(LED_1, LED_OFF);
	}
	/* Clear LED2 when the play/pause key (>/||) is touched.*/
	if(*rx_payload_ptr & 0x10)
	{
	    pal_led(LED_2, LED_OFF);
	}
	/* Clear LED3 when the right key (>) is touched.*/
	if(*rx_payload_ptr & 0x20)
	{
	    pal_led(LED_3, LED_OFF);
	}
    }
}


/**
 * @brief Callback that is called once tx is done.
 *
 * @param status    Status of the transmission procedure
 */
void tal_tx_frame_done_cb(retval_t status)
{
    if (status == MAC_SUCCESS)
    {
	/* After transmission is completed, allow next transmission. */
        transmitting = false;
    }
}
/* EOF */
