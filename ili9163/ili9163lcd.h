/**
 * @file ili9163lcd.h
 * @brief ILI9163 128x128 LCD Driver (Header file) , 1.44" Serial 128X128 SPI Color TFT LCD Module 16bit Display (RGB16)
 *
 * This code has been ported from the ili9163lcd library for mbed made by Jun Morita.
 *
 * This code has been ported from the ili9163lcd library for avr made
 * by Simon Inns, to run on a msp430.
 *
 * This code has been optimised and ported to C++ as a class
 * by Radu Motisan, radu.motisan@gmail.com , www.pocketmagic.net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Jun Morita (iccraft)
 * @author Simon Inns <simon.inns@gmail.com>
 * @author Christopher Vagnetoft (NoccyLabs)
 * @author Radu Motisan <radu.motisan@gmail.com>
 * @copyright (C) 2012 Simon Inns
 * @copyright parts (C) 2012 NoccyLabs
 * @copyright parts (C) 2015 pocketmagic.net
 */

#pragma once

#include <string.h>
#include "font5x8.h"
// some local libs
#include "../digitalpin/DigitalPin.h"
#include "../timeout.h"



// Definitions for control lines (port C)
#define LCD_WR      (1 << 2)
#define LCD_RS      (1 << 4)
#define LCD_RD      (1 << 5)
#define LCD_CS      (1 << 6)
#define LCD_RESET   (1 << 7)

// ILI9163 LCD Controller Commands
#define NOP                     0x00
#define SOFT_RESET              0x01
#define GET_RED_CHANNEL         0x06
#define GET_GREEN_CHANNEL       0x07
#define GET_BLUE_CHANNEL        0x08
#define GET_PIXEL_FORMAT        0x0C
#define GET_POWER_MODE          0x0A
#define GET_ADDRESS_MODE        0x0B
#define GET_DISPLAY_MODE        0x0D
#define GET_SIGNAL_MODE         0x0E
#define GET_DIAGNOSTIC_RESULT   0x0F
#define ENTER_SLEEP_MODE        0x10
#define EXIT_SLEEP_MODE         0x11
#define ENTER_PARTIAL_MODE      0x12
#define ENTER_NORMAL_MODE       0x13
#define EXIT_INVERT_MODE        0x20
#define ENTER_INVERT_MODE       0x21
#define SET_GAMMA_CURVE         0x26
#define SET_DISPLAY_OFF         0x28
#define SET_DISPLAY_ON          0x29
#define SET_COLUMN_ADDRESS      0x2A
#define SET_PAGE_ADDRESS        0x2B
#define WRITE_MEMORY_START      0x2C
#define WRITE_LUT               0x2D
#define READ_MEMORY_START       0x2E
#define SET_PARTIAL_AREA        0x30
#define SET_SCROLL_AREA         0x33
#define SET_TEAR_OFF            0x34
#define SET_TEAR_ON             0x35
#define SET_ADDRESS_MODE        0x36
#define SET_SCROLL_START        0X37
#define EXIT_IDLE_MODE          0x38
#define ENTER_IDLE_MODE         0x39
#define SET_PIXEL_FORMAT        0x3A
#define WRITE_MEMORY_CONTINUE   0x3C
#define READ_MEMORY_CONTINUE    0x3E
#define SET_TEAR_SCANLINE       0x44
#define GET_SCANLINE            0x45
#define READ_ID1                0xDA
#define READ_ID2                0xDB
#define READ_ID3                0xDC
#define FRAME_RATE_CONTROL1     0xB1
#define FRAME_RATE_CONTROL2     0xB2
#define FRAME_RATE_CONTROL3     0xB3
#define DISPLAY_INVERSION       0xB4
#define SOURCE_DRIVER_DIRECTION 0xB7
#define GATE_DRIVER_DIRECTION   0xB8
#define POWER_CONTROL1          0xC0
#define POWER_CONTROL2          0xC1
#define POWER_CONTROL3          0xC2
#define POWER_CONTROL4          0xC3
#define POWER_CONTROL5          0xC4
#define VCOM_CONTROL1           0xC5
#define VCOM_CONTROL2           0xC6
#define VCOM_OFFSET_CONTROL     0xC7
#define WRITE_ID4_VALUE         0xD3
#define NV_MEMORY_FUNCTION1     0xD7
#define NV_MEMORY_FUNCTION2     0xDE
#define POSITIVE_GAMMA_CORRECT  0xE0
#define NEGATIVE_GAMMA_CORRECT  0xE1
#define GAM_R_SEL               0xF2

// RGB24 to RGB16: Translates a 3 byte RGB value into a 2 byte value for the LCD (values should be 0-31)
// Radu Motisan: we need to scale the 8bit values to 5bit (for blue or red) / 6bit (for green)
inline uint16_t rgb24to16(uint8_t r, uint8_t g, uint8_t b) {
	//return ((b/8) << 11) | ((g/4) << 5) | (r/8);
	return ((b>>3) << 11) | ((g>>2) << 5) | (r>>3);
	/*
	 	 return 	((b << 8) & 0xF800) | // 5 bits for blue : most significant
			((g << 3) & 0x7E0) | // 6 bits for green : most significant
			(r >> 3);	// 5 bits for red : most significant
			*/
}

// This routine takes a row number from 0 to 20 and returns the x coordinate on the screen (0-127)
inline uint8_t textX(uint8_t x) { return x*6; }

// This routine takes a column number from 0 to 20 and returns the y coordinate on the screen (0-127)
inline uint8_t textY(uint8_t y) { return y*8; }

class ILI9163 {
	// configure data interface
	DigitalPin *m_sck, *m_sda, *m_a0, *m_reset, *m_cs;

public:
	enum ORIENTATION {
		TOP = 0, LEFT = 96, RIGHT = 160, BOTTOM = 192
	};

	// configure the pins used to connect to this LCD
	inline void setup(DigitalPin *sck, DigitalPin *sda, DigitalPin *a0, DigitalPin *reset, DigitalPin *cs) {
		m_sck = sck; m_sda = sda; m_a0 = a0; m_reset = reset; m_cs = cs;
	}

	// Low-level LCD driving functions

	// Reset the LCD hardware
	void reset(void);
	void writeCommand(uint8_t address);
	void writeParameter(uint8_t parameter);
	//void writeData(uint8_t dataByte1, uint8_t dataByte2);
	void writeData(uint16_t word);
	void init(uint8_t orientation);

	void clear(uint16_t colour);
	void drawPixel(uint8_t x, uint8_t y, uint16_t colour);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour);
	void drawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour);
	void drawRectFilled(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour);
	void drawCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour);

	void drawChar(unsigned char character, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour);
	void drawString(const char *string, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour);


};



