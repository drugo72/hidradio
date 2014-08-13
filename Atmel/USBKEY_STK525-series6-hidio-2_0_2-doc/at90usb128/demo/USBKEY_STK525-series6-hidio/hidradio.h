#ifndef HIDRADIO_H
#define HIDRADIO_H

#include <stddef.h>
#include <stdint.h>

/*
 * Message types
 */
#define HR_TYPE_GET_REQ 0x00
#define HR_TYPE_GET_CNF 0x80

#define HR_TYPE_SET_REQ 0x02
#define HR_TYPE_SET_CNF 0x82

#define HR_TYPE_DATA_REQ 0x08
#define HR_TYPE_DATA_CNF 0x88

#define HR_TYPE_DATA_IND 0x8a

#define HR_TYPE_ECHO_REQ 0x20
#define HR_TYPE_ECHO_RESP 0xa0

#define HR_TYPE_UNSUPPORTED_MSG 0xff

/*
 * Confirm statuses
 */
#define HR_STATUS_SUCCESS 0x00
#define HR_STATUS_CHANNEL_ACCESS_FAILURE 0xe1
#define HR_STATUS_NO_ACK 0xe9
#define HR_STATUS_INVALID_PARAMETER 0xe8
#define HR_STATUS_UNSUPPORTED_ATTRIBUTE 0xf4

/*
 * PHY PIB attribute IDs
 */
#define HR_ATTR_PHY_CURRENT_CHANNEL 0x00
#define HR_ATTR_PHY_CHANNELS_SUPPORTED 0x01
#define HR_ATTR_PHY_TRANSMIT_POWER 0x02
#define HR_ATTR_PHY_CCA_MODE 0x03

/*
 * MAC PHY attribute IDs
 */
#define HR_ATTR_MAC_MAX_CSMA_BACKOFFS 0x4e
#define HR_ATTR_MAC_MIN_BE 0x4f
#define HR_ATTR_MAC_MAX_FRAME_RETRIES 0x59
#define HR_ATTR_MAC_PAN_ID 0x50
#define HR_ATTR_MAC_PROMISCUOUS_MODE 0x51
#define HR_ATTR_MAC_RX_ON_WHEN_IDLE 0x52
#define HR_ATTR_MAC_SHORT_ADDRESS 0x53

/* TODO: In order to operate properly, the transceiver needs to know if we are the coordinator */
#define HR_ATTR_I_AM_COORD 0x0B
#define HR_ATTR_MAC_IEEE_ADDRESS 0xf0

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


#define HR_TX_OPTS_WITHOUT_CSMA_CA 0x01
typedef struct {
  uint8_t tx_opts; /* bitmap of HR_TX_OPTS_CSMA_CA, HR_TX_OPTS_ACK_REQ */
  uint8_t psdu_len;
  uint8_t psdu[1]; // Placeholder for PSDU. MPDU's FCS is computed by hw and MUST NOT be included.
} hr_data_req_t;

typedef struct {
  // The tranceiver RX state is controlled by the MIB attribute macRxOnWhenIdle.
  // so it's OK to issue a data request at any time. The transceiver takes care of turning the
  // receiver off and, if the case, turning it on at the end of transmission.
  // If the data request was issued with no tx_opts the only status possible is SUCCESS.
  // Otherwise, other than SUCCESS, the status can be either CHANNEL_ACCESS_FAILURE (HR_TX_OPTS_CSMA_CA set) or NO_ACK (HR_TX_OPTS_ACK_REQ set).
  uint8_t status;
} hr_data_cnf_t;

typedef struct {
  uint8_t timestamp[4];
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

/*
 * Copies a 32 bit unsigned integer to a byte array in network order
 * (little endian or, better, less significant first).
 */
#define U32TONA(s, d) do {\
    (d)[0] = (s) & 0xff;\
    (d)[1] = ((s) >> 8) & 0xff;\
    (d)[2] = ((s) >> 16) & 0xff;\
    (d)[3] = ((s) >> 24) & 0xff;\
  } while(0)

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define MAX_REPORT_ID 3

/*
 * Q: Why to use HID?
 * A: HID uses interrupt endpoints with guaranteed latency transfers (down to 1ms for USB 2.0).
 *    It is also driverless allowing to talk to a device from userspace using e.g. hidapi.
 * Q: Why to use multiple reports of different lengths instead of a single report of maximum length?
 * A: Simply to keep the transfer latency to a minimum. There are certain timings in the 802.15.4 spec
 *    that must be met. For example sleeping devices will extract data from the coordinator using
 *    a MAC data request command frame. The coordinator needs to respond to this request within 20ms.
 *    The PSDU containing the data request command frame fits in a report that can be transfered in
 *    just a single transaction (<= 1ms) to the host.
 *    Of course this is a best case scenario. If we were busy transfering a report of maximum length
 *    when the frame arrived, this has to wait for the current transfer to complete.
 *    As we will see the maximum length report neededed is less than 192 bytes and its transfer takes
 *    up to 3ms. Hopefully this worst case will not happen very often.
 *
 * We need to use multiple reports whose lengths may be greater than the EP size (64 bytes).
 * We could use multiple reports whose lengths are 64, 128 and 192 but this choice has a drawback.
 * As explained here:
 * http://www.microchip.com/forums/m485841.aspx
 * and here:
 * http://stackoverflow.com/questions/8145957/multiple-hid-input-reports
 * IN reports whose length is a multiple of the EP size (but the longest report) need to
 * be terminated by a Zero Lenght Packet (ZLP), requiring an extra transaction.
 * We then choose the lengths 63, 127, 192.
 * In this way the last transaction of the report is always 1 byte less than the EP size
 * and the transfer is complete.
 *
 * OUT reports does not need to follow the above rules and their lengths can be set to 64, 128, 192.
 *
 * For IN reports we choose the length of the report according to the following formula:
 * 
 * Report length = (report ID * 64) - 1 
 */

/* Report length = (report ID * 64) - 1 */
#define REPORT_LEN(id) (((uint16_t)(id) << 6) - 1)
#define MSG_DATA_LEN(id) (REPORT_LEN((id)) - (offsetof(hr_report_t, msg) + offsetof(hr_msg_t, data)))

#define MAX_REPORT_LEN REPORT_LEN(MAX_REPORT_ID)
#define MAX_MSG_DATA_LEN MSG_DATA_LEN(MAX_REPORT_ID)

#endif /* HIDRADIO_H */
