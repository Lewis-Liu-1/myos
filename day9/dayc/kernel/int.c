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

struct FIFO8 keyfifo;

void asm_inthandler21(void);

void keyboard_handler(int *esp);

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

#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47

void wait_KBC_sendready(void)
{
	/* �L�[�{�[�h�R���g���[�����f�[�^���M�\�ɂȂ�̂�҂� */
	for (;;) {
		if ((inb(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}

void init_keyboard(void)
{
	/* �L�[�{�[�h�R���g���[���̏����� */
	wait_KBC_sendready();
	outb(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	outb(PORT_KEYDAT, KBC_MODE);
	return;
}

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4
struct FIFO8 mousefifo;

void enable_mouse(void)
{
	/* active mouse */
	wait_KBC_sendready();
	outb(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	outb(PORT_KEYDAT, MOUSECMD_ENABLE);
	return; /* return ACK 0xfa */
}

//mouse interrupt handler
void inthandler2c(int *esp)
/* PS/2 interrupt? */
{
	unsigned char data;
	outb(PIC1_OCW2, 0x64);	/* inform PIC1 irq 12 processed */
	outb(PIC0_OCW2, 0x62);	/* inform PIC0 irq 2 processed */
	data = inb(PORT_KEYDAT);
	fifo8_put(&mousefifo, data);
}

void inthandler27(int *esp)
/* PIC0����̕s���S���荞�ݑ΍� */
/* Athlon64X2�@�Ȃǂł̓`�b�v�Z�b�g�̓s���ɂ��PIC�̏��������ɂ��̊��荞�݂�1�x���������� */
/* ���̊��荞�ݏ����֐��́A���̊��荞�݂ɑ΂��ĉ������Ȃ��ł��߂��� */
/* �Ȃ��������Ȃ��Ă����́H
	��  ���̊��荞�݂�PIC���������̓d�C�I�ȃm�C�Y�ɂ���Ĕ����������̂Ȃ̂ŁA
		�܂��߂ɉ����������Ă��K�v���Ȃ��B									*/
{
	outb(PIC0_OCW2, 0x67); /* IRQ-07��t������PIC�ɒʒm */
	return;
}


int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
	if (mdec->phase == 0) {
		/* �}�E�X��0xfa��҂��Ă���i�K */
		if (dat == 0xfa) {
			mdec->phase = 1;
		}
		return 0;
	}
	if (mdec->phase == 1) {
		/* �}�E�X��1�o�C�g�ڂ�҂��Ă���i�K */
		if ((dat & 0xc8) == 0x08) {
			/* ������1�o�C�g�ڂ����� */
			mdec->buf[0] = dat;
			mdec->phase = 2;
		}
		return 0;
	}
	if (mdec->phase == 2) {
		/* �}�E�X��2�o�C�g�ڂ�҂��Ă���i�K */
		mdec->buf[1] = dat;
		mdec->phase = 3;
		return 0;
	}
	if (mdec->phase == 3) {
		/* �}�E�X��3�o�C�g�ڂ�҂��Ă���i�K */
		mdec->buf[2] = dat;
		mdec->phase = 1;
		mdec->btn = mdec->buf[0] & 0x07;
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];
		if ((mdec->buf[0] & 0x10) != 0) {
			mdec->x |= 0xffffff00;
		}
		if ((mdec->buf[0] & 0x20) != 0) {
			mdec->y |= 0xffffff00;
		}
		mdec->y = - mdec->y; /* �}�E�X�ł�y�����̕�������ʂƔ��� */
		return 1;
	}
	return -1; /* �����ɗ��邱�Ƃ͂Ȃ��͂� */
}
