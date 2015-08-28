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
#include "ili9163lcd.h"
#include "../spi/spi.h"

// Initialise the display with the require screen orientation
void ILI9163::init(Rotation rotation) {

	width = ILI9163_WIDTH;
	height = ILI9163_HEIGHT;

    m_cs->write(1);
    m_reset->write(1);
   
    // Hardware reset the LCD
    reset();
    
    writeCommand(ILI9163_CMD_EXIT_SLEEP_MODE);
    _delay_ms(5); // Wait for the screen to wake up
    
    writeCommand(ILI9163_CMD_SET_PIXEL_FORMAT);
    writeData(0x05); // 16 bits per pixel
   
    writeCommand(ILI9163_CMD_SET_GAMMA_CURVE);
    writeData(0x04); // Select gamma curve 3
    
    writeCommand(ILI9163_CMD_GAM_R_SEL);
    writeData(0x01); // Gamma adjustment enabled
    
    writeCommand(ILI9163_CMD_POSITIVE_GAMMA_CORRECT);
    writeData(0x3f); // 1st Parameter
    writeData(0x25); // 2nd Parameter
    writeData(0x1c); // 3rd Parameter
    writeData(0x1e); // 4th Parameter
    writeData(0x20); // 5th Parameter
    writeData(0x12); // 6th Parameter
    writeData(0x2a); // 7th Parameter
    writeData(0x90); // 8th Parameter
    writeData(0x24); // 9th Parameter
    writeData(0x11); // 10th Parameter
    writeData(0x00); // 11th Parameter
    writeData(0x00); // 12th Parameter
    writeData(0x00); // 13th Parameter
    writeData(0x00); // 14th Parameter
    writeData(0x00); // 15th Parameter
     
    writeCommand(ILI9163_CMD_NEGATIVE_GAMMA_CORRECT);
    writeData(0x20); // 1st Parameter
    writeData(0x20); // 2nd Parameter
    writeData(0x20); // 3rd Parameter
    writeData(0x20); // 4th Parameter
    writeData(0x05); // 5th Parameter
    writeData(0x00); // 6th Parameter
    writeData(0x15); // 7th Parameter
    writeData(0xa7); // 8th Parameter
    writeData(0x3d); // 9th Parameter
    writeData(0x18); // 10th Parameter
    writeData(0x25); // 11th Parameter
    writeData(0x2a); // 12th Parameter
    writeData(0x2b); // 13th Parameter
    writeData(0x2b); // 14th Parameter
    writeData(0x3a); // 15th Parameter
    
    writeCommand(ILI9163_CMD_FRAME_RATE_CONTROL1);
    writeData(0x08); // DIVA = 8
    writeData(0x08); // VPA = 8
    
    writeCommand(ILI9163_CMD_DISPLAY_INVERSION);
    writeData(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)
   
    writeCommand(ILI9163_CMD_POWER_CONTROL1);
    writeData(0x0a); // VRH = 10:  GVDD = 4.30
    writeData(0x02); // VC = 2: VCI1 = 2.65
      
    writeCommand(ILI9163_CMD_POWER_CONTROL2);
    writeData(0x02); // BT = 2: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 5xVCI1, VGL = -2xVCI1

    writeCommand(ILI9163_CMD_VCOM_CONTROL1);
    writeData(0x50); // VMH = 80: VCOMH voltage = 4.5
    writeData(0x5b); // VML = 91: VCOML voltage = -0.225
    
    writeCommand(ILI9163_CMD_VCOM_OFFSET_CONTROL);
    writeData(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML
    
    writeCommand(ILI9163_CMD_SET_COLUMN_ADDRESS);
    writeData(0x00); // XSH
    writeData(0x00); // XSL
    writeData(0x00); // XEH
    writeData(0x7f); // XEL (128 pixels x)
   
    writeCommand(ILI9163_CMD_SET_PAGE_ADDRESS);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0x7f); // 128 pixels y
    
    // Select display orientation
    //writeCommand(ILI9163_CMD_SET_ADDRESS_MODE);
    //writeData(rotation);
    setRotation(rotation);

    // Set the display to on
    writeCommand(ILI9163_CMD_SET_DISPLAY_ON);
    writeCommand(ILI9163_CMD_WRITE_MEMORY_START);
}

