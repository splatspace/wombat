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


/* TYPES *******************************/

Cons* cons(void* car, void* cdr) {
  Cons* c = (Cons*)malloc(sizeof(Cons));
  c->type = CONS;
  c->car = car;
  c->cdr = cdr;
  return c;
}

Atom* make_atom() {
  return (Atom*)malloc(sizeof(Atom));
}

Atom* i(int ival) {
  Atom* a = make_atom();
  a->type = INT;
  a->v.ival = ival;
  return a;
}

Atom* s(char *s) {
  Atom* a = make_atom();
  a->type = SYMBOL;
  int len = strlen(s);
  a->v.sval = (char*)malloc(sizeof(char)*(len+1));
  strncpy(a->v.sval, s, len+1);
  a->hashcode = hash(s);
  return a;
}

void rfree(void* x) {
  switch(TYPE(x)) {
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
