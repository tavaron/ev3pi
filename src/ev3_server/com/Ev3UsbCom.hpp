//      Ev3UsbCom.hpp
//      
//      Copyright 2014 Marius Christ <marius@erebos>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#ifndef __EV3USBCOM_HPP
#define __EV3USBCOM_HPP

#include "Ev3BaseCom.hpp"

/* Error codes */
#define EV3_USB_WRITE_ERROR         4
#define EV3_USB_READ_ERROR          5
#define EV3_USB_CONNECTION_CLOSED	6
#define EV3_USB_WAIT_TO_CONNECT		7

#define EV3_USB_TIMEOUT             2000    // Milliseconds
#define EV3_USB_NAME_LENGTH         32

/* These informations can be obtained with lsusb -v */
#define EV3_INTERFACE_NUMBER        0
#define EV3_CONFIGURATION_NB        1
#define EV3_EP_OUT                  0x01
#define EV3_EP_IN                   0x81


// USB VID & PID
enum ev3_usb_ids {
  VENDOR_LEGO   = 0x0694,
  PRODUCT_EV3   = 0x0005
};


struct ev3_t {
  struct usb_device     *dev;
  struct usb_dev_handle *hdl;
};

typedef struct ev3_t	ev3_t;

class Ev3UsbCom : public Ev3BaseCom {
	private:
		// usb functions
		ev3_error_t ev3_init( ev3_t **ev3 );
		ev3_error_t ev3_find( ev3_t *ev3 );
		ev3_error_t ev3_open( ev3_t *ev3 );
		ev3_error_t ev3_close( ev3_t *ev3 );
		ev3_error_t ev3_send_buf( ev3_t *ev3, unsigned char *buf, int len );
		ev3_error_t ev3_recv_buf( ev3_t *ev3, char *buf, int len );
		
	protected:
		virtual ev3_error_t send(unsigned char* arr);
		ev3_t* ev3;
		
	public:
		Ev3UsbCom(unsigned char Motor_L, unsigned char Motor_R);
		virtual ev3_error_t status(bool echo);
};

#include "Ev3UsbCom.cpp"

#endif
