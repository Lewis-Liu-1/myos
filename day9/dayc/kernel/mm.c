
#include "bootpack.h"
#include "mm.h"

#define EFLAGS_AC_BIT 0x00040000
#define CR0_CACHE_DISABLE 0x60000000

unsigned int memtest(unsigned int start, unsigned int end)
{
    char flg486 = 0;
    unsigned int eflg, cr0, i;

    /* 386 or 486 */
    eflg = io_load_eflags();
    eflg |= EFLAGS_AC_BIT; /* AC-bit = 1 */
    io_store_eflags(eflg);
    eflg = io_load_eflags();
    if ((eflg & EFLAGS_AC_BIT) != 0)
    { /* if it's 386, even set AC=1, it will return to 0 */
        flg486 = 1;
    }
    eflg &= ~EFLAGS_AC_BIT; /* AC-bit = 0 */
    io_store_eflags(eflg); 

    if (flg486 != 0)
    {
        cr0 = load_cr0();
        cr0 |= CR0_CACHE_DISABLE; /* stop cache */
        store_cr0(cr0);
    }

    i = memtest_sub(start, end);

    if (flg486 != 0)
    {
        cr0 = load_cr0();
        cr0 &= ~CR0_CACHE_DISABLE; /* allow cache */
        store_cr0(cr0);
    }

    return i;
}


void memman_init(struct MEMMAN *man)
{
	man->frees = 0;			/* usable info */
	man->maxfrees = 0;		/* maxm */
	man->lostsize = 0;		/* free failed mem total */
	man->losts = 0;			/* free failed times */
	return;
}

unsigned int memman_total(struct MEMMAN *man)
/* free total */
{
	unsigned int i, t = 0;
	for (i = 0; i < man->frees; i++) {
		t += man->free[i].size;
	}
	return t;
}

unsigned int memman_alloc(struct MEMMAN *man, unsigned int size)
/* allocate */
{
	unsigned int i, a;
	for (i = 0; i < man->frees; i++) {
		if (man->free[i].size >= size) {
			/* found chunk memory */
			a = man->free[i].addr;
			man->free[i].addr += size;
			man->free[i].size -= size;
			if (man->free[i].size == 0) {
				/* free[i] will be removed */
				man->frees--;
				for (; i < man->frees; i++) {
					man->free[i] = man->free[i + 1]; /*  */
				}
			}
			return a;
		}
	}
	return 0; /* no more memory */
}

int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size)
/* free */
{
	int i, j;
	/* free[] according addr setup */
	/*  */
	for (i = 0; i < man->frees; i++) {
		if (man->free[i].addr > addr) {
			break;
		}
	}
	/* free[i - 1].addr < addr < free[i].addr */
	if (i > 0) {
		/* usable memory before */
		if (man->free[i - 1].addr + man->free[i - 1].size == addr) {
			/* �O�̂����̈�ɂ܂Ƃ߂��� */
			man->free[i - 1].size += size;
			if (i < man->frees) {
				/* usable behind */
				if (addr + size == man->free[i].addr) {
					/* �Ȃ�ƌ��Ƃ��܂Ƃ߂��� */
					man->free[i - 1].size += man->free[i].size;
					/* man->free[i] remove */
					/* free[i] become 0 combined to front */
					man->frees--;
					for (; i < man->frees; i++) {
						man->free[i] = man->free[i + 1]; /* �\���̂̑�� */
					}
				}
			}
			return 0; /* success */
		}
	}
	/* not able combine with front */
	if (i < man->frees) {
		/* behind has */
		if (addr + size == man->free[i].addr) {
			/* combined with behind */
			man->free[i].addr = addr;
			man->free[i].size += size;
			return 0; /* success */
		}
	}
	/* not able to combine with front and behind */
	if (man->frees < MEMMAN_FREES) {
		/* free[i] move behind, free space before */
		for (j = man->frees; j > i; j--) {
			man->free[j] = man->free[j - 1];
		}
		man->frees++;
		if (man->maxfrees < man->frees) {
			man->maxfrees = man->frees; /* update max */
		}
		man->free[i].addr = addr;
		man->free[i].size = size;
		return 0; /* success */
	}
	/* can't move behind */
	man->losts++;
	man->lostsize += size;
	return -1; /* failed */
}
