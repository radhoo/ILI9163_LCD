/*
 File:      DigitalPin.cpp
 Version:   0.1 - first version
 Date:      February 12, 2015
 License:	GPL v2

 A digital pin class, used for setting the state or reading the state of a pin

 ****************************************************************************
 Copyright (C) 2015 Radu Motisan  <radu.motisan@gmail.com>

 http://www.pocketmagic.net

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ****************************************************************************
 */


#include "DigitalPin.h"

// Set the output, specified as 0 or 1 (int)
void DigitalPin::write(bool state) {
	//state? *m_pport |= (1<<m_dq) :  *m_pport &= ~(1<<m_dq);
	state? *m_pport |= mask1 : *m_pport &= mask0;
}

// Return the output setting, represented as 0 or 1 (bool)
bool DigitalPin::read() {
	//return *PORT2PIN(m_pport) & (1<<m_dq);
	return *PORT2PIN(m_pport) & mask1;
}

// negate state
bool DigitalPin::toggle() {
	write(!read());
}

