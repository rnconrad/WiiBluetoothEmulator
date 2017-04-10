#include "wiimote.h"
#include "wm_reports.h"
#include "wm_crypto.h"
#include "wm_eeprom.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/endian.h>

#include <cp0defs.h>
#include <xc.h>

#define CPU_SPEED 48000000UL //48Mhz
#define US_SCALE (CPU_SPEED/2000000)

int tries = 0;
unsigned long last_report;

struct wiimote_state wiimote;

void _wiimote_recv_ctrl(const uint8_t * buf, int len)
{
    return;
}

int32_t _wiimote_get_ctrl(uint8_t * buf)
{
    return 0;
}

void _wiimote_recv_data(const uint8_t * buf, int len)
{
    wiimote_recv_report(&wiimote, buf, len);
}

int32_t _wiimote_get_data(uint8_t * buf)
{
    return wiimote_get_report(&wiimote, buf);
}

int wiimote_recv_report(struct wiimote_state * state, const uint8_t * buf, int len)
{
  struct report_data * data = (struct report_data *)buf;

  //every output report contains rumble info
  state->sys.rumble = data->buf[0] & 0x01;

  switch (data->type)
  {
    case 0x11: //player LEDs
    {
      struct report_leds * rpt = (struct report_leds *)data->buf;

      state->sys.led_1 = rpt->led_1;
      state->sys.led_2 = rpt->led_2;
      state->sys.led_3 = rpt->led_3;
      state->sys.led_4 = rpt->led_4;

      report_queue_push_ack(state, data->type, 0x00);
      break;
    }
    case 0x12: //data reporting mode
    {
      struct report_mode * rpt = (struct report_mode *)data->buf;

      state->sys.reporting_continuous = rpt->continuous;
      state->sys.reporting_mode = rpt->mode;

      report_queue_push_ack(state, data->type, 0x00);
      break;
    }
    case 0x13:
    case 0x1a: //ir camera enable
    {
      struct report_ir_enable * rpt = (struct report_ir_enable *)data->buf;

      state->sys.ircam_enabled = rpt->enabled;

      report_queue_push_ack(state, data->type, 0x00);
      break;
    }
    case 0x14:
    case 0x19: //speaker enable
    {
      struct report_speaker_enable * rpt = (struct report_speaker_enable *)data->buf;

      state->sys.speaker_enabled = !rpt->muted;

      report_queue_push_ack(state, data->type, 0x00);
      break;
    }
    case 0x15: //status information request
      //send a status report 0x20
      report_queue_push_status(state);
      break;
    case 0x16: //write memory
    {
      struct report_mem_write * rpt = (struct report_mem_write *)data->buf;

      if (rpt->source0 || rpt->source1)
      {
        write_register(state, ntohl(OFFSET24(rpt->offset)), rpt->size, rpt->data);
      }
      else
      {
        write_eeprom(state, ntohl(OFFSET24(rpt->offset)), rpt->size, rpt->data);
      }
      break;
    }
    case 0x17: //read memory
    {
      struct report_mem_read * rpt = (struct report_mem_read *)data->buf;

      if (rpt->source0 || rpt->source1)
      {
        read_register(state, ntohl(OFFSET24(rpt->offset)), ntohs(rpt->size));
      }
      else
      {
        read_eeprom(state, ntohl(OFFSET24(rpt->offset)), ntohs(rpt->size));
      }

      break;
    }
  }

  return 0;
}

