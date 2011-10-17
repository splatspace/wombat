/* LISP TYPES *******************************/

enum type { CONS, SYMBOL, INT };

typedef struct {
  enum type type;
} Type;

typedef struct {
  enum type type;
  unsigned long hashcode;
  union Value {
    int ival;
    char *sval;
  } v;
} Atom;

typedef struct cons {
  enum type type;
  void *car;
  void *cdr;
} Cons;

/* TYPE MACROS *******************************/

#define TYPE(x) ((Type*)x)->type
#define CONS(x) ((Cons*)x)
#define CAR(x)  ((Cons*)x)->car
#define CDR(x)  ((Cons*)x)->cdr
#define ATOM(x) ((Atom*)x)
#define IVAL(x) ((Atom*)x)->v.ival
#define SVAL(x) ((Atom*)x)->v.sval
#define CONS_P(x) TYPE(x) == CONS ? 1 : 0
#define SYMBOL_P(x) TYPE(x) == SYMBOL ? 1 : 0
#define INT_P(x) TYPE(x) == INT ? 1 : 0
#define PRINT_TYPE(x) printf("type: %s\n", TYPE(x) == CONS ? "CONS" : TYPE(x) == INT ? "INT" : "SYMBOL");

Cons* cons(void* car, void* cdr);
Atom* i(int ival);
Atom* s(char *s);
void rfree(void* x);
