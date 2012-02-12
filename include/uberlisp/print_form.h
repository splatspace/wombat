#ifndef _UBERLISP_PRINT_FORM_H
#define _UBERLISP_PRINT_FORM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <uberlisp/types.h>

/* Prints form, a Lisp data structure, to stdout. */
void print_form(uoff_t x);
/*void dbg_pf(void *form);*/

#endif
