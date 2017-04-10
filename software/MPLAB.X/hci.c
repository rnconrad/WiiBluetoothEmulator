#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> //memcpy
#include "hci.h"

#include "wiimote.h"

uint8_t host_ready = 0;

static uint16_t data_packets_flushed = 0;

static uint16_t pending_evt = 0x0FF;
static uint8_t evt_buffer[257];
static int32_t evt_length = 0;
static int32_t evt_offset = 0;

static uint8_t cmd_buffer[64];
static int32_t cmd_length = 0;

struct hci_connection hci_connections[MAX_HCI_CONNECTIONS];
static int process_conn = 0;

void hci_reset()
{
    int i = 0;
    for (i = 0; i < MAX_HCI_CONNECTIONS; i++)
    {
        hci_connections[i].active = true;
        hci_connections[i].handle = 0x000B;
        hci_connections[i].l2cap_recv_len = 0;
        hci_connections[i].l2cap_send_len = 0;
        hci_connections[i].l2cap_send_offset = 0;
        
        l2cap_init_connections(hci_connections[i].connections);
    }
    
    init_wiimote(&wiimote);
}

void hci_send_evt(int32_t len)
{
    evt_length = len;
    evt_offset = 0;
}

void hci_evt_sent()
{
  struct hci_evt * result = (struct hci_evt *)evt_buffer;

  switch (pending_evt)
  {
    case 0x003: //0x14: //Mode Change
      result->code = 0x14;
      result->param_len = 6;

      //status
      result->data[0] = 0x00;

      //connection handle
      result->data[1] = 0x0B;
      result->data[2] = 0x00;

      result->data[3] = 0x02;
      result->data[4] = 0x08;
      result->data[5] = 0x00;

      hci_send_evt(8);
      break;
    case 0x005: //0x03: //Connection Complete
      result->code = 0x03;
      result->param_len = 11;

      //status
      result->data[0] = 0x00;

      //new connection handle
      result->data[1] = 0x0B;
      result->data[2] = 0x00;

      //remote bdaddr
      result->data[3] = 0x3D;
      result->data[4] = 0x34;
      result->data[5] = 0x8A;
      result->data[6] = 0x32;
      result->data[7] = 0x1F;
      result->data[8] = 0x00;

      //link type (ACL)
      result->data[9] = 0x01;

      //encryption enabled (no)
      result->data[10] = 0x0;

      hci_send_evt(13);
      break;
    case 0x00C: //0x16: //PIN Code Request
      result->code = 0x16;
      result->param_len = 6;

      //remote bdaddr
      result->data[0] = 0x3D;
      result->data[1] = 0x34;
      result->data[2] = 0x8A;
      result->data[3] = 0x32;
      result->data[4] = 0x1F;
      result->data[5] = 0x00;
      
      hci_send_evt(8);
      break;
    case 0x00D: //0x18: //Link Key Notification
      result->code = 0x18;
      result->param_len = 23;

      //remote bdaddr
      result->data[0] = 0x3D;
      result->data[1] = 0x34;
      result->data[2] = 0x8A;
      result->data[3] = 0x32;
      result->data[4] = 0x1F;
      result->data[5] = 0x00;
      
      result->data[6] = 0xBE;
      result->data[7] = 0x12;
      result->data[8] = 0x3C;
      result->data[9] = 0xB6;
      result->data[10] = 0x8D;
      result->data[11] = 0x11;
      result->data[12] = 0xF2;
      result->data[13] = 0x80;
      result->data[14] = 0x3F;
      result->data[15] = 0x1B;
      result->data[16] = 0x2C;
      result->data[17] = 0x11;
      result->data[18] = 0x1F;
      result->data[19] = 0x77;
      result->data[20] = 0xBB;
      result->data[21] = 0x8F;
      result->data[22] = 0x00;
      
      hci_send_evt(25);
      pending_evt = 0x10D;
      return;
    case 0x10D: //0x06: //Authentication Complete
      result->code = 0x06;
      result->param_len = 3;

      //status
      result->data[0] = 0x00;
      
      //connection handle
      result->data[1] = 0x0B;
      result->data[2] = 0x00;
      
      hci_send_evt(5);
      break;
    case 0x011: //0x17: //Link Key Request
      result->code = 0x17;
      result->param_len = 6;

      //remote bdaddr
      result->data[0] = 0x3D;
      result->data[1] = 0x34;
      result->data[2] = 0x8A;
      result->data[3] = 0x32;
      result->data[4] = 0x1F;
      result->data[5] = 0x00;
      
      hci_send_evt(8);
      break;
    case 0x019: //0x07: //Remote Name Request Complete
      result->code = 0x07;
      result->param_len = 255;

      //status
      result->data[0] = 0x00;

      //remote bdaddr
      result->data[1] = 0x3D;
      result->data[2] = 0x34;
      result->data[3] = 0x8A;
      result->data[4] = 0x32;
      result->data[5] = 0x1F;
      result->data[6] = 0x00;

      //remote name
      result->data[7] = 'N';
      result->data[8] = 'i';
      result->data[9] = 'n';
      result->data[10] = 't';
      result->data[11] = 'e';
      result->data[12] = 'n';
      result->data[13] = 'd';
      result->data[14] = 'o';
      result->data[15] = ' ';
      result->data[16] = 'R';
      result->data[17] = 'V';
      result->data[18] = 'L';
      result->data[19] = '-';
      result->data[20] = 'C';
      result->data[21] = 'N';
      result->data[22] = 'T';
      result->data[23] = '-';
      result->data[24] = '0';
      result->data[25] = '1';
      result->data[26] = 0x0;

      hci_send_evt(257);
      break;

    case 0x01B: //0x0B: //Read Remote Supported Features Complete
      result->code = 0x0B;
      result->param_len = 11;

      //status
      result->data[0] = 0x00;

      //connection handle
      result->data[1] = 0x0B;
      result->data[2] = 0x00;

      //features
      result->data[3] = 0xBC;
      result->data[4] = 0x02;
      result->data[5] = 0x04;
      result->data[6] = 0x38;
      result->data[7] = 0x08;
      result->data[8] = 0x00;
      result->data[9] = 0x00;
      result->data[10] = 0x00;

      hci_send_evt(13);
      break;

    case 0x01D: //0x0C: //Read Remote Version Information Complete
      result->code = 0x0C;
      result->param_len = 8;

      //status
      result->data[0] = 0x00;

      //connection handle
      result->data[1] = 0x0B;
      result->data[2] = 0x00;

      //version
      result->data[3] = 0x03;

      //manufacturer name
      result->data[4] = 0x0F;
      result->data[5] = 0x00;

      //subversion
      result->data[6] = 0x1C;
      result->data[7] = 0x03;

      hci_send_evt(10);
      break;

    case 0x01F: //0x1C: //Read Clock Offset Complete
      result->code = 0x1C;
      result->param_len = 5;

      //status
      result->data[0] = 0x00;

      //connection handle
      result->data[1] = 0x0B;
      result->data[2] = 0x00;

      //clock offset
      result->data[3] = 0xE9;
      result->data[4] = 0x43;

      hci_send_evt(7);
      break;

    case 0x01: //0x22: //Inqiry Result with RSSI
      result->code = 0x22;
      result->param_len = 15;

      //num responses
      result->data[0] = 0x01;

      //remote bdaddr
      result->data[1] = 0x3D;
      result->data[2] = 0x34;
      result->data[3] = 0x8A;
      result->data[4] = 0x32;
      result->data[5] = 0x1F;
      result->data[6] = 0x00;

      //page scan repetition mode
      result->data[7] = 0x01;

      //reserved
      result->data[8] = 0x00;

      //remote class
      result->data[9] = 0x04;
      result->data[10] = 0x25;
      result->data[11] = 0x00;

      //clock offset
      result->data[12] = 0xEA;
      result->data[13] = 0x43;

      //rssi
      result->data[14] = 0xBF;

      hci_send_evt(17);
      break;
    case 0xFF: //Sync button press
      result->code = 0xFF; //command complete
      result->param_len = 1;
      result->data[0] = 0x08;

      hci_send_evt(3);
      break;
  }

  pending_evt = 0x00;
}

