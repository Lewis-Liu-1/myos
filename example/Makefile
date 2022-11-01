IMAGE=boot.img
all:
	nasm -f bin -o boot.com boot.asm
	dd if=boot.com of=boot.img bs=512 count=1
	dd if=/dev/zero of=/tmp/empty.img bs=512 count=2880
	dd if=/tmp/empty.img of=boot.img skip=1 seek=1 bs=512 count=2879

run :	#only this option works
	qemu-system-i386 -fda $(IMAGE)	
