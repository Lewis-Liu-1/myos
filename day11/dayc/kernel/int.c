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

void inthandler21(int *esp)
{
	unsigned char data;
	outb(PIC0_OCW2, 0x61);	/* IRQ-01ót®¹ðPICÉÊm */
	data = inb(PORT_KEYDAT);
	fifo8_put(&keyfifo, data);
	return;
}

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


