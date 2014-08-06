//      Ev3DummyCom.hpp
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


#ifndef __EV3DUMMYCOM_HPP
#define __EV3DUMMYCOM_HPP

#include "Ev3BaseCom.hpp"

class Ev3DummyCom : public Ev3BaseCom {
	protected:
		virtual ev3_error_t send(unsigned char* arr) 
			{ std::cout << "Dummy send()\n"; return 0; }
	public:
		Ev3DummyCom() : Ev3BaseCom(0,0)	{ }
		Ev3DummyCom(unsigned char Motor_L, unsigned char Motor_R)
		: Ev3BaseCom(Motor_L, Motor_R)
		{ }

};

#endif
