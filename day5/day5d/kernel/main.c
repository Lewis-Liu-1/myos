#include "x86.h"
#include "header.h"
#include "fontascii.h"

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

void init_screen(char *vram, int x, int y)
{
	boxfill8(vram, x, COL8_008484,  0,     0,      x -  1, y - 29);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void putfont8_asc(char* vram, int xsize, int x, int y, char c, char *s){
	//其实这就是一个简易版本的printf
	extern char hankaku[2048];
	for(; *s!=0x00; s++){
		putfont8(vram, xsize, x, y, c, hankaku+*s*16);
		x+=8;
	}
	return;
}

struct BOOTINFO {
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

void bootmain(void)
{
    //注意这里的函数名字为bootmain，因为在entry.S中设定的入口名字也是bootmain，两者要保持一致
	struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
	static char font_A[16] = {
		0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24,
		0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x00, 0x00
	};

	init_palette();
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	//putfont8(binfo->vram, binfo->scrnx, 10, 10, COL8_FFFFFF, font_A);

	putfont8_asc(binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "ChrisZZ");
	putfont8_asc(binfo->vram, binfo->scrnx, 31, 31, COL8_000000, "Haribote OS");
	putfont8_asc(binfo->vram, binfo->scrnx, 32, 32, COL8_FFFFFF, "Haribote OS");

    for (;;)
    {
        io_halt();
    }
}