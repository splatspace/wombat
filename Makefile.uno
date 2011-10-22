# For Arduino Uno. See:
# http://balau82.wordpress.com/2011/03/29/programming-arduino-uno-in-pure-c/

# Your TTY might be different.  Look for /dev/tty.serial or .modem
# devices, and set TTY accordingly:
TTY        = /dev/ttyUSB0
BAUD       = 57600

DEVICE     = atmega328p
CLOCK      = 16000000UL
OBJECTS    = alist.o main.o print_form.o read_form.o types.o

AVRDUDE = avrdude -c arduino -p $(DEVICE) -P $(TTY) -b $(BAUD)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

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
