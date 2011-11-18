#include <uberlisp/symbols.h>

void init_syms(uptr_t *env) {
  __mk_sym(S_LET);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_FN);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_QUOTE);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_CAR);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_CDR);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_CONS);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_PRINT);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_DEF);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_IF);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_EVAL);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_PLUS);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_MINUS);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_LT);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_SREG);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_SLP);
  assoc(env, (SEND_p-4), (SEND_p-4));

  // Registers
  __mk_sym(S__DDRB);
  assoc(env, (SEND_p-4), DDRB);

  __mk_sym(S__PORTB);
  assoc(env, (SEND_p-4), PORTB);
}
