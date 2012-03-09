#include <uberlisp/symbols.h>

void init_syms(uptr_t *env) {
  __mk_sym(S_LET);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_FN);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_DO);
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

  __mk_sym(S_AND);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_OR);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_NOT);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_TRUE);
  assoc(env, (SEND_p-4), (SEND_p-4));
  TRUE_p = SEND_p - 4;

  __mk_sym(S_EVAL);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_PLUS);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_MINUS);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_MULT);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_DIV);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_EQL);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_LT);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_LTE);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_GT);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_GTE);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_SREG);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_SLP);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_LOOP);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_RECUR);
  assoc(env, (SEND_p-4), (SEND_p-4));

  __mk_sym(S_WHEN);
  assoc(env, (SEND_p-4), (SEND_p-4));

  // Registers
  __mk_sym(S__DDRB);
  assoc(env, (SEND_p-4), (0x04 + __SFR_OFFSET));

  __mk_sym(S__PORTB);
  assoc(env, (SEND_p-4), (0x05 + __SFR_OFFSET));
}
