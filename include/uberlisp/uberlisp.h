#ifndef _UBERLISP_H
#define _UBERLISP_H

#include <stdint.h>

#include <uberlisp/types.h>
/*
#include <uberlisp/read_form.h>
#include <uberlisp/print_form.h>
#include <uberlisp/alist.h>
*/

#ifdef ARDUINO

#include <uberlisp/arduino_io.h>

#define UPTR(cptr) ((uptr_t)(cptr))
#define CPTR(uptr) ((void *)(TO_PTR(uptr)))

#else

#include <uberlisp/fake_arduino.h>

#define PSTR(string) string
#define UPTR(cptr) ((uptr_t)((intptr_t)(cptr) - (intptr_t)MPOOL))
#define CPTR(uptr) ((void *)((intptr_t)(TO_PTR(uptr)) + (intptr_t)MPOOL))

#endif

#endif
