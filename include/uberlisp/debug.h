#ifndef _UBERLISP_DEBUG_H
#define _UBERLISP_DEBUG_H

#include <stdio.h>
#include <avr/pgmspace.h>
#include <uberlisp/types.h>
#include <uberlisp/print_form.h>

void print_mem();
void print_env(uptr_t env);

#endif
