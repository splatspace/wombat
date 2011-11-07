#ifndef _ARDUINO_IO_H
#define _ARDUINO_IO_H

void buf_stdin();
void init_env();

extern char __heap_start;

char * const STDIN_BUF;

#endif
