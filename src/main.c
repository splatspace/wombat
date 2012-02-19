#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#include <uberlisp/arduino_io.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>
#include <uberlisp/read_form.h>
#include <uberlisp/print_form.h>
#include <uberlisp/gc.h>
#include <uberlisp/debug.h>

#include <stdio.h>

uptr_t eval(uptr_t *env, uptr_t form);

uptr_t _fn(uptr_t *env, uptr_t fn, uptr_t args) {
  uptr_t *lvars_p = refer(CADR(fn)),
    *body_p = refer(CDDR(fn)),
    *args_p = refer(args),
    *local_env = refer(*env);

  while (*lvars_p && *args_p) {
    assoc(local_env, CAR(*lvars_p), CAR(*args_p));
    *lvars_p = CDR(*lvars_p);
    *args_p = CDR(*args_p);
  }

  uptr_t rval = NIL;

  while(*body_p) {
    rval = eval(local_env, CAR(*body_p));
    *body_p = CDR(*body_p);
  }

  release(4); // lvars_p, body_p, args_p, local_env
  return rval;
}

uptr_t let(uptr_t *env, uptr_t args) {
  uptr_t *bindings_p = refer(CAR(args)),
    *body_p = refer(CDR(args)),
    *local_env = refer(*env);

  while (*bindings_p) {
    assoc(local_env, CAR(*bindings_p), eval(local_env, CADR(*bindings_p)));
    *bindings_p = CDDR(*bindings_p);
  }

  uptr_t rval = NIL;
  while(*body_p) {
    rval = eval(local_env, CAR(*body_p));
    *body_p = CDR(*body_p);
  }

  release(3); // bindings_p, body_p, local_env

  return rval;
}

uptr_t loop(uptr_t *env, uptr_t form) {
  uptr_t *bindings_p = refer(CAR(form)),
    *body_p = refer(CDR(form)),
    *form_p = refer(form),
    *local_env = refer(*env);

  while (*bindings_p) {
    assoc(local_env, CAR(*bindings_p), eval(local_env, CADR(*bindings_p)));
    *bindings_p = CDDR(*bindings_p);
  }
  //  print_env(local_env);

  uptr_t rval = NIL,
    *new_env = refer(NIL),
    *new_vals = refer(NIL);
  while (*body_p) {
    if (IS_SYM(CAAR(*body_p)) && SVAL(CAAR(*body_p)) == S_RECUR) {
      *new_env = *env;
      *new_vals = CDAR(*body_p);
      *bindings_p = CAR(*form_p);
      while (*new_vals && *bindings_p) {
        assoc(new_env, CAR(*bindings_p), eval(local_env, CAR(*new_vals)));
        *bindings_p = CDDR(*bindings_p);
        *new_vals = CDR(*new_vals);
      }
      *body_p = CDR(*form_p);
      *local_env = *new_env;
      //      print_env(local_env);
    } else {
      rval = eval(local_env, CAR(*body_p));
      *body_p = CDR(*body_p);
    }
  }
 
  release(6); // bindings_p, body_p, form_p, local_env, new_env, new_vals
  return rval;
}

