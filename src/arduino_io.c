#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <uberlisp/arduino_io.h>

static int serial_write(char c, FILE *stream)
{
  if (c == '\n')
    serial_write('\r', stream);

  while ( !(UCSR0A & (1 << UDRE0)) )
    ;
  UDR0 = c;
  return 0;
}

static int serial_read(FILE *stream)
{
  while( !(UCSR0A & (1 << RXC0)) )
    ;
  return (int)UDR0;
}


static FILE mystdin = FDEV_SETUP_STREAM(NULL, &serial_read, _FDEV_SETUP_READ);
static FILE mystdout = FDEV_SETUP_STREAM(&serial_write, NULL, _FDEV_SETUP_WRITE);

void init_env() {
  stdin = &mystdin;
  stdout = &mystdout;

  uint16_t bittimer = (F_CPU / 57600 / 16) - 1;
  /* Set the baud rate */
  UBRR0H = (uint8_t) (bittimer >> 8);
  UBRR0L = (uint8_t) bittimer;
  /* set the framing to 8N1 */
  UCSR0C = (3 << UCSZ00);
  /* Engage! */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

