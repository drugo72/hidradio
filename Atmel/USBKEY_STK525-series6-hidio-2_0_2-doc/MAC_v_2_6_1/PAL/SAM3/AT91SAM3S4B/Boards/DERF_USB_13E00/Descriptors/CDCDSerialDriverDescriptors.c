 /* @file CDCDSerialDriverDescriptors.c
 *
 * @brief USB Descriptor related code
 *
 * This file implements USB Descriptor
 * and reception functions.
 *
 * $Id: CDCDSerialDriverDescriptors.c 24193 2010-11-15 07:31:12Z mahendran.p $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === Includes =========================================================*/
#include "CDCDSerialDriverDescriptors.h"
#include "pal.h"
#include "pal_config.h"
#include <usb/common/core/USBGenericDescriptor.h>
#include <usb/common/core/USBConfigurationDescriptor.h>
#include <usb/common/core/USBEndpointDescriptor.h>
#include <usb/common/core/USBStringDescriptor.h>
#include <usb/common/core/USBGenericRequest.h>
#include <usb/common/cdc/CDCGenericDescriptor.h>
#include <usb/common/cdc/CDCDeviceDescriptor.h>
#include <usb/common/cdc/CDCCommunicationInterfaceDescriptor.h>
#include <usb/common/cdc/CDCDataInterfaceDescriptor.h>
#include <usb/common/cdc/CDCHeaderDescriptor.h>
#include <usb/common/cdc/CDCCallManagementDescriptor.h>
#include <usb/common/cdc/CDCAbstractControlManagementDescriptor.h>
#include <usb/common/cdc/CDCUnionDescriptor.h>

/* === Macros =========================================================*/

#define PRODUCTID_PERYTONS_FW_DE                    0x0018
#define PRODUCTID_PERYTONS_FW_RZ_SUB                0x2115
#define VENDORID_ATMEL                              0x03EB
#define VENDORID_DE                                 0x1CF1
#define RELEASE_NO                                  0x0100


#define CDCDSerialDriverDescriptors_PRODUCTID       PRODUCTID_PERYTONS_FW_DE
#define CDCDSerialDriverDescriptors_VENDORID        VENDORID_DE
#define CDCDSerialDriverDescriptors_RELEASE         RELEASE_NO

/* Returns the minimum between two values */
#define MIN(a, b)       ((a < b) ? a : b)

/* === Typedefs =========================================================*/

/*Configuration descriptor list for a device implementing a CDC serial driver*/
typedef struct {

    /* Standard configuration descriptor */
    USBConfigurationDescriptor configuration;
    /* Communication interface descriptor*/
    USBInterfaceDescriptor  communication;
    /* CDC header functional descriptor*/
    CDCHeaderDescriptor header;
    /* CDC call management functional descriptor*/
    CDCCallManagementDescriptor callManagement;
    /* CDC abstract control management functional descriptor */
    CDCAbstractControlManagementDescriptor abstractControlManagement;
    /* CDC union functional descriptor (with one slave interface)*/
    CDCUnionDescriptor union1;
    /* Notification endpoint descriptor*/
    USBEndpointDescriptor notification;
    /* Data interface descriptor*/
    USBInterfaceDescriptor data;
    /* Data OUT endpoint descriptor*/
    USBEndpointDescriptor dataOut;
    /* Data IN endpoint descriptor*/
    USBEndpointDescriptor dataIn;

} __attribute__ ((packed)) CDCDSerialDriverConfigurationDescriptors;

/* === Globals =========================================================*/

/* Standard USB device descriptor for the CDC serial driver */
const USBDeviceDescriptor deviceDescriptor = {

    sizeof(USBDeviceDescriptor),
    USBGenericDescriptor_DEVICE,
    USBDeviceDescriptor_USB2_00,
    CDCDeviceDescriptor_CLASS,
    CDCDeviceDescriptor_SUBCLASS,
    CDCDeviceDescriptor_PROTOCOL,
    CHIP_USB_ENDPOINTS_MAXPACKETSIZE(0),
    CDCDSerialDriverDescriptors_VENDORID,
    CDCDSerialDriverDescriptors_PRODUCTID,
    CDCDSerialDriverDescriptors_RELEASE,
    0, /* No string descriptor for manufacturer */
    1, /* Index of product string descriptor is #1 */
    0, /* Index of string descriptor for serial number */
    1  /* Device has 1 possible configuration */
};


