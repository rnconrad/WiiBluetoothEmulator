#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "usb.h"
#include "hci.h"
#include <xc.h>
#include <string.h>
#include "usb_config.h"
#include "usb_ch9.h"
#include "usb_hid.h"
#include "hardware.h"

#include "wiimote.h"

#ifdef MULTI_CLASS_DEVICE
static uint8_t hid_interfaces[] = { 0 };
#endif

static int32_t length = 0;

int main(void)
{
    //TODO: replace this simple control test

    //set up i/o as inputs
    TRISAbits.TRISA1 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB8 = 1;
    TRISBbits.TRISB15 = 1;

    //enable internal pull up
    CNPUAbits.CNPUA1 = 1;
    CNPUBbits.CNPUB3 = 1;
    CNPUBbits.CNPUB5 = 1;
    CNPUBbits.CNPUB7 = 1;
    CNPUBbits.CNPUB8 = 1;
    CNPUBbits.CNPUB15 = 1;

    //set all pins as digital
    ANSELAbits.ANSA1 = 0;
    ANSELBbits.ANSB3 = 0;
    ANSELBbits.ANSB15 = 0;

	hardware_init();

#ifdef MULTI_CLASS_DEVICE
	hid_set_interface_list(hid_interfaces, sizeof(hid_interfaces));
#endif

	usb_init();

	while (1)
    {
		if (usb_is_configured())
        {
            if (!usb_in_endpoint_busy(1))
            {
                int32_t len = hci_get_event(usb_get_in_buffer(1));
                if (len > 0)
                {
                    usb_send_in_buffer(1, len);
                }
            }

            if (!usb_in_endpoint_busy(2))
            {
                int32_t len = hci_get_data(usb_get_in_buffer(2));
                if (len > 0)
                {
                    usb_send_in_buffer(2, len);
                }
            }
		}

        //PORTAbits.RA

        //wiimote.usr.a = !PORTDbits.RD6;

        uint8_t x = 32;
        uint8_t y = 32;
        if (!PORTBbits.RB3) x += 10;
        if (!PORTBbits.RB5) x -= 10;
        if (!PORTBbits.RB7) y += 5;
        if (!PORTBbits.RB8) y -= 5;

        wiimote.usr.classic.ls_y = y;//32 + (!PORTBbits.RB15) * 10 - (!PORTBbits.RB14) * 10;
        wiimote.usr.classic.ls_x = x;//32 + (!PORTBbits.RB9) * 10 - (!PORTBbits.RB8) * 10;

        //wiimote.usr.classic.up = !PORTBbits.RB15;
        //wiimote.usr.classic.left = !PORTBbits.RB14;
        //wiimote.usr.classic.down = !PORTBbits.RB13;
        //wiimote.usr.classic.right = !PORTBbits.RB9;

        wiimote.usr.classic.a = !PORTBbits.RB15;
        //wiimote.usr.classic.b = !PORTBbits.RB7;

        wiimote.usr.home = !PORTAbits.RA1;
        //wiimote.usr.plus = !PORTBbits.RB4;

		#ifndef USB_USE_INTERRUPTS
		usb_service();
		#endif
	}

	return 0;
}

/* Callbacks. These function names are set in usb_config.h. */
void app_set_configuration_callback(uint8_t configuration)
{

}

uint16_t app_get_device_status_callback()
{
	return 0x0000;
}

void app_endpoint_halt_callback(uint8_t endpoint, bool halted)
{

}

int8_t app_set_interface_callback(uint8_t interface, uint8_t alt_setting)
{
	return 0;
}

int8_t app_get_interface_callback(uint8_t interface)
{
	return 0;
}

void app_out_transaction_callback(uint8_t endpoint)
{
    //received data from the bulk endpoint

    uint8_t len;
    uint8_t * buf;
    len = usb_get_out_buffer(2, (const unsigned char **)&buf);

    hci_recv_data(buf, len);

    usb_arm_out_endpoint(2);
}

void app_in_transaction_complete_callback(uint8_t endpoint)
{

}

int8_t ep0_cb(bool data_ok, void * context)
{
    if (data_ok)
    {
        hci_recv_command(length);
        return 0;
    }
    else
    {
        return -1;
    }
}

int8_t app_unknown_setup_request_callback(const struct setup_packet *setup)
{
	/* To use the HID device class, have a handler for unknown setup
	 * requests and call process_hid_setup_request() (as shown here),
	 * which will check if the setup request is HID-related, and will
	 * call the HID application callbacks defined in usb_hid.h. For
	 * composite devices containing other device classes, make sure
	 * MULTI_CLASS_DEVICE is defined in usb_config.h and call all
	 * appropriate device class setup request functions here.
	 */

    if (setup->REQUEST.type == 1) //class request (HCI command)?
    {
        length = setup->wLength;
        usb_start_receive_ep0_data_stage(hci_get_cmd_buffer(), setup->wLength, (usb_ep0_data_stage_callback)ep0_cb, NULL);
        return 0;
    }
    else
    {
        return -1;
    }
}

int16_t app_unknown_get_descriptor_callback(const struct setup_packet *pkt, const void **descriptor)
{
	return 0;
}

//void app_start_of_frame_callback(void)
//{
//
//}
//
//void app_usb_reset_callback(void)
//{
//
//}


#ifdef _PIC14E
void interrupt isr()
{
	usb_service();
}
#elif _PIC18

#ifdef __XC8
void interrupt high_priority isr()
{
	usb_service();
}
#elif _PICC18
#error need to make ISR
#endif

#endif
