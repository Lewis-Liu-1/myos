#pragma once
#include "mm.h"

/* asmhead.nas */
struct BOOTINFO
{				/* 0x0ff0-0x0fff */
	char cyls;	/* �u�[�g�Z�N�^�͂ǂ��܂Ńf�B�X�N��ǂ񂾂̂� */
	char leds;	/* �u�[�g���̃L�[�{�[�h��LED�̏�� */
	char vmode; /* �r�f�I���[�h  ���r�b�g�J���[�� */
	char reserve;
	short scrnx, scrny; /* ��ʉ𑜓x */
	char *vram;
};

#define ADR_BOOTINFO 0x00000ff0

struct MOUSE_DEC
{
	unsigned char buf[3], phase;
	int x, y, btn;
};

/* naskfunc.nas */
// void io_hlt(void);
// void io_cli(void);
// void io_sti(void);
// void outb(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr1(void *addr);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
void enable_mouse(struct MOUSE_DEC *mdec);

/* fifo.c */
struct FIFO8
{
	unsigned char *buf;
	int p, q, size, free, flags;
};
void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf);
int fifo8_put(struct FIFO8 *fifo, unsigned char data);
int fifo8_get(struct FIFO8 *fifo);
int fifo8_status(struct FIFO8 *fifo);

/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
				 int pysize, int px0, int py0, char *buf, int bxsize);
#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

/* dsctbl.c */
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

#define ADR_IDT 0x0026f800
#define LIMIT_IDT 0x000007ff
#define ADR_GDT 0x00270000
#define LIMIT_GDT 0x0000ffff
#define ADR_BOTPAK 0x00280000
#define LIMIT_BOTPAK 0x0007ffff
#define AR_DATA32_RW 0x4092
#define AR_CODE32_ER 0x409a
#define AR_INTGATE32 0x008e

/* int.c */
#define PIC0_ICW1 0x0020
#define PIC0_OCW2 0x0020
#define PIC0_IMR 0x0021
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021
#define PIC1_ICW1 0x00a0
#define PIC1_OCW2 0x00a0
#define PIC1_IMR 0x00a1
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1

void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);

/* sheet.c */
#define MAX_SHEETS 256
struct SHEET
{
	unsigned char *buf; // picture content address
	int bxsize, bysize; // picture size
	int vx0, vy0;		// picture location
	int col_inv;		// color number
	int height;			// picture layer
	int flags;
};
struct SHTCTL
{
	unsigned char *vram;//VRAM location
	int xsize, ysize;//picture size 
	int top;//top most height
	struct SHEET *sheets[MAX_SHEETS];
	struct SHEET sheets0[MAX_SHEETS];
};
struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize);
struct SHEET *sheet_alloc(struct SHTCTL *ctl);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv);
void sheet_updown(struct SHTCTL *ctl, struct SHEET *sht, int height);
void sheet_refresh(struct SHTCTL *ctl);
void sheet_slide(struct SHTCTL *ctl, struct SHEET *sht, int vx0, int vy0);
void sheet_free(struct SHTCTL *ctl, struct SHEET *sht);
void debug_sheet(struct SHTCTL *ctl, int va);