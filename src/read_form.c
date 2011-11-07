#include <uberlisp/arduino_io.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uberlisp/types.h>
#include <uberlisp/read_form.h>

// TODO: Support quoting
uptr_t read_form() {
  buf_stdin();
  char *cur = STDIN_BUF;

  if (isdigit(*cur))
    return INTERN_INT(atoi(cur));

  if (isalpha(*cur))
    return build_symbol(cur);

  if (*cur == '\'')
    return NIL;

  char *openp, *closep = NULL;
  if (*cur == '(') {
    openp = cur;
    while ((++cur) != ')') {
      if (*cur == '(') openp = cur;
    }

    while((--cur) > openp)
  }

}
