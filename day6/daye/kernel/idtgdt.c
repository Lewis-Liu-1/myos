#include "x86.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21

#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define PIC_EOI 0x20
#define PIC_1_CTRL 0x20
#define PIC_2_CTRL 0xA0
#define PIC_1_DATA 0x21
#define PIC_2_DATA 0xA1

struct SEGMENT_DESCRIPTOR
{
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR
{
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};

struct idt_entry
{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short int offset_higherbits;
} __attribute__((packed));

struct idt_pointer
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt_table[256];
struct idt_pointer idt_ptr;

void asm_inthandler21(void);
void load_gdtr(int limit, int addr);
void load_idtr1(void *ptr); // int limit, int addr);

static void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
    if (limit > 0xfffff)
    {
        ar |= 0x8000; /* G_bit = 1 */
        limit /= 0x1000;
    }
    sd->limit_low = limit & 0xffff;
    sd->base_low = base & 0xffff;
    sd->base_mid = (base >> 16) & 0xff;
    sd->access_right = ar & 0xff;
    sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->base_high = (base >> 24) & 0xff;
    return;
}

static void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
    gd->offset_low = offset & 0xffff;
    gd->selector = selector;
    gd->dw_count = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high = (offset >> 16) & 0xffff;
    return;
}

static void init_gdt(void)
{
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)0x00270000;
    int i;

    /* GDT�̏����� */
    // for (i = 0; i < 8192; i++)
    for (i = 0; i < 0xffff / 8; i++)
    {
        set_segmdesc(gdt + i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092); // AR_DATA32_RW
    set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a); // AR_CODE32_ER

    // load segment upper limit and address to GDTR (48 bits register)
    load_gdtr(0xffff, 0x00270000);
}

static void load_idt_entry(int isr_number, unsigned long base, short int selector, unsigned char flags)
{
    idt_table[isr_number].offset_lowerbits = base & 0xFFFF;
    idt_table[isr_number].offset_higherbits = (base >> 16) & 0xFFFF;
    idt_table[isr_number].selector = selector;
    idt_table[isr_number].flags = flags;
    idt_table[isr_number].zero = 0;
}

static void initialize_idt_pointer()
{
    idt_ptr.limit = (sizeof(struct idt_entry) * 256) - 1;
    idt_ptr.base = (unsigned int)&idt_table;
}

static void initialize_pic(void)
{
    /* ICW1 - begin initialization */
    outb(PIC_1_CTRL, 0x11);
    outb(PIC_2_CTRL, 0x11);

    /* ICW2 - remap offset address of idt_table */
    /*
     * In x86 protected mode, we have to remap the PICs beyond 0x20 because
     * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
     */
    outb(PIC_1_DATA, 0x20);
    outb(PIC_2_DATA, 0x28);

    /* ICW3 - setup cascading */
    outb(PIC_1_DATA, 0x00);
    outb(PIC_2_DATA, 0x00);

    /* ICW4 - environment info */
    outb(PIC_1_DATA, 0x01);
    outb(PIC_2_DATA, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    outb(0x21, 0xff);
    outb(0xA1, 0xff);
}

static void init_idt(void)
{
    initialize_pic();
    initialize_idt_pointer();
    load_idtr1(&idt_ptr);

    load_idt_entry(0x21, (unsigned long)asm_inthandler21, 0x08, 0x8e);
}

void init_gdt_idt1(void)
{
    // init_gdt();
    init_idt();
}