#include <stdio.h>
#include "types.h"
#include "print.h"

void print(void* form) {
  if(form) {
    switch(type(form)) {
    case INT:
      printf("%d", IVAL(form));
      break;
    case SYMBOL:
      printf("%s", SVAL(form));
      break;
    case CONS:
      {
        printf("(");
        print(CAR(form));

        if(CDR(form))
          printf(" ");

        print(CDR(form));
        printf(")");
        break;
      }
    }
  }
}
