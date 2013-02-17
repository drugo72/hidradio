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
/** Device product ID. */
#define CDCDSerialDriverDescriptors_PRODUCTID       0x6119
/** Device vendor ID (Atmel). */
#define CDCDSerialDriverDescriptors_VENDORID        0x03EB
/** Device release number. */
#define CDCDSerialDriverDescriptors_RELEASE         0x0100

/** Returns the minimum between two values. */
#define MIN(a, b)       ((a < b) ? a : b)

/* === Typedefs =========================================================*/
/* Configuration descriptor list for a device implementing a
 *        CDC serial driver */
typedef struct _CDCDSerialDriverConfigurationDescriptors {

    /* Standard configuration descriptor. */
    USBConfigurationDescriptor configuration;
#if defined(CHIP_USB_OTGHS)
    /* OTG descriptor */
    USBOtgDescriptor otgDescriptor;
#endif
    /* Communication interface descriptor. */
    USBInterfaceDescriptor  communication;
    /* CDC header functional descriptor. */
    CDCHeaderDescriptor header;
    /* CDC call management functional descriptor. */
    CDCCallManagementDescriptor callManagement;
    /* CDC abstract control management functional descriptor. */
    CDCAbstractControlManagementDescriptor abstractControlManagement;
    /* CDC union functional descriptor (with one slave interface). */
    CDCUnionDescriptor union1;
    /* Notification endpoint descriptor. */
    USBEndpointDescriptor notification;
    /* Data interface descriptor. */
    USBInterfaceDescriptor data;
    /* Data OUT endpoint descriptor. */
    USBEndpointDescriptor dataOut;
    /* Data IN endpoint descriptor. */
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
    0, /* No string descriptor for serial number */
    1 /* Device has 1 possible configuration */
};

#if defined(CHIP_USB_UDPHS) || defined(CHIP_USB_OTGHS)

/* USB device qualifier descriptor. */
const USBDeviceQualifierDescriptor qualifierDescriptor = {

    sizeof(USBDeviceQualifierDescriptor),
    USBGenericDescriptor_DEVICEQUALIFIER,
    USBDeviceDescriptor_USB2_00,
    CDCDeviceDescriptor_CLASS,
    CDCDeviceDescriptor_SUBCLASS,
    CDCDeviceDescriptor_PROTOCOL,
    CHIP_USB_ENDPOINTS_MAXPACKETSIZE(0),
    1, /* Device has one possible configuration */
    0 /* Reserved */
};

#endif

/* Standard USB configuration descriptor for the CDC serial driver */
const CDCDSerialDriverConfigurationDescriptors configurationDescriptorsFS = {

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
#if defined(CHIP_USB_OTGHS)
    /* OTG descriptor */
    {
        sizeof(USBOtgDescriptor),
        USBGenericDescriptor_OTG,
        USBOTGDescriptor_HNP_SRP
    },
#endif
    /* Communication class interface standard descriptor */
    {
        sizeof(USBInterfaceDescriptor),
        USBGenericDescriptor_INTERFACE,
        0, /* This is interface #0 */
        0, /* This is alternate setting #0 for this interface */
        1, /* This interface uses 1 endpoint */
        CDCCommunicationInterfaceDescriptor_CLASS,
        CDCCommunicationInterfaceDescriptor_ABSTRACTCONTROLMODEL,
        CDCCommunicationInterfaceDescriptor_NOPROTOCOL,
        0  /* No string descriptor for this interface */
    },
    /* Class-specific header functional descriptor */
    {
        sizeof(CDCHeaderDescriptor),
        CDCGenericDescriptor_INTERFACE,
        CDCGenericDescriptor_HEADER,
        CDCGenericDescriptor_CDC1_10
    },
    /* Class-specific call management functional descriptor */
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
    /* Class-specific union functional descriptor with one slave interface */
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
        10 /* Endpoint is polled every 10ms */
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
    }
};

/* Language ID string descriptor */
const unsigned char languageIdStringDescriptor[] = {

    USBStringDescriptor_LENGTH(1),
    USBGenericDescriptor_STRING,
    USBStringDescriptor_ENGLISH_US
};

