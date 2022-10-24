/* ^C}ÖW */

#include "bootpack.h"
#include "x86.h"

#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040

struct TIMERCTL timerctl;

void init_pit(void)
{
	outb(PIT_CTRL, 0x34);
	outb(PIT_CNT0, 0x9c);
	outb(PIT_CNT0, 0x2e);
	timerctl.count = 0;
	return;
}

void inthandler20(int *esp)
{
	outb(PIC0_OCW2, 0x60);	/* IRQ-00ót®¹ðPICÉÊm */
	timerctl.count++;
	return;
}