/*
 * HCI command packet received via a USB class request
 */
void hci_process_cmd()
{
  struct hci_cmd * header = (struct hci_cmd *)cmd_buffer;
  uint16_t opcode = header->opcode;
  uint16_t ogf = opcode >> 10;
  uint16_t ocf = opcode & 0x3FF;

  struct hci_evt * result = (struct hci_evt *)evt_buffer;
  struct hci_evt_cmdcomplete * cmdcomplete = (struct hci_evt_cmdcomplete *)(result->data);
  struct hci_evt_cmdstatus * cmdstatus = (struct hci_evt_cmdstatus *)(result->data);

  switch (ogf)
  {
    case 0x01: //link control commands
      switch (ocf)
      {
        case 0x0001: //HCI_Inquiry
        case 0x0005: //HCI_Create_Connection
        case 0x0011: //HCI_Authentication_Requested
        case 0x0019: //HCI_Remote_Name_Request
        case 0x001B: //HCI_Read_Remote_Supported_Features
        case 0x001D: //HCI_Read_Remote_Version_Information
        case 0x001F: //HCI_Read_Clock_Offset
          result->code = 0x0F; //command status
          result->param_len = 4;

          cmdstatus->status = 0x0;
          cmdstatus->allowed_pkts = 1;
          cmdstatus->opcode = opcode;

          //schedule follow up event
          pending_evt = ocf;

          hci_send_evt(6);
          break;
        case 0x000C: //HCI_Link_Key_Request_Negative_Reply
        case 0x000D: //HCI_Pin_Code_Request_Reply
          result->code = 0x0E; //command complete
          result->param_len = 10;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status
          cmdcomplete->data[1] = 0x3D; //status
          cmdcomplete->data[2] = 0x34; //status
          cmdcomplete->data[3] = 0x8A; //status
          cmdcomplete->data[4] = 0x32; //status
          cmdcomplete->data[5] = 0x1F; //status
          cmdcomplete->data[6] = 0x00; //status

          //schedule follow up event
          pending_evt = ocf;

          hci_send_evt(12);
          break;
        case 0x000F: //HCI_Change_Connection_Packet_Type

          //wii sends invalid params, so just send an error (not sure why)

          result->code = 0x0F; //command status
          result->param_len = 4;

          cmdstatus->status = 0x12;
          cmdstatus->allowed_pkts = 1;
          cmdstatus->opcode = opcode;

          //schedule follow up event
          pending_evt = ocf;

          hci_send_evt(6);
          break;

        case 0x0002: //HCI_Inquiry_Cancel
          result->code = 0x0E; //command complete
          result->param_len = 4;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          hci_send_evt(6);
          break;
      }
      break;

    case 0x02: //link policy commands
      switch (ocf)
      {
        case 0x0003: //HCI_Sniff_Mode
          result->code = 0x0F; //command status
          result->param_len = 4;

          cmdstatus->status = 0x0;
          cmdstatus->allowed_pkts = 1;
          cmdstatus->opcode = opcode;

          //schedule follow up event
          pending_evt = ocf;

          hci_send_evt(6);
          break;
        case 0x000d: //HCI_Write_Link_Policy_Settings
          result->code = 0x0E; //command complete
          result->param_len = 6;

          cmdcomplete->data[0] = 0x00; //status

          //connection handle -- needs to be copied before it is overwritten
          cmdcomplete->data[1] = header->data[0];
          cmdcomplete->data[2] = header->data[1];

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          hci_send_evt(8);
          break;

        //unimplemented commands -- just send command complete for now

        case 0x000f: //HCI_Write_Default_Link_Policy_Settings
          result->code = 0x0E; //command complete
          result->param_len = 4;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          hci_send_evt(6);
          break;
      }
      break;

    case 0x03: //host controller & baseband commands
      switch (ocf)
      {
        case 0x0003: //HCI_Reset
          hci_reset();

          result->code = 0x0E; //command complete
          result->param_len = 4;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          hci_send_evt(6);
          break;
        case 0x000D: //HCI_Read_Stored_Link_Key
          result->code = 0x0E; //command complete
          result->param_len = 8;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //max num keys
          result->data[4] = 0x10;
          result->data[5] = 0x00;

          //number of link keys read
          result->data[6] = 0x00;
          result->data[7] = 0x00;

          hci_send_evt(10);
          break;
        case 0x0011: //HCI_Write_Stored_Link_Key
          result->code = 0x0E; //command complete
          result->param_len = 5;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          //status
          cmdcomplete->data[0] = 0x00;

          //number of keys written
          cmdcomplete->data[1] = 0x01;

          hci_send_evt(7);
          break;
        case 0x0012: //HCI_Delete_Stored_Link_Key
          result->code = 0x0E; //command complete
          result->param_len = 6;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //number of link keys deleted
          result->data[4] = 0x00;
          result->data[5] = 0x00;

          hci_send_evt(8);
          break;
        case 0x0014: //HCI_Read_Local_Name
          result->code = 0x0E; //command complete
          result->param_len = 252;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          result->data[4] = 'B';
          result->data[5] = 'C';
          result->data[6] = 'M';
          result->data[7] = '2';
          result->data[8] = '0';
          result->data[9] = '4';
          result->data[10] = '5';
          result->data[11] = 'B';
          result->data[12] = '2';
          result->data[13] = ' ';
          result->data[14] = 'R';
          result->data[15] = 'O';
          result->data[16] = 'M';
          result->data[17] = ' ';
          result->data[18] = '+';
          result->data[19] = ' ';
          result->data[20] = 'E';
          result->data[21] = 'E';
          result->data[22] = 'P';
          result->data[23] = 'R';
          result->data[24] = 'O';
          result->data[25] = 'M';
          result->data[26] = 0x0;

          hci_send_evt(254);
          break;
        case 0x001a: //HCI_Write_Scan_Enable
          if (header->data[0] == 0x02)
          {
            //this is the last command in init
            host_ready = 1;
          }

          result->code = 0x0E; //command complete
          result->param_len = 4;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          hci_send_evt(6);
          break;
        case 0x001b: //HCI_Read_Page_Scan_Activity
          result->code = 0x0E; //command complete
          result->param_len = 8;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //device class
          result->data[4] = 0x00;
          result->data[5] = 0x01;
          result->data[6] = 0x2c;
          result->data[7] = 0x00;

          hci_send_evt(10);
          break;
        case 0x0023: //HCI_Read_Class_of_Device
          result->code = 0x0E; //command complete
          result->param_len = 7;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //device class
          result->data[4] = 0x00;
          result->data[5] = 0x00;
          result->data[6] = 0x00;

          hci_send_evt(9);
          break;
        case 0x0025: //HCI_Read_Voice_Setting
          result->code = 0x0E; //command complete
          result->param_len = 6;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //voice setting (initial value copied from BCM2045A)
          result->data[4] = 0x60;
          result->data[5] = 0x00;

          hci_send_evt(8);
          break;
        case 0x0037: //HCI_Write_Link_Supervision_Timeout
          result->code = 0x0E; //command complete
          result->param_len = 6;

          cmdcomplete->data[0] = 0x00; //status

          //connection handle -- needs to be copied before it is overwritten
          cmdcomplete->data[1] = header->data[0];
          cmdcomplete->data[2] = header->data[1];

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          hci_send_evt(8);
          break;
        case 0x0038: //HCI_Read_Number_Of_Supported_IAC
          result->code = 0x0E; //command complete
          result->param_len = 5;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //inquiry access codes supported (copied from BCM2045A)
          result->data[4] = 0x01;

          hci_send_evt(7);
          break;
        case 0x0039: //HCI_Read_Current_IAC_LAP
          result->code = 0x0E; //command complete
          result->param_len = 8;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //num current IAC, default GIAC
          result->data[4] = 0x01;
          result->data[5] = 0x33;
          result->data[6] = 0x8b;
          result->data[7] = 0x9e;

          hci_send_evt(10);
          break;
        case 0x0046: //HCI_Read_Page_Scan_Type
          result->code = 0x0E; //command complete
          result->param_len = 5;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //interlaced scan (default on BCM2045A)
          result->data[4] = 0x01;

          hci_send_evt(7);
          break;

        //unimplemented commands -- just send command complete for now

        case 0x0001: //HCI_Set_Event_Mask
        case 0x0005: //HCI_Set_Event_Filter
        case 0x000a: //HCI_Write_PIN_Type
        case 0x0013: //HCI_Write_Local_Name
        case 0x0016: //HCI_Write_Connection_Accept_Timeout
        case 0x0018: //HCI_Write_Page_Timeout
        case 0x001c: //HCI_Write_Page_Scan_Activity
        case 0x0024: //HCI_Write_Class_of_Device
        case 0x0033: //HCI_Host_Buffer_Size
        case 0x0045: //HCI_Write_Inquiry_Mode
        case 0x0043: //HCI_Write_Inquiry_Scan_Type
        case 0x0047: //HCI_Write_Page_Scan_Type
          result->code = 0x0E; //command complete
          result->param_len = 4;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          hci_send_evt(6);
          break;
      }
      break;

    case 0x04: //informational parameters
      switch (ocf)
      {
        case 0x0001: //HCI_Read_Local_Version_Information
          result->code = 0x0E; //command complete
          result->param_len = 12;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //version info (copied from BCM2045A)
          result->data[4] = 0x03;
          result->data[5] = 0xa7;
          result->data[6] = 0x40;
          result->data[7] = 0x03;
          result->data[8] = 0x0f;
          result->data[9] = 0x00;
          result->data[10] = 0x0e;
          result->data[11] = 0x43;

          hci_send_evt(14);
          break;
        case 0x0002: //HCI_Read_Local_Supported_Commands
          result->code = 0x0E; //command complete
          result->param_len = 68;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //commands list (copied from BCM2045A)
          result->data[4] = 0xff;
          result->data[5] = 0xff;
          result->data[6] = 0xff;
          result->data[7] = 0x03;
          result->data[8] = 0xfe;
          result->data[9] = 0xff;
          result->data[10] = 0xcf;
          result->data[11] = 0xff;
          result->data[12] = 0xff;
          result->data[13] = 0xff;
          result->data[14] = 0xff;
          result->data[15] = 0x1f;
          result->data[16] = 0xf2;
          result->data[17] = 0x0f;
          result->data[18] = 0xf8;
          result->data[19] = 0xff;
          result->data[20] = 0x3f;
          //rest 0...
          //hacky, for now just the ones known to be non-zero
          result->data[21] = 0;
          result->data[22] = 0;
          result->data[23] = 0;
          result->data[24] = 0;
          result->data[25] = 0;

          hci_send_evt(70);
          break;
        case 0x0003: //HCI_Read_Local_Supported_Features
          result->code = 0x0E; //command complete
          result->param_len = 12;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //features list (copied from BCM2045A)
          result->data[4] = 0xff;
          result->data[5] = 0xff;
          result->data[6] = 0x8d;
          result->data[7] = 0xfe;
          result->data[8] = 0x9b;
          result->data[9] = 0xf9;
          result->data[10] = 0x00;
          result->data[11] = 0x80;

          hci_send_evt(14);
          break;
        case 0x0004: //HCI_Read_Local_Extended_Features
          result->code = 0x0E; //command complete
          result->param_len = 14;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //extended features list (copied from BCM2045A)
          result->data[4] = 0x01;
          result->data[5] = 0x00;
          result->data[6] = 0x00;
          result->data[7] = 0x00;
          result->data[8] = 0x00;
          result->data[9] = 0x00;
          result->data[10] = 0x00;
          result->data[11] = 0x00;
          result->data[12] = 0x00;
          result->data[13] = 0x00;

          hci_send_evt(16);
          break;
        case 0x0005: //HCI_Read_Buffer_Size
          result->code = 0x0E; //command complete
          result->param_len = 11;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //buffer sizes (copied from BCM2045A)
          result->data[4] = 0x53;
          result->data[5] = 0x01;
          result->data[6] = 0x40;
          result->data[7] = 0x0a;
          result->data[8] = 0x00;
          result->data[9] = 0x00;
          result->data[10] = 0x00;

          hci_send_evt(13);
          break;
        case 0x0009: //HCI_Read_BD_ADDR
          result->code = 0x0E; //command complete
          result->param_len = 10;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          //bdaddr (just a typical Wii BDADDR)
          result->data[4] = 0x65;
          result->data[5] = 0xe6;
          result->data[6] = 0xb8;
          result->data[7] = 0x44;
          result->data[8] = 0x24;
          result->data[9] = 0x00;

          hci_send_evt(12);
          break;
      }
      break;

    case 0x3f: //vendor specific
      switch (ocf)
      {
        case 0x004c: //???
        case 0x004f: //???

          result->code = 0x0E; //command complete
          result->param_len = 4;

          cmdcomplete->allowed_pkts = 1;
          cmdcomplete->opcode = opcode;

          cmdcomplete->data[0] = 0x00; //status

          hci_send_evt(6);
          break;
      }
      break;

    case 0x05: //status parameters
    case 0x06: //testing commands
    default: //unknown
      break;
  }

}

