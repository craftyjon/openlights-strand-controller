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

#include "globals.h"
#include "usb.h"
#include "commands.h"
#include "pins.h"


void process_usb(uint8_t byte)
{
	PORTA.OUTTGL = (1<<6);	// ACT

	switch (g_cmdState) {
		case STATE_IDLE:
			// Wait for start sequence
			if (byte == USB_START_BYTE) {
				g_cmdState = STATE_ADDRESS;					
			}
			break;

		case STATE_ADDRESS:
			if ((byte == 0x00) || byte == g_address) {
				g_usbDataCount = 0;
				g_cmdState = STATE_START;
			} else {
				g_cmdState = STATE_IDLE;
				// TODO: It might be better to read in the data length and know
				// to ignore the next N bytes
			}
			break;

		case STATE_START:
			PORTA.OUTTGL = (1<<7);  // DATA
			if (g_usbDataCount == 0) {
				g_usbCommand = byte;
				g_usbDataCount++;
			} else if (g_usbDataCount == 1) {
				g_usbDataLength = byte;
				g_usbDataCount++;
			} else {
				g_usbDataLength += (byte << 8);
				if (g_usbDataLength > (USB_BUFFER_SIZE - 1)) {
					g_usbDataLength = USB_BUFFER_SIZE - 1;
				}
				
				g_usbDataCount = 0;
				g_cmdState = STATE_RECEIVE;
			}
			break;

		case STATE_RECEIVE:
			if (g_usbDataCount < g_usbDataLength) {
				g_usbDataBuffer[g_usbDataCount] = byte;
				g_usbDataCount++;
			} else {
				g_cmdState = STATE_CHECKSUM;
				
				if (usb_checksum(byte)) {
					process_command();
				}
				g_cmdState = STATE_IDLE;
			}
			
			break;

		case STATE_CHECKSUM:
			if (usb_checksum(byte)) {
				process_command();
			}

			g_cmdState = STATE_IDLE;
			break;
	}
}


bool usb_checksum(uint8_t input)
{
	uint8_t calc = 0;
	uint16_t i;
	
	if (g_usbDataLength == 1) {
		return (input == g_usbDataBuffer[0]);
	}
	
	for (i = 0; i < g_usbDataLength; i++) {
		calc ^= g_usbDataBuffer[i];
	}
	
	return (input == calc);
}