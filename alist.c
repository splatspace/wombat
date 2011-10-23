#include <stdio.h>
#include "types.h"
#include "alist.h"
#include "print_form.h"

void* get(void *alist, void* k) {
  void *pair;
  if(CAR(alist)) {
    if(equal(CAR(CAR(alist)), k)) return CAR(alist);
  } else if(CDR(alist)) {
    return get(alist, k);
  } else {
    return NIL;
  }
}

void assoc(void **alist, void* k, void* v) {
  *alist = cons(cons(k, v), *alist);
}