int wiimote_get_report(struct wiimote_state * state, uint8_t * buf)
{
  if (_CP0_GET_COUNT() - last_report < US_SCALE * 5500)
  {
    return 0;
  }

  int len;

  struct report_data * data = (struct report_data *)buf;
  uint8_t * contents;

  if (!state->sys.reporting_continuous && !state->sys.report_changed && state->sys.queue == NULL)
    return 0;

  if (state->sys.queue == NULL)
  {
    //regular report
    memset(data, 0, sizeof(struct report_data));
    len = 2;
    data->io = 0xa1;
    data->type = state->sys.reporting_mode;
  }
  else
  {
    //queued report (acknowledgement, response, etc)
    struct report * rpt;
    rpt = report_queue_peek(state);
    len = rpt->len;
    memcpy(data, &rpt->data, sizeof(struct report_data));
    report_queue_pop(state);
  }

  contents = data->buf;

  //fill report
  switch (data->type)
  {
    case 0x30: // core buttons
      report_append_buttons(state, contents);
      len += 2;
      break;
    case 0x31: // core buttons + accelerometer
      report_append_buttons(state, contents);
      report_append_accelerometer(state, contents);
      len += 2 + 3;
      break;
    case 0x32: // core buttons + 8 extension bytes
      report_append_buttons(state, contents);
      len += 2 + 8;
      break;
    case 0x33: // core buttons + accelerometer + 12 ir bytes
      report_append_buttons(state, contents);
      report_append_accelerometer(state, contents);
      report_append_ir_12(state, contents + 5);
      len += 2 + 3 + 12;
      break;
    case 0x34: // core buttons + 19 extension bytes
      report_append_buttons(state, contents);
      report_append_extension(state, contents + 2, 19);
      len += 2 + 19;
      break;
    case 0x35: // core buttons + accelerometer + 16 extension bytes
      report_append_buttons(state, contents);
      report_append_accelerometer(state, contents);
      report_append_extension(state, contents + 5, 16);
      len += 2 + 3 + 16;
      break;
    case 0x36: // core buttons + 10 ir bytes + 9 extension bytes
      report_append_buttons(state, contents);
      report_append_ir_10(state, contents + 2);
      report_append_extension(state, contents + 12, 9);
      len += 2 + 10 + 9;
      break;
    case 0x37: // core buttons + accelerometer + 10 ir bytes + 6 extension bytes
      report_append_buttons(state, contents);
      report_append_accelerometer(state, contents);
      report_append_ir_10(state, contents + 5);
      report_append_extension(state, contents + 15, 6);
      len += 2 + 3 + 10 + 6;
      break;
    case 0x3d: // 21 extension bytes
      report_append_extension(state, contents, 21);
      len += 21;
      break;
    case 0x3e: // interleaved core buttons + accelerometer with 36 ir bytes pt I
    case 0x3f: // interleaved core buttons + accelerometer with 36 ir bytes pt II
      report_append_buttons(state, contents);
      report_append_interleaved(state, contents);
      len += 21;
      break;
    default: // special output report (acknowledgement, status, or memory read)
      report_append_buttons(state, contents);
      break;
  }

  last_report = _CP0_GET_COUNT();
  return len;
}

void ir_object_clear(struct wiimote_state * state, uint8_t num)
{
  memset(&(state->usr.ir_object[num]), 0xff, sizeof(struct wiimote_ir_object));
}

void read_eeprom(struct wiimote_state * state, uint32_t offset, uint16_t size)
{
  const uint8_t * buffer;
  struct report * rpt;
  int i;

  //addresses greater than 0x16FF cannot be read or written
  if (offset + size > 0x16FF)
  {
    rpt = report_queue_push(state);
    report_format_mem_resp(rpt, 0xf, 0x8, offset, NULL);
    return;
  }

  buffer = wiimote_eeprom + offset + 0x70;

  //equivalent to ceil(size / 0x10)
  int totalpackets = (size + 0x10 - 1) / 0x10;

  //allocate all the needed reports
  for (i=0; i<totalpackets; i++)
  {
    report_queue_push(state);
  }

  //copy packet data
  struct queued_report * currentrpt = state->sys.queue;

  for (i=0; i<totalpackets; i++)
  {
    rpt = &currentrpt->rpt;
    int psize = (i == totalpackets-1) ? (size % 0x10) : 0xf;
    report_format_mem_resp(rpt, psize, 0x0, offset + i*0x10, &buffer[i*0x10]);
    currentrpt = currentrpt->next;
  }
}

void write_eeprom(struct wiimote_state * state, uint32_t offset, uint8_t size, const uint8_t * buf)
{
//  FILE * file;
  struct report * rpt;

  //Open file
//  file = fopen("eeprom.bin", "rb");
//  if (!file)
//  {
//    printf("Unable to open eeprom file");
//    return;
//  }

  //addresses greater than 0x16FF cannot be read or written
  if (offset + size > 0x16FF)
  {
    rpt = report_queue_push(state);
    report_format_mem_resp(rpt, 0xf, 0x8, offset, NULL);
//    fclose(file);
    return;
  }

  //start reading at the memory offset (0x70 is the physical address at 0x00 virtual)
//  fseek(file, offset + 0x70, SEEK_SET);

  //write buffer into the eeprom file
//  fwrite(buf, 1, size, file);
//  fclose(file);

  report_queue_push_ack(state, 0x16, 0x00);
}

