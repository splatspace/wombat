#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "print_form.h"

/* Printing *******************************/

void print_form(void* form) {
  switch(type(form)) {
  case INT:
    printf("%d", IVAL(form));
    break;
  case SYMBOL:
    printf("%s", SVAL(form));
    break;
  case SPECIAL:
    printf("#<special:%s>", ((Special*)form)->name);
    break;
  case FUNCTION:
    printf("#<function>");
    break;
  case CONS:
    printf("(");
    do {
      if(CAR(form)) {
        print_form(CAR(form));
        if(CDR(form)) {
          if(type(CDR(form)) == CONS) {
            if(CAR(CDR(form))) {
              printf(" ");
            }
          } else {
            printf(" . ");
            print_form(CDR(form));
            break;
          }
        }
      } else {
        /* printf("CAR was empty.\n"); */
      }
    } while((form = CDR(form)));
    printf(")");
    break;
  }
}

void dbg_pfn(void *form, int tabc) {
  char *tabs = malloc(3*tabc + 1);
  memset(tabs, ' ', 3*tabc);
  tabs[3*tabc] = '\0';

  if (form == NULL) {
    printf("%sNULL\n", tabs);
    return;
  }

  switch(type(form)) {
    case SPECIAL: 
      ;
      Special *spcl = SPECIAL(form);
      printf("%stype: SPECIAL\n", tabs);
      printf("%sargeval: %d\n", tabs, spcl->argeval);
      printf("%sname: %s\n", tabs, spcl->name);
      printf("%s*fn: %p\n", tabs, spcl->fn);
      break;
    case SYMBOL:
      ;
      Atom *atm = ATOM(form);
      printf("%stype: SYMBOL\n", tabs);
      printf("%shashcode: %lu\n", tabs, atm->hashcode);
      printf("%ssval: %s\n", tabs, atm->v.sval);
      break;
    case CONS:
      ;
      Cons *cns = CONS(form);
      printf("%stype: CONS\n", tabs);
      printf("%scar: %p\n", tabs, CAR(cns));
      dbg_pfn(CAR(cns), tabc+1);
      printf("%scdr:\n", tabs);
      dbg_pfn(CDR(cns), tabc+1);
      break;
    case INT:
      ;
      Atom *intgr = ATOM(form);
      printf("%stype: INTEGER\n", tabs);
      printf("%shashcode: %lu\n", tabs, intgr->hashcode);
      printf("%sival: %d\n", tabs, intgr->v.ival);
      break;
    default:
      printf("%sI have no idea what you passed in here\n", tabs);
  }
}

void dbg_pf(void *form) {
  dbg_pfn(form, 0);
}
