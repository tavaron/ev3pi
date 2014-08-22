//      Ev3SerialCom.hpp
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


#ifndef __EV3SERIALCOM_HPP
#define __EV3SERIALCOM_HPP

#include "Ev3BaseCom.hpp"

/* Error codes */
#define EV3_SERIAL_OPEN_PORT_ERROR	16
#define EV3_SERIAL_IO_ERROR			17


class Ev3SerialCom : public Ev3BaseCom {
	protected:
		int portd;
		
		virtual ev3_error_t send(unsigned char* arr);
		
	public:
		Ev3SerialCom(char* tty, unsigned char Motor_L, unsigned char Motor_R);
		virtual ev3_error_t status(bool echo);
};

#endif
