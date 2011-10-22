/* Returns value at k or NIL if not found. */
void* get(void *l, void* k);

/* Associates k with v, manipulating alist in place. */
void assoc(void** l, void* k, void* v);
