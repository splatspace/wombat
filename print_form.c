#include <stdio.h>

#include "types.h"
#include "print_form.h"

/* PRINTING *******************************/

void print_form(void* x) {
  if(x) {
    switch(TYPE(x)) {
    case INT:
      printf("%d", IVAL(x));
      break;
    case SYMBOL:
      printf("%s", SVAL(x));
      break;
    case CONS:
      {
        printf("(");
        print_form(CAR(x));

        if(CDR(x))
          printf(" ");

        print_form(CDR(x));
        printf(")");
        break;
      }
    }
  }
}
