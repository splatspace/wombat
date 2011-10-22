/* Lisp Types *******************************/

enum types { CONS, SYMBOL, INT };

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

typedef struct cons {
  enum types type;
  void *car;
  void *cdr;
} Cons;

/* Booleans **********************************/

extern void *TRUE;
extern void *FALSE;

/* Type Helpers ******************************/

enum types type(void* expr);
Cons* cons(void* car, void* cdr);
Atom* int_atom(int ival);
Atom* sym_atom(char *s);
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
