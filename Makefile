
#########  AVR Project Makefile Template   #########
######										######
######	Copyright (C) 2003-2005,Pat Deegan, ######
######			Psychogenic Inc			 ######
######		  All Rights Reserved		   ######
######										######
###### You are free to use this code as part  ######
###### of your own applications provided	  ######
###### you keep this copyright notice intact  ######
###### and acknowledge its authorship with	######
###### the words:							 ######
######										######
###### "Contains software by Pat Deegan of	######
###### Psychogenic Inc (www.psychogenic.com)" ######
######										######
###### If you use it as part of a web site	######
###### please include a link to our site,	 ######
###### http://electrons.psychogenic.com  or   ######
###### http://www.psychogenic.com			 ######
######										######
####################################################


MCU=atxmega32a4

PROGRAMMER_MCU=x32a4

PROJECTNAME=strandcontroller

#export PATH=/usr/local/avr8-gnu-toolchain-linux_x86_64/avr/bin:$PATH
#export PREFIX=/usr/local/avr8-gnu-toolchain-linux_x86_64/

# Source files
# List C/C++/Assembly source files:
# (list all files to compile, e.g. 'a.c b.cpp as.S'):
# Use .cc, .cpp or .C suffix for C++ files, use .S 
# (NOT .s !!!) for assembly source code files.
PRJSRC=./src/asf/common/boards/user_board/init.c \
	./src/asf/common/services/serial/usart_serial.c \
	./src/asf/common/utils/stdio/read.c \
	./src/asf/common/utils/stdio/write.c \
	./src/asf/xmega/drivers/usart/usart.c \
	./src/commands.c \
	./src/usb.c \
	./src/init.c \
	./src/asf/common/services/clock/xmega/sysclk.c \
	./src/asf/common/services/ioport/xmega/ioport_compat.c \
	./src/asf/common/services/sleepmgr/xmega/sleepmgr.c \
	./src/asf/common/services/spi/xmega_spi/spi_master.c \
	./src/asf/common/services/usb/class/cdc/device/udi_cdc.c \
	./src/asf/common/services/usb/class/cdc/device/udi_cdc_desc.c \
	./src/asf/common/services/usb/udc/udc.c \
	./src/asf/xmega/drivers/cpu/ccp.S \
	./src/asf/xmega/drivers/nvm/nvm.c \
	./src/asf/xmega/drivers/nvm/nvm_asm.S \
	./src/asf/xmega/drivers/spi/spi.c \
	./src/asf/xmega/drivers/tc/tc.c \
	./src/asf/xmega/drivers/usb/usb_device.c \
	./src/main.c


# additional includes (e.g. -I/path/to/mydir)
INC=-I/usr/local/include/ \
	-I/usr/local/src/asf/xmega/drivers/nvm/ \
	-I./src/ \
	-I./src/asf/ \
	-I./src/asf/common/ \
	-I./src/asf/common/boards/ \
	-I./src/asf/common/boards/user_board/ \
	-I./src/asf/common/services/ \
	-I./src/asf/common/services/clock/ \
	-I./src/asf/common/services/clock/xmega/ \
	-I./src/asf/common/services/delay/ \
	-I./src/asf/common/services/delay/xmega/ \
	-I./src/asf/common/services/ioport/ \
	-I./src/asf/common/services/ioport/xmega/ \
	-I./src/asf/common/services/serial/ \
	-I./src/asf/common/services/serial/xmega_usart/ \
	-I./src/asf/common/services/sleepmgr/ \
	-I./src/asf/common/services/sleepmgr/xmega/ \
	-I./src/asf/common/services/spi/ \
	-I./src/asf/common/services/spi/xmega_spi/ \
	-I./src/asf/common/services/usb/ \
	-I./src/asf/common/services/usb/class/ \
	-I./src/asf/common/services/usb/class/cdc/ \
	-I./src/asf/common/services/usb/class/cdc/device/ \
	-I./src/asf/common/services/usb/udc/ \
	-I./src/asf/common/utils/ \
	-I./src/asf/common/utils/interrupt/ \
	-I./src/asf/common/utils/make/ \
	-I./src/asf/common/utils/stdio/ \
	-I./src/asf/common/utils/stdio/stdio_serial/ \
	-I./src/asf/xmega/ \
	-I./src/asf/xmega/drivers/ \
	-I./src/asf/xmega/drivers/cpu/ \
	-I./src/asf/xmega/drivers/nvm/ \
	-I./src/asf/xmega/drivers/pmic/ \
	-I./src/asf/xmega/drivers/sleep/ \
	-I./src/asf/xmega/drivers/spi/ \
	-I./src/asf/xmega/drivers/tc/ \
	-I./src/asf/xmega/drivers/usart/ \
	-I./src/asf/xmega/drivers/usb/ \
	-I./src/asf/xmega/utils/ \
	-I./src/asf/xmega/utils/assembler/ \
	-I./src/asf/xmega/utils/bit_handling/ \
	-I./src/asf/xmega/utils/preprocessor/ \
	-I./src/config/ \


# libraries to link in (e.g. -lmylib)
LIBS=

OPTLEVEL=1

AVRDUDE_PROGRAMMERID=dragon_pdi

AVRDUDE_PORT=usb


##### Flags ####

# HEXFORMAT -- format for .hex file output
HEXFORMAT=ihex

