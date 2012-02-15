#include <uberlisp/debug.h>

void print_mem() {
    printf_P(PSTR("Total mem:\t%dB\nFree mem:\t%dB\tUsed mem:\t%dB\nCons mem:\t%dB\tSymbol mem:\t%dB\n"),
             TOTALMEM(), FREEMEM(), USEDMEM(), CONSMEM(), SYMMEM());
}

void print_env(uptr_t env) {
    printf_P(PSTR("env: "));
    print_form(env);
    printf_P(PSTR("\n"));
}
