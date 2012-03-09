#include <uberlisp/symbols.h>

void init_syms(uptr_t *env) {
  __mk_sym(S_LET);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_LET (SSTART_p + 0x0)

  __mk_sym(S_FN);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_FN (SSTART_p + 0x4)

  __mk_sym(S_DO);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_DO (SSTART_p + 0x8)

  __mk_sym(S_QUOTE);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_QUOTE (SSTART_p + 0xC)

  __mk_sym(S_CAR);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_CAR (SSTART_p + 0x10)

  __mk_sym(S_CDR);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_CDR (SSTART_p + 0x14)

  __mk_sym(S_CONS);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_CONS (SSTART_p + 0x18)

  __mk_sym(S_PRINT);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_PRINT (SSTART_p + 0x1C)

  __mk_sym(S_DEF);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_DEF (SSTART_p + 0x20)

  __mk_sym(S_IF);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_IF (SSTART_p + 0x24)

  __mk_sym(S_AND);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_AND (SSTART_p + 0x28)

  __mk_sym(S_OR);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_OR (SSTART_p + 0x2C)

  __mk_sym(S_NOT);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_NOT (SSTART_p + 0x30)

  __mk_sym(S_TRUE);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_TRUE (SSTART_p + 0x34)

  __mk_sym(S_EVAL);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_EVAL (SSTART_p + 0x38)

  __mk_sym(S_PLUS);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_PLUS (SSTART_p + 0x3C)

  __mk_sym(S_MINUS);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_MINUS (SSTART_p + 0x40)

  __mk_sym(S_MULT);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_MULT (SSTART_p + 0x44)

  __mk_sym(S_DIV);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_DIV (SSTART_p + 0x48)

  __mk_sym(S_EQL);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_EQL (SSTART_p + 0x4C)

  __mk_sym(S_NEQL);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_NEQL (SSTART_p + 0x50)

  __mk_sym(S_LT);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_LT (SSTART_p + 0x54)

  __mk_sym(S_LTE);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_LTE (SSTART_p + 0x58)

  __mk_sym(S_GT);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_GT (SSTART_p + 0x5C)

  __mk_sym(S_GTE);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_GTE (SSTART_p + 0x60)

  __mk_sym(S_SREG);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_SREG (SSTART_p + 0x64)

  __mk_sym(S_SLP);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_SLP (SSTART_p + 0x68)

  __mk_sym(S_LOOP);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_LOOP (SSTART_p + 0x6C)

  __mk_sym(S_RECUR);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_RECUR (SSTART_p + 0x70)

  __mk_sym(S_WHEN);
  assoc(env, (SEND_p-4), (SEND_p-4));
#define PS_WHEN (SSTART_p + 0x74)

  // Registers
  __mk_sym(S__DDRB);
  assoc(env, (SEND_p-4), (0x04 + __SFR_OFFSET));
#define PS__DDRB (SSTART_p + 0x78)

  __mk_sym(S__PORTB);
  assoc(env, (SEND_p-4), (0x05 + __SFR_OFFSET));
#define PS__PORTB (SSTART_p + 0x7C)
}
