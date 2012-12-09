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

#include <stdio.h>
#include <string.h>
#include <asf.h>
#include <util/delay.h>

#define GLOBALS_ALLOC
#include "globals.h"
#undef GLOBALS_ALLOC

#include "pins.h"
#include "usb.h"
#include "commands.h"

void spi_init_pins(void);
void spi_init_module(void);

struct spi_device dummy_device = {
	.id = IOPORT_CREATE_PIN(PORTA, 1)
};


static void strand_output_isr(void)
{
	if (dirty) {
		dirty = 0;
		spi_write_packet(&SPIC, data_buffer, (3 * num_leds));
		PORTA.OUTCLR = (1<<6);
	}
}


static void tick_isr(void)
{
	ticks++;
	
	// Refresh the strand periodically even if no new data
	if (g_cmdState == STATE_IDLE) {
		PORTA.OUTSET = (1<<6);
		PORTA.OUTCLR = (1<<7);
		dirty = 1;
	}
}


// Disable RS485 transmitter after successful send
ISR(USARTC0_DRE_vect)
{
	ioport_set_pin_low(RS485_DE);
	ioport_set_pin_low(RS485_REn);
}

ISR(USARTC0_RXC_vect)
{
	PORTA.OUTSET = (1<<6);
	g_usartData = USARTC0.DATA;
	process_usb(true);
}

void spi_init_pins(void)
{
	PORTC.DIRSET = (1<<4) | (1<<5) | (1<<7);
	PORTC.PIN4CTRL = PORT_OPC_PULLUP_gc;
	PORTC.PIN5CTRL = PORT_OPC_PULLUP_gc;
	PORTC.PIN7CTRL = PORT_OPC_PULLUP_gc;
}

void spi_init_module(void)
{
	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &dummy_device, SPI_MODE_0, 2500000, 0);
	spi_enable(&SPIC);
}


int main (void)
{
	sysclk_init();
	pmic_init();
	irq_initialize_vectors();
	cpu_irq_enable();

	struct usart_rs232_options usart_params;
	
	usart_params.baudrate = 115200;
	usart_params.charlength = USART_CHSIZE_8BIT_gc;
	usart_params.paritytype = USART_PMODE_DISABLED_gc;
	usart_params.stopbits = false;
	stdio_serial_init(&USARTE0, &usart_params);

	usart_params.baudrate = 2000000;
	usart_params.charlength = USART_CHSIZE_8BIT_gc;
	usart_params.paritytype = USART_PMODE_DISABLED_gc;
	usart_params.stopbits = false;
	usart_init_rs232(&USARTC0, &usart_params);

	usart_set_rx_interrupt_level(&USARTC0, USART_INT_LVL_MED);

	board_init();
	
	//printf("OpenLights Strand Controller\r\n");
	//printf("Firmware revision %d.%d", FW_REV_MAJOR, FW_REV_MINOR);
	
	spi_init_pins();
	spi_init_module();
	
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
	
	// Init USB communications
	g_cmdState = STATE_IDLE;
	g_usbDataLength = 0;
	g_usbDataCount = 0;
	//num_bytes = 3 * num_leds;
	
	udc_start();
	udc_attach();
	usart_rx_enable(&USARTC0);
	
	// Init LED data buffers
	dirty = 1;
	memset(&data_buffer, 0, sizeof(data_buffer));
	//memset(&back_buffer, 0, sizeof(back_buffer));
	spi_write_packet(&SPIC, data_buffer, sizeof(data_buffer));
	
	// Setup timer interrupt for strand output
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, strand_output_isr);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 2083);	// about 60 fps
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV64_gc);	// 125 khz
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_HI);
	
	
	// Low-frequency timer
	tc_enable(&TCC1);
	tc_set_overflow_interrupt_callback(&TCC1, tick_isr);
	tc_set_wgm(&TCC1, TC_WG_NORMAL);
	tc_write_period(&TCC1, 0xFFFF);
	tc_write_clock_source(&TCC1, TC_CLKSEL_DIV256_gc);
	tc_set_overflow_interrupt_level(&TCC1, TC_INT_LVL_LO);

	
	// run the "Identify" command at startup
	g_usbCommand = IDENTIFY;
	process_command();
	
	while (1) {
		if (!ioport_get_pin_level(SW5)) {	// Test mode
			for (uint16_t i = 0; i < 3 * num_leds; i++) {
				data_buffer[i] = 100;
			}
			dirty = 1;
		} else {
			
			if (udi_cdc_is_tx_ready()) {
				process_usb(false);
			}/* else if (usart_rx_is_complete(&USARTC0)) {
				//PORTA.OUTTGL = (1<<7);
				g_usartData = USARTC0.DATA;
				process_usb(true);
			}*/
		}		
	}
}
