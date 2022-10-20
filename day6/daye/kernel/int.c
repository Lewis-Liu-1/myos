#include "x86.h"

struct IDT_entry
{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

// Variables for printing ==
unsigned int location = 0;
char *vga = (char *)0x000a0000; // 0xb8000;
char letter;
// =========================
#ifdef __x86_64__
typedef unsigned long long int uword_t;
#else
typedef unsigned int uword_t;
#endif

struct interrupt_frame;

void asm_inthandler21(void);
#if 1
//__attribute__((interrupt))

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

void inthandler2c(int *esp)
/* PS/2}EX©çÌèÝ */
{
    struct IDT_entry IDT[256];
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

void inthandler27(int *esp)
/* PIC0©çÌs®SèÝÎô */
/* Athlon64X2@ÈÇÅÍ`bvZbgÌsÉæèPICÌú»É±ÌèÝª1xŸ¯š±é */
/* ±ÌèÝÖÍA»ÌèÝÉÎµÄœàµÈ¢Åâèß²· */
/* ÈºœàµÈ­Ä¢¢ÌH
    š  ±ÌèÝÍPICú»ÌdCIÈmCYÉæÁÄ­¶µœàÌÈÌÅA
        Ü¶ßÉœ©µÄâéKvªÈ¢B									*/
{
    outb(PIC0_OCW2, 0x67); /* IRQ-07ót®¹ðPICÉÊm(7-1QÆ) */
    return;
}