void ILI9163::writeCommand(uint8_t address) {
    uint8_t i;
    m_cs->write(0);
    // set D/C pin for command
    m_a0->write(0);
    // send data by SPI
    spi_send(address);
    m_cs->write(1);
}

void ILI9163::writeData(uint8_t data) {
    m_cs->write(0);
    // set D/C pin for data
    m_a0->write(1);
    // send data by SPI
    spi_send(data);
    m_cs->write(1);
}

void ILI9163::writeData16(uint16_t word) {
	m_cs->write(0);
	// set D/C pin for data
	m_a0->write(1);
	// send data by SPI
	spi_send((word >> 8) & 0x00FF);
	spi_send(word & 0x00FF);
	m_cs->write(1);
}

//set coordinate for print or other function
void ILI9163::setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2) {
	writeCommand(ILI9163_CMD_SET_COLUMN_ADDRESS);
	writeData16(x1);
	writeData16(x2);

	writeCommand(ILI9163_CMD_SET_PAGE_ADDRESS);
	writeData16(y1);
	writeData16(y2);
	// memory write
	writeCommand(ILI9163_CMD_WRITE_MEMORY_START);
}

// Reset the LCD hardware
void ILI9163::reset(void) {
    // Reset pin is active low (0 = reset, 1 = ready)
	m_reset->write(0);
    _delay_ms(50);

    m_reset->write(1);
    _delay_ms(120);
}

void ILI9163::drawPixel(uint8_t x, uint8_t y, uint16_t colour) {
	if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) return;
	setAddress(x, y, x + 1, y + 1);
	writeData16(colour);
}
void ILI9163::drawPixel(uint8_t x, uint8_t y, uint8_t size, uint16_t colour) {
	if (size == 1) // default size or big size
		drawPixel(x, y, colour);
	else
		drawRectFilled(x, y, size, size, colour);
}

// Draw a line
// Note:    This is a version of Bresenham's line drawing algorithm
//          It only draws lines from left to right!
void ILI9163::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour) {
    int16_t dy = y1 - y0;
    int16_t dx = x1 - x0;
    int16_t stepx, stepy;

    if (dy < 0) {
        dy = -dy; stepy = -1; 
    }
    else stepy = 1; 

    if (dx < 0) {
        dx = -dx; stepx = -1; 
    }
    else stepx = 1; 

    dy <<= 1;                           // dy is now 2*dy
    dx <<= 1;                           // dx is now 2*dx
 
    drawPixel(x0, y0, colour);

    if (dx > dy) {
        int fraction = dy - (dx >> 1);  // same as 2*dy - dx
        while (x0 != x1) {
            if (fraction >= 0) {
                y0 += stepy;
                fraction -= dx;         // same as fraction -= 2*dx
            }

            x0 += stepx;
            fraction += dy;                 // same as fraction -= 2*dy
            drawPixel(x0, y0, colour);
        }
    } else {
        int fraction = dx - (dy >> 1);
        while (y0 != y1) {
            if (fraction >= 0) {
                x0 += stepx;
                fraction -= dy;
            }

            y0 += stepy;
            fraction += dx;
            drawPixel(x0, y0, colour);
        }
    }
}

// Draw a rectangle between x0, y0 and x1, y1
void ILI9163::drawRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour) {
	// two horizontal lines
	drawLine(x, y, x + w, y, colour);
	drawLine(x, y + h, x + w, y + h, colour);
	// two vertical lines
	drawLine(x, y, x, y + h, colour);
	drawLine(x + w, y, x + w, y + h, colour);
}