/*
 * Commands are received by passing cmd_buffer to the USB functions when
 * requested. Once the buffer is filled with valid data, its length will be
 * passed to hci_recv_command.
 */
uint8_t * hci_get_cmd_buffer()
{
    return cmd_buffer;
}

void hci_recv_command(int32_t len)
{
    cmd_length = len;
}

int32_t hci_get_event(uint8_t * buf)
{
    int32_t len = 0;
    
    if (evt_length - evt_offset > 0)
    {
        //there is buffered data to send
        len = evt_length - evt_offset;
        if (len > 16) len = 16;

        memcpy(buf, evt_buffer + evt_offset, len);

        evt_offset += len;
    }
    else if (pending_evt != 0x00 && host_ready)
    {
        hci_evt_sent();
    }
    else if (data_packets_flushed >= 2)
    {
        //Need to indicate packets being flushed
        //TODO: This is duct taped together to just barely work.
        //Make it work for more scenarios.

        struct hci_evt * result = (struct hci_evt *)buf;
        
        result->code = 0x13;
        result->param_len = 5;
        
        //number of connections included
        result->data[0] = 0x01;
        
        //connection handles
        result->data[1] = 0x0B;
        result->data[2] = 0x00;
        
        //number of packets received
        result->data[3] = data_packets_flushed & 0xFF;
        result->data[4] = 0x00;
        
        len = 7;
        data_packets_flushed = 0;
    }
    else if (cmd_length > 0)
    {
        /*
         * Process next command only after all events for a given command are
         * sent. Note that the host will only send one outstanding HCI command
         * at a time. This simplifies things and saves memory.
         */

        hci_process_cmd();
        cmd_length = 0;
    }
    
    return len;
}

