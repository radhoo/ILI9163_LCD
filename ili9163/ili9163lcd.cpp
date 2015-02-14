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



// Reset the LCD hardware
void ILI9163::reset(void) {
    // Reset pin is active low (0 = reset, 1 = ready)
	m_reset->write(0);
    _delay_ms(50);

    m_reset->write(1);
    _delay_ms(120);
}

void ILI9163::writeCommand(uint8_t address) {
    uint8_t i;
    m_cs->write(0);
    m_a0->write(0);
    for(i=0;i<8;i++) {
        if(address & 128) m_sda->write(1); else m_sda->write(0);
        m_sck->write(1);
        address <<= 1;
        m_sck->write(0);
    }
    _delay_us(1);
    m_cs->write(1);
}

void ILI9163::writeParameter(uint8_t parameter) {
    uint8_t i;

    m_cs->write(0);
    m_a0->write(1);
    for(i=0;i<8;i++) {
        if(parameter & 128) m_sda->write(1); else m_sda->write(0);
        m_sck->write(1);
        parameter <<= 1;
        m_sck->write(0);
    }
    //_delay_us(1);
    m_cs->write(1);
}

void ILI9163::writeData(uint16_t word) {
	m_cs->write(0);
	m_a0->write(1);
	for (int i=0;i<16;i++) {
		if (word & 0x8000) m_sda->write(1); else m_sda->write(0);
		m_sck->write(1);
		m_sck->write(0);
		word <<=1;
	}
	m_cs->write(1);
}

// Initialise the display with the require screen orientation
void ILI9163::init(uint8_t orientation) {
    m_cs->write(1);
    m_sck->write(0);
    m_reset->write(1);
   
    // Hardware reset the LCD
    reset();
    
    writeCommand(EXIT_SLEEP_MODE);
    _delay_ms(5); // Wait for the screen to wake up
    
    writeCommand(SET_PIXEL_FORMAT);
    writeParameter(0x05); // 16 bits per pixel
   
    writeCommand(SET_GAMMA_CURVE);
    writeParameter(0x04); // Select gamma curve 3
    
    writeCommand(GAM_R_SEL);
    writeParameter(0x01); // Gamma adjustment enabled
    
    writeCommand(POSITIVE_GAMMA_CORRECT);
    writeParameter(0x3f); // 1st Parameter
    writeParameter(0x25); // 2nd Parameter
    writeParameter(0x1c); // 3rd Parameter
    writeParameter(0x1e); // 4th Parameter
    writeParameter(0x20); // 5th Parameter
    writeParameter(0x12); // 6th Parameter
    writeParameter(0x2a); // 7th Parameter
    writeParameter(0x90); // 8th Parameter
    writeParameter(0x24); // 9th Parameter
    writeParameter(0x11); // 10th Parameter
    writeParameter(0x00); // 11th Parameter
    writeParameter(0x00); // 12th Parameter
    writeParameter(0x00); // 13th Parameter
    writeParameter(0x00); // 14th Parameter
    writeParameter(0x00); // 15th Parameter
     
    writeCommand(NEGATIVE_GAMMA_CORRECT);
    writeParameter(0x20); // 1st Parameter
    writeParameter(0x20); // 2nd Parameter
    writeParameter(0x20); // 3rd Parameter
    writeParameter(0x20); // 4th Parameter
    writeParameter(0x05); // 5th Parameter
    writeParameter(0x00); // 6th Parameter
    writeParameter(0x15); // 7th Parameter
    writeParameter(0xa7); // 8th Parameter
    writeParameter(0x3d); // 9th Parameter
    writeParameter(0x18); // 10th Parameter
    writeParameter(0x25); // 11th Parameter
    writeParameter(0x2a); // 12th Parameter
    writeParameter(0x2b); // 13th Parameter
    writeParameter(0x2b); // 14th Parameter
    writeParameter(0x3a); // 15th Parameter
    
    writeCommand(FRAME_RATE_CONTROL1);
    writeParameter(0x08); // DIVA = 8
    writeParameter(0x08); // VPA = 8
    
    writeCommand(DISPLAY_INVERSION);
    writeParameter(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)
   
    writeCommand(POWER_CONTROL1);
    writeParameter(0x0a); // VRH = 10:  GVDD = 4.30
    writeParameter(0x02); // VC = 2: VCI1 = 2.65
      
    writeCommand(POWER_CONTROL2);
    writeParameter(0x02); // BT = 2: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 5xVCI1, VGL = -2xVCI1

    writeCommand(VCOM_CONTROL1);
    writeParameter(0x50); // VMH = 80: VCOMH voltage = 4.5
    writeParameter(0x5b); // VML = 91: VCOML voltage = -0.225
    
    writeCommand(VCOM_OFFSET_CONTROL);
    writeParameter(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML
    
    writeCommand(SET_COLUMN_ADDRESS);
    writeParameter(0x00); // XSH
    writeParameter(0x00); // XSL
    writeParameter(0x00); // XEH
    writeParameter(0x7f); // XEL (128 pixels x)
   
    writeCommand(SET_PAGE_ADDRESS);
    writeParameter(0x00);
    writeParameter(0x00);
    writeParameter(0x00);
    writeParameter(0x7f); // 128 pixels y
    
    // Select display orientation
    writeCommand(SET_ADDRESS_MODE);
    writeParameter(orientation);

    // Set the display to on
    writeCommand(SET_DISPLAY_ON);
    writeCommand(WRITE_MEMORY_START);
}

// LCD graphics functions -----------------------------------------------------------------------------------

void ILI9163::clear(uint16_t colour) {
    uint16_t pixel;
  
    // Set the column address to 0-127
    writeCommand(SET_COLUMN_ADDRESS);
    writeParameter(0x00);
    writeParameter(0x00);
    writeParameter(0x00);
    writeParameter(0x7f);

    // Set the page address to 0-127
    writeCommand(SET_PAGE_ADDRESS);
    writeParameter(0x00);
    writeParameter(0x00);
    writeParameter(0x00);
    writeParameter(0x7f);
  
    // Plot the pixels
    writeCommand(WRITE_MEMORY_START);
    for(pixel = 0; pixel < 16385; pixel++) writeData(colour);
}

void ILI9163::drawPixel(uint8_t x, uint8_t y, uint16_t colour) {
    // Horizontal Address Start Position
    writeCommand(SET_COLUMN_ADDRESS);
    writeParameter(0x00);
    writeParameter(x);
    writeParameter(0x00);
    writeParameter(0x7f);
  
    // Vertical Address end Position
    writeCommand(SET_PAGE_ADDRESS);
    writeParameter(0x00);
    writeParameter(y);
    writeParameter(0x00);
    writeParameter(0x7f);

    // Plot the point
    writeCommand(WRITE_MEMORY_START);
    writeData(colour);
}

// Draw a line from x0, y0 to x1, y1
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
void ILI9163::drawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour) {
    drawLine(x0, y0, x0, y1, colour);
    drawLine(x0, y1, x1, y1, colour);
    drawLine(x1, y0, x1, y1, colour);
    drawLine(x0, y0, x1, y0, colour);
}

