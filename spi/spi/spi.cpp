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

#include "spi.h"

/**
 * set spi speed and settings
 */
void spi_init(void) {
	/*SPI_DDR |= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_MISO); 	// set MOSI and SCK as output
	SPI_PORT|= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_MISO);
	SPCR |= (1<<SPE) | (1<<MSTR);
	SPSR = 0x1; // needed on mega128*/
	PORTB = 0b10110101;			/* Enable drivers */
	DDRB  = 0b11000111;
	SPCR = (1<<SPE) | (1<<MSTR);
	SPSR |= (1<<SPI2X);
}


/**
 * send spi data
 */
void spi_send(unsigned char data) {
	SPDR = data;//move data into spdr
	while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
}
