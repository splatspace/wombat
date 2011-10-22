#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#include "read_form.h"
#include "print_form.h"
#include "alist.h"
#include "arduino_io.h"

void* car(void** env, void* expr) {
  return CAR(CONS(expr));
}

void* cdr(void** env, void* expr) {
  return CDR(CONS(expr));
}

void* quote(void** env, void* expr) {
  return expr;
}
void* eq(void** env, void* expr) {
  return equal(CAR(expr), CDR(expr));
}

void* eval(void** env, void* expr) {
  void *op, *arg, *args;

  switch(type(expr)) {
  case CONS:
    if (!CAR(expr) && !CDR(expr)) {
      return expr;
    } else {
      op = get(*env, CAR(expr));
      args = CDR(expr);
      while((args)) {
        arg = CAR(args);
        printf("arg: ");
        print_form(arg);
        printf("\n");
        args = CDR(args) ;
      }
      switch(type(op)) {
      case SPECIAL:
        return(expr);
        /* return SPECIAL(op)->fn(*env, list); */
      default:
        return expr;
      }
    }
  case SYMBOL:
    return get(*env, expr);
  default:
    return expr;
  }
}

int main(int argc, char *argv[]) {

  ARDUINO_INIT_IO(9600);

  /* populate env with special forms */
  void *env = empty();
  Special Car = { SPECIAL, "car", &car };
  Special Cdr = { SPECIAL, "cdr", &cdr };
  Special Quote = { SPECIAL, "quote", &quote };
  Special Eq = { SPECIAL, "eq", &eq };
  Special Eval = { SPECIAL, "eval", &eval };
  assoc(&env, sym("car"), (void*)&Car);
  assoc(&env, sym("cdr"), (void*)&Cdr);
  assoc(&env, sym("quote"), (void*)&Quote);
  assoc(&env, sym("eq"), (void*)&Eq);
  assoc(&env, sym("eval"), (void*)&Eval);

  print_form(cons(sym("eq"), cons(integer(1), integer(2))));
  printf("\n");
  print_form(eval(&env, cons(sym("eq"), cons(integer(2), integer(3)))));
  printf("\n");

  printf("\n");
  print_form(eval(&env, cons(sym("eq"), cons(integer(1), cons(integer(2), integer(3))))));
  printf("\n");

  while(1) {
    printf("=> ");
    print_form(read_form(stdin));
    printf("\n");
  }

  return 0;
}