/*
 * HCI data packet received on the USB bulk endpoint
 * This is called from an interrupt handler and is not meant to do much other
 * than copy the received data to the appropriate place for processing.
 */
void hci_recv_data(uint8_t * buf, int32_t len)
{
    struct hci_acl * header = (struct hci_acl *)buf;
    int i = 0;
    
    //search for the connection to pass incoming data to
    for (i = 0; i < MAX_HCI_CONNECTIONS; i++)
    {
        if (hci_connections[i].handle == header->handle)
        {
            //note: it is possible for l2cap data to be sent across multiple
            //HCI transfers, but in this case it is not known to happen

            memcpy(hci_connections[i].l2cap_recv, header->data, header->data_len);
            hci_connections[i].l2cap_recv_len = header->data_len;
            break;
        }
    }
}

int32_t hci_get_data(uint8_t * buf)
{
    int32_t len = 0;
    struct hci_connection * conn = &hci_connections[process_conn];
    
    if (conn->l2cap_recv_len > 0)
    {
        //deliver received data if there is any
        
        l2cap_recv_data(conn->connections, conn->l2cap_recv, conn->l2cap_recv_len);
        conn->l2cap_recv_len = 0;
        data_packets_flushed++;
    }
    
    if (conn->l2cap_send_len - conn->l2cap_send_offset <= 0)
    {
        //provide opportunity to generate data if there is none buffered
        
        conn->l2cap_send_len = l2cap_get_data(conn->connections, conn->l2cap_send);
        
        if (conn->l2cap_send_len > 0)
        {
            conn->l2cap_send_offset = 0;
        }
    }
    
    if (conn->l2cap_send_len - conn->l2cap_send_offset > 0)
    {
        //there is buffered data to send
        
        struct hci_acl * header = (struct hci_acl *)buf;
        
        header->broadcast = 0;
        header->packet_boundary = (conn->l2cap_send_offset > 0) ? 1 : 2;
        header->handle = conn->handle;
        
        len = conn->l2cap_send_len - conn->l2cap_send_offset;
        if (len > 27) len = 27;
        header->data_len = len;

        memcpy(header->data, conn->l2cap_send + conn->l2cap_send_offset, len);

        conn->l2cap_send_offset += len;
        len += sizeof(struct hci_acl);
    }
    
    //simple round robin processing
    process_conn = (process_conn + 1) % MAX_HCI_CONNECTIONS;
    
    return len;
}