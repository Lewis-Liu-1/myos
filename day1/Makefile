
IMAGE = loader.img
BINFILE=ipl.bin

build: $(IMAGE)

default :
	$(MAKE) img

$(BINFILE) : ipl.nas Makefile
	nasm ipl.nas -o $@
	nasm ipl.nas -o ipl.img

img : $(BINFILE) 
	mformat -f 1440 -C -B $(BINFILE)  -i helloos.img ::

$(IMAGE): $(BINFILE)  empty.img
	dd if=$(BINFILE)  of=$@
	dd if=empty.img of=$@ bs=512 skip=1 seek=1

empty.img:
	dd if=/dev/zero of=$@ bs=512 count=2880
	mkdosfs $@

asm :
	$(MAKE) $(BINFILE) 

run :	#only this option works
#	qemu-system-i386 helloos.img
#	boot from hard disk, it will show Load Error
#	qemu-system-i386 $(IMAGE)
#   boot from floppy, it will not show anything, it's a success!		
	qemu-system-i386 -fda $(IMAGE)	

test: # not working
	qemu-system-i386 -fda ipl.img

#	qemu-system-i386  -drive file=helloos.img,if=floppy	
#	qemu-system-x86_64 -m 32 -fda helloos.img
#	qemu-system-x86_64 -fda helloos.img

install :
	$(MAKE) img
	$(IMGTOL) w a: helloos.img

clean :
	-$(RM) *.img *.bin

src_only :
	$(MAKE) clean
	-$(DEL) helloos.img
