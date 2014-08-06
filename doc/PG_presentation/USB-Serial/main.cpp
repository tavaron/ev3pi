//      main.cpp
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


#include <ncurses.h> 		// TUI toolkit
#include <SDL/SDL.h> 		// SDL for Joystick support
#include "ev3_usbcom.hpp"	// EV3-Zeug


// Controls for the xbox 360 controller with xboxdrv
#define BTN_BRAKE 			0 	// A
#define BTN_BRAKE_MODE 		2 	// X
#define BTN_ACC_STEERING	9 	// LStick
#define BTN_BOOST_SPEED		3	// Y
#define BTN_QUIT 			8	// XboxButton
#define JS_AXIS_SPEED		1	// RY
#define JS_AXIS_TURN		0	// LX


// initialise EV3 with motors on ports B and C
ev3_USBCom *ev3 = new ev3_USBCom(0x02, 0x04);



// drive with Keyboard
// DEPRECATED
int driveKey(int steps) {
	
	drive_t drv;
	drv.speed = 0;
	drv.turn = 0;
	char c = 32;
	noecho();
	clear();
	mvprintw(2, 2,"Sp:\t%d\tTu:\t%d\n", drv.speed, drv.turn);
	mvprintw(6, 2,"Pressed: %d\n", c);
	while( (c = getch()) != 'q') {	
		switch(c) { 
			case 2:
				drv.speed -= steps;
				ev3->drive(drv.speed, drv.turn);
				break;
			case  3:
				drv.speed += steps;
				ev3->drive(drv.speed, drv.turn);
				break;
			case  4:
				drv.turn -= steps;
				ev3->drive(drv.speed, drv.turn);
				break;
			case  5:
				drv.turn += steps;
				ev3->drive(drv.speed, drv.turn);
				break;
			case 32:
				ev3->stop(true);
				break;
			case 48:
				ev3->stop(false);
				break;
		}
		echo();
		clear();
		ev3->status(true);
		mvprintw(2,2,"Sp:\t%d\tTu:\t%d\n", drv.speed, drv.turn);
		mvprintw(6,2,"Pressed: %d\n", c);
		refresh();
	}
	return 0;
}


