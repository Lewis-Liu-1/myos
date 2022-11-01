APP=boot_sect.bin
SOURCE=boot_sect.asm

OBJS = boot_sect.o

all: $(APP)

$(APP): $(SOURCE)
	nasm $^ -f bin -o $@

%.o : %.asm
	nasm $< -f bin -o $@

run:
	qemu-system-i386 $(APP)

hard:
	qemu-system-i386 -hda $(APP)
#	-drive file=myos.img,if=floppy

show: $(APP)
	od -t x1 -A n $(APP)

hex:
	hexdump -C $(APP)

clean:
	rm -rf *.bin
