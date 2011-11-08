#include <uberlisp/arduino_io.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uberlisp/types.h>
#include <uberlisp/read_form.h>

uptr_t _read_integer(FILE *f, char pfx) {
  char buf[8];
  memset(buf, '\0', 7);
  int n = 0;
  char c;

  if (pfx) buf[n++] = pfx;

  for(; n < 7; ++n) {
    c = getc(f);

    if(isdigit(c))
      buf[n] = c;
    else {
      ungetc(c,f);
      break;
    }
  }
  return INTERN_INT(atoi(buf));
}

int _is_sym_char(char c) {
  return !isspace(c) && c != '(' && c != ')' && c != '\'' && c != '"';
}

uptr_t _read_symbol(FILE *f, char pfx) {
  char buf[7]; /* 6 character limit on symbols */
  memset(buf, '\0', 7);
  int n = 0;
  char c;

  if (pfx) buf[n++] = pfx;

  // read_form() guarantees that first char is not a digit
  // The rest could be
  for (; n < 6; ++n) {
    c = getc(f);
    if(_is_sym_char(c))
      buf[n] = c;
    else {
      ungetc(c,f);
      break;
    }
  }
  return build_symbol(buf);
}

int _is_whitespace(char c) {
  return isspace(c);
}

void _gobble_whitespace(FILE* f) {
  char c;
  while(_is_whitespace(c = (getc(f))));
  ungetc(c, f);
}

uptr_t _read_list(FILE* f) {
  _gobble_whitespace(f);
  char c = getc(f);
  if (c == ')')
    return NIL;
  else if (c == '.') {
    _gobble_whitespace(f);
    return read_form(f);
  } else {
    ungetc(c, f);
    uptr_t car = read_form(f);
    uptr_t cdr = _read_list(f);
    
    return build_cons(car, cdr);
  }
}

uptr_t read_form(FILE* f) {
  char c = getc(f);
  if(c == '-') {
    if (isdigit(c = getc(f))) {
      ungetc(c, f);
      return _read_integer(f, '-');
    } else {
      ungetc(c, f);
      return _read_symbol(f, '-');
    }
  } else if (isdigit(c)) {
    ungetc(c, f);
    return _read_integer(f, 0);
  } else if (_is_sym_char(c)) { // !isdigit() is implicit because of ordering
    ungetc(c, f);
    return _read_symbol(f, 0);
  } else if(c == '(') {
    return _read_list(f);
  } else if(c == '\'') {
    return build_cons(build_symbol("quote"), build_cons(read_form(f), NIL));
  }
  return read_form(f);
}

