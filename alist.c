#include <stdio.h>
#include "types.h"
#include "alist.h"
#include "print.h"

void* get(void *alist, void* k) {
  void *pair;
  if(CAR(alist)) {
    while((pair = CAR(alist))) {
      if(truthy(equal(CAR(pair), k))) return CDR(pair);
      if(!(alist = CDR(alist))) break;
    }
  }
  return NIL;
}

void assoc(void **alist, void* k, void* v) {
  *alist = cons(cons(k, v), *alist);
}
