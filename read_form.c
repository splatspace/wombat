#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "read_form.h"

/* READING *******************************/

void* read_int(FILE* f) {
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
  return (void*)i(atoi(buf));
}

void* read_symbol(FILE *f) {
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
  return (void*)s(buf);
}

int is_whitespace(char c) {
  return isblank(c);
}

void gobble_whitespace(FILE* f) {
  char c;
  while(is_whitespace(c = (getc(f))));
  ungetc(c, f);
}

void* read_list(FILE* f) {
  char c;
  Cons* list = cons(NULL,NULL);
  Cons* cell = list;
  void* form;
  while ((c = getc(f)) != ')'){
    ungetc(c, f);
    form = read_form(f);
    /* T(form); */
    if(CAR(cell) && !CDR(cell)) {
      cell->cdr = form;
    } else if (CAR(cell) && CDR(cell)){
      cell->cdr = cons(cell->cdr, form);
      cell = cell->cdr;
    } else {
      cell->car = form;
    }
    gobble_whitespace(f);
  }

  return (void*)list;
}

void* read_form(FILE* f) {
  char c = getc(f);
  if(isdigit(c)) {
    ungetc(c, f);
    return read_int(f);
  } else if(isalpha(c)) {
    ungetc(c, f);
    return read_symbol(f);
  } else if(c == '(') {
    return read_list(f);
  } else if(c == '\'') {
    return cons(s("quote"), read_form(f));
  }
  return read_form(f);
}