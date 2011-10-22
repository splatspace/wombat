/* Returns value at k or NIL if not found. */
void* get(void *alist, void* k);

/* Associates k with v, manipulating alist in place. */
void assoc(void** alist, void* k, void* v);
