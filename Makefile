################################################################################
# These are variables for the GBA toolchain build
# You can add others if you wish to
# ***** YOUR NAME HERE *****
################################################################################

# The name of your desired GBA game
# This should be a just a name i.e MyFirstGBAGame
# No SPACES AFTER THE NAME.
PROGNAME = SpaceDocker

# The object files you want to compile into your program
# This should be a space (SPACE!) separated list of .o files
OFILES = main.o mylib.o

# The header files you have created.
# This is necessary to determine when to recompile for files.
# This should be a space (SPACE!) separated list of .h files
HFILES = gba_std.h

################################################################################
# These are various settings used to make the GBA toolchain work
# DO NOT EDIT BELOW.
################################################################################

TOOLDIR  = /Users/andy/devkitPro/devkitARM
ARMLIB   = $(TOOLDIR)/arm-none-eabi/lib
CFLAGS   = -Wall -Werror -std=c99 -pedantic -Wextra
CFLAGS   += -mthumb  # !!important
CFLAGS   += -mthumb-interwork -mlong-calls -nostartfiles -MMD -MP -I $(TOOLDIR)/include
LDFLAGS = -L $(TOOLDIR)/lib \
		  -L $(TOOLDIR)/lib/gcc/arm-none-eabi/5.3.0/thumb \
		  -L $(ARMLIB) \
		  --specs=$(ARMLIB)/gba.specs
CDEBUG   = -g -DDEBUG
CRELEASE = -O2
CC       = $(TOOLDIR)/bin/arm-none-eabi-gcc
AS       = $(TOOLDIR)/bin/arm-none-eabi-as
LD       = $(TOOLDIR)/bin/arm-none-eabi-gcc
OBJCOPY  = $(TOOLDIR)/bin/arm-none-eabi-objcopy
GDB      = $(TOOLDIR)/bin/arm-none-eabi-gdb
CFILES   = $(OFILES:.o=.c)

################################################################################
# These are the targets for the GBA build system
################################################################################

all : CFLAGS += $(CRELEASE)
all : $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

.PHONY : all clean

$(PROGNAME).gba : $(PROGNAME).elf
	@echo "[LINK] Linking objects together to create $(PROGNAME).gba"
	@$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba

$(PROGNAME).elf : $(OFILES)
	@$(LD) $(LDFLAGS) -o $(PROGNAME).elf $^ -lgcc -lc -lgcc $(LDDEBUG)
	@rm -f *.d

%.o : %.c
	@echo "[COMPILE] Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.s
	@echo "[ASSEMBLE] Assembling $<"
	@$(AS) $< -o $@ -mthumb -mthumb-interwork

clean :
	@echo "[CLEAN] Removing all compiled files"
	@rm -f *.o *.elf *.gba *.d

vba : CFLAGS += $(CRELEASE)
vba : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator VBA-M"
	@open $(PROGNAME).gba

med : CFLAGS += $(CRELEASE)
med : $(PROGNAME).gba
	@echo "[EXECUTE] Running emulator Mednafen"
	@mednafen $(PROGNAME).gba > /dev/null 2>&1

-include $(CFILES:%.c=%.d)

