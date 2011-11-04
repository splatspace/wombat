#ifndef _MMGMT_H
#define _MMGMT_H

uptr_t CSTART_p;
uptr_t CEND_p;
uptr_t SSTART_p;
uptr_t SEND_p;

void init_mem();
uptr_t build_cons(uptr_t car, uptr_t cdr);
uptr_t symbol_alloc(char *name);

#endif