void read_register(struct wiimote_state *state, uint32_t offset, uint16_t size)
{
  uint8_t * buffer;
  struct report * rpt;
  int i;

  switch ((offset >> 16) & 0xfe) //select register, ignore lsb
  {
    case 0xa2: //speaker
      buffer = register_a2 + (offset & 0xff);
      break;
    case 0xa4: //extension
      if (state->sys.wmp_state == 1)
      {
        //i guess this isn't needed after all
        //^^this is an old comment, so is this needed or not?
        if (((offset & 0xff) == 0xf6) || ((offset & 0xff) == 0xf7))
        {
          tries += 1;
          printf("%d \n", tries);
          if (tries == 5)
          {
            register_a6[0xf7] = 0x0e;
          }
        }

        buffer = register_a6 + (offset & 0xff);
      }
      else
      {
        buffer = register_a4 + (offset & 0xff);
      }
      break;
    case 0xa6: //motionplus
      if (state->sys.wmp_state == 1)
      {
         rpt = report_queue_push(state);
         report_format_mem_resp(rpt, 0xf, 0x7, offset, NULL);
         return;
      }
      buffer = register_a6 + (offset & 0xff);
      break;
    case 0xb0: //ir camera
      buffer = register_b0 + (offset & 0xff);
      break;
    default: //???
      break;
  }

  //equivalent to math.ceil(size / 0x10)
  int totalpackets = (size + 0x10 - 1) / 0x10;

  //allocate all the needed reports
  for (i=0; i<totalpackets; i++)
  {
    report_queue_push(state);
  }

  //copy packet data
  struct queued_report * currentrpt = state->sys.queue;

  for (i=0; i<totalpackets; i++)
  {
    rpt = &currentrpt->rpt;
    int psize = (i == totalpackets-1) ? (size % 0x10) : 0xf;
    report_format_mem_resp(rpt, psize, 0x0, offset + i*0x10, &buffer[i*0x10]);
    currentrpt = currentrpt->next;
  }
}

