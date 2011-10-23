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

void* car(void** env, void* expr) {
  return CAR(CONS(expr));
}

void* cdr(void** env, void* expr) {
  return CDR(CONS(expr));
}

void* quote(void** env, void* expr) {
  return CAR(expr);
}

void* eq(void** env, void* expr) {
  return equal(CAR(expr), CAR(CDR(expr)));
}

void* _cons(void** env, void* expr) {
  return cons(CAR(expr), CAR(CDR(expr)));
}

void* eval(void** env, void* expr) {
  void *op, *args, *eroot, *eargs, *sym;
  switch(type(expr)) {
  case CONS:
    {
      /* The empty list is self-quoting. */
      if(!CAR(expr) && !CDR(expr)) {
        return expr;
      }

      if(type(CAR(expr)) != SYMBOL) {
        print_form(expr);
        PRINT_TYPE(CAR(expr));
        fprintf(stderr, "Error: Only symbols can be in function position.\n");
        return NIL;
      }

      if ((op = get(env, CAR(expr))) == NIL) {
        fprintf(stderr, "Error: Symbol '%s' is not defined.\n", SVAL(op));
        return NIL;
      }

      if(type(op) == SPECIAL) {
        if(SPECIAL(op)->argeval) {
          /* Special forms that require argument evaluation */
          args = CDR(expr);
          eroot = eargs = empty();
          while(1) {
            if(CAR(args)) eargs = append(eargs, eval(env, CAR(args)));
            if(!(args = CDR(args))) break;
          }
          return SPECIAL(op)->fn(env, CDR(eroot));
        } else {
          /* Special forms that do not require argument evaluation */
          return SPECIAL(op)->fn(env, CDR(expr));
        }
      }

      if(type(op) == FUNCTION) {
        /* TODO */
      }

      return NIL;
    }
    case SYMBOL:
      if (HASHCODE(expr) == djb2_hash("nil")) {
        return NIL;
      } else if ((sym = get(env, expr)) != NIL) {
        return sym;
      } else {
        fprintf(stderr, "Error: Symbol '%s' is not defined.\n", SVAL(expr));
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

    void *env = empty();
    Special Car = { SPECIAL, 1, "car", &car };
    Special Cdr = { SPECIAL, 1, "cdr", &cdr };
    Special _Cons = { SPECIAL, 1, "cons", &_cons };
    Special Quote = { SPECIAL, 0, "quote", &quote };
    Special Eq = { SPECIAL, 1, "eq", &eq };
    Special Eval = { SPECIAL, 1, "eval", &eval };

    assoc(&env, sym("car"), (void*)&Car);
    assoc(&env, sym("cdr"), (void*)&Cdr);
    assoc(&env, sym("cons"), (void*)&_Cons);
    assoc(&env, sym("quote"), (void*)&Quote);
    assoc(&env, sym("eq"), (void*)&Eq);
    assoc(&env, sym("eval"), (void*)&Eval);

    /* NIL handled specially above. */
    assoc(&env, sym("false"), FALSE);
    assoc(&env, sym("true"), TRUE);

    assoc(&env, sym("ENV"), env);

    while(1) {
      printf("=> ");
      print_form(eval(env, read_form(stdin)));
      /* print_form(read_form(stdin)); */
      printf("\n");
    }

    return 0;
  }