#if defined(CHIP_USB_UDPHS) || defined(CHIP_USB_OTGHS)
/* Other-speed configuration descriptor (when in full-speed). */
const CDCDSerialDriverConfigurationDescriptors otherSpeedDescriptorsFS = {

    /* Standard configuration descriptor */
    {
        sizeof(USBConfigurationDescriptor),
        USBGenericDescriptor_OTHERSPEEDCONFIGURATION,
        sizeof(CDCDSerialDriverConfigurationDescriptors),
        2, /* There are two interfaces in this configuration */
        1, /* This is configuration #1 */
        0, /* No string descriptor for this configuration */
        BOARD_USB_BMATTRIBUTES,
        USBConfigurationDescriptor_POWER(100)
    },
#if defined(CHIP_USB_OTGHS)
    /* OTG descriptor */
    {
        sizeof(USBOtgDescriptor),
        USBGenericDescriptor_OTG,
        USBOTGDescriptor_HNP_SRP
    },
#endif
    /* Communication class interface standard descriptor */
    {
        sizeof(USBInterfaceDescriptor),
        USBGenericDescriptor_INTERFACE,
        0, /* This is interface #0 */
        0, /* This is alternate setting #0 for this interface */
        1, /* This interface uses 1 endpoint */
        CDCCommunicationInterfaceDescriptor_CLASS,
        CDCCommunicationInterfaceDescriptor_ABSTRACTCONTROLMODEL,
        CDCCommunicationInterfaceDescriptor_NOPROTOCOL,
        0  /* No string descriptor for this interface */
    },
    /* Class-specific header functional descriptor */
    {
        sizeof(CDCHeaderDescriptor),
        CDCGenericDescriptor_INTERFACE,
        CDCGenericDescriptor_HEADER,
        CDCGenericDescriptor_CDC1_10
    },
    /* Class-specific call management functional descriptor */
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
    /* Class-specific union functional descriptor with one slave interface */
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
            USBEndpointDescriptor_MAXINTERRUPTSIZE_HS),
        10 /* Endpoint is polled every 10ms */
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
            USBEndpointDescriptor_MAXBULKSIZE_HS),
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
            USBEndpointDescriptor_MAXBULKSIZE_HS),
        0 /* Must be 0 for full-speed bulk endpoints */
    }
};


/* Configuration descriptor (when in high-speed). */
const CDCDSerialDriverConfigurationDescriptors configurationDescriptorsHS = {

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
#if defined(CHIP_USB_OTGHS)
    /* OTG descriptor */
    {
        sizeof(USBOtgDescriptor),
        USBGenericDescriptor_OTG,
        USBOTGDescriptor_HNP_SRP
    },
#endif
    /* Communication class interface standard descriptor */
    {
        sizeof(USBInterfaceDescriptor),
        USBGenericDescriptor_INTERFACE,
        0, /* This is interface #0 */
        0, /* This is alternate setting #0 for this interface */
        1, /* This interface uses 1 endpoint */
        CDCCommunicationInterfaceDescriptor_CLASS,
        CDCCommunicationInterfaceDescriptor_ABSTRACTCONTROLMODEL,
        CDCCommunicationInterfaceDescriptor_NOPROTOCOL,
        0  /* No string descriptor for this interface */
    },
    /* Class-specific header functional descriptor */
    {
        sizeof(CDCHeaderDescriptor),
        CDCGenericDescriptor_INTERFACE,
        CDCGenericDescriptor_HEADER,
        CDCGenericDescriptor_CDC1_10
    },
    /* Class-specific call management functional descriptor */
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
    /* Class-specific union functional descriptor with one slave interface */
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
            USBEndpointDescriptor_MAXINTERRUPTSIZE_HS),
        8  /* Endpoint is polled every 16ms */
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
            USBEndpointDescriptor_MAXBULKSIZE_HS),
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
            USBEndpointDescriptor_MAXBULKSIZE_HS),
        0 /* Must be 0 for full-speed bulk endpoints */
    }
};

