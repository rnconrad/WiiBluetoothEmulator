#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hci.h"
#include "l2cap.h"
#include "sdp.h"

#include "wiimote.h"

static int cmd_state = 0;
static uint16_t cmd_cid = 0;
static uint8_t cmd_id = 0;
static struct l2cap_connection * curr_connections;

void l2cap_init_connections(struct l2cap_connection connections[])
{
    //signaling channel (always active)
    connections[0].active = true;
    connections[0].channel = 0x0001;
    connections[0].recv_data = l2cap_recv_command;
    connections[0].get_data = l2cap_get_command;
    
    //sdp psm
    connections[1].active = false;
    connections[1].psm = 0x0001;
    connections[1].recv_data = sdp_recv_data;
    connections[1].get_data = sdp_get_data;
    
    //hid ctrl psm
    connections[2].active = false;
    connections[2].psm = 0x0011;
    connections[2].recv_data = _wiimote_recv_ctrl;
    connections[2].get_data = _wiimote_get_ctrl;
    
    //hid interrupt psm
    connections[3].active = false;
    connections[3].psm = 0x0013;
    connections[3].recv_data = _wiimote_recv_data;
    connections[3].get_data = _wiimote_get_data;
}

void l2cap_recv_command(uint8_t * buf, int32_t len)
{
    struct l2cap_command * command = (struct l2cap_command *)buf;
    int i = 0;
    
    switch (command->code)
    {
        case 0x02: //connection request
        {
            struct l2cap_connection_request * request = (struct l2cap_connection_request *)command->data;

            //set up the connection
            for (i = 0; i < NUM_L2CAP_CHANNELS; i++)
            {
                if (curr_connections[i].psm == request->psm)
                {
                    curr_connections[i].active = true;
                    curr_connections[i].channel = request->source_cid;
                    break;
                }
            }
            
            //data for response
            cmd_state = 0x02;
            cmd_cid = request->source_cid;
            cmd_id = command->identifier;
            
            break;
        }
        case 0x03: //connection response (not used)
            break;
        case 0x04: //configure request
        {
            struct l2cap_config_request * request = (struct l2cap_config_request *)command->data;
            
            cmd_state = 0x04;
            cmd_cid = request->dest_cid;
            cmd_id = command->identifier;
            
            break;
        }
        case 0x05: //configure response (received, but not used)
            break;
        case 0x06: //disconnection request
        {
            struct l2cap_disconnection_request * request = (struct l2cap_disconnection_request *)command->data;
            
            //set up the connection
            for (i = 0; i < NUM_L2CAP_CHANNELS; i++)
            {
                if (curr_connections[i].channel == request->dest_cid)
                {
                    curr_connections[i].active = false;
                    break;
                }
            }
         
            cmd_state = 0x06;
            cmd_cid = request->dest_cid;
            cmd_id = command->identifier;
            
            break;
        }
        case 0x07: //disconnection response (not used)
            break;
        default: //unknown or unimplemented
            break;
    }
}

int32_t l2cap_get_command(uint8_t * buf)
{
    int32_t len = 0;

    switch (cmd_state)
    {
        case 0x02:
        {
            struct l2cap_command * command = (struct l2cap_command *)buf;
            struct l2cap_connection_response * response = (struct l2cap_connection_response *)command->data;
            
            response->dest_cid = cmd_cid; //arbitrarily chosen
            response->source_cid = cmd_cid; //must match request
            response->result = 0x0000; //connection successful
            response->status = 0x0000; //no further information available
            
            len = sizeof(struct l2cap_connection_response);
            
            command->code = 0x03;
            command->identifier = cmd_id;
            command->length = len;
            
            len += sizeof(struct l2cap_command);
            cmd_state = 0;
            
            break;
        }
        case 0x04:
        {
            struct l2cap_command * command = (struct l2cap_command *)buf;
            struct l2cap_config_response * response = (struct l2cap_config_response *)command->data;
            
            //source_cid must match channel id originally paired with
            //it will because the dest_cid was arbitrarily chosen to match (above)
            response->source_cid = cmd_cid;
            response->flags = 0x0000; //none
            response->result = 0x0000; //success
            response->config[0] = 0x01; //copy the provided config
            response->config[1] = 0x02;
            response->config[2] = 0x00;
            response->config[3] = 0x01;
            response->config[4] = 0x02;
            response->config[5] = 0x02;
            response->config[6] = 0xFF;
            response->config[7] = 0xFF;
            
            len = sizeof(struct l2cap_config_response);
            
            command->code = 0x05;
            command->identifier = cmd_id;
            command->length = len;
            
            len += sizeof(struct l2cap_command);
            cmd_state = 0x05;
            
            break;
        }
        case 0x05:
        {
            struct l2cap_command * command = (struct l2cap_command *)buf;
            struct l2cap_config_request * request = (struct l2cap_config_request *)command->data;
            
            request->dest_cid = cmd_cid;
            request->flags = 0x0000; //none
            request->config[0] = 0x01;
            request->config[1] = 0x02;
            request->config[2] = 0xB9;
            request->config[3] = 0x00;
            
            len = sizeof(struct l2cap_config_request);
            
            command->code = 0x04;
            command->identifier = cmd_id + 1;
            command->length = len;
            
            len += sizeof(struct l2cap_command);
            cmd_state = 0x00;

            break;
        }
        case 0x06:
        {
            struct l2cap_command * command = (struct l2cap_command *)buf;
            struct l2cap_disconnection_response * response = (struct l2cap_disconnection_response *)command->data;
            
            response->dest_cid = cmd_cid; //must match request
            response->source_cid = cmd_cid; //must match request

            len = sizeof(struct l2cap_disconnection_response);
            
            command->code = 0x07;
            command->identifier = cmd_id;
            command->length = len;
            
            len += sizeof(struct l2cap_command);
            cmd_state = 0;
            
            break;
        }
    }
    
    return len;
}

void l2cap_recv_data(struct l2cap_connection connections[], uint8_t * buf, int32_t len)
{
    struct l2cap_header * header = (struct l2cap_header *)buf;
    int i = 0;
    
    for (i = 0; i < NUM_L2CAP_CHANNELS; i++)
    {
        if (connections[i].active && connections[i].channel == header->channel)
        {
            connections[i].recv_data(header->data, header->length);
        }
    }
}

int32_t l2cap_get_data(struct l2cap_connection connections[], uint8_t * buf)
{   
    //poll each channel in order (lower items have priority)
    //this shouldn't lead to any starvation because multiple channels are
    //rarely trying sending data simultaneously (if ever)
    
    struct l2cap_header * header = (struct l2cap_header *)buf;
    int32_t len = 0;
    int i = 0;
    
    curr_connections = connections;
    
    for (i = 0; i < NUM_L2CAP_CHANNELS; i++)
    {
        if (connections[i].active)
        {
            len = connections[i].get_data(header->data);
            if (len > 0)
            {
                header->length = len;
                header->channel = connections[i].channel;
                len += sizeof(struct l2cap_header);
                break;
            }
        }
    }
    
    return len;
}