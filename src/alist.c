#include <stdio.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>
#include <uberlisp/print_form.h>

void* get(Cons *alist, void* k) {
  if(CAR(alist)) {
    return (truthy(equal(CAR(CAR(alist)), k))) ? CDR(CAR(alist)) : get(CDR(alist), k);
  } else if(CDR(alist)) {
    return get(CDR(alist), k);
  } else {
    return NIL;
  }
}

void assoc(Cons **alist, void* k, void* v) {
  *alist = cons(cons(k, v), *alist);
}
