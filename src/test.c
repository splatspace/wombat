#include <string.h>
#include <stdio.h>

#include <uberlisp/uberlisp.h>

int main() {
  init_env();
  init_mem();

  uptr_t foobar = symbol_alloc("foobar");
  uptr_t lte = symbol_alloc("<=");
  uptr_t five = symbol_alloc("fivec");

  char buf[7];
  memset(buf, 0, 7);
  unhash_sym(buf, foobar);

  printf("foobar: %s\n", buf);

  memset(buf, 0, 7);
  unhash_sym(buf, lte);

  printf("<=: %s\n", buf);

  memset(buf, 0, 7);
  unhash_sym(buf, five);

  printf("fivec: %s\n", buf);

  return 0;

}
