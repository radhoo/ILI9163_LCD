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
#include <stdio.h>
#include "font5x8.h"
#include "colors.h"
// some local libs
#include "../digitalpin/DigitalPin.h"
#include "../config.h"

// define size
#define ILI9163_HEIGHT 						128
#define ILI9163_WIDTH 						128


// ILI9163 LCD Controller Commands
#define ILI9163_CMD_NOP                     0x00
#define ILI9163_CMD_SOFT_RESET              0x01
#define ILI9163_CMD_GET_RED_CHANNEL         0x06
#define ILI9163_CMD_GET_GREEN_CHANNEL       0x07
#define ILI9163_CMD_GET_BLUE_CHANNEL        0x08
#define ILI9163_CMD_GET_PIXEL_FORMAT        0x0C
#define ILI9163_CMD_GET_POWER_MODE          0x0A
#define ILI9163_CMD_GET_ADDRESS_MODE        0x0B
#define ILI9163_CMD_GET_DISPLAY_MODE        0x0D
#define ILI9163_CMD_GET_SIGNAL_MODE         0x0E
#define ILI9163_CMD_GET_DIAGNOSTIC_RESULT   0x0F
#define ILI9163_CMD_ENTER_SLEEP_MODE        0x10
#define ILI9163_CMD_EXIT_SLEEP_MODE         0x11
#define ILI9163_CMD_ENTER_PARTIAL_MODE      0x12
#define ILI9163_CMD_ENTER_NORMAL_MODE       0x13
#define ILI9163_CMD_EXIT_INVERT_MODE        0x20
#define ILI9163_CMD_ENTER_INVERT_MODE       0x21
#define ILI9163_CMD_SET_GAMMA_CURVE         0x26
#define ILI9163_CMD_SET_DISPLAY_OFF         0x28
#define ILI9163_CMD_SET_DISPLAY_ON          0x29
#define ILI9163_CMD_SET_COLUMN_ADDRESS      0x2A
#define ILI9163_CMD_SET_PAGE_ADDRESS        0x2B
#define ILI9163_CMD_WRITE_MEMORY_START      0x2C
#define ILI9163_CMD_WRITE_LUT               0x2D
#define ILI9163_CMD_READ_MEMORY_START       0x2E
#define ILI9163_CMD_SET_PARTIAL_AREA        0x30
#define ILI9163_CMD_SET_SCROLL_AREA         0x33
#define ILI9163_CMD_SET_TEAR_OFF            0x34
#define ILI9163_CMD_SET_TEAR_ON             0x35
#define ILI9163_CMD_SET_ADDRESS_MODE        0x36
#define ILI9163_CMD_SET_SCROLL_START        0X37
#define ILI9163_CMD_EXIT_IDLE_MODE          0x38
#define ILI9163_CMD_ENTER_IDLE_MODE         0x39
#define ILI9163_CMD_SET_PIXEL_FORMAT        0x3A
#define ILI9163_CMD_WRITE_MEMORY_CONTINUE   0x3C
#define ILI9163_CMD_READ_MEMORY_CONTINUE    0x3E
#define ILI9163_CMD_SET_TEAR_SCANLINE       0x44
#define ILI9163_CMD_GET_SCANLINE            0x45
#define ILI9163_CMD_READ_ID1                0xDA
#define ILI9163_CMD_READ_ID2                0xDB
#define ILI9163_CMD_READ_ID3                0xDC
#define ILI9163_CMD_FRAME_RATE_CONTROL1     0xB1
#define ILI9163_CMD_FRAME_RATE_CONTROL2     0xB2
#define ILI9163_CMD_FRAME_RATE_CONTROL3     0xB3
#define ILI9163_CMD_DISPLAY_INVERSION       0xB4
#define ILI9163_CMD_SOURCE_DRIVER_DIRECTION 0xB7
#define ILI9163_CMD_GATE_DRIVER_DIRECTION   0xB8
#define ILI9163_CMD_POWER_CONTROL1          0xC0
#define ILI9163_CMD_POWER_CONTROL2          0xC1
#define ILI9163_CMD_POWER_CONTROL3          0xC2
#define ILI9163_CMD_POWER_CONTROL4          0xC3
#define ILI9163_CMD_POWER_CONTROL5          0xC4
#define ILI9163_CMD_VCOM_CONTROL1           0xC5
#define ILI9163_CMD_VCOM_CONTROL2           0xC6
#define ILI9163_CMD_VCOM_OFFSET_CONTROL     0xC7
#define ILI9163_CMD_WRITE_ID4_VALUE         0xD3
#define ILI9163_CMD_NV_MEMORY_FUNCTION1     0xD7
#define ILI9163_CMD_NV_MEMORY_FUNCTION2     0xDE
#define ILI9163_CMD_POSITIVE_GAMMA_CORRECT  0xE0
#define ILI9163_CMD_NEGATIVE_GAMMA_CORRECT  0xE1
#define ILI9163_CMD_GAM_R_SEL               0xF2

// RGB24 to RGB16: Translates a 3 byte RGB value into a 2 byte value for the LCD (values should be 0-31)
// Radu Motisan: we need to scale the 8bit values to 5bit (for blue or red) / 6bit (for green)
inline uint16_t rgb24to16(uint8_t r, uint8_t g, uint8_t b) {
	return ((b>>3) << 11) | ((g>>2) << 5) | (r>>3);
}

// This routine takes a row number from 0 to 20 and returns the x coordinate on the screen (0-127)
inline uint16_t textX(uint8_t x, int size) { return x * size * FONT_WIDTH; }

// This routine takes a column number from 0 to 20 and returns the y coordinate on the screen (0-127)
inline uint16_t textY(uint8_t y, int size) { return y * size * FONT_HEIGHT; }

#define CENTERX -1
#define CENTERY -1

class ILI9163 {
	// configure data interface
	DigitalPin *m_a0, *m_reset, *m_cs;
	// the serial comm is done over SPI, so make sure you init SPI in your program

public:
	enum Rotation {
		ROT0 = 0,	// portrait
		ROT90 = 96,	// landscape
		ROT180 = 160,	// flipped portrait
		ROT270 = 192	// flipped landscape
	};

	uint16_t width, height;

	// configure the pins used to connect to this LCD
	// the SCK is SPI SCK and the SDA is SPI MOSI, these two pins will be controlled over SPI directly
	// the A0 is D/C: 0 for command, 1 for data
	ILI9163(DigitalPin *a0, DigitalPin *reset, DigitalPin *cs) {
		m_a0 = a0; m_reset = reset; m_cs = cs;
	}

	void init(Rotation rotation);

	void writeCommand(uint8_t address);
	void writeData(uint8_t data);
	void writeData16(uint16_t word);
	void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
	void reset(void);

	void drawPixel(uint8_t x, uint8_t y, uint16_t colour);
	void drawPixel(uint8_t x, uint8_t y, uint8_t size, uint16_t colour);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour);
	void drawRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour);
	void drawRectFilled(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour);
	void drawCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour);
	void drawClear(uint16_t colour);
	void setRotation(Rotation rotation);

	void drawChar( uint8_t x, uint8_t y, char c, uint8_t size, uint16_t colour, uint16_t bg);
	void drawString(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg,  const char *string);
	void drawStringF(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg, char *szFormat, ...);

};



