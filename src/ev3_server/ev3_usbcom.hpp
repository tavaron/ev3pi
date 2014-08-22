//      ev3_serialcom.hpp
//
//      Copyright 2014 Marius Christ <marius.christ@gmail.com>
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


/* USB-Communication stolen from
 * jacques.gangloff@unistra.fr, 5/9/13
 * github.com/mindboards/ev3sources/wiki/Usb-motor-control
 */


#include <usb.h>

#ifndef __EV3_USBCOM_HPP
#define __EV3_USBCOM_HPP

#define EV3_USB_TIMEOUT             2000    // Milliseconds
#define EV3_USB_NAME_LENGTH         32

/* These informations can be obtained with lsusb -v */
#define EV3_INTERFACE_NUMBER        0
#define EV3_CONFIGURATION_NB        1
#define EV3_EP_OUT                  0x01
#define EV3_EP_IN                   0x81

/* Error codes */
#define EV3_OK                      0
#define EV3_NOT_PRESENT             1
#define EV3_CONFIGURATION_ERROR     2
#define EV3_IN_USE                  3
#define EV3_USB_WRITE_ERROR         4
#define EV3_USB_READ_ERROR          5
#define EV3_USB_CONNECTION_CLOSED	6
#define EV3_USB_WAIT_TO_CONNECT		7
#define EV3_NOT_INITIALIZED			8


struct drive_t {
	int speed,
		turn;
	bool brakes,
		 brake_mode, // coast == 0, brake == 1
		 acc_steering; // true: maxTurn = 62, false: maxTurn = 31
};

// USB VID & PID
enum ev3_usb_ids {
  VENDOR_LEGO   = 0x0694,
  PRODUCT_EV3   = 0x0005
};


struct ev3_t {
  struct usb_device     *dev;
  struct usb_dev_handle *hdl;
};


typedef unsigned char               ev3_error_t;
typedef struct ev3_t                ev3_t;
typedef struct drive_t				drive_t;


class ev3_USBCom {
	private:

		// command byte arrays
		unsigned char	eng_lr[15], eng_left[15], eng_right[15], eng_stop[11],
					//	eng_sync[14], eng_time[15], eng_step[15], // deprecated command arrays
						sensor[9], sensorMode[13];

		ev3_error_t err;	// Error Code
		ev3_t *ev3;			// EV3 USB device

		// usb functions
		ev3_error_t ev3_init( ev3_t **ev3 );
		ev3_error_t ev3_find( ev3_t *ev3 );
		ev3_error_t ev3_open( ev3_t *ev3 );
		ev3_error_t ev3_close( ev3_t *ev3 );
		ev3_error_t ev3_send_buf( ev3_t *ev3, unsigned char *buf, int len );
		ev3_error_t ev3_recv_buf( ev3_t *ev3, char *buf, int len );




	public:
		ev3_USBCom(unsigned char Motor_L, unsigned char Motor_R);

		~ev3_USBCom();

		ev3_error_t open();
		ev3_error_t close();

		ev3_error_t drive(drive_t drv);
		ev3_error_t drive(int speed, int turn);

		ev3_error_t setSensorMode(int port, int mode);
		/* deprecated drive functions
		ev3_error_t driveSync(int speed, int turn, int ms);
		ev3_error_t driveTime(int speed, int turn, int time);
		ev3_error_t driveStep(int speed, int turn, int step);
		*/
		ev3_error_t stop(bool brakes);
		int readSensor(int sensorPort);
		ev3_error_t status(bool echo);


		/*	convert speed to an unsigned range for bytecode
		 * 	negative values get added 63
		 * 	1-31 is forward
		 *  63-32 is backward */
		unsigned char convertSpeed(int s);
};


#endif