/* Standard USB configuration descriptor for the CDC serial driver */
const CDCDSerialDriverConfigurationDescriptors configurationDescriptors = {

    /* Standard configuration descriptor */
    {
        sizeof(USBConfigurationDescriptor),
        USBGenericDescriptor_CONFIGURATION,
        sizeof(CDCDSerialDriverConfigurationDescriptors),
        2, /* There are two interfaces in this configuration */
        1, /* This is configuration #1 */
        0, /* No string descriptor for this configuration */
        BOARD_USB_BMATTRIBUTES,
        USBConfigurationDescriptor_POWER(100)
    },
    /* Communication class interface standard descriptor */
    {
        sizeof(USBInterfaceDescriptor),
        USBGenericDescriptor_INTERFACE,
        0, /* This is interface #0 */
        0, /* This is alternate setting #0 for this interface */
        1, /* This interface uses 1 endpoint*/
        CDCCommunicationInterfaceDescriptor_CLASS,
        CDCCommunicationInterfaceDescriptor_ABSTRACTCONTROLMODEL,
        CDCCommunicationInterfaceDescriptor_NOPROTOCOL,
        0  /* No string descriptor for this interface*/
    },
    /* Class-specific header functional descriptor */
    {
        sizeof(CDCHeaderDescriptor),
        CDCGenericDescriptor_INTERFACE,
        CDCGenericDescriptor_HEADER,
        CDCGenericDescriptor_CDC1_10
    },
    /* Class-specific call management functional descriptor*/
    {
        sizeof(CDCCallManagementDescriptor),
        CDCGenericDescriptor_INTERFACE,
        CDCGenericDescriptor_CALLMANAGEMENT,
        CDCCallManagementDescriptor_SELFCALLMANAGEMENT,
        0 /* No associated data interface */
    },
    /* Class-specific abstract control management functional descriptor */
    {
        sizeof(CDCAbstractControlManagementDescriptor),
        CDCGenericDescriptor_INTERFACE,
        CDCGenericDescriptor_ABSTRACTCONTROLMANAGEMENT,
        CDCAbstractControlManagementDescriptor_LINE
    },
    /* Class-specific union functional descriptor with one slave interface*/
    {
        sizeof(CDCUnionDescriptor),
        CDCGenericDescriptor_INTERFACE,
        CDCGenericDescriptor_UNION,
        0, /* Number of master interface is #0 */
        1 /* First slave interface is #1 */
    },
    /* Notification endpoint standard descriptor */
    {
        sizeof(USBEndpointDescriptor),
        USBGenericDescriptor_ENDPOINT,
        USBEndpointDescriptor_ADDRESS(USBEndpointDescriptor_IN,
                                      CDCDSerialDriverDescriptors_NOTIFICATION),
                                      USBEndpointDescriptor_INTERRUPT,
                                      MIN(CHIP_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_NOTIFICATION),
                                      USBEndpointDescriptor_MAXINTERRUPTSIZE_FS),
                                      100 /* Endpoint is polled every 100ms */
},
    /* Data class interface standard descriptor */
    {
        sizeof(USBInterfaceDescriptor),
        USBGenericDescriptor_INTERFACE,
        1, /* This is interface #1 */
        0, /* This is alternate setting #0 for this interface */
        2, /* This interface uses 2 endpoints */
        CDCDataInterfaceDescriptor_CLASS,
        CDCDataInterfaceDescriptor_SUBCLASS,
        CDCDataInterfaceDescriptor_NOPROTOCOL,
        0  /* No string descriptor for this interface */
    },
    /* Bulk-OUT endpoint standard descriptor */
    {
        sizeof(USBEndpointDescriptor),
        USBGenericDescriptor_ENDPOINT,
        USBEndpointDescriptor_ADDRESS(USBEndpointDescriptor_OUT,
                                      CDCDSerialDriverDescriptors_DATAOUT),
        USBEndpointDescriptor_BULK,
        MIN(CHIP_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAOUT),
            USBEndpointDescriptor_MAXBULKSIZE_FS),
        0 /* Must be 0 for full-speed bulk endpoints */
    },
    /* Bulk-IN endpoint descriptor */
    {
        sizeof(USBEndpointDescriptor),
        USBGenericDescriptor_ENDPOINT,
        USBEndpointDescriptor_ADDRESS(USBEndpointDescriptor_IN,
                                      CDCDSerialDriverDescriptors_DATAIN),
        USBEndpointDescriptor_BULK,
        MIN(CHIP_USB_ENDPOINTS_MAXPACKETSIZE(CDCDSerialDriverDescriptors_DATAIN),
            USBEndpointDescriptor_MAXBULKSIZE_FS),
        0 /* Must be 0 for full-speed bulk endpoints */
    },
};

/* Language ID string descriptor */
const unsigned char languageIdStringDescriptor[] = {

    USBStringDescriptor_LENGTH(1),
    USBGenericDescriptor_STRING,
    USBStringDescriptor_ENGLISH_US
};


/* Product string descriptor */
const unsigned char productStringDescriptor[] = {
    USBStringDescriptor_LENGTH(12),
    USBGenericDescriptor_STRING,
    USBStringDescriptor_UNICODE('d'),
    USBStringDescriptor_UNICODE('e'),
    USBStringDescriptor_UNICODE('R'),
    USBStringDescriptor_UNICODE('F'),
    USBStringDescriptor_UNICODE('u'),
    USBStringDescriptor_UNICODE('s'),
    USBStringDescriptor_UNICODE('b'),
    USBStringDescriptor_UNICODE('1'),
    USBStringDescriptor_UNICODE('3'),
    USBStringDescriptor_UNICODE('E'),
    USBStringDescriptor_UNICODE('0'),
    USBStringDescriptor_UNICODE('4')
};

/* List of string descriptors used by the device */
const unsigned char *stringDescriptors[] = {
    languageIdStringDescriptor,
    productStringDescriptor
};

/* List of standard descriptors for the serial driver */
USBDDriverDescriptors cdcdSerialDriverDescriptors = {

    &deviceDescriptor,
    (USBConfigurationDescriptor *) &(configurationDescriptors),
    0, /* No full-speed device qualifier descriptor */
    0, /* No full-speed other speed configuration */
    0, /* No high-speed device descriptor */
    0, /* No high-speed configuration descriptor */
    0, /* No high-speed device qualifier descriptor */
    0, /* No high-speed other speed configuration descriptor*/
    stringDescriptors,
    2  /* string descriptors in list */
};


