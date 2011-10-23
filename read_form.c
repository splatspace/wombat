#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "read_form.h"

void* _read_integer(FILE* f) {
  char buf[17]; /* 16 digit limit on numbers */
  memset(buf, '\0', 17);
  int n = 0;
  char c;
  while ((c = getc(f))) {
    if(isdigit(c)) {
      buf[n++] = c;
    } else {
      ungetc(c,f);
      break;
    }
  }
  return (void*)integer(atoi(buf));
}

void* _read_symbol(FILE *f) {
  char buf[17]; /* 16 character limit on symbols */
  memset(buf, '\0', 17);
  int n = 0;
  char c;
  while ((c = getc(f))) {
    if(isalpha(c)) {
      buf[n++] = c;
    } else {
      ungetc(c,f);
      break;
    }
  }
  return (void*)sym(buf);
}

int _is_whitespace(char c) {
  return isspace(c);
}

void _gobble_whitespace(FILE* f) {
  char c;
  while(_is_whitespace(c = (getc(f))));
  ungetc(c, f);
}

void* _read_list(FILE* f) {
  _gobble_whitespace(f);
  char c = getc(f);
  if (c == ')')
    return NIL;
  else if (c == '.') {
    _gobble_whitespace(f);
    return read_form(f);
  } else {
    ungetc(c, f);
    void *car = read_form(f);
    void *cdr = _read_list(f);
    
    return cons(car, cdr);
  }
}

void* read_form(FILE* f) {
  char c = getc(f);
  if(isdigit(c)) {
    ungetc(c, f);
    return _read_integer(f);
  } else if(isalpha(c)) {
    ungetc(c, f);
    return _read_symbol(f);
  } else if(c == '(') {
    return _read_list(f);
  } else if(c == '\'') {
    return cons(sym("quote"), cons(read_form(f), NULL));
  }
  return read_form(f);
}
