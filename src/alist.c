#include <stdio.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>
#include <uberlisp/print_form.h>

uptr_t get(uptr_t alist, uptr_t k) {
  if (IS_NIL(alist))
    return NIL;

  if (EQ(CAR(alist), k))
    return CADR(alist);

  return get(CDDR(alist), k);
}

void assoc(uptr_t *alist, uptr_t k, uptr_t v) {
  *alist = build_cons(k, build_cons(v, *alist));
}
