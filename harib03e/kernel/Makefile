OBJDIR=.
APP=kernel

CFLAGS := $(CFLAGS) -O1 -fno-builtin -I$(OBJDIR) -MD                                                                                        
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -Wall -Wno-format -Wno-unused -Werror -gstabs -m32
CFLAGS += -fno-stack-protector
# -mgeneral-regs-only

LDFLAGS=-m elf_i386

OBJS =asmhead.o bootpack.o dsctbl.o graphic.o int.o naskfunc.o print.o

all:$(APP)

$(APP): $(OBJS)
#	ld $(LDFLAGS) -N -e start -Ttext $(addr) -o $@.out $^
	ld $(LDFLAGS) -Tloader.lds -o $@.out $^
	objdump -S $@.out >$@.asm
	objcopy -S -O binary $@.out $@

%.o:%.c
	gcc -nostdinc $(CFLAGS) -Os -c -o $@ $<

%.o:%.nas
	nasm -f elf -g -F stabs $<
	
clean:
	rm -f *.o *.out *.asm *.d $(APP)
