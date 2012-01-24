#include <uberlisp/symbols.h>

void init_syms() {
  __mk_sym(S_LET);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_FN);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_QUOTE);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_CAR);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_CDR);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_CONS);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_PRINT);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_DEF);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_IF);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_EVAL);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_PLUS);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_MINUS);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_LT);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_SREG);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_SLP);
  assoc((SEND_p-4), (SEND_p-4));

  __mk_sym(S_LOOP);
  assoc((SEND_p-4), (SEND_p-4));

  // Registers
  __mk_sym(S__DDRB);
  assoc((SEND_p-4), (0x04 + __SFR_OFFSET));

  __mk_sym(S__PORTB);
  assoc((SEND_p-4), (0x05 + __SFR_OFFSET));
}
