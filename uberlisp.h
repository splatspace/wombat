#ifndef _UBERLISP_H
#define _UBERLISP_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "read_form.h"
#include "print_form.h"
#include "alist.h"

#ifdef ARDUINO

#include "arduino_io.h"

typedef void * uptr_t;
#define UPTR(cptr) ((uptr_t)(cptr))
#define CPTR(uptr) ((void *)(uptr))

#else

#include "fake_arduino.h"

typedef uint16_t uptr_t;
#define PSTR(string) string
#define UPTR(cptr) ((uptr_t)((intptr_t)(cptr) - (intptr_t)MPOOL))
#define CPTR(uptr) ((void *)((intptr_t)(uptr) + (intptr_t)MPOOL))

#endif

#endif
