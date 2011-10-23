#include <stdlib.h>
#include <string.h>
#include "types.h"

/* Utilities *******************************/

/* sdbm hash, http://www.cse.yorku.ca/~oz/hash.html */
unsigned long hash(char *str)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}


/* Type Initializers *******************/

Cons* _cons(void* car, void* cdr) {
  Cons* c = (Cons*)malloc(sizeof(Cons));
  c->type = CONS;
  c->car = car;
  c->cdr = cdr;
  return c;
}

Cons* cons(void* car, void* cdr) {
  Cons* c;
  if(cdr) {
    if(type(cdr) == CONS) {
      c = (Cons*)cdr;
      if(c->car && !c->cdr) {
        c->cdr = c->car;
        c->car = car;
        return c;
      } else if (c->car && c->cdr){
        return _cons(car, c);
      } else {
        c->car = car;
        return c;
      }
    }
  }
  return _cons(car, cdr);
}

Cons* empty() {
  return cons(NULL, NULL);
}

Atom* _make_atom() {
  return (Atom*)malloc(sizeof(Atom));
}

Atom* integer(int ival) {
  Atom* a = _make_atom();
  a->type = INT;
  a->v.ival = ival;
  return a;
}

Atom* sym(char *s) {
  Atom* a = _make_atom();
  a->type = SYMBOL;
  int len = strlen(s);
  a->v.sval = (char*)malloc(sizeof(char)*(len+1));
  strncpy(a->v.sval, s, len+1);
  a->hashcode = hash(s);
  return a;
}

/* Boolean Initializers *********/

Atom _TRUE = { SYMBOL, 2090770405, {"true"}};
Atom _FALSE = { SYMBOL, 258723568, {"false"}};
Atom _NIL = { SYMBOL, 193500360, {"nil"}};
void *TRUE = (void*)&_TRUE;
void *FALSE = (void*)&_FALSE;
void *NIL = (void*)&_NIL;

enum types type(void* expr) {
  return ((Type*)expr)->type;
}

int truthy(void* x) {
  return (x != NIL) && (x != FALSE);
}

/* Equality ****************/

void* equal(void* x, void* y) {
  if (x == y) {
    return TRUE;
  } else {
    switch(type(x)) {
    case INT:
      return (type(y) != INT)
        ? FALSE
        : (IVAL(x) == IVAL(y))
          ? TRUE
          : FALSE;
    case SYMBOL:
      return (type(y) != SYMBOL)
        ? FALSE
        : (HASHCODE(x) == HASHCODE(y))
          ? TRUE
          : FALSE;
    case CONS:
      return (type(y) != CONS)
        ? FALSE
        : equal(CAR(x), CAR(y))
          ? equal(CDR(x), CDR(y))
            ? TRUE
            : FALSE
          : FALSE;
    default:
      return FALSE;
    }
  }
}

void rfree(void* x) {
  if ((x != NIL) &&
      (x != TRUE) &&
      (x != FALSE) &&
      (x != NULL)) {
    switch(type(x)) {
    case SPECIAL:
      /* can't be freed */
      return;
    case FUNCTION:
      /* TODO */
      return;
    case INT:
      /* no members to free */
      break;
    case SYMBOL:
      free(SVAL(x));
      break;
    case CONS:
      rfree(CAR(x));
      rfree(CDR(x));
      break;
    }
    free(x);
  }
}