uptr_t exec_special(uptr_t *env, uptr_t form) {
  uptr_t fn = CAR(form);
  uptr_t args = CDR(form);

  switch(SVAL(fn)) {
  case S_LET:
    return let(env, args);

  case S_FN:
    return form;

  case S_LOOP:
    return loop(env, args);

  case S_QUOTE:
    return CAR(args);

  case S_CAR:
    return CAR(eval(env, CAR(args)));

  case S_CDR:
    return CDR(eval(env, CAR(args)));

  case S_IF: {
    uptr_t rval = NIL, *clauses = refer(args);

    if (eval(env, CAR(*clauses)) && CDR(*clauses))
      rval = eval(env, CADR(*clauses));
    else if (CDDR(*clauses))
      rval = eval(env, CADDR(*clauses));

    release(1); // clauses
    return rval;
  }
    
  case S_CONS: {
    uptr_t rval = NIL, *args_p = refer(args);
    rval = build_cons(eval(env, CAR(*args_p)), eval(env, CADR(*args_p)));
    release(1); // args_p
    return rval;
  }

  case S_PRINT:
    print_form(eval(env, CAR(args)));
    printf_P(PSTR("\n"));
    return NIL;

  case S_DEF: {
    uptr_t *args_p = refer(args),
      *binding = refer(eval(env, CADR(args)));
    assoc(env, CAR(*args_p), *binding);
    release(2); // args_p, binding
    return *binding; // Yeah, it's been "released", but the pointer is still valid.
  }

  case S_EVAL:
    return eval(env, eval(env, CAR(args)));

  case S_PLUS: {
    int sum = 0;
    uptr_t *rem_args = refer(args);
    while (*rem_args) {
      sum += eval(env, CAR(*rem_args));
      *rem_args = CDR(*rem_args);
    }
    release(1); // rem_args
    return INTERN_INT(sum);
  }

  case S_LT: {
    if IS_NIL(args) return NIL;
    
    uptr_t *args_p = refer(args);
    while(!IS_NIL(CDR(*args_p)) && (eval(env, CAR(*args_p)) < eval(env, CADR(*args_p))))
      *args_p = CDR(*args_p);
    
    uptr_t rval = NIL;
    if (IS_NIL(CDR(*args_p)))
      rval = eval(env, CAR(*args_p));
    release(1); // args_p
    return rval;
  }
      
  case S_MINUS: {
    uptr_t *rem_args = refer(args);
    int diff = eval(env, CAR(*rem_args));
    *rem_args = CDR(*rem_args);
    while (*rem_args) {
      diff -= eval(env, CAR(*rem_args));
      *rem_args = CDR(*rem_args);
    }
    release(1); // rem_args
    return INTERN_INT(diff);
  }

  case S_SREG: {
    uptr_t *args_p = refer(args),
      reg = eval(env, CAR(*args_p));
    if (IS_REG(reg))
      *BYTE_PTR(reg) = eval(env, CADR(*args_p));
    else {
      printf_P(PSTR("Invalid register: "));
      print_form(reg);
      printf_P(PSTR("\n"));
    }
    release(1); // args_p
    return NIL;
  }

  case S_SLP:
    _delay_ms(TO_INT(eval(env, CAR(args))));
    return NIL;

  default:
    printf_P(PSTR("ERROR: "));
    print_form(fn);
    printf_P(PSTR(" is not a function.\n"));
    return NIL;
  }
}

uptr_t eval_list(uptr_t *env, uptr_t list) {
  if (IS_NIL(list))
    return NIL;
  
  uptr_t *list_p = refer(list), rval;
  rval = build_cons(eval(env, CAR(*list_p)), eval_list(env, CDR(*list_p)));
  release(1); // list_p
  return rval;
}

uptr_t eval(uptr_t *env, uptr_t form) {
  if (IS_INT(form) || IS_NIL(form))
    return form;

  if (IS_SYM(form))
    return get(*env, form);

  if (IS_CONS(form)) {
    uptr_t *form_p = refer(form), 
      fn = eval(env, CAR(*form_p)), 
      rval;

    if (IS_SYM(fn)) {
      rval = exec_special(env, *form_p);
    } else if (IS_CONS(fn) && SVAL(CAR(fn)) == S_FN) {
      rval = _fn(env, fn, eval_list(env, CDR(*form_p)));
    } else {
      printf_P(PSTR("ERROR: "));
      print_form(CAR(*form_p));
      printf_P(PSTR(" cannot be in function position.\n"));
      rval = NIL;
    }

    release(1); // form_p
    return rval;
  }
  
  return NIL;
}

int main() {

  init_env(); // Poorly named. Has nothing to do with env alist.
  init_mem();

  uptr_t *env = refer(NIL);
  init_syms(env);

  uptr_t *form_p = refer(NIL);
  while(1) {
    print_env(env);
    print_mem();

    printf_P(PSTR("> "));
    *form_p = read_form(stdin);
    while(getc(stdin) != '\r');
    print_form(eval(env, *form_p));
    printf_P(PSTR("\n"));

    //    print_mem();
    __GC__();
  }

  release(2); // Just a formality really...

  return 0;
}
