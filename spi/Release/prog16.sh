#!/bin/sh
#avrdude -v
echo "*** writing fuses for 16MHz external crystal"
avrdude -p atmega128 -c usbasp -U lfuse:w:0xCE:m 	-U hfuse:w:0x9F:m 	-U efuse:w:0xFF:m 
echo "*** writting flash"
#extra options -vvvv -B 1..4
/usr/local/AVRMacPack-20071126/bin/avrdude -p atmega128 -c usbasp -U flash:w:lcd_ili9163.hex:i
