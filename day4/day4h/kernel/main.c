#include "x86.h"

#include "header.h"

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

extern void init_palette(void);
extern void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

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

void boxfill8(char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    int x, y;
    for (y = y0; y <= y1; y++)
    {
        for (x = x0; x <= x1; x++)
            vram[y * xsize + x] = c;
    }
    return;
}

void bootmain(void)
{
    //注意这里的函数名字为bootmain，因为在entry.S中设定的入口名字也是bootmain，两者要保持一致
	char *vram;
	int xsize, ysize;

	init_palette();
	vram = (char *) 0xa0000;
	xsize = 320;
	ysize = 200;

	boxfill8(vram, xsize, COL8_008484,  0,         0,          xsize -  1, ysize - 29);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 28, xsize -  1, ysize - 28);
	boxfill8(vram, xsize, COL8_FFFFFF,  0,         ysize - 27, xsize -  1, ysize - 27);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 26, xsize -  1, ysize -  1);

	boxfill8(vram, xsize, COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
	boxfill8(vram, xsize, COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
	boxfill8(vram, xsize, COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
	boxfill8(vram, xsize, COL8_000000, 60,         ysize - 24, 60,         ysize -  3);

	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);

    for (;;)
    {
        io_halt();
    }
}