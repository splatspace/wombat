#include <stdio.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>
#include <uberlisp/print_form.h>

uptr_t get(uptr_t alist, uptr_t k) {
  if (IS_NIL(alist))
    return NIL;

  if (EQ(CAAR(alist), k))
    return CDAR(alist);

  return get(CDR(alist), k);
}

void assoc(uptr_t *alist, uptr_t k, uptr_t v) {
  *alist = build_cons(build_cons(k, v), *alist);
}
