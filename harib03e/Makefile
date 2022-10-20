subdir=./kernel

all:run

asm:
	cd boot ; make
	cd kernel ; make clean ; make

img:asm
	dd if=boot/bootloader of=myos.img bs=512 count=1
	dd if=/dev/zero of=myos.img bs=512 seek=1 skip=1 count=2879

copy:img
	mkdir -p /tmp/floppy
	mount -o loop myos.img /tmp/floppy -o fat=12
	sleep 1
	cp $(subdir)/kernel /tmp/floppy
	sleep 1
	umount /tmp/floppy
run:copy
	qemu-system-i386 -drive file=myos.img,if=floppy

clean:	
	rm myos.img
	cd boot ; make clean
	cd kernel ; make clean
