
LDFLAGS=-m elf_i386

BOOT_OBJ=build/entry.o

SRC=$(wildcard kernel/*.c)
OBJS= $(patsubst kernel/%.c,build/%.o,$(SRC))

all: test img

test:
	if [ ! -d "build" ]; then mkdir build; fi

boot.bin: boot/boot.asm
	nasm -o boot.bin boot/boot.asm

kernel.bin: $(BOOT_OBJ) $(OBJS) 
	ld $(LDFLAGS) -Tloader.lds -o $@.out $^
	objcopy -S -O binary $@.out $@
#	gcc -m 32 -s -o $@ -Ttext 0xc400 $^
#	ld -m elf_i386 -s -o $@ -Ttext 0xc400 $^ --oformat binary

#cat $(BOOT_OBJ) $(OBJS)>kernel.bin

build/%.o: asm/%.asm
	nasm $< -f elf -o $@

build/%.o: kernel/%.c
	gcc -fno-pie -m32 -ffreestanding -c $< -o $@

boot: boot.bin

kernel: kernel.bin

img: boot kernel
	dd if=boot.bin of=myos.img bs=512 count=1
	dd if=/dev/zero of=myos.img bs=512 seek=1 skip=1 count=2879

copy: test img
	mkdir -p /tmp/floppy
	sudo mount -o loop myos.img /tmp/floppy -o fat=12
	sleep 1
	sudo cp kernel.bin /tmp/floppy
	sleep 1
	sudo umount /tmp/floppy

run: copy
	qemu-system-i386 -drive file=myos.img,if=floppy

clean:
	@rm -rf *.bin *.img build *.out
