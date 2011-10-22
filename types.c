#include <stdlib.h>
#include <string.h>

#include "types.h"

/* UTIL *******************************/

/* sdbm hash, http://www.cse.yorku.ca/~oz/hash.html */
unsigned long hash(char *str) {
  unsigned long hash = 0;
  int c;

  while ((c = *str++))
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

/* TYPE INITIALIZERS *******************/

Cons* cons(void* car, void* cdr) {
  Cons* c = (Cons*)malloc(sizeof(Cons));
  c->type = CONS;
  c->car = car;
  c->cdr = cdr;
  return c;
}

Atom* _make_atom() {
  return (Atom*)malloc(sizeof(Atom));
}

Atom* int_atom(int ival) {
  Atom* a = _make_atom();
  a->type = INT;
  a->v.ival = ival;
  return a;
}

Atom* sym_atom(char *s) {
  Atom* a = _make_atom();
  a->type = SYMBOL;
  int len = strlen(s);
  a->v.sval = (char*)malloc(sizeof(char)*(len+1));
  strncpy(a->v.sval, s, len+1);
  a->hashcode = hash(s);
  return a;
}

/* BOOLEAN INITIALIZERS *********/
/* TRUE and FALSE are externs in types.h. */

Atom _TRUE = { SYMBOL, 84, { "T" }};
Cons _FALSE = { CONS, NULL, NULL };
void *TRUE = (void*)&_TRUE;
void *FALSE = (void*)&_FALSE;

enum types type(void* expr) {
  return ((Type*)expr)->type;
}

/* EQUALITY ****************/

void* _eq(void* x, void* y) {
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
        : _eq(CAR(x), CAR(y))
          ? _eq(CDR(x), CDR(y))
            ? TRUE
            : FALSE
          : FALSE;
    default:
      return FALSE;
    }
  }
}

void rfree(void* x) {
  switch(type(x)) {
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
