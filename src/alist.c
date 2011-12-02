#include <stdio.h>
#include <uberlisp/types.h>
#include <uberlisp/alist.h>
#include <uberlisp/print_form.h>

uptr_t _get(uptr_t alist, uptr_t k) {
  if (IS_NIL(alist))
    return NIL;

  if (EQ(CAR(alist), k))
    return CADR(alist);

  return _get(CDDR(alist), k);
}

uptr_t get(uptr_t k) {
  return _get(ENV_p, k);
}

void assoc(uptr_t k, uptr_t v) {
  ENV_p = build_cons(k, build_cons(v, ENV_p));
}

void __set_binding(uptr_t k, uptr_t v) {
  uptr_t alist = ENV_p;
  while (alist && !EQ(CAR(alist), k)) alist = CDDR(alist);
  if (alist) {
    *UPTR_PTR(CDR(alist)) = v | CADR_FLAG;
  } else
    assoc(k, v);
}
