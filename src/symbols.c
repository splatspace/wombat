#include <uberlisp/symbols.h>

void init_syms(uptr_t *env) {
  SVAL(PS_LET) = S_LET;
  assoc(env, PS_LET, PS_LET);

  SVAL(PS_FN) = S_FN;
  assoc(env, PS_FN, PS_FN);

  SVAL(PS_DO) = S_DO;
  assoc(env, PS_DO, PS_DO);

  SVAL(PS_QUOTE) = S_QUOTE;
  assoc(env, PS_QUOTE, PS_QUOTE);

  SVAL(PS_CAR) = S_CAR;
  assoc(env, PS_CAR, PS_CAR);

  SVAL(PS_CDR) = S_CDR;
  assoc(env, PS_CDR, PS_CDR);

  SVAL(PS_CONS) = S_CONS;
  assoc(env, PS_CONS, PS_CONS);

  SVAL(PS_PRINT) = S_PRINT;
  assoc(env, PS_PRINT, PS_PRINT);

  SVAL(PS_DEF) = S_DEF;
  assoc(env, PS_DEF, PS_DEF);

  SVAL(PS_IF) = S_IF;
  assoc(env, PS_IF, PS_IF);

  SVAL(PS_AND) = S_AND;
  assoc(env, PS_AND, PS_AND);

  SVAL(PS_OR) = S_OR;
  assoc(env, PS_OR, PS_OR);

  SVAL(PS_NOT) = S_NOT;
  assoc(env, PS_NOT, PS_NOT);

  SVAL(PS_TRUE) = S_TRUE;
  assoc(env, PS_TRUE, PS_TRUE);

  SVAL(PS_EVAL) = S_EVAL;
  assoc(env, PS_EVAL, PS_EVAL);

  SVAL(PS_PLUS) = S_PLUS;
  assoc(env, PS_PLUS, PS_PLUS);

  SVAL(PS_MINUS) = S_MINUS;
  assoc(env, PS_MINUS, PS_MINUS);

  SVAL(PS_MULT) = S_MULT;
  assoc(env, PS_MULT, PS_MULT);

  SVAL(PS_DIV) = S_DIV;
  assoc(env, PS_DIV, PS_DIV);

  SVAL(PS_EQL) = S_EQL;
  assoc(env, PS_EQL, PS_EQL);

  SVAL(PS_NEQL) = S_NEQL;
  assoc(env, PS_NEQL, PS_NEQL);

  SVAL(PS_LT) = S_LT;
  assoc(env, PS_LT, PS_LT);

  SVAL(PS_LTE) = S_LTE;
  assoc(env, PS_LTE, PS_LTE);

  SVAL(PS_GT) = S_GT;
  assoc(env, PS_GT, PS_GT);

  SVAL(PS_GTE) = S_GTE;
  assoc(env, PS_GTE, PS_GTE);

  SVAL(PS_SREG) = S_SREG;
  assoc(env, PS_SREG, PS_SREG);

  SVAL(PS_SLP) = S_SLP;
  assoc(env, PS_SLP, PS_SLP);

  SVAL(PS_LOOP) = S_LOOP;
  assoc(env, PS_LOOP, PS_LOOP);

  SVAL(PS_RECUR) = S_RECUR;
  assoc(env, PS_RECUR, PS_RECUR);

  SVAL(PS_WHEN) = S_WHEN;
  assoc(env, PS_WHEN, PS_WHEN);

  SVAL(PS_BAND) = S_BAND;
  assoc(env, PS_BAND, PS_BAND);

  SVAL(PS_BOR) = S_BOR;
  assoc(env, PS_BOR, PS_BOR);

  SVAL(PS_BXOR) = S_BXOR;
  assoc(env, PS_BXOR, PS_BXOR);

  SVAL(PS_BSL) = S_BSL;
  assoc(env, PS_BSL, PS_BSL);

  SVAL(PS_BSR) = S_BSR;
  assoc(env, PS_BSR, PS_BSR);

  // Must be updated when symbols are added
  SEND_p = PS_BSR + 4;

  // Registers
  __mk_sym(S__DDRB);
  assoc(env, (SEND_p-4), (0x04 + __SFR_OFFSET));

  __mk_sym(S__PORTB);
  assoc(env, (SEND_p-4), (0x05 + __SFR_OFFSET));
}
