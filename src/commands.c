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

#include <string.h>

#include "globals.h"
#include "commands.h"


void process_command()
{
	uint16_t i, j;

	switch (g_usbCommand) {
		case SET_ARRAY:
			printf("[CMD] Set Array\r\n");
			j = min(g_usbDataLength, (3 * num_leds));
			for (i = 0; i < j; i++) {
				data_buffer[i] = g_usbDataBuffer[i];
			}
			dirty = 1;
			break;

		case BLACKOUT:
			printf("[CMD] Blackout\r\n");
			memset(&data_buffer, 0, sizeof(data_buffer));
			dirty = 1;
			break;

		case SET_ALL:
			printf("[CMD] Set all\r\n");
			memset(&data_buffer, g_usbDataBuffer[0], sizeof(data_buffer));
			dirty = 1;
			break;
			
		case SET_ALL_RGB:
		printf("[CMD] Set All RGB\r\n");
			for (i = 0; i < (num_leds); i++) {
				data_buffer[3 * i] = g_usbDataBuffer[0];
				data_buffer[(3 * i) + 1] = g_usbDataBuffer[1];
				data_buffer[(3 * i) + 2] = g_usbDataBuffer[2];
			}
			dirty = 1;
			break;
		
		case SET_NUM_LEDS:
			num_leds = g_usbDataBuffer[0] + (g_usbDataBuffer[1] << 8);
			identify();
			break;
			
		case GET_NUM_LEDS:
			break;
			
		case IDENTIFY:
			printf("[CMD] Identify\r\n");
			identify();
			break;
	}
}


void identify(void)
{
	memset(&data_buffer, 0, sizeof(data_buffer));
	data_buffer[1] = 0xFF;
	data_buffer[(3 * (num_leds - 1)) + 2] = 0xFF;
	
	// (2) for start/stop, (4) for address
	if (num_leds > 6) {
		for (uint16_t i = 0; i < 6; i++) {
			if (g_address & (1<<i)) {
				data_buffer[3 + (3 * i)] = 0xcc;
				data_buffer[3 + (3 * i) + 1] = 0xcc;
				data_buffer[3 + (3 * i) + 2] = 0xcc;
			} else {
				data_buffer[3 + (3 * i)] = 0x55;
				data_buffer[3 + (3 * i) + 1] = 0x00;
				data_buffer[3 + (3 * i) + 2] = 0x00;
			}
		}
	}

	dirty = 1;
}