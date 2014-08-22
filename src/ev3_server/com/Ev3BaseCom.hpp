//      Ev3BaseCom.hpp
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

#ifndef __EV3BASECOM_HPP
#define __EV3BASECOM_HPP 

#include <unistd.h>

/* Error codes */
#define EV3_OK                      0
#define EV3_NOT_PRESENT             1
#define EV3_CONFIGURATION_ERROR     2
#define EV3_IN_USE                  3
#define EV3_NOT_INITIALIZED			128


struct drive_t {
	int speed,
		turn,
		ms,
		steps;
	bool brakes,
		 brake_mode, // coast == 0, brake == 1
		 acc_steering; // true: maxTurn = 62, false: maxTurn = 31
};

typedef unsigned char               ev3_error_t;
typedef struct drive_t				drive_t;


class Ev3BaseCom {
	protected:
		unsigned char	eng_lr[15], eng_left[15], eng_right[15], eng_stop[11],
					//	eng_sync[14], eng_time[15], eng_step[15], // deprecated command arrays
						sensor[9], sensorMode[13];
		ev3_error_t err;	// Error Code
		
		virtual ev3_error_t send(unsigned char* arr) = 0;
		unsigned char convertSpeed(int s);
		
		
	public:
		Ev3BaseCom(unsigned char Motor_L, unsigned char Motor_R);
		virtual ev3_error_t drive(drive_t drv);
		virtual ev3_error_t stop(bool brakes);
		virtual ev3_error_t status(bool echo);
		
};

#endif
