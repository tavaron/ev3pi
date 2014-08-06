//      test.cpp
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


#include <iostream>
#include "Ev3DummyCom.hpp"


int main(int argc, char** argv)
{
	Ev3BaseCom* ev3 = new Ev3DummyCom(0x04, 0x08);
	drive_t drv;
	ev3->drive(drv);
	
	sleep(1000);
	return 0;
}