/* Other-speed configuration descriptor (when in high-speed). */
const CDCDSerialDriverConfigurationDescriptors otherSpeedDescriptorsHS = {

    /* Standard configuration descriptor */
    {
        sizeof(USBConfigurationDescriptor),
        USBGenericDescriptor_OTHERSPEEDCONFIGURATION,
        sizeof(CDCDSerialDriverConfigurationDescriptors),
        2, /* There are two interfaces in this configuration */
        1, /* This is configuration #1 */
        0, /* No string descriptor for this configuration */
        BOARD_USB_BMATTRIBUTES,
        USBConfigurationDescriptor_POWER(100)
    },
#if defined(CHIP_USB_OTGHS)
    /* OTG descriptor */
    {
        sizeof(USBOtgDescriptor),
        USBGenericDescriptor_OTG,
        USBOTGDescriptor_HNP_SRP
    },
#endif
    /* Communication class interface standard descriptor */
    {
        sizeof(USBInterfaceDescriptor),
        USBGenericDescriptor_INTERFACE,
        0, /* This is interface #0 */
        0, /* This is alternate setting #0 for this interface */
        1, /* This interface uses 1 endpoint */
        CDCCommunicationInterfaceDescriptor_CLASS,
        CDCCommunicationInterfaceDescriptor_ABSTRACTCONTROLMODEL,
        CDCCommunicationInterfaceDescriptor_NOPROTOCOL,
        0  /* No string descriptor for this interface */
    },
    /* Class-specific header functional descriptor */
    {
        sizeof(CDCHeaderDescriptor),
        CDCGenericDescriptor_INTERFACE,
        CDCGenericDescriptor_HEADER,
        CDCGenericDescriptor_CDC1_10
    },
    /* Class-specific call management functional descriptor */
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
    /* Class-specific union functional descriptor with one slave interface */
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
        10 /* Endpoint is polled every 10ms */
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
    }
};
#endif

/* Product string descriptor */
const unsigned char productStringDescriptor[] = {

    USBStringDescriptor_LENGTH(13),
    USBGenericDescriptor_STRING,
    USBStringDescriptor_UNICODE('A'),
    USBStringDescriptor_UNICODE('T'),
    USBStringDescriptor_UNICODE('9'),
    USBStringDescriptor_UNICODE('1'),
    USBStringDescriptor_UNICODE('S'),
    USBStringDescriptor_UNICODE('A'),
    USBStringDescriptor_UNICODE('M'),
    USBStringDescriptor_UNICODE('3'),
    USBStringDescriptor_UNICODE('S'),
    USBStringDescriptor_UNICODE('-'),
    USBStringDescriptor_UNICODE('E'),
    USBStringDescriptor_UNICODE('K'),
    USBStringDescriptor_UNICODE(' ')
};

/* List of string descriptors used by the device */
const unsigned char *stringDescriptors[] = {

    languageIdStringDescriptor,
    productStringDescriptor,
};

/* List of standard descriptors for the serial driver. */
USBDDriverDescriptors cdcdSerialDriverDescriptors = {

    &deviceDescriptor,
    (USBConfigurationDescriptor *) &(configurationDescriptorsFS),
#if defined(CHIP_USB_UDPHS) || defined(CHIP_USB_OTGHS)
    &qualifierDescriptor,
    (USBConfigurationDescriptor *) &(otherSpeedDescriptorsFS),
    &deviceDescriptor,
    (USBConfigurationDescriptor *) &(configurationDescriptorsHS),
    &qualifierDescriptor,
    (USBConfigurationDescriptor *) &(otherSpeedDescriptorsHS),
#else
    0, /* No full-speed device qualifier descriptor */
    0, /* No full-speed other speed configuration */
    0, /* No high-speed device descriptor */
    0, /* No high-speed configuration descriptor */
    0, /* No high-speed device qualifier descriptor */
    0, /* No high-speed other speed configuration descriptor */

#endif
    stringDescriptors,
    2 /* 2 string descriptors in list */
};
/*EOF*/


