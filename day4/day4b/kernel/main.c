#include "x86.h"

#include "header.h"

extern void init_palette(void);
extern void set_palette(int start, int end, unsigned char *rgb);

#if 0
void set_palette1(int start, int end, unsigned char *rgb)
{
    int i, eflags;
    eflags = io_load_eflags(); /* record interrupt enable flags */
    io_cli();                  /* disable interrupt */
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++)
    {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    // io_store_eflags(eflags); /* restore interrupt */
    return;
}

void init_palette1(void)
{
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00, /*  0: black */
        0xff, 0x00, 0x00, /*  1: sharp green */
        0x00, 0xff, 0x00, /*  2: sharp blue */
        0xff, 0xff, 0x00, /*  3:Ÿé¢©F */
        0x00, 0x00, 0xff, /*  4:Ÿé¢Â */
        0xff, 0x00, 0xff, /*  5:Ÿé¢ */
        0x00, 0xff, 0xff, /*  6:Ÿé¢F */
        0xff, 0xff, 0xff, /*  7: */
        0xc6, 0xc6, 0xc6, /*  8:Ÿé¢DF */
        0x84, 0x00, 0x00, /*  9:Ã¢Ô */
        0x00, 0x84, 0x00, /* 10:Ã¢Î */
        0x84, 0x84, 0x00, /* 11:Ã¢©F */
        0x00, 0x00, 0x84, /* 12:Ã¢Â */
        0x84, 0x00, 0x84, /* 13:Ã¢ */
        0x00, 0x84, 0x84, /* 14:Ã¢F */
        0x84, 0x84, 0x84  /* 15: dark green */
    };
    set_palette(0, 15, table_rgb);
    return;

    /* static char œßÍAf[^Éµ©gŠÈ¢¯ÇDBœß */
}
#endif

void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;
    eflags = read_eflags(); //替代作者的io_load_eflags()
    io_cli();
    outb(0x03c8, start); //替代作者的io_out8()
    for (i = start; i <= end; i++)
    {
        outb(0x03c9, rgb[0] / 4);
        outb(0x03c9, rgb[1] / 4);
        outb(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    write_eflags(eflags); //替代作者的io_store_eflags(eflags)
    return;
}

void init_palette(void)
{
    // 16种color，每个color三个字节。
    static unsigned char table_rgb[16 * 3] =
        {
            0x00, 0x00, 0x00, /*0:black*/
            0xff, 0x00, 0x00, /*1:light red*/
            0x00, 0xff, 0x00, /*2:light green*/
            0xff, 0xff, 0x00, /*3:light yellow*/

            0x00, 0x00, 0xff, /*4:light blue*/
            0xff, 0x00, 0xff, /*5:light purper*/
            0x00, 0xff, 0xff, /*6:light blue*/
            0xff, 0xff, 0xff, /*7:white*/

            0xc6, 0xc6, 0xc6, /*8:light gray*/
            0x84, 0x00, 0x00, /*9:dark red*/
            0x00, 0x84, 0x00, /*10:dark green*/
            0x84, 0x84, 0x00, /*11:dark yellow*/

            0x00, 0x00, 0x84, /*12:dark 青*/
            0x84, 0x00, 0x84, /*13:dark purper*/
            0x00, 0x84, 0x84, /*14:light blue*/
            0x84, 0x84, 0x84, /*15:dark gray*/
        };
    set_palette(0, 15, table_rgb);
    return;
}

void bootmain(void){
	//注意这里的函数名字为bootmain，因为在entry.S中设定的入口名字也是bootmain，两者要保持一致
	int i;
	char *p;
	init_palette();
	for(i=0xa0000; i<=0xaffff; i++){
		//write_mem8(i, i&0x0f);
		p=(char *)i;
		*p=i&0x0f;
	}

	for(;;){
		io_halt();
	}
}