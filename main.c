#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#include "read_form.h"
#include "print_form.h"
#include "alist.h"

void* car(void** env, void* args) {
  return CAR(CONS(args));
}

void* cdr(void** env, void* args) {
  return CDR(CONS(args));
}

void* quote(void** env, void* expr) {
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

static int serial_read(FILE *stream)
{
  while( !(UCSR0A & (1 << RXC0)) )
    ;
  return (int)UDR0;
}

static int serial_write(char c, FILE *stream)
{
  if (c == '\n')
    serial_write('\r', stream);

  while ( !(UCSR0A & (1 << UDRE0)) )
    ;
  UDR0 = c;
  return 0;
}

void* eq(void** env, void* args) {
  return equal(CAR(CONS(args)), CDR(CONS(args)));
}

void* eval(void** env, void* args) {
  switch(type(args)) {
  case SYMBOL:
    return get(*env, args);
  default:
    return args;
  }
}

#define SPEED 9600

int main(int argc, char *argv[]) {
  /* let the preprocessor calculate this */
  serial_init( ( F_CPU / SPEED / 16 ) - 1);
  fdevopen(serial_write, NULL);
  fdevopen(NULL, serial_read);

  /* populate env with special forms */
  void *env = empty_cons();
  Special Car = { SPECIAL, "car", car };
  Special Cdr = { SPECIAL, "cdr", cdr };
  Special Quote = { SPECIAL, "quote", quote };
  Special Eq = { SPECIAL, "eq", eq };
  Special Eval = { SPECIAL, "eval", eval };  
  assoc(&env, sym("car"), (void*)&Car);
  assoc(&env, sym("cdr"), (void*)&Cdr);
  assoc(&env, sym("quote"), (void*)&Quote);
  assoc(&env, sym("eq"), (void*)&Eq);
  assoc(&env, sym("eval"), (void*)&Eval);      

  printf("Environment:\n");
  print_form(env);
  printf("\n-------\n");

  eval(&env, cons(sym("eq"), cons(integer(1), integer(2))));
  
  return 0;
}
