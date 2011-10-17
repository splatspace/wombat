#include <stdio.h>

#include "types.h"
#include "print.h"

/* PRINTING *******************************/

void print(void* x) {
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
        print(CAR(x));

        if(CDR(x))
          printf(" ");

        print(CDR(x));
        printf(")");
        break;
      }
    }
  }
}
