#include <stdio.h>
#include "types.h"
#include "print_form.h"

/* PRINTING *******************************/
void print_form(void* form) {
  switch(type(form)) {
  case INT:
    printf("%d", IVAL(form));
    break;
  case SYMBOL:
    printf("%s", SVAL(form));
    break;
  case SPECIAL:
    printf("#<special:%s>", ((Special*)form)->name);
    break;
  case FUNCTION:
    printf("#<function>");
    break;
  case CONS:
    {
      printf("(");

      if(CAR(form)) print_form(CAR(form));

      if(CDR(form)) {
        printf(" ");
        print_form(CDR(form));        
      }

      printf(")");
      break;
    }
  }
}