// drive with joystick
int driveJoy(int deadzone_pct) {
	
	// Initialise Variables
	deadzone_pct /= 100;
    SDL_Joystick *joy;
	
	
	// Show window
	echo();
	clear();
	ev3->status(true);
	ev3->stop(false);
	refresh();
	
	// Init joystick
	if ( SDL_InitSubSystem ( SDL_INIT_JOYSTICK ) < 0 )
	{
		mvprintw(4, 2, "Unable to initialize Joystick: %s\n", SDL_GetError() );
		return -1;
	}
	
	// if joysticks available
	if(SDL_NumJoysticks()>0)
    {
        // Open joystick
        joy=SDL_JoystickOpen(0);
        
        // Manually update joystick status
		SDL_JoystickEventState ( SDL_QUERY );
		
		// Initialise variables
		int x = 0,
			y = 0,
			cmd_counter = 0;
		
		// debouncing variables
		bool	btn_brake_mode	= false,
				btn_brakes		= false,
				btn_acc_steering= false,
				send 		= false;	// send command?
		
		// set default values
		drive_t drv;
			drv.speed = 0;
			drv.turn = 0;
			drv.brakes = false;			// Don't turn brakes on at startup
			drv.brake_mode = false; 	// Coast
			drv.acc_steering = false; 	// Use full steering while driving
			
		
		// Main loop
		while( true ) {	
			
			// Show driving information
			clear();
			mvprintw(2, 2, "Speed:  %2d  Turn:  %2d  CS: %3d\n", drv.speed, drv.turn, drv.convSpeed);
			mvprintw(3, 2, "LX: %5d  RY: %5d", x, y );
			ev3->status(true);
			refresh();
			
			// Initialise variables
			int tmpx = 0,
				tmpy = 0,
				tol  = 32768 * deadzone_pct; // determine controller tolerance
			int button[4];	
			send = false;
			
			// Update joystick status
			SDL_JoystickUpdate ();
			
			// read buttons
			button[0] = SDL_JoystickGetButton( joy, BTN_BRAKE );
			button[1] = SDL_JoystickGetButton( joy, BTN_BRAKE_MODE );
			button[2] = SDL_JoystickGetButton( joy, BTN_ACC_STEERING );
			button[3] = SDL_JoystickGetButton( joy, BTN_QUIT );
			
			// show button information
			mvprintw(5,2,"Brake:   %d", (int)(drv.brakes), button[0]);
			mvprintw(6,2,"BrMode:  %d", (int)(drv.brakes), button[1]);
			mvprintw(7,2,"AccMode: %d", (int)(drv.acc_steering), button[2]);
			mvprintw(9,2,"Sent %d commands",cmd_counter);
			refresh();
			
			// don't drive the EV3 mad
			usleep(10000);
			
			// check for active brakes
			if(!drv.brakes) {
				
				// read axis for speed control
				tmpy = SDL_JoystickGetAxis(joy, JS_AXIS_SPEED);
				
				// check for significant change
				if( (tmpy + tol) < y || (tmpy - tol) > y ) {
					y = tmpy;
					
					// use full speed while driving backwards
					if(y == 32767)
						y++;
					
					// some deadzone
					if(y >= 768 || y <= -768) {
						
						// convert speed
						drv.speed = (int)(y / -32768.0 * 100.0);
					
					} else // set speed to 0 if value is in deadzone
						drv.speed = 0;
					
					// register for sending command to EV3
					send = true;
				}
				
				// read turn axis
				tmpx = SDL_JoystickGetAxis(joy, JS_AXIS_TURN);
				
				// check for significant change
				if( (tmpx + tol) < x  || (tmpx - tol) > x ) {
					x = tmpx;
					
					// use full turn
					if(x == 32767)
						x++;
					
					// some deadzone
					if(x >= 768 || x <= -768) {
						
						// convert speed
						drv.turn = (int)(x / 32768.0 * 100.0);
						if(drv.acc_steering)
							drv.turn *= 2;
					
					} else // set turn to 0 if value is in deadzone
						drv.turn = 0;
					
					// register for sending command to EV3
					send = true;
				}	
			} // if !brakes
			
			
			
			
			// check debouncer and brake mode button
			if( !(btn_brake_mode) && button[1] ) {
				
				// switch brake mode
				drv.brake_mode = !(drv.brake_mode);
				
				// apply new brake mode if necessary
				if(drv.brakes)
					send = true;
					
				// debounce on
				btn_brake_mode = true;
			}
			// debounce off
			else if( btn_brake_mode && !(button[1]) )
				btn_brake_mode = false;
			
			
			
			// check debouncer and brake button
			if( !(btn_brakes) && button[0] ) {
				
				// switch brake state
				drv.brakes = !(drv.brakes);
				
				// apply brake state
				send = true;
					
				// debounce on
				btn_brakes = true;
			}
			// debounce off
			else if( btn_brakes && !(button[0]) )
				btn_brakes = false;
				
			
			// check debouncer and acc steering button
			if( !(btn_acc_steering) && button[2] ) {
				
				// switch steering behaviour
				drv.acc_steering = !drv.acc_steering;
				
				// debounce on
				btn_acc_steering = true;
			}
			// debounce off
			else if( btn_acc_steering && !(button[2]) )
				btn_acc_steering = false;
				

			
			
			// exit loop on quit button
			if(button[3])
				break;
				
			// send data if available
			if(send) {
				ev3->drive(drv);
				cmd_counter++;
			}
			
		} // while(jsOpen)
		
		// refresh window
		refresh();
		
		// Close if joystick opened
		if(SDL_JoystickOpened(0))
			SDL_JoystickClose(joy);
		
	} // fail w/o joysticks
	
	return 0;
}

int main(int argc, char** argv)
{
	// init ncurses
	initscr();
	keypad(stdscr, TRUE);
	
	// if driveJoy fails call driveKey
	if(ev3->status(false) == EV3_OK && driveJoy(10))
		driveKey(2);
	
	// exit ncurses
	refresh();
	endwin();
	
	return 0;
}
