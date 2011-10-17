#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "read.h"
#include "print.h"

/* SPECIAL FORMS *******************************/

void* car(void* arglist) {
  if(CONS_P(arglist)) {
    return CONS(arglist)->car;
  } else {
    fprintf(stderr, "Can't get car of atom:\n");
    print(arglist);
    return NULL;
  }
}

void* cdr(void* arglist) {
  if(CONS_P(arglist)) {
    return CONS(arglist)->cdr;
  } else {
    fprintf(stderr, "Can't get cdr of atom:\n");
    print(arglist);
    return NULL;
  }
}

void* quote(void* expr) {
  return expr;
}

int main(int argc, char *argv[]) {

  while(1) {
    printf("=> ");
    print(read(stdin));
    printf("\n");
  }

  return 0;
}