# compiler
CFLAGS=-I. $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-DBOARD=USER_BOARD -DIOPORT_XMEGA_COMPAT \
	-D__AVR_ATxmega32A4U__ \
	-fpack-struct -fshort-enums			 \
	-funsigned-bitfields -funsigned-char	\
	-Wall -Wstrict-prototypes -std=gnu99			   \
	-Wa,-ahlms=$(firstword				  \
	$(filter %.lst, $(<:.c=.lst)))

# c++ specific flags
CPPFLAGS=-fno-exceptions			   \
	-Wa,-ahlms=$(firstword		 \
	$(filter %.lst, $(<:.cpp=.lst))\
	$(filter %.lst, $(<:.cc=.lst)) \
	$(filter %.lst, $(<:.C=.lst)))

# assembler
ASMFLAGS =-I. $(INC) -mmcu=$(MCU)		\
	-x assembler-with-cpp			\
	-Wa,-gdwarf2


# linker
LDFLAGS=-Wl,-Map=$(TRG).map -Wl,-u,vfprintf -Wl,--start-group  -Wl,--end-group -Wl,--gc-sections -mrelax -Wl,--relax  -mmcu=atxmega32a4u \
	-lm $(LIBS)

##### executables ####
CC=avr-gcc
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
SIZE=avr-size
AVRDUDE=avrdude
REMOVE=rm -f

##### automatic target names ####
TRG=$(PROJECTNAME).out
DUMPTRG=$(PROJECTNAME).s

HEXROMTRG=$(PROJECTNAME).hex 
HEXTRG=$(HEXROMTRG) $(PROJECTNAME).ee.hex
GDBINITFILE=gdbinit-$(PROJECTNAME)

# Define all object files.

# Start by splitting source files by type
#  C++
CPPFILES=$(filter %.cpp, $(PRJSRC))
CCFILES=$(filter %.cc, $(PRJSRC))
BIGCFILES=$(filter %.C, $(PRJSRC))
#  C
CFILES=$(filter %.c, $(PRJSRC))
#  Assembly
ASMFILES=$(filter %.S, $(PRJSRC))


# List all object files we need to create
OBJDEPS=$(CFILES:.c=.o)	\
	$(CPPFILES:.cpp=.o)\
	$(BIGCFILES:.C=.o) \
	$(CCFILES:.cc=.o)  \
	$(ASMFILES:.S=.o)

# Define all lst files.
LST=$(filter %.lst, $(OBJDEPS:.o=.lst))

# All the possible generated assembly 
# files (.s files)
GENASMFILES=$(filter %.s, $(OBJDEPS:.o=.s)) 


.SUFFIXES : .c .cc .cpp .C .o .out .s .S \
	.hex .ee.hex .h .hh .hpp


.PHONY: writeflash clean stats gdbinit stats

# Make targets:
# all, disasm, stats, hex, writeflash/install, clean
all: $(TRG) hex

disasm: $(DUMPTRG) stats

stats: $(TRG)
	$(OBJDUMP) -h $(TRG)
	$(SIZE) $(TRG) 

hex: $(HEXTRG)


writeflash: hex
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	 -p $(PROGRAMMER_MCU) -P $(AVRDUDE_PORT) -e		\
	 -U flash:w:$(HEXROMTRG)

install: writeflash

$(DUMPTRG): $(TRG) 
	$(OBJDUMP) -S  $< > $@


$(TRG): $(OBJDEPS) 
	$(CC) $(LDFLAGS) -o $(TRG) $(OBJDEPS)


#### Generating assembly ####
# asm from C
%.s: %.c
	$(CC) -S $(CFLAGS) $< -o $@

# asm from (hand coded) asm
%.s: %.S
	$(CC) -S $(ASMFLAGS) $< > $@


# asm from C++
.cpp.s .cc.s .C.s :
	$(CC) -S $(CFLAGS) $(CPPFLAGS) $< -o $@



#### Generating object files ####
# object from C
.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@


# object from C++ (.cc, .cpp, .C files)
.cc.o .cpp.o .C.o :
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# object from asm
.S.o :
	$(CC) $(ASMFLAGS) -c $< -o $@


#### Generating hex files ####
# hex files from elf
#####  Generating a gdb initialisation file	#####
.out.hex:
	$(OBJCOPY) -j .text					\
		-j .data					   \
		-O $(HEXFORMAT) $< $@

.out.ee.hex:
	$(OBJCOPY) -j .eeprom				  \
		--change-section-lma .eeprom=0 \
		-O $(HEXFORMAT) $< $@


#####  Generating a gdb initialisation file	#####
##### Use by launching simulavr and avr-gdb:   #####
#####   avr-gdb -x gdbinit-myproject		   #####
#gdbinit: $(GDBINITFILE)

#$(GDBINITFILE): $(TRG)
#	@echo "file $(TRG)" > $(GDBINITFILE)
#	
#	@echo "target remote localhost:1212" \
#						>> $(GDBINITFILE)
#	
#	@echo "load"		>> $(GDBINITFILE) 
#	@echo "break main"  >> $(GDBINITFILE)
#	@echo "continue"	>> $(GDBINITFILE)
#	@echo
#	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"


#### Cleanup ####
clean:
	$(REMOVE) $(TRG) $(TRG).map $(DUMPTRG)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST) $(GDBINITFILE)
	$(REMOVE) $(GENASMFILES)
	$(REMOVE) $(HEXTRG)
