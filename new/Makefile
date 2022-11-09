LDFLAGS=-m elf_i386
CFLAGS := $(CFLAGS) -O1 -fno-builtin -I$(OBJDIR) -MD -fno-omit-frame-pointer
CFLAGS += -Wall -Wno-format -Wno-unused -Werror -gstabs -m32 -Iinclude
addr=0xc400

BUILD_DIR=build
KERNEL=$(BUILD_DIR)/kernel.bin
BOOTLOADER=$(BUILD_DIR)/boot.bin

BOOT_SRC=boot/old_boot.nas
ENTRY=$(BUILD_DIR)/new_entry.o

SRC=$(wildcard kernel/*.c)

OBJS= $(patsubst kernel/%.c,build/%.o,$(SRC))
API_OBJS= $(patsubst api/%.asm,build/%.o,$(API_SRC))

all: build $(BOOTLOADER) $(KERNEL)

build:
	if [ ! -d "build" ]; then mkdir build; fi

$(BOOTLOADER):$(BOOT_SRC)
	nasm -o $@ $< 

$(BUILD_DIR)/%.o:%.nas
	nasm $< -f elf -o $@

$(BUILD_DIR)/%.o:kernel/%.S
	gcc -nostdinc $(CFLAGS) -c -o $@ $<

build/%.o: kernel/%.c
	gcc -fno-pie -m32 -ffreestanding $(CFLAGS) -c $< -o $@

$(KERNEL): $(ENTRY) $(OBJS)
	ld $(LDFLAGS) -N -e start -Ttext $(addr) -o $@.out $^
	objdump -S $@.out >$@.tmp
	objcopy -S -O binary $@.out $@

myos.img: $(BOOTLOADER) $(KERNEL)
	dd if=$(BOOTLOADER) of=$@ bs=512 count=1
	dd if=/dev/zero of=$@ bs=512 seek=1 skip=1 count=2879

copy: myos.img
	mkdir -p /tmp/floppy
	sudo mount -o loop myos.img /tmp/floppy -o fat=12
	sleep 1
	sudo cp $(KERNEL) /tmp/floppy
	sleep 1
	sudo umount /tmp/floppy
run: build copy
	qemu-system-i386 -drive file=myos.img,if=floppy

clean:
	rm -rf build *.img
	sudo umount /tmp/floppy
