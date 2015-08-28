/*
** Config file
** Copyright (C) 2009 - 2015 Radu Motisan, radu.motisan@gmail.com
** General settings for the project.
**
** www.pocketmagic.net
**
** This file is a part of "Portable Environmental Monitor" open source project presented on
** https://hackaday.io/project/4977-portable-environmental-monitor
**
** This project is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 3 of the License,
** or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


// Change to match your board! //
#define ATMEGA128 1

#define F_CPU 	16000000UL


#ifndef ALIBC_OLD
	#include <util/delay.h>
#else
	#include <avr/delay.h>
#endif