//draw colour filled rectangle
void ILI9163::drawRectFilled(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour) {
	if ((x >= width) || (y >= height)) return;
	if ((x + w - 1) >= width) w = width - x;
	if ((y + h - 1) >= height) h = height - y;
	setAddress(x, y, x + w - 1, y + h - 1);
	for (y = h; y > 0; y--)
		for (x = w; x > 0; x--)
			writeData16(colour);
}

// Draw a circle
// Note:    This is another version of Bresenham's line drawing algorithm.
//          There's plenty of documentation on the web if you are curious
//          how this works.
void ILI9163::drawCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour) {
    int16_t x = 0, y = radius;
    int16_t d = 3 - (2 * radius);
 
    while(x <= y) {
        drawPixel(xCentre + x, yCentre + y, colour);
        drawPixel(xCentre + y, yCentre + x, colour);
        drawPixel(xCentre - x, yCentre + y, colour);
        drawPixel(xCentre + y, yCentre - x, colour);
        drawPixel(xCentre - x, yCentre - y, colour);
        drawPixel(xCentre - y, yCentre - x, colour);
        drawPixel(xCentre + x, yCentre - y, colour);
        drawPixel(xCentre - y, yCentre + x, colour);

        if (d < 0)
        	d += (4 * x) + 6;
        else {
            d += (4 * (x - y)) + 10;
            y -= 1;
        }

        x++;
    }
}

//rotate screen at desired orientation
void ILI9163::setRotation(Rotation rotation) {
	writeCommand(ILI9163_CMD_SET_ADDRESS_MODE);
	writeData(rotation);
}

// LCD graphics functions -----------------------------------------------------------------------------------

void ILI9163::drawClear(uint16_t colour) {
	drawRectFilled(0,0,width, height, colour);
}


// LCD text manipulation functions --------------------------------------------------------------------------
void ILI9163::drawChar( uint8_t x, uint8_t y, char c, uint8_t size, uint16_t colour, uint16_t bg) {
	// draw. optimisation:6th font line is set as 0, to lower font array size
	for (int8_t i=0; i < FONT_WIDTH; i++ ) {
		uint8_t line = (i == FONT_WIDTH-1)? 0 : pgm_read_byte(font5x8 + (c * (FONT_WIDTH - 1)) + i);
		for (int8_t j = 0; j < FONT_HEIGHT; j++) {
			if (line & 0x1) {
				if (colour != TRANSPARENT) drawPixel(x + i*size, y + j*size, size, colour);
			}
			else {
				if (bg != TRANSPARENT) drawPixel(x + i*size, y + j*size, size, bg);
			}
			line >>= 1;
		}
	}
}

// Plot a string of characters to the LCD: with the current font the screen allows up to 21x16 characters
void ILI9163::drawString(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg,  const char *string) {
	if (x == CENTERX) {
		x = (width - size * FONT_WIDTH * strlen(string)) / 2;
	}
	if (y == CENTERY) {
		y = (height - size * FONT_HEIGHT) / 2;
	}
    unsigned char c;
    while ( (c = *string++) ) {
		if (c == '\r') continue;
		if (c != '\n') drawChar(x, y, c, size, colour, bg);
		// new line check
        if(c == '\n' || x > width - 2 * FONT_WIDTH * size) {
        	x = 0;
			y += size * FONT_HEIGHT;
        } else {
        	x += size * FONT_WIDTH;
        }
    }
}

void ILI9163::drawStringF(uint16_t x, uint16_t y, uint8_t size, uint16_t colour, uint16_t bg, char *szFormat, ...) {
    char szBuffer[256]; //in this buffer we form the message
    int NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
    int LASTCHAR = NUMCHARS - 1;
    va_list pArgs;
    va_start(pArgs, szFormat);
    vsnprintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
    va_end(pArgs);

    drawString(x, y, size, colour, bg, szBuffer);
}