void write_register(struct wiimote_state *state, uint32_t offset, uint8_t size, const uint8_t * buf)
{
  uint8_t * reg;

  switch ((offset >> 16) & 0xfe) //select register, ignore lsb
  {
    case 0xa2: //speaker
      reg = register_a2;
      memcpy(reg + (offset & 0xff), buf, size);
      break;
    case 0xa4: //extension
      if (state->sys.wmp_state == 1)
      {
        reg = register_a6;
      }
      else
      {
        reg = register_a4;
      }

      memcpy(reg + (offset & 0xff), buf, size);

      //TODO: double check what this does, the buf location it's looking for
      if (((offset & 0xff) == 0xf0) && (buf[0] == 0x55) && (state->sys.wmp_state == 1)) //deactivate wmp
      {
        state->sys.wmp_state = 3;

        init_extension(state);

        report_queue_push_ack(state, 0x16, 0x00);
        state->sys.extension_connected = 0;
        report_queue_push_status(state);
        state->sys.extension_connected = 1;
        report_queue_push_status(state);
        return;
      }
      else if (((offset & 0xff) == 0xfe) && (buf[0] == 0x00) && (state->sys.wmp_state == 1)) //also deactivate wmp?
      {
        state->sys.wmp_state = 0;

        init_extension(state);

        report_queue_push_ack(state, 0x16, 0x00);
        state->sys.extension_connected = 0;
        report_queue_push_status(state);
        state->sys.extension_connected = 1;
        report_queue_push_status(state);
        return;
      }
      else if ((offset & 0xff) == 0x4c) //last part of encryption code
      {
        generate_tables();
        state->sys.extension_encrypted = 1;
      }
      /*
      else if (((offset & 0xff) == 0xf0) && (buf[0] == 0xaa))
      {
        //technically this sets the encryption
        //state->sys.extension_encrypted = 1;
      }
      */
      else if ((offset & 0xff) == 0xf1)
      {
        register_a6[0xf7] = 0x1a;

        //idk why or how, but sometimes this must be updated
        register_a6[0x50] = 0xe7;
        register_a6[0x51] = 0x98;
        register_a6[0x52] = 0x31;
        register_a6[0x53] = 0x8a;
        register_a6[0x54] = 0x18;
        register_a6[0x55] = 0x82;
        register_a6[0x56] = 0x37;
        register_a6[0x57] = 0x5e;
        register_a6[0x58] = 0x02;
        register_a6[0x59] = 0x4f;
        register_a6[0x5a] = 0x68;
        register_a6[0x5b] = 0x47;
        register_a6[0x5c] = 0x78;
        register_a6[0x5d] = 0xef;
        register_a6[0x5e] = 0xbb;
        register_a6[0x5f] = 0xd7;

        register_a6[0x60] = 0x86;
        register_a6[0x61] = 0xc8;
        register_a6[0x62] = 0x95;
        register_a6[0x63] = 0xbd;
        register_a6[0x64] = 0x20;
        register_a6[0x65] = 0x9b;
        register_a6[0x66] = 0xeb;
        register_a6[0x67] = 0x8b;
        register_a6[0x68] = 0x79;
        register_a6[0x69] = 0x81;
        register_a6[0x6a] = 0xdc;
        register_a6[0x6b] = 0x61;
        register_a6[0x6c] = 0x13;
        register_a6[0x6d] = 0x54;
        register_a6[0x6e] = 0x79;
        register_a6[0x6f] = 0x4c;

        register_a6[0x70] = 0xb7;
        register_a6[0x71] = 0x26;
        register_a6[0x72] = 0x82;
        register_a6[0x73] = 0x17;
        register_a6[0x74] = 0xe8;
        register_a6[0x75] = 0x0f;
        register_a6[0x76] = 0xa9;
        register_a6[0x77] = 0xb5;
        register_a6[0x78] = 0x45;
        register_a6[0x79] = 0xa0;
        register_a6[0x7a] = 0x38;
        register_a6[0x7b] = 0x8e;
        register_a6[0x7c] = 0x9e;
        register_a6[0x7d] = 0x86;
        register_a6[0x7e] = 0x72;
        register_a6[0x7f] = 0x55;

        register_a6[0x80] = 0x3d;
        register_a6[0x81] = 0x46;
        register_a6[0x82] = 0x2e;
        register_a6[0x83] = 0x3e;
        register_a6[0x84] = 0x10;
        register_a6[0x85] = 0x1f;
        register_a6[0x86] = 0x8e;
        register_a6[0x87] = 0x0c;
        register_a6[0x88] = 0xf4;
        register_a6[0x89] = 0x04;
        register_a6[0x8a] = 0x89;
        register_a6[0x8b] = 0x4c;
        register_a6[0x8c] = 0xca;
        register_a6[0x8d] = 0x3e;
        register_a6[0x8e] = 0x9f;
        register_a6[0x8f] = 0x36;
      }

      break;
    case 0xa6: //motionplus
      reg = register_a6;
      memcpy(reg + (offset & 0xff), buf, size);

      if (((offset & 0xff) == 0xfe) && ((buf[0] >> 2) & 0x1)) //activate wmp
      {
        state->sys.wmp_state = 1;
        state->sys.extension_report_type = (buf[0] & 0x7);
        printf("activate wmp\n");

        init_extension(state);

        report_queue_push_ack(state, 0x16, 0x00);
        state->sys.extension_connected = 0;
        report_queue_push_status(state);
        state->sys.extension_connected = 1;
        report_queue_push_status(state);
        return;
      }

      break;
    case 0xb0: //ir camera
      reg = register_b0;
      memcpy(reg + (offset & 0xff), buf, size);
      break;
    default: //???
      break;
  }


  report_queue_push_ack(state, 0x16, 0x00);
}


