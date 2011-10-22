#ifndef _UBERLISP_TYPES_H
#define _UBERLISP_TYPES_H

/* Lisp Types *******************************/

enum types { SPECIAL, FUNCTION, SYMBOL, CONS, INT };

typedef struct {
  enum types type;
} Type;

typedef struct {
  enum types type;
  unsigned long hashcode;
  union Value {
    char *sval;
    int ival;
  } v;
} Atom;

typedef struct special {
  enum types type;
  char *name;
  void *fn;
} Special;

typedef struct function {
  enum types type;
  char *name;  
  void *fn;
} Function;

typedef struct cons {
  enum types type;
  void *car;
  void *cdr;
} Cons;

/* Booleans **********************************/

extern void *TRUE;
extern void *FALSE;
extern void *NIL;

/* Type Helpers ******************************/

enum types type(void* expr);
Cons* cons(void* car, void* cdr);
Cons* empty_cons();
Atom* integer(int ival);
Atom* sym(char *s);
int truthy(void* x);
void* equal(void* x, void* y);
void rfree(void* x);

/* Type Macros *******************************/

#define CONS(x) ((Cons*)x)
#define CAR(x)  ((Cons*)x)->car
#define CDR(x)  ((Cons*)x)->cdr
#define ATOM(x) ((Atom*)x)
#define IVAL(x) ((Atom*)x)->v.ival
#define SVAL(x) ((Atom*)x)->v.sval
#define HASHCODE(x) ((Atom*)x)->hashcode
#define PRINT_TYPE(x) printf("type: %s\n", type(x) == CONS ? "CONS" : type(x) == INT ? "INT" : "SYMBOL");

#endif
