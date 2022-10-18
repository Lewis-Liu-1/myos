
#include "fontascii.h"
#include "stdio.h"

void SET_COLOR(int index, int color)
{
}
void putfont8(char *vram, int xsize, int x, int y, char color, char *font)
{
    int i;
    char *p, d /* data */;
    for (i = 0; i < 16; i++)
    {
        p = vram + (y + i) * xsize + x;
        d = font[i];
        printf("0x%02x ", d);
        if ((d & 0x80) != 0)
        {
            SET_COLOR(0, color);
        }
        if ((d & 0x40) != 0)
        {
            SET_COLOR(1, color);
        }
        if ((d & 0x20) != 0)
        {
            SET_COLOR(2, color);
        }
        if ((d & 0x10) != 0)
        {
            SET_COLOR(3, color);
        }
        if ((d & 0x08) != 0)
        {
            SET_COLOR(4, color);
        }
        if ((d & 0x04) != 0)
        {
            SET_COLOR(5, color);
        }
        if ((d & 0x02) != 0)
        {
            SET_COLOR(6, color);
        }
        if ((d & 0x01) != 0)
        {
            SET_COLOR(7, color);
        }
    }
    printf("\r\n");
    return;
}

void putfont8_asc(char *vram, int xsize, int x, int y, char color, char *s)
{
    //其实这就是一个简易版本的printf
    extern char hankaku[2048];
    for (; *s != 0x00; s++)
    {
        printf("%c: ", *s);
        putfont8(vram, xsize, x, y, color, hankaku + *s * 16);
        x += 8;
    }
    return;
}

struct BOOTINFO
{
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

int main(void)
{
    struct BOOTINFO ob;
    struct BOOTINFO *binfo = &ob;
    putfont8_asc(binfo->vram, binfo->scrnx, 8, 8, 0, "ChrisZZ");
    putfont8_asc(binfo->vram, binfo->scrnx, 31, 31, 1, "Haribote OS");
    putfont8_asc(binfo->vram, binfo->scrnx, 32, 32, 1, "Haribote OS");
}