// Draw a filled rectangle
// Note:    y1 must be greater than y0  and x1 must be greater than x0
//          for this to work
void ILI9163::drawRectFilled(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour) {
    uint16_t pixels;
            
    // To speed up plotting we define a x window with the width of the 
    // rectangle and then just output the required number of bytes to
    // fill down to the end point
    
    writeCommand(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
    writeParameter(0x00);
    writeParameter(x0);
    writeParameter(0x00);
    writeParameter(x1);
  
    writeCommand(SET_PAGE_ADDRESS); // Vertical Address end Position
    writeParameter(0x00);
    writeParameter(y0);
    writeParameter(0x00);
    writeParameter(0x7f);
        
    writeCommand(WRITE_MEMORY_START);
    
    for (pixels = 0; pixels < (((x1 - x0) + 1) * ((y1 - y0) + 1)); pixels++)
        writeData(colour);
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

// LCD text manipulation functions --------------------------------------------------------------------------

// Plot a character at the specified x, y co-ordinates (top left hand corner of character): with the current font the screen allows up to 21x16 characters
void ILI9163::drawChar(unsigned char character, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour) {
    uint8_t row, column;
    
    // To speed up plotting we define a x window of 6 pixels and then
    // write out one row at a time.  This means the LCD will correctly
    // update the memory pointer saving us a good few bytes
    
    writeCommand(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
    writeParameter(0x00);
    writeParameter(x);
    writeParameter(0x00);
    writeParameter(x+5);
  
    writeCommand(SET_PAGE_ADDRESS); // Vertical Address end Position
    writeParameter(0x00);
    writeParameter(y);
    writeParameter(0x00);
    writeParameter(0x7f);
        
    writeCommand(WRITE_MEMORY_START);
    
    // Plot the font data
    for (row = 0; row < 8; row++) {
        for (column = 0; column < 6; column++) {
            //if ((font5x8[character][column]) & (1 << row)) // using PROGMEM instead
        	if (pgm_read_byte(&font5x8[character][column]) & (1 << row))
        	    writeData(fgColour);
            else
            	writeData(bgColour);
        }
    }
}

// Plot a string of characters to the LCD: with the current font the screen allows up to 21x16 characters
void ILI9163::drawString(const char *string, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour) {
    uint8_t origin = x;

    for (uint8_t characterNumber = 0; characterNumber < strlen(string); characterNumber++) {
        // Check if we are out of bounds and move to 
        // the next line if we are
        if (x > 121) {
            x = origin;
            y += 8;
        }

        // If we move past the bottom of the screen just exit
        if (y > 120) break;

        // Plot the current character
        drawChar(string[characterNumber], x, y, fgColour, bgColour);
        x += 6;
    }
}
