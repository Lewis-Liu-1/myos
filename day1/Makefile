led.bin : start.o led.o
	arm-linux-ld -Ttext 0x00000000 -o led.elf $^
	arm-linux-objcopy -O binary -S led.elf $@
%.o : %.c
	arm-linux-gcc -c -o $@ $<
%.o : %.S
	arm-linux-gcc -c -o $@ $<
clean :
	rm -f *.o *.bin *.elf
