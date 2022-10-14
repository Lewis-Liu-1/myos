
all:img

boot:
	nasm -o boot.bin ipl.nas
	nasm -o os.bin os.nas
	
img:boot
	dd if=boot.bin of=myos.img count=1 bs=512
	dd if=/dev/zero of=myos.img bs=512 seek=1 skip=1 count=2879
	
copy:
	mkdir -p /tmp/floppy
	sudo mount -o loop myos.img /tmp/floppy -o fat=12
	sleep 1
	sudo cp os.bin /tmp/floppy
	sleep 1
	sudo umount /tmp/floppy
run:copy
	qemu-system-i386 -drive file=myos.img,if=floppy

clean:
	rm *.bin *.img

