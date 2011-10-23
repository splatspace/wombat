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
  void *(*fn)(void **env, void *expr);
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

/* Booleans and Nil **************************/

extern void *TRUE;
extern void *FALSE;
extern void *NIL;

/* Type Helpers ******************************/

/* Gets an object's type */
enum types type(void* expr);

/* Lists */
Cons* cons(void* car, void* cdr);
void* append(void* tail, void* form);
Cons* empty();

/* Numerics */
Atom* integer(int ival);

/* Symbols */
Atom* sym(char *s);

/* Equality */
int truthy(void* x);
void* equal(void* x, void* y);

/* Management */
void rfree(void* x);

/* Type Macros *******************************/

#define CONS(x) ((Cons*)x)
#define CAR(x)  ((Cons*)x)->car
#define CDR(x)  ((Cons*)x)->cdr
#define ATOM(x) ((Atom*)x)
#define SPECIAL(x) ((Special*)x)
#define IVAL(x) ((Atom*)x)->v.ival
#define SVAL(x) ((Atom*)x)->v.sval
#define HASHCODE(x) ((Atom*)x)->hashcode
#define PRINT_TYPE(x) printf("type: %s\n", type(x) == CONS ? "CONS" : type(x) == INT ? "INT" : "SYMBOL");

#endif
