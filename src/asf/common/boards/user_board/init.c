/* Openlights Strand Controller (12009) Firmware
 * http://openlights.org
 * 
 * Copyright (c) 2012 Jonathan Evans <jon@craftyjon.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#include "pins.h"
#include "globals.h"

void board_init(void)
{
	num_leds = 8;
	ioport_enable_pin(SW0);
	ioport_enable_pin(SW1);
	ioport_enable_pin(SW2);
	ioport_enable_pin(SW3);
	ioport_enable_pin(SW4);
	ioport_enable_pin(SW5);
	ioport_enable_pin(RS485_REn);
	ioport_enable_pin(RS485_DE);	
	
	ioport_set_pin_mode(SW0, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(SW1, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(SW2, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(SW3, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(SW4, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(SW5, IOPORT_MODE_PULLUP);
	
	ioport_set_pin_dir(SW0, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SW1, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SW2, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SW3, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SW4, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SW5, IOPORT_DIR_INPUT);
	
	ioport_set_pin_dir(RS485_REn, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(RS485_DE, IOPORT_DIR_OUTPUT);
	
	ioport_set_pin_low(RS485_REn);	// Active low enable receive
	ioport_set_pin_low(RS485_DE);
	
	PORTA.DIRSET = (1<<6) | (1<<7);

	PORTC.DIRSET = PIN3_bm;
	PORTC.DIRCLR = PIN2_bm;
}
