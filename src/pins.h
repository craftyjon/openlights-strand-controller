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

#ifndef PINS_H_
#define PINS_H_

#define SW0    IOPORT_CREATE_PIN(PORTB, 0)
#define SW1    IOPORT_CREATE_PIN(PORTB, 1)
#define SW2    IOPORT_CREATE_PIN(PORTB, 2)
#define SW3    IOPORT_CREATE_PIN(PORTB, 3)
#define SW4    IOPORT_CREATE_PIN(PORTC, 0)
#define SW5    IOPORT_CREATE_PIN(PORTC, 1)

#define RS485_REn	IOPORT_CREATE_PIN(PORTC, 6)
#define RS485_DE	IOPORT_CREATE_PIN(PORTC, 4)


#endif /* BOARD_H_ */