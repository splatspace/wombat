#ifdef ARDUINO
#include <uberlisp/arduino_io.h>
#else
#include <uberlisp/fake_arduino.h>
#endif
/*#include <avr/pgmspace.h>*/
#include <string.h>
#include <stdio.h>

#include <uberlisp/read_form.h>
#include <uberlisp/print_form.h>

int main() {
  init_env();
  init_mem();

  /* printf_P(PSTR("__bss_end: %p\\n"), &__bss_end); */
  /* printf_P(PSTR("__heap_start: %p\\n"), &__heap_start); */
  /* printf_P(PSTR("CSTART_p: %p\\n"), CSTART_p); */
  /* printf_P(PSTR("CEND_p: %p\\n"), CEND_p); */
  /* printf_P(PSTR("SSTART_p: %p\\n"), SSTART_p); */
  /* printf_P(PSTR("SEND_p: %p\\n"), SEND_p); */

  /* uptr_t foobar = build_symbol("foobar"); */
  /* uptr_t lte = build_symbol("<="); */
  /* uptr_t five = build_symbol("fivec"); */
  /* uptr_t lte2 = build_symbol("<="); */
  /* uptr_t foobar2 = build_symbol("foobar"); */

  /* char buf[7]; */
  /* memset(buf, 0, 7); */
  /* unhash_sym(buf, foobar); */

  /* printf("foobar: %s\\n", buf); */

  /* memset(buf, 0, 7); */
  /* unhash_sym(buf, lte); */

  /* printf("<=: %s\\n", buf); */

  /* memset(buf, 0, 7); */
  /* unhash_sym(buf, five); */

  /* printf("fivec: %s\\n", buf); */

  /* printf("%d == %d\\n%d == %d\\n", foobar, foobar2, lte, lte2); */

  /* printf_P(PSTR("__bss_end: %p\\n"), &__bss_end); */
  /* printf_P(PSTR("__heap_start: %p\\n"), &__heap_start); */
  /* printf_P(PSTR("CSTART_p: %p\\n"), CSTART_p); */
  /* printf_P(PSTR("CEND_p: %p\\n"), CEND_p); */
  /* printf_P(PSTR("SSTART_p: %p\\n"), SSTART_p); */
  /* printf_P(PSTR("SEND_p: %p\\n"), SEND_p); */

  while(1) {
    printf("=> ");
    print_form(read_form(stdin));
    printf("\n");
  }


  return 0;

}
