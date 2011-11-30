#include <uberlisp/symbols.h>

void init_syms() {
  __mk_sym(S_LET);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_FN);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_QUOTE);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_CAR);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_CDR);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_CONS);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_PRINT);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_DEF);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_IF);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_EVAL);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_PLUS);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_MINUS);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_LT);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_SREG);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_SLP);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  __mk_sym(S_LOOP);
  assoc(UPTR(SEND_p-1), UPTR(SEND_p-1));

  // Registers
  __mk_sym(S__DDRB);
  assoc(UPTR(SEND_p-1), (0x04 + __SFR_OFFSET));

  __mk_sym(S__PORTB);
  assoc(UPTR(SEND_p-1), (0x05 + __SFR_OFFSET));
}