void init_extension(struct wiimote_state *state)
{

    if (state->sys.wmp_state == 1)
    {
        //register_a6[0xfa] = 0x00;
        //register_a6[0xfb] = 0x00;
        //register_a6[0xfc] = 0xa4;
        //register_a6[0xfd] = 0x20;
        //this might not be needed now that memory is proper
        //register_a6[0xfe] = state->sys.extension_report_type;
        //register_a6[0xff] = 0x05;
        register_a6[0xfc] = 0xa4;

        state->sys.extension_encrypted = 0;

        //random guess, pulled from wiimote, not sure what this is for
        register_a6[0xf0] = 0x55;
        register_a6[0xf1] = 0xff;
        register_a6[0xf2] = 0xff;
        register_a6[0xf3] = 0xff;
        register_a6[0xf4] = 0xff;
        register_a6[0xf5] = 0xff;
        register_a6[0xf6] = 0x00;

        //a4 40 post init
        register_a6[0x40] = 0x81;
        register_a6[0x41] = 0x80;
        register_a6[0x42] = 0x80;
        register_a6[0x43] = 0x28;
        register_a6[0x44] = 0xb4;
        register_a6[0x45] = 0xb3;
        register_a6[0x46] = 0xb3;
        register_a6[0x47] = 0x26;
        register_a6[0x48] = 0xe3;
        register_a6[0x49] = 0x22;
        register_a6[0x4a] = 0x7a;
        register_a6[0x4b] = 0xd8;
        register_a6[0x4c] = 0x1b;
        register_a6[0x4d] = 0x81;
        register_a6[0x4e] = 0x31;
        register_a6[0x4f] = 0x86;

        register_a6[0x20] = 0x7c;
        register_a6[0x21] = 0x97;
        register_a6[0x22] = 0x7f;
        register_a6[0x23] = 0x0a;
        register_a6[0x24] = 0x7c;
        register_a6[0x25] = 0xa8;
        register_a6[0x26] = 0x33;
        register_a6[0x27] = 0xb7;
        register_a6[0x28] = 0xcc;
        register_a6[0x29] = 0x12;
        register_a6[0x2a] = 0x33;
        register_a6[0x2b] = 0x08;
        register_a6[0x2c] = 0xc8;
        register_a6[0x2d] = 0x01;
        register_a6[0x2e] = 0x72;
        register_a6[0x2f] = 0xd4;

        register_a6[0x30] = 0x7c;
        register_a6[0x31] = 0x53;
        register_a6[0x32] = 0x87;
        register_a6[0x33] = 0x58;
        register_a6[0x34] = 0x7c;
        register_a6[0x35] = 0x9f;
        register_a6[0x36] = 0x36;
        register_a6[0x37] = 0xb2;
        register_a6[0x38] = 0xc9;
        register_a6[0x39] = 0x34;
        register_a6[0x3a] = 0x35;
        register_a6[0x3b] = 0xf8;
        register_a6[0x3c] = 0x2d;
        register_a6[0x3d] = 0x60;
        register_a6[0x3e] = 0xd7;
        register_a6[0x3f] = 0xd5;

        //not sure block, this may not be needed
        register_a6[0x50] = 0x15;
        register_a6[0x51] = 0x6d;
        register_a6[0x52] = 0xe0;
        register_a6[0x53] = 0x23;
        register_a6[0x54] = 0x20;
        register_a6[0x55] = 0x79;
        register_a6[0x56] = 0xd3;
        register_a6[0x57] = 0x73;
        register_a6[0x58] = 0x01;
        register_a6[0x59] = 0xa9;
        register_a6[0x5a] = 0xf0;
        register_a6[0x5b] = 0x25;
        register_a6[0x5c] = 0xb0;
        register_a6[0x5d] = 0xbc;
        register_a6[0x5e] = 0xff;
        register_a6[0x5f] = 0xe1;

        register_a6[0x60] = 0xd8;
        register_a6[0x61] = 0x3f;
        register_a6[0x62] = 0x82;
        register_a6[0x63] = 0x52;
        register_a6[0x64] = 0x75;
        register_a6[0x65] = 0x99;
        register_a6[0x66] = 0xbe;
        register_a6[0x67] = 0xdb;
        register_a6[0x68] = 0xcb;
        register_a6[0x69] = 0x61;
        register_a6[0x6a] = 0x60;
        register_a6[0x6b] = 0x0f;
        register_a6[0x6c] = 0x35;
        register_a6[0x6d] = 0xbd;
        register_a6[0x6e] = 0xd4;
        register_a6[0x6f] = 0x4d;

        register_a6[0x70] = 0x5c;
        register_a6[0x71] = 0x9f;
        register_a6[0x72] = 0x5d;
        register_a6[0x73] = 0x81;
        register_a6[0x74] = 0x71;
        register_a6[0x75] = 0xde;
        register_a6[0x76] = 0x22;
        register_a6[0x77] = 0xe6;
        register_a6[0x78] = 0xb9;
        register_a6[0x79] = 0x23;
        register_a6[0x7a] = 0xa4;
        register_a6[0x7b] = 0x58;
        register_a6[0x7c] = 0xb7;
        register_a6[0x7d] = 0x62;
        register_a6[0x7e] = 0x33;
        register_a6[0x7f] = 0xa4;

        register_a6[0x80] = 0xcd;
        register_a6[0x81] = 0x8b;
        register_a6[0x82] = 0x3a;
        register_a6[0x83] = 0xfe;
        register_a6[0x84] = 0x98;
        register_a6[0x85] = 0xf0;
        register_a6[0x86] = 0xd9;
        register_a6[0x87] = 0x57;
        register_a6[0x88] = 0x0c;
        register_a6[0x89] = 0xe8;
        register_a6[0x8a] = 0x27;
        register_a6[0x8b] = 0x51;
        register_a6[0x8c] = 0xb6;
        register_a6[0x8d] = 0xea;
        register_a6[0x8e] = 0xe5;
        register_a6[0x8f] = 0x78;


        //init progress byte, set it to done
        register_a6[0xf7] = 0x0c;
        register_a6[0xf8] = 0x00;
        register_a6[0xf9] = 0x00;

    }
    else
    {
        register_a6[0xfa] = 0x00;
        register_a6[0xfb] = 0x00;
        register_a6[0xfc] = 0xa6;
        register_a6[0xfd] = 0x20;
        //register_a6[0xfe] = 0x00; //leave this be
        register_a6[0xff] = 0x05;

        register_a6[0xf7] = 0x0c;
        register_a6[0xf8] = 0xff;
        register_a6[0xf9] = 0xff;

        state->sys.extension_report_type = state->sys.extension;

        if (state->sys.extension == EXT_NUNCHUCK)
        {
            register_a4[0xfa] = 0x00;
            register_a4[0xfb] = 0x00;
            register_a4[0xfc] = 0xa4;
            register_a4[0xfd] = 0x20;
            register_a4[0xfe] = 0x00;
            register_a4[0xff] = 0x00;
        }
        else if (state->sys.extension == EXT_CLASSIC)
        {
            register_a4[0xfa] = 0x00;
            register_a4[0xfb] = 0x00;
            register_a4[0xfc] = 0xa4;
            register_a4[0xfd] = 0x20;
            register_a4[0xfe] = 0x01;
            register_a4[0xff] = 0x01;
        }
        else
        {
            register_a4[0xfa] = 0xff;
            register_a4[0xfb] = 0xff;
            register_a4[0xfc] = 0xff;
            register_a4[0xfd] = 0xff;
            register_a4[0xfe] = 0xff;
            register_a4[0xff] = 0xff;
        }
    }
}

