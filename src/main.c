#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#include <uberlisp/arduino_io.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>
#include <uberlisp/read_form.h>
#include <uberlisp/print_form.h>

#include <stdio.h>

uptr_t eval(uptr_t form);
uptr_t eval_list(uptr_t list);

uptr_t _fn(uptr_t fn, uptr_t args) {
  uptr_t lvars = CADR(fn);
  uptr_t body = CDDR(fn);
  uptr_t env = ENV_p;

  while (lvars && args) {
    assoc(CAR(lvars), CAR(args));
    lvars = CDR(lvars);
    args = CDR(args);
  }

  uptr_t rval = NIL;
  while(body) {
    rval = eval(CAR(body));
    body = CDR(body);
  }

  __set_env(env);
  return rval;
}

uptr_t let(uptr_t args) {
  uptr_t bindings = CAR(args);
  uptr_t body = CDR(args);
  uptr_t env = ENV_p;

  while (bindings) {
    assoc(CAR(bindings), eval(CADR(bindings)));
    bindings = CDDR(bindings);
  }

  uptr_t rval = NIL;
  while(body) {
    rval = eval(CAR(body));
    body = CDR(body);
  }

  __set_env(env);
  return rval;
}

uptr_t loop(uptr_t form) {
  uptr_t bindings = CAR(form);
  uptr_t body = CDR(form);
  uptr_t env = ENV_p;

  while (bindings) {
    assoc(CAR(bindings), eval(CADR(bindings)));
    bindings = CDDR(bindings);
  }

  uptr_t rval = NIL;
  while (body) {
    if (IS_SYM(CAAR(body)) && SVAL(CAAR(body)) == S_RECUR) {
      uptr_t new_vals = eval_list(CDAR(body));
      bindings = CAR(form);
      while (new_vals && bindings) {
        __set_binding(CAR(bindings), CAR(new_vals));
        bindings = CDDR(bindings);
        new_vals = CDR(new_vals);
      }
      body = CDR(form);
    } else {
      rval = eval(CAR(body));
      body = CDR(body);
    }
  }
 
  __set_env(env);
  return rval;
}

uptr_t exec_special(uptr_t form) {
  uptr_t fn = CAR(form);
  uptr_t args = CDR(form);

  switch(SVAL(fn)) {
  case S_LET:
    return let(args);

  case S_FN:
    return form;

  case S_LOOP:
    return loop(args);

  case S_QUOTE:
    return CAR(args);

  case S_CAR:
    return CAR(eval(CAR(args)));

  case S_CDR:
    return CDR(eval(CAR(args)));

  case S_IF:
    if (eval(CADR(form)))
      return CDDR(form) ? eval(CADDR(form)) : NIL;
    else
      return CDDDR(form) ? eval(CAR(CDDDR(form))) : NIL;
    
  case S_CONS:
    return build_cons(eval(CAR(args)), eval(CADR(args)));

  case S_PRINT:
    print_form(eval(CAR(args)));
    printf_P(PSTR("\n"));
    return NIL;

  case S_DEF: {
    uptr_t binding = eval(CADR(args));
    __set_binding(CAR(args), binding);
    return binding;
  }

  case S_EVAL:
    return eval(eval(CAR(args)));

  case S_PLUS: {
    int sum = 0;
    uptr_t rem_args = args;
    while (rem_args) {
      sum += eval(CAR(rem_args));
      rem_args = CDR(rem_args);
    }
    return INTERN_INT(sum);
  }

  case S_LT:
    while(1) {
      if (IS_NIL(args))
        return NIL;
      if (IS_NIL(CDR(args)))
        return eval(CAR(args));
      if (eval(CAR(args)) >= eval(CADR(args)))
        return NIL;

      args = CDR(args);
    }
      
  case S_MINUS: {
    int diff = eval(CAR(args));
    uptr_t rem_args = CDR(args);
    while (rem_args) {
      diff -= eval(CAR(rem_args));
      rem_args = CDR(rem_args);
    }
    return INTERN_INT(diff);
  }

  case S_SREG: {
    uptr_t reg = eval(CAR(args));
    if (IS_REG(reg))
      *BYTE_PTR(reg) = eval(CADR(args));
    else {
      printf_P(PSTR("Invalid register: "));
      print_form(reg);
      printf_P(PSTR("\n"));
    }
    return NIL;
  }

  case S_SLP:
    _delay_ms(TO_INT(eval(CAR(args))));
    return NIL;

  default:
    printf_P(PSTR("ERROR: "));
    print_form(fn);
    printf_P(PSTR(" is not a function.\n"));
    return NIL;
  }
}

uptr_t eval_list(uptr_t list) {
  if (IS_NIL(list))
    return NIL;

  return build_cons(eval(CAR(list)), eval_list(CDR(list)));
}

uptr_t eval(uptr_t form) {
  if (IS_INT(form) || IS_NIL(form))
    return form;

  if (IS_SYM(form))
    return get(form);

  if (IS_CONS(form)) {
    uptr_t fn = eval(CAR(form));
    if (IS_SYM(fn))
      return exec_special(form);

    if(IS_CONS(fn) && SVAL(CAR(fn)) == S_FN) {
      return _fn(fn, eval_list(CDR(form)));
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

  init_syms();

  uptr_t form;
  while(1) {
    printf_P(PSTR("Total mem:\t%dB\nFree mem:\t%dB\tUsed mem:\t%dB\nCons mem:\t%dB\tSymbol mem:\t%dB\n"),
             TOTALMEM(), FREEMEM(), USEDMEM(), CONSMEM(), SYMMEM());
    printf_P(PSTR("env: "));
    print_form(ENV_p);
    printf_P(PSTR("\n"));
    printf_P(PSTR("> "));
    
    form = read_form(stdin);
    CRECENT_p = NIL;
    
    while(getc(stdin) != '\r');
    print_form(eval(form));
    printf_P(PSTR("\n"));
  }

  return 0;
}
