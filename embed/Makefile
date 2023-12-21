# upload port - replace with name of your upload port
UPLOAD_PORT = /dev/ttyACM0

# name of the output hex file
PRJ = main

################################################################
# It's probably a bad idea to touch things after here unless
# you know what you're doing or something has gone very wrong
# The remaining settings (that shouldn't be changed) and the
# actual build options are below
################################################################

PRG = stk500v2 -P $(UPLOAD_PORT) 
# avr mcu
MCU = atmega324a
# mcu clock frequency
CLK = 16000000

# program source files (not including external libraries)
SRC = $(wildcard *.c)
# where to look for external libraries 
EXT =


# include path
INCLUDE := $(foreach dir, $(EXT), -I$(dir))
# c/assembly flags
CFLAGS    = -Wall -Os -DF_CPU=$(CLK) -mmcu=$(MCU) $(INCLUDE)
# any aditional flags for c++
CPPFLAGS =

# executables
AVRDUDE = avrdude -c $(PRG) -p $(MCU)
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size --format=avr --mcu=$(MCU)
CC      = avr-gcc

# generate list of objects
ASMFILES  = $(filter %.S, $(SRC))
CFILES    = $(filter %.c, $(SRC))
EXTC     := $(foreach dir, $(EXT), $(wildcard $(dir)/*.c))
CPPFILES  = $(filter %.cpp, $(SRC))
EXTCPP   := $(foreach dir, $(EXT), $(wildcard $(dir)/*.cpp))
OBJ       = $(CFILES:.c=.o) $(EXTC:.c=.o) $(CPPFILES:.cpp=.o) $(EXTCPP:.cpp=.o) $(ASMFILES:.c=.o)

# user targets
# compile all files
all: $(PRJ).hex

# test programmer connectivity
test:
	$(AVRDUDE) -v

# upload program to mcu
upload: all
	$(AVRDUDE) -U flash:w:$(PRJ).hex:i

# generate disassembly files for debugging
disasm: $(PRJ).elf
	$(OBJDUMP) -d $(PRJ).elf

# remove compiled files
clean:
	rm -f *.hex *.elf *.o
	$(foreach dir, $(EXT), rm -f $(dir)/*.o;)

# other targets
# objects from c files
.S.o:
	$(CC) $(CFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# objects from c++ files
.cpp.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# elf file
$(PRJ).elf: $(OBJ)
	$(CC) $(CFLAGS) -o $(PRJ).elf $(OBJ)

# hex file
$(PRJ).hex: $(PRJ).elf
	rm -f $(PRJ).hex
	$(OBJCOPY) -j .text -j .data -O ihex $(PRJ).elf $(PRJ).hex
	$(SIZE) $(PRJ).elf
