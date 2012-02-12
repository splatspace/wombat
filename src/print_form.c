#include <uberlisp/print_form.h>

/* Printing *******************************/

void print_list(uoff_t list) {
  DECL(list);
  print_form(CAR(P_list));
  
  if (CDR(P_list) != NIL) {
    printf_P(PSTR(" "));
    if (IS_CONS(CDR(P_list))) {
      print_list(CDR(P_list));
    } else {
      printf_P(PSTR(". "));
      print_form(CDR(P_list));
    }
  }
  UDECL(list);
  __free(1);
}

void print_form(uoff_t form) {

  DECL(form);

  if (IS_NIL(P_form)) {
    printf_P(PSTR("()"));
  } else if (IS_REG(P_form)) {
    printf_P(PSTR("R:%p"), TO_PTR(P_form));
  } else if (IS_INT(P_form)) {
    printf_P(PSTR("%d"), TO_INT(P_form));
  } else if (IS_SYM(P_form)) {
    char buf[7];
    memset(buf, 0, 7);
    unhash_sym(buf, P_form);
    printf_P(PSTR("%s"), buf);
  } else {
    printf_P(PSTR("("));
    print_list(P_form);
    printf_P(PSTR(")"));
  }

  UDECL(form);
  __free(1);
}

