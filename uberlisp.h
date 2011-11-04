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
#include "mmgmt.h"

#ifdef ARDUINO

#include "arduino_io.h"

#define UPTR(cptr) ((uptr_t)(cptr))
#define CPTR(uptr) ((void *)(uptr))

#else

#include "fake_arduino.h"

#define PSTR(string) string
#define UPTR(cptr) ((uptr_t)((intptr_t)(cptr) - (intptr_t)MPOOL))
#define CPTR(uptr) ((void *)((intptr_t)(uptr) + (intptr_t)MPOOL))

#endif

#define UPTR_PTR(uptr) ((uptr_t *)CPTR(uptr))
#define BYTE_PTR(uptr) ((uint8_t *)CPTR(uptr))
#define SYM_PTR(uptr) ((uint32_t *)CPTR(uptr))

#endif
