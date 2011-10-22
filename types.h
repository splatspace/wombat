/* LISP TYPES *******************************/

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

/* BOOLEANS **********************************/

extern void *TRUE;
extern void *FALSE;

/* TYPE HELPERS ******************************/

enum types type(void* expr);
Cons* cons(void* car, void* cdr);
Atom* int_atom(int ival);
Atom* sym_atom(char *s);
void* _eq(void* x, void* y);
void rfree(void* x);

/* TYPE MACROS *******************************/

#define CONS(x) ((Cons*)x)
#define CAR(x)  ((Cons*)x)->car
#define CDR(x)  ((Cons*)x)->cdr
#define ATOM(x) ((Atom*)x)
#define IVAL(x) ((Atom*)x)->v.ival
#define SVAL(x) ((Atom*)x)->v.sval
#define HASHCODE(x) ((Atom*)x)->hashcode
