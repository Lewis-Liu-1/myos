/* èÝÖW */

#include "header.h"
#include "color.h"

void init_pic(void)
/* PICÌú» */
{
    outb(PIC0_IMR, 0xff); /* SÄÌèÝðó¯t¯È¢ */
    outb(PIC1_IMR, 0xff); /* SÄÌèÝðó¯t¯È¢ */

    outb(PIC0_ICW1, 0x11);   /* GbWgK[h */
    outb(PIC0_ICW2, 0x20);   /* IRQ0-7ÍAINT20-27Åó¯é */
    outb(PIC0_ICW3, 1 << 2); /* PIC1ÍIRQ2ÉÄÚ± */
    outb(PIC0_ICW4, 0x01);   /* mobt@[h */

    outb(PIC1_ICW1, 0x11); /* GbWgK[h */
    outb(PIC1_ICW2, 0x28); /* IRQ8-15ÍAINT28-2fÅó¯é */
    outb(PIC1_ICW3, 2);    /* PIC1ÍIRQ2ÉÄÚ± */
    outb(PIC1_ICW4, 0x01); /* mobt@[h */

    outb(PIC0_IMR, 0xfb); /* 11111011 PIC1ÈOÍSÄÖ~ */
    outb(PIC1_IMR, 0xff); /* 11111111 SÄÌèÝðó¯t¯È¢ */

    return;
}

void inthandler21(int *esp)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");
    for (;;)
    {
        io_halt();
    }
}

void inthandler2c(int *esp)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
    for (;;)
    {
        io_halt();
    }
}

void inthandler27(int *esp)
{
    outb(PIC0_OCW2, 0x67); /* IRQ-07ót®¹ðPICÉÊm(7-1QÆ) */
    return;
}