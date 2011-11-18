#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#include <uberlisp/arduino_io.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>
#include <uberlisp/read_form.h>
#include <uberlisp/print_form.h>

#include <stdio.h>

uptr_t eval(uptr_t *env, uptr_t form);

uptr_t _fn(uptr_t *env, uptr_t fn, uptr_t args) {
  uptr_t lvars = CADR(fn);
  uptr_t body = CDDR(fn);
  uptr_t local_env = *env;

  while (lvars && args) {
    assoc(&local_env, CAR(lvars), CAR(args));
    lvars = CDR(lvars);
    args = CDR(args);
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
    assoc(&local_env, CAR(bindings), eval(&local_env, CADR(bindings)));
    bindings = CDDR(bindings);
  }

  uptr_t rval = NIL;
  while(body) {
    rval = eval(&local_env, CAR(body));
    body = CDR(body);
  }

  return rval;
}

uptr_t exec_special(uptr_t *env, uptr_t form) {
  uptr_t fn = CAR(form);
  uptr_t args = CDR(form);

  if (S_LET == SVAL(fn))
    return let(env, args);

  if (S_FN == SVAL(fn))
    return form;

  if (S_QUOTE == SVAL(fn))
    return CAR(args);

  if (S_CAR == SVAL(fn))
    return CAR(eval(env, CAR(args)));

  if (S_CDR == SVAL(fn))
    return CDR(eval(env, CAR(args)));

  if (S_IF == SVAL(fn)) {
    if (eval(env, CADR(form)))
      return CDDR(form) ? eval(env, CADDR(form)) : NIL;
    else
      return CDDDR(form) ? eval(env, CAR(CDDDR(form))) : NIL;
  }

  if (S_CONS == SVAL(fn))
    return build_cons(eval(env, CAR(args)), eval(env, CADR(args)));

  if (S_PRINT == SVAL(fn)) {
    print_form(eval(env, CAR(args)));
    printf_P(PSTR("\n"));
    return NIL;
  }

  if (S_DEF == SVAL(fn)) {
    uptr_t binding = eval(env, CADR(args));
    assoc(env, CAR(args), binding);
    return binding;
  }

  if (S_EVAL == SVAL(fn))
    return eval(env, eval(env, CAR(args)));

  if (S_PLUS == SVAL(fn)) {
    int sum = 0;
    uptr_t rem_args = args;
    while (rem_args) {
      sum += eval(env, CAR(rem_args));
      rem_args = CDR(rem_args);
    }
    return INTERN_INT(sum);
  }

  if (S_LT == SVAL(fn)) {
    while(1) {
      if (IS_NIL(args))
        return NIL;
      if (IS_NIL(CDR(args)))
        return CAR(args);
      if (eval(env, CAR(args)) >= eval(env, CADR(args)))
        return NIL;

      args = CDR(args);
    }
  }

  if (S_MINUS == SVAL(fn)) {
    int diff = eval(env, CAR(args));
    uptr_t rem_args = CDR(args);
    while (rem_args) {
      diff -= eval(env, CAR(rem_args));
      rem_args = CDR(rem_args);
    }
    return INTERN_INT(diff);
  }

  if (S_SREG == SVAL(fn)) {
    uptr_t reg = eval(env, CAR(args));
    if (IS_REG(reg))
      *BYTE_PTR(reg) = eval(env, CADR(args));
    else {
      printf_P(PSTR("Invalid register: "));
      print_form(reg);
      printf_P(PSTR("\n"));
    }

    return NIL;
  }

  if (S_SLP == SVAL(fn)) {
    _delay_ms(TO_INT(eval(env, CAR(args))));
    return NIL;
  }

  printf_P(PSTR("ERROR: "));
  print_form(fn);
  printf_P(PSTR(" is not a function.\n"));
  return NIL;
}

uptr_t eval_list(uptr_t *env, uptr_t list) {
  if (IS_NIL(list))
    return NIL;

  return build_cons(eval(env, CAR(list)), eval_list(env, CDR(list)));
}

uptr_t eval(uptr_t *env, uptr_t form) {
  if (IS_INT(form) || IS_NIL(form))
    return form;

  if (IS_SYM(form))
    return get(*env, form);

  if (IS_CONS(form)) {
    uptr_t fn = eval(env, CAR(form));
    if (IS_SYM(fn))
      return exec_special(env, form);

    if(IS_CONS(fn) && SVAL(CAR(fn)) == S_FN) {
      return _fn(env, fn, eval_list(env, CDR(form)));
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
  init_syms(&env);

  printf_P(PSTR("env: "));
  print_form(env);
  printf_P(PSTR("\n"));

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
