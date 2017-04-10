#ifndef WIIMOTE_H
#define	WIIMOTE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define EXT_NUNCHUCK 0x1
#define EXT_CLASSIC 0x2

struct wiimote_ir_object
{
  uint16_t x;
  uint16_t y;
  uint8_t size;
  uint8_t xmin;
  uint8_t ymin;
  uint8_t xmax;
  uint8_t ymax;
  uint8_t intensity;
};

struct wiimote_nunchuck
{
  uint16_t accel_x;
  uint16_t accel_y;
  uint16_t accel_z;
  uint8_t x;
  uint8_t y;
  bool c;
  bool z;
};

struct wiimote_classic
{
  bool a;
  bool b;
  bool x;
  bool y;
  bool minus;
  bool plus;
  bool rtrigger;
  bool ltrigger;
  bool home;
  bool rz;
  bool lz;
  bool up;
  bool down;
  bool left;
  bool right;
  uint8_t ls_x;
  uint8_t ls_y;
  uint8_t rs_x;
  uint8_t rs_y;
  uint8_t lt;
  uint8_t rt;

};

struct wiimote_motionplus
{
  uint16_t yaw_down;
  uint16_t roll_left;
  uint16_t pitch_left;
  bool yaw_slow;
  bool roll_slow;
  bool pitch_slow;
};

struct wiimote_state_usr
{
  //main controller buttons
  bool a;
  bool b;
  bool minus;
  bool plus;
  bool home;
  bool one;
  bool two;
  bool up;
  bool down;
  bool left;
  bool right;

  //special buttons
  bool sync;
  bool power;

  //accelerometer (10 bit range)
  //0 acceleration is approximately 0x80
  uint16_t accel_x;
  uint16_t accel_y;
  uint16_t accel_z;

  //four ir camera dots
  //x, y, size, x min, y min, x max, y max, intensity
  struct wiimote_ir_object ir_object[4];

  struct wiimote_nunchuck nunchuck;
  struct wiimote_classic classic;
  struct wiimote_motionplus motionplus;
};

struct wiimote_state_sys
{
  //controller led status
  bool led_1;
  bool led_2;
  bool led_3;
  bool led_4;

  bool rumble;

  bool ircam_enabled;
  bool speaker_enabled;

  uint8_t battery_level;
  bool low_battery;

  bool extension_connected;
  bool extension_report;
  bool extension_encrypted;
  uint8_t extension;
  uint8_t extension_report_type;
  uint8_t wmp_state; //0 inactive, 1 active, 2 deactivated

  uint8_t reporting_mode;
  bool reporting_continuous;
  bool report_changed;

  struct queued_report * queue;
  struct queued_report * queue_end;

  //extensions:
  //none, nunchuck, classic, wm+, wm+ and nunchuck, wm+ and classic

};

struct wiimote_state
{
  struct wiimote_state_sys sys;
  struct wiimote_state_usr usr;
};

extern struct wiimote_state wiimote;

//574bytes
//TODO: move this to the wiimote struct where it belongs
uint8_t register_a2[0x09 + 1]; //speaker
uint8_t register_a4[0xff + 1]; //extension
uint8_t register_a6[0xff + 1]; //wii motion plus
uint8_t register_b0[0x33 + 1]; //ir camera

void init_wiimote(struct wiimote_state * state);
void destroy_wiimote(struct wiimote_state * state);

void _wiimote_recv_ctrl(const uint8_t *buf, int len);
int32_t _wiimote_get_ctrl(uint8_t * buf);
void _wiimote_recv_data(const uint8_t * buf, int len);
int32_t _wiimote_get_data(uint8_t * buf);

int wiimote_recv_report(struct wiimote_state * state, const uint8_t * buf, int len);
int wiimote_get_report(struct wiimote_state * state, uint8_t * buf);

void ir_object_clear(struct wiimote_state * state, uint8_t num);

void read_eeprom(struct wiimote_state * state, uint32_t offset, uint16_t size);
void write_eeprom(struct wiimote_state * state, uint32_t offset, uint8_t size, const uint8_t * buf);
void read_register(struct wiimote_state * state, uint32_t offset, uint16_t size);
void write_register(struct wiimote_state * state, uint32_t offset, uint8_t size, const uint8_t * buf);

void init_extension(struct wiimote_state * state);

#ifdef	__cplusplus
}
#endif

#endif	/* WIIMOTE_H */
