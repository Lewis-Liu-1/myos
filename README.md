
IPL             -- initial program loader
MOV AX, 0       -- AX=0

# Build an empty floppy image file using dd command:
```
[root]# dd if=/dev/zero of=./floppy.img bs=512 count=2880
2880+0 records in
2880+0 records out
1474560 bytes (1.5 MB) copied, 0.00821857 s, 179 MB/s
```
# Set up a loop device with the image file:

[root]# losetup /dev/loop0 ./floppy.img
# Create the file system with the loop device:

[root]# mkfs.msdos /dev/loop0
mkfs.msdos 3.0.16 (01 Mar 2013)
# Check the filesystem:

[root]# fsck.msdos /dev/loop0
dosfsck 3.0.16, 01 Mar 2013, FAT32, LFN
Logical sector size is zero.
# Clean up and delete the loop device:

[root]# losetup -d /dev/loop0 

# Mount the loop device:
[root]# mount -o loop -t auto floppy.img /mnt/floppy/


OSAK PAGE 113 explains GDT IDT
