/* �}�E�X��E�B���h�E�̏d�ˍ��킹���� */

#include "bootpack.h"

#define SHEET_USE 1

struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize)
{
    struct SHTCTL *ctl;
    int i;
    ctl = (struct SHTCTL *)memman_alloc_4k(memman, sizeof(struct SHTCTL));
    if (ctl == 0)
    {
        goto err;
    }
    ctl->vram = vram;
    ctl->xsize = xsize;
    ctl->ysize = ysize;
    ctl->top = -1; /* no sheet yet */
    for (i = 0; i < MAX_SHEETS; i++)
    {
        ctl->sheets0[i].flags = 0; /* not been used */
    }
err:
    return ctl;
}

struct SHEET *sheet_alloc(struct SHTCTL *ctl)
{
    struct SHEET *sht;
    int i;
    for (i = 0; i < MAX_SHEETS; i++)
    {
        if (ctl->sheets0[i].flags == 0)
        {
            sht = &ctl->sheets0[i];
            sht->flags = SHEET_USE; /* being used now */
            sht->height = -1;       /* hide */
            return sht;
        }
    }
    return 0; /* all sheets being used */
}

void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv)
{
    sht->buf = buf;
    sht->bxsize = xsize;
    sht->bysize = ysize;
    sht->col_inv = col_inv;
    return;
}

void sheet_updown(struct SHTCTL *ctl, struct SHEET *sht, int height)
{
    int h, old = sht->height; /* store origin height info */

    /* if top too high, adjust */
    if (height > ctl->top + 1)
    {
        height = ctl->top + 1;
    }
    if (height < -1)
    {
        height = -1;
    }
    sht->height = height; /* set height */

    /* sort sheets[] */
    if (old > height)
    { /* lower than previous */
        if (height >= 0)
        {
            /* move up middle one */
            for (h = old; h > height; h--)
            {
                ctl->sheets[h] = ctl->sheets[h - 1];
                ctl->sheets[h]->height = h;
            }
            ctl->sheets[height] = sht;
        }
        else
        { /* hide */
            if (ctl->top > old)
            {
                /* reduce upper layers */
                for (h = old; h < ctl->top; h++)
                {
                    ctl->sheets[h] = ctl->sheets[h + 1];
                    ctl->sheets[h]->height = h;
                }
            }
            ctl->top--; /* top most reduce */
        }
        sheet_refresh(ctl); /* according new layer realocate pictures */
    }
    else if (old < height)
    { /* higher than previous */
        if (old >= 0)
        {
            /* pull down middles */
            for (h = old; h < height; h++)
            {
                ctl->sheets[h] = ctl->sheets[h + 1];
                ctl->sheets[h]->height = h;
            }
            ctl->sheets[height] = sht;
        }
        else
        { /* hide become visible */
            /* tops move up */
            for (h = ctl->top; h >= height; h--)
            {
                ctl->sheets[h + 1] = ctl->sheets[h];
                ctl->sheets[h + 1]->height = h + 1;
            }
            ctl->sheets[height] = sht;
            ctl->top++; /* top most */
        }
        sheet_refresh(ctl); /* redraw */
    }
    return;
}

void debug_sheet(struct SHTCTL *ctl, int va)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    int i, j;
    int x, y;
    for (x = 0; x < ctl->xsize; x++)
    {
        for (y = 0; y <= ctl->ysize; y++)
            binfo->vram[y * ctl->xsize + x] = va;
    }
}

void sheet_refresh(struct SHTCTL *ctl)
{
    int h, bx, by, vx, vy;
    unsigned char *buf, c, *vram = ctl->vram;
    struct SHEET *sht;
    for (h = 0; h <= ctl->top; h++)
    {
        sht = ctl->sheets[h];
        buf = sht->buf;
        for (by = 0; by < sht->bysize; by++)
        {
            vy = sht->vy0 + by;
            for (bx = 0; bx < sht->bxsize; bx++)
            {
                vx = sht->vx0 + bx;
                c = buf[by * sht->bxsize + bx];
                if (c != sht->col_inv)
                {
                    vram[vy * ctl->xsize + vx] = c;
                }
            }
        }
    }
    return;
}

void sheet_slide(struct SHTCTL *ctl, struct SHEET *sht, int vx0, int vy0)
{
    sht->vx0 = vx0;
    sht->vy0 = vy0;
    if (sht->height >= 0)
    {                       /* if displaying now */
        sheet_refresh(ctl); /* redraw */
    }
    return;
}

void sheet_free(struct SHTCTL *ctl, struct SHEET *sht)
{
    if (sht->height >= 0)
    {
        sheet_updown(ctl, sht, -1); /* if it's displaying, hide it */
    }
    sht->flags = 0; /* not used flag */
    return;
}
