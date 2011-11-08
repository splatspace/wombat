#include <uberlisp/arduino_io.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>
#include <avr/pgmspace.h>

#include <uberlisp/read_form.h>
#include <uberlisp/print_form.h>

#include <stdio.h>

uptr_t eval(uptr_t *env, uptr_t form);

uptr_t fn(uptr_t *env, uptr_t args) {
  uptr_t bindings = CAR(args);
  uptr_t body = CDR(args);
  uptr_t local_env = *env;

  while (bindings) {
    assoc(&local_env, CAR(bindings), CADR(bindings));
    bindings = CDDR(bindings);
  }

  uptr_t rval = NIL;
  while(body) {
    rval = eval(&local_env, CAR(body));
    body = CDR(body);
  }

  return rval;
}

uptr_t let(uptr_t *env, uptr_t args) {
  uptr_t bindings = CAR(args);
  uptr_t body = CDR(args);
  uptr_t local_env = *env;

  while (bindings) {
    assoc(&local_env, CAR(bindings), CADR(bindings));
    bindings = CDDR(bindings);
  }

  uptr_t rval = NIL;
  while(body) {
    rval = eval(&local_env, CAR(body));
    body = CDR(body);
  }

  return rval;
}

uptr_t exec_special(uptr_t *env, uptr_t fn, uptr_t args) {
  if (hash_sym("let") == SVAL(fn))
    return let(env, args);

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

  if (hash_sym("def") == SVAL(fn)) {
    uptr_t binding = eval(env, CADR(args));
    assoc(env, CAR(args), binding);
    return binding;
  }

  if (hash_sym("eval") == SVAL(fn))
    return eval(env, eval(env, CAR(args)));

  if (hash_sym("+") == SVAL(fn)) {
    int sum = 0;
    uptr_t rem_args = args;
    while (rem_args) {
      sum += eval(env, CAR(rem_args));
      rem_args = CDR(rem_args);
    }
    return INTERN_INT(sum);
  }

  if (hash_sym("-") == SVAL(fn)) {
    int diff = eval(env, CAR(args));
    uptr_t rem_args = CDR(args);
    while (rem_args) {
      diff -= eval(env, CAR(rem_args));
      rem_args = CDR(rem_args);
    }
    return INTERN_INT(diff);
  }

  printf_P(PSTR("ERROR: "));
  print_form(fn);
  printf_P(PSTR(" is not a function.\n"));
  return NIL;
}

uptr_t eval(uptr_t *env, uptr_t form) {
  if (IS_INT(form) || IS_NIL(form))
    return form;

  if (IS_SYM(form))
    return get(*env, form);

  if (IS_CONS(form)) {
    uptr_t fn = eval(env, CAR(form));
    if (IS_SYM(fn))
      return exec_special(env, fn, CDR(form));

    if(IS_CONS(fn)) {
      printf_P(PSTR("SHIT JUST GOT REAL\n"));
      return NIL;
    }

    printf_P(PSTR("ERROR: "));
    print_form(CAR(form));
    printf_P(PSTR(" cannot be in function position.\n"));
    return NIL;

  }

  return NIL;
}

int main(int argc, char *argv[]) {

  init_env(); // Poorly named. Has nothing to do with env alist.
  init_mem();

  uptr_t env = NIL;
  assoc(&env, build_symbol("let"), build_symbol("let"));
  assoc(&env, build_symbol("quote"), build_symbol("quote"));
  assoc(&env, build_symbol("car"), build_symbol("car"));
  assoc(&env, build_symbol("cdr"), build_symbol("cdr"));
  assoc(&env, build_symbol("cons"), build_symbol("cons"));
  assoc(&env, build_symbol("print"), build_symbol("print"));
  assoc(&env, build_symbol("def"), build_symbol("def"));
  assoc(&env, build_symbol("eval"), build_symbol("eval"));
  assoc(&env, build_symbol("+"), build_symbol("+"));
  assoc(&env, build_symbol("-"), build_symbol("-"));

  uptr_t form;
  while(1) {
    printf_P(PSTR("Total mem:\t%dB\nFree mem:\t%dB\tUsed mem:\t%dB\nCons mem:\t%dB\tSymbol mem:\t%dB\n"),
        (CEND_p - SSTART_p),
        (CSTART_p - SEND_p),
        (CEND_p - CSTART_p)+(SEND_p-SSTART_p),
        (CEND_p - CSTART_p),
        (SEND_p - SSTART_p));
    printf_P(PSTR("> "));
    form = read_form(stdin);
    while(getc(stdin) != '\r');
    print_form(eval(&env, form));
    printf_P(PSTR("\n"));
  }

  return 0;
}
