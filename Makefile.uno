# For Arduino Uno. See:
# http://balau82.wordpress.com/2011/03/29/programming-arduino-uno-in-pure-c/

# Your TTY might be different.  Look for /dev/tty.serial or .modem
# devices, and set TTY accordingly:
TTY        = /dev/tty.usbmodemfa141

DEVICE     = atmega328p
CLOCK      = 16000000UL
PROGRAMMER = -P $(TTY) -c arduino
OBJECTS    = main.o print.o read.o types.o

AVRDUDE = avrdude -F -V -c arduino -p $(DEVICE) $(PROGRAMMER) -b 115200
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
