//      Ev3UsbCom.cpp
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

#ifndef __EV3USBCOM_CPP
#define __EV3USBCOM_CPP

#include <usb.h>

Ev3UsbCom::Ev3UsbCom(unsigned char Motor_L, unsigned char Motor_R)
: Ev3BaseCom(Motor_L, Motor_R)
{
	/* init, find and open connection to EV3 */
	if(err == EV3_NOT_INITIALIZED) {
		err = ev3_init(&ev3);
		if(err == EV3_NOT_PRESENT) {
			err = ev3_find(ev3);
			if(err == EV3_USB_WAIT_TO_CONNECT) {
				err = ev3_open(ev3);
			}
		}
	}
}


ev3_error_t Ev3UsbCom::send(unsigned char* arr) {
	err = ev3_send_buf(ev3, arr, sizeof(arr));
	return err;
}

ev3_error_t Ev3UsbCom::status(bool echo) {
	/* check if ncurses is used */
	#ifdef __NCURSES_H
	if(echo) {
		mvprintw( 0, 2, "Status:");
		switch(err) {

			case EV3_USB_WRITE_ERROR:
				mvprintw( 0, 10, "USB Ausgabefehler");
				break;
			case EV3_USB_READ_ERROR:
				mvprintw( 0, 10, "USB Eingabefehler");
				break;
			case EV3_USB_CONNECTION_CLOSED:
				mvprintw( 0, 10, "USB Verbindung geschlossen");
				break;
			case EV3_USB_WAIT_TO_CONNECT:
				mvprintw( 0, 10, "Warte auf Verbindungsaufbau");
				break;
			default:
				Ev3BaseCom::status(echo);
		}
	}
	#endif
	
	/* return error code of EV3 */
	return err;
}


/* USB-Communication stolen from
 * jacques.gangloff@unistra.fr, 5/9/13
 * github.com/mindboards/ev3sources/wiki/Usb-motor-control
 */

ev3_error_t Ev3UsbCom::ev3_init( ev3_t **ev3 )  {
	if(err == EV3_NOT_INITIALIZED) {
		usb_init( );
		*ev3 = (ev3_t*)(calloc( 1, sizeof( **ev3 ) ));
		err = EV3_NOT_PRESENT;
	}

	return err;
} // ev3_init


ev3_error_t Ev3UsbCom::ev3_find( ev3_t *ev3 )  {
	if(err == EV3_NOT_PRESENT) {
		struct usb_bus *busses, *bus;

		usb_find_busses( );
		usb_find_devices( );

		busses = usb_get_busses( );

		for ( bus = busses; bus != NULL; bus = bus->next )  {
			struct usb_device *dev;

			for( dev = bus->devices; dev != NULL; dev = dev->next )  {
				if( dev->descriptor.idVendor == VENDOR_LEGO &&
					dev->descriptor.idProduct == PRODUCT_EV3 )  {
						ev3->dev = dev;
						err = EV3_USB_WAIT_TO_CONNECT;
						return err;
				}
			}
		}

		err = EV3_NOT_PRESENT;
	}
	return err;
} // ev3_find


ev3_error_t Ev3UsbCom::ev3_open( ev3_t *ev3 )  {
	char bound_driver_name[EV3_USB_NAME_LENGTH];
	int ret;

	ev3->hdl = usb_open( ev3->dev );

	/* Detach possible kernel driver bound to interface */
	if ( usb_get_driver_np( ev3->hdl, EV3_INTERFACE_NUMBER, bound_driver_name, EV3_USB_NAME_LENGTH ) == 0 )  {
		if ( usb_detach_kernel_driver_np( ev3->hdl, EV3_INTERFACE_NUMBER ) < 0 )  {
			err = EV3_IN_USE;
			return err;
		}
	}

	ret = usb_set_configuration( ev3->hdl, EV3_CONFIGURATION_NB );

	if ( ret < 0 )  {
		usb_close( ev3->hdl );
		err = EV3_CONFIGURATION_ERROR;
		return err;
	}

	ret = usb_claim_interface( ev3->hdl, EV3_INTERFACE_NUMBER );

	if ( ret < 0 )  {
		usb_close( ev3->hdl );
		err = EV3_IN_USE;
		return err;
	}
	err = EV3_OK;
	return err;
} // ev3_open


ev3_error_t Ev3UsbCom::ev3_close( ev3_t *ev3 )  {

	usb_release_interface( ev3->hdl, EV3_INTERFACE_NUMBER );
	usb_close( ev3->hdl );
	free( ev3 );
	err = EV3_NOT_INITIALIZED;

	return err;
} // ev3_close


ev3_error_t Ev3UsbCom::ev3_send_buf( ev3_t *ev3, unsigned char *buf, int len )  {

	int ret = usb_bulk_write( ev3->hdl, EV3_EP_OUT, (char*)buf, len, EV3_USB_TIMEOUT );

	if ( ret < 0 ) {
		err = EV3_USB_WRITE_ERROR;
		return err;
	}
	err = EV3_OK;
	return err;
} // ev3_send_buf


ev3_error_t Ev3UsbCom::ev3_recv_buf( ev3_t *ev3, char *buf, int len )  {

	int ret = usb_bulk_read( ev3->hdl, EV3_EP_IN, buf, len, EV3_USB_TIMEOUT );

	if ( ret < 0 ) {
		err = EV3_USB_READ_ERROR;
		return err;
	}

  return EV3_OK;
} // ev3_recv_buf

#endif
