/* $CSK: lesson5.c,v 1.3 2009/05/17 06:22:44 ckuethe Exp $ */
/*
 * Copyright (c) 2008 Chris Kuethe <chris.kuethe@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <avr/io.h>
#include <util/delay.h>

void serial_init(unsigned int bittimer)
{
  /* Set the baud rate */
  UBRR0H = (unsigned char) (bittimer >> 8);
  UBRR0L = (unsigned char) bittimer;
  /* set the framing to 8N1 */
  UCSR0C = (3 << UCSZ00);
  /* Engage! */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  return;
}

unsigned char serial_read(void)
{
  while( !(UCSR0A & (1 << RXC0)) )
    ;
  return UDR0;
}

void serial_write(unsigned char c)
{
  while ( !(UCSR0A & (1 << UDRE0)) )
    ;
  UDR0 = c;
}

#define SPEED 9600
int main (void)
{
  /* let the preprocessor calculate this */
  serial_init( ( F_CPU / SPEED / 16 ) - 1);

  while (1) {
    /* read a character and echo back the next one */
    serial_write( serial_read() + 1);
  }
  return 0;
}

