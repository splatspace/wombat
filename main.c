#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#include "read_form.h"
#include "print_form.h"
#include "alist.h"

#ifdef ARDUINO
#include "arduino_io.h"
#endif

void *eval(Cons **env, void *expr);

void* car(Cons** env, void* arglist) {
  return CAR(CAR(arglist));
}

void* cdr(Cons** env, void* arglist) {
  return CDR(CAR(arglist));
}

void* quote(Cons** env, void* expr) {
  return CAR(expr);
}

void* eq(Cons** env, void* expr) {
  return equal(CAR(expr), CAR(CDR(expr)));
}

void* _cons(Cons** env, void* expr) {
  return cons(CAR(expr), CAR(CDR(expr)));
}

void *def(Cons **env, void *arglist) {
  Atom *var = CAR(arglist);
  Cons *binding = eval(env, CAR(CDR(arglist)));
  assoc(env, sym(SVAL(var)), binding);
  return binding;
}

void *_eval(Cons **env, void *arglist) {
  return eval(env, CAR(arglist));
}

Cons *eval_list(Cons **env, Cons *list) {
  if (list == NIL) return list;

  void *car = eval(env, CAR(list));
  void *cdr = eval_list(env, CDR(list));

  return cons(car, cdr);
}

void *execute_special(Cons **env, Special *op, Cons *arglist) {
  if (op->argeval) {
    return op->fn(env, eval_list(env, arglist));
  } else {
    return op->fn(env, arglist);
  }
}

void *eval(Cons **env, void *expr) {
  switch (type(expr)) {
    case SYMBOL:
      ; /* GCC is stupid */
      void *binding = get(*env, expr);
      if (binding != NIL)
        return binding;
      else {
        fprintf(stderr, "Error: Symbol '%s' is not defined.\n", SVAL(expr));
        return NIL;
      }
    case CONS:
      /* The empty list is self quoting */
      if (expr == NIL)
        return expr;

      void *op = eval(env, CAR(expr));

      if (op == NIL) {
        printf("nil op\n");
        return NIL;
      }

      void *arglist = CDR(expr);
      if(type(op) == SPECIAL)
        return execute_special(env, op, arglist);
      else {
        printf("not special: %d\n", type(op));
        return NIL;
      }

    default:
      return expr;
  }
}

  int main(int argc, char *argv[]) {

#ifdef ARDUINO
    ARDUINO_INIT_IO(9600);
#endif

    Cons *env = NIL;
    Special Car = { SPECIAL, 1, "car", &car };
    Special Cdr = { SPECIAL, 1, "cdr", &cdr };
    Special _Cons = { SPECIAL, 1, "cons", &_cons };
    Special Quote = { SPECIAL, 0, "quote", &quote };
    Special Eq = { SPECIAL, 1, "eq", &eq };
    Special Eval = { SPECIAL, 1, "eval", &_eval };
    Special Def = { SPECIAL, 0, "def", &def };

    assoc(&env, sym("car"), (void*)&Car);
    assoc(&env, sym("cdr"), (void*)&Cdr);
    assoc(&env, sym("cons"), (void*)&_Cons);
    assoc(&env, sym("quote"), (void*)&Quote);
    assoc(&env, sym("eq"), (void*)&Eq);
    assoc(&env, sym("eval"), (void*)&Eval);
    assoc(&env, sym("def"), (void*)&Def);

    /* NIL handled specially above. */
    assoc(&env, sym("false"), FALSE);
    assoc(&env, sym("true"), TRUE);

    assoc(&env, sym("ENV"), env);

    while(1) {
      printf("=> ");
      print_form(eval(&env, read_form(stdin)));
      /* print_form(read_form(stdin)); */
      printf("\n");
    }

    return 0;
  }
