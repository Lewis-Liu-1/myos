#include "x86.h"
#include "keyboard_map.h"
#include "header.h"
#include "color.h"
#include "bootpack.h"

#include <stdio.h>

unsigned int location = 0;
char *vga = (char *)0x000a0000; // 0xb8000;
int letter;

#ifdef __x86_64__
typedef unsigned long long int uword_t;
#else
typedef unsigned int uword_t;
#endif

struct interrupt_frame;

void asm_inthandler21(void);

void keyboard_handler(int *esp);

void keyboard_handler(int *esp)
{
    if (inb(0x64) & 0x01 && (letter = inb(0x60)) > 0)
    {
        vga[location] = 'a'; // keyboard_map[letter];
        vga[location + 1] = 0x4;
        location += 2;
    }
}

#endif

    /* 0xFD is 11111101 - enables only IRQ1 (keyboard) on master pic
       by clearing bit 1. bit is clear for enabled and bit is set for disabled */
    outb(0x21, curmask_master & 0xFD);
}

#if 0
void keyboard_handler1(int *esp)
{
	//for (;;)
	{
	//	io_halt();
	}

    signed char keycode;

    keycode = inb(0x60);
    /* Only print characters on keydown event that have
     * a non-zero mapping */
    if (keycode >= 0 && keyboard_map[keycode])
    {

        struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
        boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
        char s[2];
        sprintf(s, "%c", 'b');//keyboard_map[keycode]);
        putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");

        // vga[location++] = keyboard_map[keycode];
        /* Attribute 0x07 is white on black characters */
        // vga[location++] = 0x07;
    }

    /*
                PIC1   PIC2
    Commands    0x20   0xA0
    Data        0x21   0xA1

    // if (inb(0x64) & 0x01 && (letter = inb(0x60)) > 0)
    {
        //    vga[location] = keyboard_map[letter];
        //    vga[location + 1] = 0x4;
        //    location += 2;
    }
}
#endif
#define PORT_KEYDAT		0x0060

void keyboard_handler(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char data, s[4];
	outb(PIC0_OCW2, 0x61);	/* inform PIC irq 1 processed end */
	data = inb(PORT_KEYDAT);

	(sprintf)(s, "%d", data);
	boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);

    /* Send End of Interrupt (EOI) to master PIC */
    outb(0x20, 0x20);
	return;
}

