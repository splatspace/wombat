#ifdef ARDUINO
#include <uberlisp/arduino_io.h>
#else
#include <uberlisp/fake_arduino.h>
#endif
#include <string.h>
#include <stdio.h>

#include <uberlisp/read_form.h>
#include <uberlisp/print_form.h>

int main() {
  init_env();
  init_mem();

  uptr_t foobar = build_symbol("foobar");
  uptr_t lte = build_symbol("<=");
  uptr_t five = build_symbol("fivec");
  uptr_t lte2 = build_symbol("<=");
  uptr_t foobar2 = build_symbol("foobar");

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

  printf("%d == %d\n%d == %d\n", foobar, foobar2, lte, lte2);

  while(1) {
    printf("=> ");
    print_form(read_form(stdin));
    printf("\n");
  }

  return 0;

}
