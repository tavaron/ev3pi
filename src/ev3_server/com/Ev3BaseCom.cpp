//      Ev3BaseCom.cpp
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

#ifndef __EV3BASECOM_CPP
#define __EV3BASECOM_CPP

Ev3BaseCom::Ev3BaseCom(unsigned char Motor_L, unsigned char Motor_R) {
	
	err = EV3_NOT_INITIALIZED;
	
	/* default command arrays (see c_com.h) */
	eng_lr = 	{ 	0x0D, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
					0xA5, 0x00, (unsigned char)(Motor_L | Motor_R), 0x81, 0x00 /*speed*/,
					0xA6, 0x00, (unsigned char)(Motor_L | Motor_R) };
	eng_left = 	{ 	0x0D, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
					0xA5, 0x00, Motor_L, 0x81, 0x00,
					0xA6, 0x00, Motor_L };
	eng_right = { 	0x0D, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
					0xA5, 0x00, Motor_R, 0x81, 0x00,
					0xA6, 0x00, Motor_R };
	eng_stop = 	{ 	0x09, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
					0xA3, 0x00, (unsigned char)(Motor_L | Motor_R), 0x00 };
	sensorMode= { 	0x0B, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
					0x9A, 0x00, 0x00 /*Port 0-3*/, 0x00, 0x00 /*mode*/, 0x60 };
}

unsigned char Ev3BaseCom::convertSpeed(int s) {
	unsigned char r = 0;
	
	/* check if argument is valid */
	if( s >= -100 && s <= 100 ) {
		r = (unsigned char)(s);
	}
	
	/* limit invalid values */
	else if( s < -100 )
		r = -100;
	else if( s > 100 )
		r = 100;

	return r;
} // convertSpeed

ev3_error_t Ev3BaseCom::drive(drive_t drv) {
	// check for available EV3
	if(err == EV3_OK) {

		// check for active brakes
		if(drv.brakes) {

			// set brake mode
			if(drv.brake_mode)
				eng_stop[10] = 0x01;
			else
				eng_stop[10] = 0x00;

			// send command
			err = send(eng_stop);
			// and exit
			return err;
		}
		// check for active steering
		else if(drv.turn) {

			bool right = false; // direction
			int sp_high = 0;	// speed on high side
			int sp_low = 0;		// speed on low side
			
			/* limit turn value */
			if(drv.turn > 100)
				drv.turn = 100;
			else if(drv.turn < -100)
				drv.turn = -100;
			
			/* apply acc steering if needed */
			if(drv.acc_steering)
				drv.turn *= 2;

			// set direction
			if(drv.turn < 0) {
				right = true;
				drv.turn *= -1;
			}

			// handle uneven integer
			if(drv.turn%2) {
				sp_high += 1;
				if( drv.turn > 0 )
					drv.turn -= 1;
				else
					drv.turn += 1;
			}

			// split turn value between both sides
			sp_high += drv.turn / 2;
			sp_low -= drv.turn / 2;
			
			/* set turn to original value */
			if(drv.acc_steering)
				drv.turn /= 2;

			// add current speed
			if(drv.speed >= 0) {
				sp_high += drv.speed;
				sp_low += drv.speed;
			} else {
				sp_high -= drv.speed;
				sp_low -= drv.speed;
			}

			// correct high values
			if(sp_high > 100) {
				sp_low -= sp_high - 100;
				sp_high = 100;
			} else if(sp_high < -100) {
				sp_low += sp_high + 100;
				sp_high = -100;
			}

			// correct low values
			if(sp_low <= -100)
				sp_low = -100;
			else if(sp_low >= 100)
				sp_low = 100;

			// correct direction
			if(drv.speed < 0) {
				sp_high *= -1;
				sp_low *= -1;
			}

			// set direction to engines
			if(right) {
				eng_right[11] = convertSpeed(sp_low);
				eng_left[11] = convertSpeed(sp_high);
			} else {
				eng_right[11] = convertSpeed(sp_high);
				eng_left[11] = convertSpeed(sp_low);
			}

			// send first command...
			err = send(eng_right);
			// ...check for error...
			if(err == EV3_OK){
				// ...wait for the EV3...
				usleep(500);
				// ...and send the second command
				err = send(eng_left);
			}
		}
		// check if we are really doing something
		else if(drv.speed) {
			// set same speed to both engines
			eng_lr[11] = convertSpeed(drv.speed);
			// send command
			err = send(eng_lr);
		}
		// else coast
		else {
			// set brake command to coast
			eng_stop[10] = 0x00;
			// send command
			err = send(eng_stop);
		}

	} // if !err

	return err;
} // drive_t drv

ev3_error_t Ev3BaseCom::status(bool echo) {
	
	/* check if ncurses is used */
	#ifdef __NCURSES_H
	if(echo) {
		mvprintw( 0, 2, "Status:");
		switch(err) {
			case EV3_OK:
				mvprintw( 0, 10, "EV3 verbunden");
				break;
			case EV3_NOT_PRESENT:
				mvprintw( 0, 10, "EV3 nicht verbunden");
				break;
			case EV3_CONFIGURATION_ERROR:
				mvprintw( 0, 10, "Konfigurationsfehler");
				break;
			case EV3_IN_USE:
				mvprintw( 0, 10, "EV3 war noch aktiv");
				break;
			default:
				mvprintw( 0, 10, "Unbekannter Fehler: %d", err);
		}
	}
	#endif
	
	/* return error code of EV3 */
	return err;
} //status

ev3_error_t Ev3BaseCom::stop(bool brakes) {
	
	/* check brake mode and set matching byte */
	if(brakes)
		eng_stop[10] = 0x01;
	else
		eng_stop[10] = 0x00;
	
	/* send command and return error code */
	err = send(eng_stop);
	return err;
} // stop

#endif
