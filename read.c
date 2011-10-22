#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "read.h"

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
  char c;
  Cons* list = cons(NULL,NULL);
  Cons* cell = list;
  void* form;
  while ((c = getc(f)) != ')'){
    ungetc(c, f);
    form = read(f);
    if(CAR(cell) && !CDR(cell)) {
      cell->cdr = form;
    } else if (CAR(cell) && CDR(cell)){
      cell->cdr = cons(cell->cdr, form);
      cell = cell->cdr;
    } else {
      cell->car = form;
    }
    _gobble_whitespace(f);
  }
  return (void*)list;
}

void* read(FILE* f) {
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
    return cons(sym("quote"), read(f));
  }
  return read(f);
}
