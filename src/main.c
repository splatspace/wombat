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
uptr_t eval_list(uptr_t *env, uptr_t list);

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
    rval = eval(local_env, CAR(*body_p));
    *body_p = CDR(*body_p);

    if (IS_CONS(rval) && IS_SYM(CAR(rval)) && SVAL(CAR(rval)) == S_RECUR) {
      *new_env = *env;
      *new_vals = CDR(rval);
      *bindings_p = CAR(*form_p);
      while (*new_vals && *bindings_p) {
        assoc(new_env, CAR(*bindings_p), eval(local_env, CAR(*new_vals)));
        *bindings_p = CDDR(*bindings_p);
        *new_vals = CDR(*new_vals);
      }
      *body_p = CDR(*form_p);
      *local_env = *new_env;
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

  case S_DO: {
    uptr_t *body_p = refer(args), rval = NIL;
    
    while (*body_p) {
      rval = eval(env, CAR(*body_p));
      *body_p = CDR(*body_p);
    }
    release(1); // body_p
    return rval;
  } 

  case S_RECUR: {
    uptr_t rval, *fn_p = refer(fn);
    rval = build_cons(*fn_p, eval_list(env, args));
    release(1); // fn_p
    return rval;
  }

  case S_QUOTE:
    return CAR(args);

  case S_CAR:
    return CAR(eval(env, CAR(args)));

  case S_CDR:
    return CDR(eval(env, CAR(args)));

  case S_AND: {
    if (IS_NIL(args)) return PS_TRUE;
    uptr_t *rem_args = refer(args),
      rval = NIL;
    while ((rval = eval(env, CAR(*rem_args))) && (*rem_args = CDR(*rem_args)));
    release(1);
    return rval;
  }
    
  case S_OR: {
    if (IS_NIL(args)) return NIL;
    uptr_t *rem_args = refer(args),
      rval = NIL;
    while (!(rval = eval(env, CAR(*rem_args))) && (*rem_args = CDR(*rem_args)));
    release(1);
    return rval;
  }

  case S_NOT: {
    if (IS_NIL(args)) return NIL;
    uptr_t rval = eval(env, CAR(args));
    return rval ? NIL : PS_TRUE;
  }
    
  case S_IF: {
    uptr_t rval = NIL, *clauses = refer(args);

    if (eval(env, CAR(*clauses)) && CDR(*clauses))
      rval = eval(env, CADR(*clauses));
    else if (CDDR(*clauses))
      rval = eval(env, CADDR(*clauses));

    release(1); // clauses
    return rval;
  }

  case S_WHEN: {
    uptr_t rval = NIL, *cond_p = refer(CAR(args)), *body_p = refer(CDR(args));

    if (eval(env, *cond_p))
      while(*body_p) {
        rval = eval(env, CAR(*body_p));
        *body_p = CDR(*body_p);
      }

    release(2); // cond_p, body_p
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

#define _COMPR(rval) {                                                  \
      if (IS_NIL(args)) return NIL;                                     \
                                                                        \
      uptr_t *args_p = refer(args);                                     \
      while(CDR(*args_p) && (eval(env, CAR(*args_p)) _COMP_OPR eval(env, CADR(*args_p)))) \
        *args_p = CDR(*args_p);                                         \
                                                                        \
      if (IS_NIL(CDR(*args_p)))                                         \
        rval = eval(env, CAR(*args_p));                                 \
      release(1);                                                       \
    }

#define _COMP_OPR ==
  case S_EQL: {
    uptr_t rval;
    _COMPR(rval);
    return rval;
  }

  case S_NEQL: {
    uptr_t rval;
    _COMPR(rval);
    return rval ? NIL : PS_TRUE;
  }
#undef _COMP_OPR

#define _COMP_OPR <
  case S_LT: {
    uptr_t rval;
    _COMPR(rval);
    return rval;
  }
#undef _COMP_OPR
      
#define _COMP_OPR <=
  case S_LTE: {
    uptr_t rval;
    _COMPR(rval);
    return rval;
  }
#undef _COMP_OPR
      
#define _COMP_OPR >
  case S_GT: {
    uptr_t rval;
    _COMPR(rval);
    return rval;
  }
#undef _COMP_OPR
      
#define _COMP_OPR >=
  case S_GTE: {
    uptr_t rval;
    _COMPR(rval);
    return rval;
  }
#undef _COMP_OPR

#define _ARITH(coll) {                                          \
      uptr_t *rem_args = refer(args);                           \
      coll = TO_INT(eval(env, CAR(*rem_args)));                 \
      *rem_args = CDR(*rem_args);                               \
      while (*rem_args) {                                       \
        coll _ARITH_OPR TO_INT(eval(env, CAR(*rem_args)));   \
        *rem_args = CDR(*rem_args);                             \
      }                                                         \
      release(1);                                               \
    }

#define _ARITH_OPR +=
  case S_PLUS: {
    if (! args) return NIL;
    if (! CDR(args)) return eval(env, CAR(args));
    int rval;
    _ARITH(rval);
    return INTERN_INT(rval);
  }
#undef _ARITH_OPR

#define _ARITH_OPR -=
  case S_MINUS: {
    if (! args) return NIL;
    if (! CDR(args)) return INTERN_INT(0 - TO_INT(eval(env, CAR(args))));
    int rval;
    _ARITH(rval);
    return INTERN_INT(rval);
  }
#undef _ARITH_OPR

#define _ARITH_OPR *=
  case S_MULT: {
    if (! args) return INTERN_INT(1);
    if (! CDR(args)) return eval(env, CAR(args));
    int rval;
    _ARITH(rval);
    return INTERN_INT(rval);
  }
#undef _ARITH_OPR

#define _ARITH_OPR /=
  case S_DIV: {
    if (! args) return NIL;
    if (! CDR(args)) return INTERN_INT(0);
    int rval;
    _ARITH(rval);
    return INTERN_INT(rval);
  }
#undef _ARITH_OPR

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
      *fn_p = refer(eval(env, CAR(*form_p))), 
      rval;

    if (IS_SYM(*fn_p)) {
      rval = exec_special(env, *form_p);
    } else if (IS_CONS(*fn_p) && SVAL(CAR(*fn_p)) == S_FN) {
      rval = _fn(env, *fn_p, eval_list(env, CDR(*form_p)));
    } else {
      printf_P(PSTR("ERROR: "));
      print_form(CAR(*form_p));
      printf_P(PSTR(" cannot be in function position.\n"));

      rval = NIL;
    }

    release(2); // form_p, fn_p
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
