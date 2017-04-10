#ifndef SDP_H
#define	SDP_H

#ifdef	__cplusplus
extern "C" {
#endif


struct sdp_pdu
{
    uint8_t pdu_id;
    uint16_t transaction_id; //big endian
    uint8_t data[];
} __attribute__((packed));

void sdp_recv_data(uint8_t * buf, int32_t len);
int32_t sdp_get_data(uint8_t * buf);

#ifdef	__cplusplus
}
#endif

#endif	/* SDP_H */
