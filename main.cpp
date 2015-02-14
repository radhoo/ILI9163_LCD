/*
 File:      main.cpp
 Version:   0.1 - first version
 Date:      February 12, 2015
 License:	GPL v2

 ILI9163 LCD Test code
 1.44" Serial 128X128 SPI Color TFT LCD Module Display

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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>
// local headers
#include "timeout.h" // defines F_CPU
#include "ili9163/ili9163lcd.h"
#include "image1.h"

FUSES =
{
	.low = 0xDC,		// external 8MHz Quartz crystal
	.high = 0x9F,
	.extended = 0xEF,
};

ILI9163 	lcd;
DigitalPin	pinSCK(&PORTA, PA2), pinSDA(&PORTA, PA3), pinA0(&PORTA, PA4), pinRESET(&PORTA, PA5), pinCS(&PORTA, PA6);

/************************************************************************************************************************************************/
/* Main entry point                                                    																			*/
/************************************************************************************************************************************************/
int main(void) {
	lcd.setup(&pinSCK, &pinSDA, &pinA0, &pinRESET, &pinCS);

	lcd.init(ILI9163::BOTTOM);

	// demo screen 1
	for (int i=0;i<21;i++)
			for (int j=0;j<16;j++)
				lcd.drawChar('0' + rand()%10, textX(i), textY(j), rgb24to16(0xC0,0xC0,0xC0), rgb24to16(0,0,0) );
	//21x16
	lcd.drawString("Hello World!", textX(4), textY(8), rgb24to16(0xFF, 0xFF, 0), rgb24to16(0, 0, 0xFF));
	lcd.drawString("www.pocketmagic.net", textX(1), textY(9), rgb24to16(0xFF, 0xFF, 0), rgb24to16(0xFF, 0, 0xFF));

	// demo image
	for (int y=0;y<128;y++)
		for (int x=0;x<128;x++)
			lcd.drawPixel(x,y, pgm_read_word(&image1[x+y*128]));
	_delay_ms(2000);

	// demo screen 2
	lcd.clear(rgb24to16(0, 0, 0));
	lcd.drawString("www.pocketmagic.net", textX(1), textY(15), rgb24to16(0xFF, 0xFF, 0), rgb24to16(0xFF, 0, 0xFF));
	// draw axis
	lcd.drawString("30*sin(x/2)*cos(x/8)", textX(0), textY(0), rgb24to16(0, 0xFF, 0), rgb24to16(0,0,0));
	lcd.drawChar('O', textX(10), textY(8), rgb24to16(0, 0xFF, 0), rgb24to16(0,0,0));
	lcd.drawLine(0,64,127,64,rgb24to16(0xFF,0xFF,0xFF));
	lcd.drawLine(64,0,64,127,rgb24to16(0xFF,0xFF,0xFF));
	// draw function
	int x0 = -64, y0 = 30*sin(0)*cos(0) + 64;
	for (int i=1; i<128;i++) {
		int x1 = -64+i, y1 = 30*sin(x1/2)*cos(x1/8) + 64;
		lcd.drawLine(i-1, y0, i, y1, rgb24to16(0xFF,0,0));
		x0 = x1; y0 = y1;
	}
}
