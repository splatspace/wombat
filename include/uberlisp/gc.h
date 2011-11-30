#ifndef _UBERLISP_GC_H
#define _UBERLISP_GC_H

#include <string.h>
#include <stdio.h>

#include <uberlisp/types.h>

#define IS_ORPHAN(uptr) (IS_PTR(uptr) && !(*cur & (GC_FLAG | CADR_FLAG)))

void __GC__();

#endif
