#ifndef HCI_H
#define	HCI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "l2cap.h"

struct hci_evt
{
  uint8_t code;
  uint8_t param_len;
  uint8_t data[];
} __attribute__((packed));

struct hci_evt_cmdcomplete
{
  uint8_t allowed_pkts;
  uint16_t opcode;
  uint8_t data[];
} __attribute__((packed));

struct hci_evt_cmdstatus
{
  uint8_t status;
  uint8_t allowed_pkts;
  uint16_t opcode;
} __attribute__((packed));

struct hci_cmd
{
  uint16_t opcode;  //little endian
  uint8_t param_len;
  uint8_t data[61];
} __attribute__((packed));

struct hci_acl
{
  uint16_t handle : 12;  //little endian
  uint8_t packet_boundary : 2;
  uint8_t broadcast : 2;

  uint16_t data_len;

  uint8_t data[];
} __attribute__((packed));

struct hci_connection
{
    bool active;

    uint16_t handle;

    uint8_t l2cap_recv[64];
    int32_t l2cap_recv_len;
    uint8_t l2cap_send[132];
    int32_t l2cap_send_len;
    int32_t l2cap_send_offset;

    struct l2cap_connection connections[NUM_L2CAP_CHANNELS];
};

#define MAX_HCI_CONNECTIONS 1

void hci_reset();

uint8_t * hci_get_cmd_buffer();
void hci_recv_command(int32_t len);
int32_t hci_get_event(uint8_t * buf);

void hci_recv_data(uint8_t * buf, int32_t len);
int32_t hci_get_data(uint8_t * buf);

#ifdef	__cplusplus
}
#endif

#endif	/* HCI_H */
