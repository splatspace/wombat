#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "read_form.h"
#include "print_form.h"

/* SPECIAL FORMS *******************************/

void* car(void* arglist) {
  if(CONS_P(arglist)) {
    return CONS(arglist)->car;
  } else {
    fprintf(stderr, "Can't get car of atom:\n");
    print_form(arglist);
    return NULL;
  }
}

void* cdr(void* arglist) {
  if(CONS_P(arglist)) {
    return CONS(arglist)->cdr;
  } else {
    fprintf(stderr, "Can't get cdr of atom:\n");
    print_form(arglist);
    return NULL;
  }
}

void* quote(void* expr) {
  return expr;
}

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

void serial_write_str(char *str) {
  char c = *str;
  while(c) {
    serial_write(c);
    c++;
  }
}



#define SPEED 9600
int main (void)
{
  /* let the preprocessor calculate this */
  serial_init( ( F_CPU / SPEED / 16 ) - 1);

  while (1) {
    serial_write_str("=> ");
    print_form(read_form(stdin));
    serial_write('\n');
  }
  return 0;
}

/*
int main(int argc, char *argv[]) {

  while(1) {
    printf("=> ");
    print(read(stdin));
    printf("\n");
  }

  return 0;
}
*/
