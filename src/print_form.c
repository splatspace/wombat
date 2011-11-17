#include <uberlisp/print_form.h>

/* Printing *******************************/

void print_list(uptr_t list) {
  print_form(CAR(list));
  
  if (CDR(list) != NIL) {
    printf_P(PSTR(" "));
    if (IS_CONS(CDR(list))) {
      print_list(CDR(list));
    } else {
      printf_P(PSTR(". "));
      print_form(CDR(list));
    }
  }
}

void print_form(uptr_t form) {

  if (IS_NIL(form)) {
    printf_P(PSTR("()"));
  } else if (IS_REG(form)) {
    printf_P(PSTR("R:%p"), TO_PTR(form));
  } else if (IS_INT(form)) {
    printf_P(PSTR("%d"), TO_INT(form));
  } else if (IS_SYM(form)) {
    char buf[7];
    memset(buf, 0, 7);
    unhash_sym(buf, form);
    printf_P(PSTR("%s"), buf);
  } else {
    printf_P(PSTR("("));
    print_list(form);
    printf_P(PSTR(")"));
  }

}

