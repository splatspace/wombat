#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uberlisp/types.h>
#include <uberlisp/print_form.h>

/* Printing *******************************/

void print_list(uptr_t list) {
  print_form(CAR(list));
  
  if (CDR(list) != NIL) {
    printf(" ");
    if (IS_CONS(CDR(list))) {
      print_list(CDR(list));
    } else {
      printf(". ");
      print_form(CDR(list));
    }
  }
}

void print_form(uptr_t form) {

  if (IS_INT(form)) {
    printf("%d", TO_INT(form));
  } else if (IS_SYM(form)) {
    char buf[7];
    memset(buf, 0, 7);
    unhash_sym(buf, form);
    printf("%s", buf);
  } else {
    printf("(");
    print_list(form);
    printf(")");
  }

}

