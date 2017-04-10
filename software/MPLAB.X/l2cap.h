#ifndef L2CAP_H
#define	L2CAP_H

#ifdef	__cplusplus
extern "C" {
#endif

struct l2cap_header
{
  uint16_t length;
  uint16_t channel;
  uint8_t data[];
} __attribute__((packed));

struct l2cap_command
{
    uint8_t code;
    uint8_t identifier;
    uint16_t length;
    uint8_t data[];
} __attribute__((packed));

struct l2cap_connection_request
{
    uint16_t psm;
    uint16_t source_cid;
} __attribute__((packed));

struct l2cap_connection_response
{
    uint16_t dest_cid;
    uint16_t source_cid;
    uint16_t result;
    uint16_t status;
} __attribute__((packed));

struct l2cap_config_request
{
    uint16_t dest_cid;
    uint16_t flags;
    uint8_t config[4];
};

struct l2cap_config_response
{
    uint16_t source_cid;
    uint16_t flags;
    uint16_t result;
    uint8_t config[8];
};

struct l2cap_disconnection_request
{
    uint16_t dest_cid;
    uint16_t source_cid;
} __attribute__((packed));

struct l2cap_disconnection_response
{
    uint16_t dest_cid;
    uint16_t source_cid;
} __attribute__((packed));

struct l2cap_connection
{
    bool active;
    uint16_t channel;
    uint16_t psm;
    void (*recv_data)(uint8_t *, int32_t len);
    int32_t (*get_data)(uint8_t *);
};

#define NUM_L2CAP_CHANNELS 4

void l2cap_init_connections(struct l2cap_connection connections[]);

void l2cap_recv_command(uint8_t * buf, int32_t len);
int32_t l2cap_get_command(uint8_t * buf);

void l2cap_recv_data(struct l2cap_connection connections[], uint8_t * buf, int32_t len);
int32_t l2cap_get_data(struct l2cap_connection connections[], uint8_t * buf);

#ifdef	__cplusplus
}
#endif

#endif	/* L2CAP_H */
