#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "read.h"
#include "print.h"

/* SPECIAL FORMS *******************************/

void* car(void* args) {
  return CAR(CONS(args));
}

void* cdr(void* args) {
  return CDR(CONS(args));
}

void* quote(void* expr) {
  return expr;
}

void* eq(void* args) {
  return _eq(CAR(CONS(args)), CDR(CONS(args)));
}

int main(int argc, char *argv[]) {

  while(1) {
    printf("=> ");
    print(read(stdin));
    printf("\n");
  }

  return 0;
}
