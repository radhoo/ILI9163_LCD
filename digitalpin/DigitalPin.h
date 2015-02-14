/*
 File:      DigitalPin.h
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
#pragma once
#include <stdint.h>
#include <avr/io.h>

class DigitalPin  {
    volatile uint8_t 	*m_pport;
    //uint8_t 			m_dq;
    uint8_t				mask1, mask0;

    // Get the other registers from the PORT
    #define PORT2DDR(x) (x - 1)
	#define PORT2PIN(x) (x - 2)

public:

    // Create a DigitalOut connected to the specified pin
    DigitalPin(volatile  uint8_t  *port, uint8_t  dq, bool state = false) {
    	// save globals
    	m_pport = port;
    	//m_dq = dq;
    	mask1 = (1<<dq);
    	mask0 = ~(1<<dq);
		// set for output
		*PORT2DDR(m_pport) |= mask1;//(1<<m_dq);
		// default state?
		write(state);
    }

    // Set the output, specified as 0 or 1 (int)
    void write(bool state);

    // Return the output setting, represented as 0 or 1 (int)
    bool read();

    // negate state
    bool toggle();

    DigitalPin& operator= (int value) {
        write(value);
        return *this;
    }

    DigitalPin& operator= (DigitalPin& rhs) {
        write(rhs.read());
        return *this;
    }
    
    /*DigitalOut& operator ! () const {
    	return *this;
    }*/

    operator int() {
        return read();
    }

};
