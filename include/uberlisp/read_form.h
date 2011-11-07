#ifndef _READ_FORM_H
#define _READ_FORM_H

#include <stdio.h>
#include <uberlisp/types.h>

/* Reads the first Lisp form from f and returns it. */
uptr_t read_form(FILE *f);

#endif
