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

#endif

void init_idt()
{
    struct IDT_entry IDT[256];
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    keyboard_address = (unsigned long) asm_inthandler21;
    //keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = 0x8;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = 0x8e;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    /*
                PIC1   PIC2
    Commands    0x20   0xA0
    Data        0x21   0xA1

    */

    // ICW1 - init
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2 - reset offset address if IDT
    // first 32 interrpts are reserved
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // ICW3 - setup cascading
    outb(0x21, 0b0);
    outb(0xA1, 0b0);

    // ICW4 - env info
    outb(0x21, 0b00000011);
    outb(0xA1, 0b00000011);
    // init finished

    // disable IRQs except IRQ1
    outb(0x21, 0xFD);
    outb(0xA1, 0xff);

    idt_address = (unsigned long)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    __asm__ __volatile__("lidt %0" ::"m"(*idt_ptr));
    __asm__ __volatile__("sti");
}