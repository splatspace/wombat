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

  if (hash_sym("let") == SVAL(fn))
    return let(env, args);

  if (hash_sym("fn") == SVAL(fn))
    return form;

  if (hash_sym("quote") == SVAL(fn))
    return CAR(args);

  if (hash_sym("car") == SVAL(fn))
    return CAR(eval(env, CAR(args)));

  if (hash_sym("cdr") == SVAL(fn))
    return CDR(eval(env, CAR(args)));

  if (hash_sym("if") == SVAL(fn)) {
    if (eval(env, CADR(form)))
      return CDDR(form) ? eval(env, CADDR(form)) : NIL;
    else
      return CDDDR(form) ? eval(env, CAR(CDDDR(form))) : NIL;
  }

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

  if (hash_sym("<") == SVAL(fn)) {
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

  if (hash_sym("-") == SVAL(fn)) {
    int diff = eval(env, CAR(args));
    uptr_t rem_args = CDR(args);
    while (rem_args) {
      diff -= eval(env, CAR(rem_args));
      rem_args = CDR(rem_args);
    }
    return INTERN_INT(diff);
  }

  if (hash_sym("sreg") == SVAL(fn)) {
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

  if (hash_sym("slp") == SVAL(fn)) {
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

    if(IS_CONS(fn) && SVAL(CAR(fn)) == hash_sym("fn")) {
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
  assoc(&env, build_symbol_P(PSTR("LET")), build_symbol_P(PSTR("LET")));
  assoc(&env, build_symbol_P(PSTR("FN")), build_symbol_P(PSTR("FN")));
  assoc(&env, build_symbol_P(PSTR("QUOTE")), build_symbol_P(PSTR("QUOTE")));
  assoc(&env, build_symbol_P(PSTR("CAR")), build_symbol_P(PSTR("CAR")));
  assoc(&env, build_symbol_P(PSTR("CDR")), build_symbol_P(PSTR("CDR")));
  assoc(&env, build_symbol_P(PSTR("CONS")), build_symbol_P(PSTR("CONS")));
  assoc(&env, build_symbol_P(PSTR("PRINT")), build_symbol_P(PSTR("PRINT")));
  assoc(&env, build_symbol_P(PSTR("DEF")), build_symbol_P(PSTR("DEF")));
  assoc(&env, build_symbol_P(PSTR("IF")), build_symbol_P(PSTR("IF")));
  assoc(&env, build_symbol_P(PSTR("EVAL")), build_symbol_P(PSTR("EVAL")));
  assoc(&env, build_symbol_P(PSTR("+")), build_symbol_P(PSTR("+")));
  assoc(&env, build_symbol_P(PSTR("-")), build_symbol_P(PSTR("-")));
  assoc(&env, build_symbol_P(PSTR("<")), build_symbol_P(PSTR("<")));
  assoc(&env, build_symbol_P(PSTR("SREG")), build_symbol_P(PSTR("SREG")));
  assoc(&env, build_symbol_P(PSTR("SLP")), build_symbol_P(PSTR("SLP")));

  // Registers
  assoc(&env, build_symbol_P(PSTR("_DDRB")), DDRB);
  assoc(&env, build_symbol_P(PSTR("_PORTB")), PORTB);

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
