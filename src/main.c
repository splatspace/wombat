#include <uberlisp/arduino_io.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>
#include <avr/pgmspace.h>

#include <uberlisp/read_form.h>
#include <uberlisp/print_form.h>

#include <stdio.h>

uptr_t eval(uptr_t *env, uptr_t form);

uptr_t exec_special(uptr_t *env, uptr_t form) {
  uptr_t fn = CAR(form);
  uptr_t args = CDR(form);

  if (hash_sym("quote") == SVAL(fn))
    return CAR(args);

  if (hash_sym("car") == SVAL(fn))
    return CAR(eval(env, CAR(args)));

  if (hash_sym("cdr") == SVAL(fn))
    return CDR(eval(env, CAR(args)));

  if (hash_sym("cons") == SVAL(fn))
    return build_cons(eval(env, CAR(args)), eval(env, CADR(args)));

  if (hash_sym("print") == SVAL(fn)) {
    print_form(eval(env, CAR(args)));
    printf_P(PSTR("\n"));
    return NIL;
  }

  printf_P(PSTR("ERROR: "));
  print_form(CAR(form));
  printf_P(PSTR(" is not a function."));
  return NIL;
}

uptr_t eval(uptr_t *env, uptr_t form) {
  if (IS_INT(form) || IS_NIL(form))
    return form;

  if (IS_SYM(form))
    return eval(env, get(*env, form));

  if (IS_CONS(form)) {
    if (!IS_SYM(CAR(form))) {
      printf_P(PSTR("ERROR: "));
      print_form(CAR(form));
      printf_P(PSTR(" cannot be in function position.\n"));
      return NIL;
    }
    return exec_special(env, form);
  }

  return NIL;
}

int main(int argc, char *argv[]) {

  init_env(); // Poorly named. Has nothing to do with env alist.
  init_mem();

  uptr_t env = NIL;

  uptr_t form;
  while(1) {
    printf_P(PSTR("=> "));
    form = read_form(stdin);
    while(getc(stdin) != '\r');
    print_form(eval(&env, form));
    printf_P(PSTR("\n"));
  }

  return 0;
}
