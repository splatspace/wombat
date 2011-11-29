#include <uberlisp/symbols.h>

void init_syms(uptr_t *env) {
  __mk_sym(S_LET);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_FN);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_QUOTE);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_CAR);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_CDR);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_CONS);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_PRINT);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_DEF);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_IF);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_EVAL);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_PLUS);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_MINUS);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_LT);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_SREG);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_SLP);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_LOOP);
  assoc(env, UPTR(SEND_p-1), UPTR(SEND_p-1));

  // Registers
  __mk_sym(S__DDRB);
  assoc(env, UPTR(SEND_p-1), (0x04 + __SFR_OFFSET));

  __mk_sym(S__PORTB);
  assoc(env, UPTR(SEND_p-1), (0x05 + __SFR_OFFSET));
}
