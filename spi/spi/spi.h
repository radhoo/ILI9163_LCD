/*
** Basic SPI interface for the Atmega128
** Version: 	0.1.0
** Date: 		August, 2015
** License:		LGPL v3
** Copyright: 	(C) 2009 - 2015 Radu Motisan, radu.motisan@gmail.com
** Description:	spi interface
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


#pragma once

#include "../config.h"
#include <avr/io.h>



#if ATMEGA8
	#define SPI_DDR DDRB
	#define SPI_MOSI PB3
	#define SPI_MISO PB4
	#define SPI_SCK PB5
#elif ATMEGA128
	#define SPI_DDR DDRB
	#define SPI_PORT PORTB
	#define SPI_SCK PB1
	#define SPI_MOSI PB2
	//#define SPI_MISO PB3
#endif

/**
 * set spi speed and settings
 */
void spi_init(void);

/**
 * send spi data
 */
void spi_send(uint8_t data);
