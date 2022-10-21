#pragma once

#define MEMMAN_FREES		4090	/* about 32KB */
#define MEMMAN_ADDR			0x003c0000

struct FREEINFO {	/* usable mem info */
	unsigned int addr, size;
};

struct MEMMAN {		/* memory management */
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};


unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
