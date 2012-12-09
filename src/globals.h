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

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "asf.h"
#include "usb.h"

#ifdef GLOBALS_ALLOC
#define EXTERN extern
#else
#define EXTERN
#endif


/* Board metadata */

#define FW_REV_MAJOR 1
#define FW_REV_MINOR 0

EXTERN uint8_t g_address;
EXTERN uint8_t g_usartData;
EXTERN uint64_t ticks;


/* LEDs */
#define MAX_LEDS 192
EXTERN uint16_t num_leds;

EXTERN uint16_t num_bytes;
EXTERN uint8_t data_buffer[3 * MAX_LEDS];
//EXTERN uint8_t back_buffer[3 * MAX_LEDS];
EXTERN uint8_t (*data_pointer)[3 * MAX_LEDS];

EXTERN volatile uint8_t dirty;


/* USB */
#define USB_BUFFER_SIZE (10 + (MAX_LEDS * 3))
#define USB_START_BYTE 0x99

EXTERN uint8_t g_cmdState;
EXTERN uint8_t g_usbCommand;
EXTERN uint16_t g_usbDataLength;
EXTERN uint16_t g_usbDataCount;
EXTERN uint8_t g_usbDataBuffer[10 + (MAX_LEDS * 3)];

#endif
