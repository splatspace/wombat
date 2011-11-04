#ifndef _ARDUINO_IO_H
#define _ARDUINO_IO_H

#define ARDUINO_INIT_IO(baud) serial_init((F_CPU / baud / 16) - 1)

void init_env();

#endif
