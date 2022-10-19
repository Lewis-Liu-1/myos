OBJDIR=.
APP=kernel

CFLAGS := $(CFLAGS) -O1 -fno-builtin -I$(OBJDIR) -MD                                                                                        
CFLAGS += -fno-omit-frame-pointer
CFLAGS += -Wall -Wno-format -Wno-unused -Werror -gstabs -m32
CFLAGS += -fno-stack-protector

LDFLAGS=-m elf_i386

all:$(APP)

$(APP):entry.o main.o screen.o print.o api.o int.o keyboard.o ps2.o
#	ld $(LDFLAGS) -N -e start -Ttext $(addr) -o $@.out $^
	ld $(LDFLAGS) -Tloader.lds -o $@.out $^
	objdump -S $@.out >$@.asm
	objcopy -S -O binary $@.out $@

entry.o:entry.S
	gcc -nostdinc $(CFLAGS) -c -o $@ $<

%.o:%.S
	gcc -nostdinc $(CFLAGS) -c -o $@ $<

%.o:%.c
	gcc -nostdinc $(CFLAGS) -Os -c -o $@ $<

%.o:%.nas
	nasm -f elf -g -F stabs $<
	
run:
	qemu-system-i386 -drive file=kernel,if=floppy

clean:
	@rm -f *.o *.out *.asm *.d $(APP)
