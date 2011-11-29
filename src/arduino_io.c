#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdio.h>
#include <ctype.h>
#include <uberlisp/arduino_io.h>

static int serial_write(char c, FILE *stream)
{
  if (c == '\n')
    serial_write('\r', stream);

  loop_until_bit_is_set(USCR0A, UDRE0);

  UDR0 = c;
  return 0;
}

static int serial_read(FILE *stream)
{
  loop_until_bit_is_set(USCR0A, RXC0);

  char c = UDR0;
  if (c == '\r') 
    serial_write('\n', stream);
  else
    serial_write(toupper(c), stream);

  return (int)c;
}


static FILE mystdin = FDEV_SETUP_STREAM(NULL, &serial_read, _FDEV_SETUP_READ);
static FILE mystdout = FDEV_SETUP_STREAM(&serial_write, NULL, _FDEV_SETUP_WRITE);

void init_env() {
  stdin = &mystdin;
  stdout = &mystdout;

  uint16_t bittimer = (F_CPU / 57600 / 16) - 1;

  UBRR0H = (uint8_t) (bittimer >> 8);
  UBRR0L = (uint8_t) bittimer;

  UCSR0C = _BV(UCSZ00);

  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
}

