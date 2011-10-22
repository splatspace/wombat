#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "read.h"
#include "print.h"
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

int main(int argc, char *argv[]) {

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
  print(env);
  printf("\n-------\n");

  eval(&env, cons(sym("eq"), cons(integer(1), integer(2))));
  
  /* while(1) { */
  /*   printf("=> "); */
  /*   printeger(read(stdin)); */
  /*   printf("\n"); */
  /* } */

  return 0;
}
