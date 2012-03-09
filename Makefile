# For Arduino Uno. See:
# http://balau82.wordpress.com/2011/03/29/programming-arduino-uno-in-pure-c/

# Your TTY might be different.  Look for /dev/tty.serial, .modem, or USB
# devices, and set TTY accordingly:

DEVICE     = atmega328p
# DEVICE     = atmega128
CLOCK      = 16000000UL
# OBJECTS    = src/arduino_io.o src/alist.o src/main.o src/print_form.o src/read_form.o src/types.o
OBJECTS    = src/arduino_io.o src/print_form.o src/read_form.o src/types.o src/alist.o src/symbols.o src/gc.o src/debug.o src/main.o

# UNOTTY = /dev/ttyACM0
UNOTTY = /dev/tty.usbmodemfd121
UNOBAUD = 115200

# DUETTY = /dev/ttyUSB0
DUETTY = /dev/tty.usbserial-A700dEu5
DUEBAUD = 57600


AVRDUDE = avrdude -c arduino -p $(DEVICE) 
UNOOPTS = -P $(UNOTTY) -b $(UNOBAUD)
DUEOPTS = -P $(DUETTY) -b $(DUEBAUD)

# COMPILE = avr-gcc -I./include -Wall -Werror -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -DARDUINO -D__DELAY_BACKWARD_COMPATIBLE__ -Wl,--defsym=__heap_start=0x800800
COMPILE = avr-gcc -I./include -Wall -Werror -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -DARDUINO -D__DELAY_BACKWARD_COMPATIBLE__ -Wl,--defsym=__heap_start=0x800700

all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash-uno: all
	$(AVRDUDE) $(UNOOPTS) -U flash:w:main.hex:i

flash-due: all
	$(AVRDUDE) $(DUEOPTS) -U flash:w:main.hex:i

tty-uno:
	screen $(UNOTTY) $(UNOBAUD)

tty-due:
	screen $(DUETTY) $(DUEBAUD)

clean:
	rm -f main.hex main.elf $(OBJECTS)

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
