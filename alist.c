#include <stdio.h>
#include "types.h"
#include "alist.h"
#include "print.h"

void* get(void *l, void* k) {
  void *pair;
  if(CAR(l)) {
    while((pair = CAR(l))) {
      if(truthy(equal(CAR(pair), k))) return CDR(pair);
      if(!(l = CDR(l))) break;
    }
  }
  return NIL;
}

void assoc(void **l, void* k, void* v) {
  *l = cons(cons(k, v), *l);
}
