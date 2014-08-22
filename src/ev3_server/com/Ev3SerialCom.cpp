//      Ev3SerialCom.cpp
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

#include "Ev3SerialCom.hpp"
#include <termios.h>
#include <fcntl.h>

Ev3SerialCom::Ev3SerialCom(char* tty, unsigned char Motor_L, unsigned char Motor_R)
: Ev3BaseCom(Motor_L, Motor_R)
{
	portd = open(tty, O_RDWR | O_NOCTTY | O_NDELAY);
	if(portd == -1)
		err = EV3_SERIAL_OPEN_PORT_ERROR;
	else
	{
		fcntl(portd, F_SETFL, 0);

		struct termios port_settings;      // structure to store the port settings in

		cfsetispeed(&port_settings, B115200);    // set baud rates
		cfsetospeed(&port_settings, B115200);

		port_settings.c_cflag |= (CLOCAL | CREAD);    // Enable the receiver and set local mode
		port_settings.c_cflag &= ~PARENB;             // No parity bit
		port_settings.c_cflag &= ~CSTOPB;             // 1 stop bit
		port_settings.c_cflag &= ~CSIZE;              // Mask data size
		port_settings.c_cflag |=  CS8;                // Select 8 data bits
		port_settings.c_cflag &= ~CRTSCTS;            // Disable hardware flow control 
		
		tcsetattr(portd, TCSANOW, &port_settings);    // apply the settings to the port
		
		err = EV3_OK;
	}
} // open & configure port


ev3_error_t Ev3SerialCom::send(unsigned char* arr) {
	
	tcflush(portd, TCIOFLUSH);
	
	if(write(portd, arr, sizeof(arr)) < 0)
		err = EV3_SERIAL_IO_ERROR;
	else
		err = EV3_OK;
	
	return err;
}


ev3_error_t Ev3SerialCom::status(bool echo) {
	/* check if ncurses is used */
	#ifdef __NCURSES_H
	if(echo) {
		mvprintw( 0, 2, "Status:");
		switch(err) {

			case EV3_SERIAL_OPEN_PORT_ERROR:
				mvprintw( 0, 10, "Serieller Verbindungsaufbau gescheitert");
				break;
			case EV3_SERIAL_IO_ERROR:
				mvprintw( 0, 10, "Serieller E/A-Fehler");
				break;
			default:
				Ev3BaseCom::status(echo);
		}
	}
	#endif
	
	/* return error code of EV3 */
	return err;
}