void destroy_wiimote(struct wiimote_state *state)
{

}

void init_wiimote(struct wiimote_state *state)
{
  memset(state, 0, sizeof(struct wiimote_state));

  state->sys.reporting_mode = 0x30;
  state->sys.battery_level = 0xff;

    //flat
  state->usr.accel_x = 0x80 << 2;
  state->usr.accel_y = 0x80 << 2;
  state->usr.accel_z = 0x97 << 2;

    /*
    //mario kart tilted
    state->usr.accel_x = 0x78 << 2;
  state->usr.accel_y = 0x80 << 2;
  state->usr.accel_z = 0x80 << 2;
    */

  ir_object_clear(state, 0);
  ir_object_clear(state, 1);
  ir_object_clear(state, 2);
  ir_object_clear(state, 3);

  state->usr.nunchuck.x = 128;
  state->usr.nunchuck.y = 128;
  state->usr.nunchuck.accel_x = 512;
  state->usr.nunchuck.accel_y = 512;
  state->usr.nunchuck.accel_z = 760;

  state->usr.classic.ls_x = 32;
  state->usr.classic.ls_y = 32;
  state->usr.classic.rs_x = 15;
  state->usr.classic.rs_y = 15;

  state->usr.motionplus.yaw_down = 0x1F7F;
  state->usr.motionplus.roll_left = 0x1F7F;
  state->usr.motionplus.pitch_left = 0x1F7F;
  state->usr.motionplus.yaw_slow = 1;
  state->usr.motionplus.roll_slow = 1;
  state->usr.motionplus.pitch_slow = 1;


  state->sys.extension = EXT_CLASSIC;
  state->sys.extension_connected = 1;
  init_extension(state);


  if (state->sys.extension != 0)
  {
    report_queue_push_status(state);
  }

  //state->sys.reporting_enabled = 0;
  //state->sys.feature_ef_byte_6 = 0xa0;

}
