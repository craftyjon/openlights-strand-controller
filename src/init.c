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


// Setup I/O pins.
void init_pins(void)
{
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

	// SPI pins for strand data output
	// PC5 = strand data
	// PC7 = strand clock
	PORTC.DIRSET = (1<<5) | (1<<7);
	PORTC.PIN5CTRL = PORT_OPC_PULLUP_gc;
	PORTC.PIN7CTRL = PORT_OPC_PULLUP_gc;
}


// Setup global variables needed before communications starts
void init_globals(void)
{
	num_leds = 8;

	// Init board address
	g_address = 0x00;
	
	if (!ioport_get_pin_level(SW0)) {
		g_address |= (1<<0);
	}
	
	if (!ioport_get_pin_level(SW1)) {
		g_address |= (1<<1);
	}
	
	if (!ioport_get_pin_level(SW2)) {
		g_address |= (1<<2);
	}
	
	if (!ioport_get_pin_level(SW3)) {
		g_address |= (1<<3);
	}

	// Init byte-oriented communications (via USB or RS485)
	g_cmdState = STATE_IDLE;
	g_usbDataLength = 0;
	g_usbDataCount = 0;
	g_usbConnected = 0;
	g_rs485data = 0;
	g_rs485rdy = 0;

	// Init strand data buffers
	memset(&data_buffer, 0, sizeof(data_buffer));
	dirty = 1;
}


// RS485 is used for board-to-board communications
void init_rs485(void)
{
	struct usart_rs232_options usart_params;

	usart_params.baudrate = RS485_BIT_RATE;
	usart_params.charlength = USART_CHSIZE_8BIT_gc;
	usart_params.paritytype = USART_PMODE_DISABLED_gc;
	usart_params.stopbits = false;

	usart_init_rs232(&USARTC0, &usart_params);
	usart_set_rx_interrupt_level(&USARTC0, USART_INT_LVL_MED);
	usart_rx_enable(&USARTC0);
}


// SPI is used for strand data output
void init_strand(void)
{
	struct spi_device dummy_device = {
		.id = IOPORT_CREATE_PIN(PORTA, 1)
	};

	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &dummy_device, SPI_MODE_0, STRAND_BIT_RATE, 0);
	spi_enable(&SPIC);
}


// USB CDC class (i.e. virtual serial port) used for Controller
void init_usb(void)
{
	udc_start();
	udc_attach();
}