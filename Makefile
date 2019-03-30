CC = avr-gcc
#Compiler wird definiert
OBJCPY = avr-objcopy
SIZE = avr-size
#Groesse vom Code
MCU = atmega328p
#Microcontrollertyp
PROGRAMMER = -c usbasp -P usb
F_CPU = 8000000
#Systemtakt in Hz
U8G2_SRC = ../../../../csrc
#Ortnerstruktur
CFLAGS = \
	-mmcu=$(MCU) \
	-DF_CPU=$(F_CPU)UL \
	-Os \
	-std=gnu99 \
	-Werror \
	-ffunction-sections \
	-fdata-sections \
	-I$(U8G2_SRC)/ \
	-I../lib/ \
	-DSCK_DDR=DDRB \
	-DSCK_BIT=5 \
	-DMOSI_DDR=DDRB \
	-DMOSI_BIT=3
LDFLAGS = \
	-Wl,--gc-sections \
	-mmcu=$(MCU)
AVRDUDE=avrdude $(PROGRAMMER) -p $(MCU) -v
PORT=usb
#Schnittstelle

SRC = $(shell ls $(U8G2_SRC)/*.c) $(shell ls ../lib/*.c) $(shell ls ./*.c)

OBJ = $(SRC:.c=.o)

main.hex: main.elf
	$(OBJCPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature main.elf main.hex
#Compiliertes Hexfile
	
all: main.elf
#Befehl zum compilieren

main.elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

size: main.elf
	$(SIZE) --mcu=$(MCU) --format=avr main.elf
#Gibt die Groesse vom Code an

clean:
	rm -f $(OBJ) main.elf main.hex
#Loescht das Compilierte Hex File

program: main.hex
	$(AVRDUDE) -Uflash:w:main.hex:i
#Befehl um den Hexcode auf den uC zu Flashen
