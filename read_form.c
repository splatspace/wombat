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

void* append(void* tail, void* form) {
  void* new_tail = empty();
  CAR(new_tail) = form;
  CDR(tail) = new_tail;
  return new_tail;
}

void* _read_list(FILE* f) {
  char c;
  Cons *list, *cell;
  list = cell = empty();
  while ((c = getc(f)) != ')'){
    ungetc(c, f);
    cell = append(cell, read_form(f));
    _gobble_whitespace(f);
  }
  return (void*)list;
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
    return cons(sym("quote"), read_form(f));
  }
  return read_form(f);
}
