#!/bin/sh
echo "*** writing fuses for 8MHz external crystal"
avrdude -p atmega128 -c usbasp -U lfuse:w:0xdc:m 	-U hfuse:w:0x9f:m 	-U efuse:w:0xff:m 
echo "*** writting flash"
avrdude -p atmega128 -c usbasp -U flash:w:lcd_ili9163.hex:i
