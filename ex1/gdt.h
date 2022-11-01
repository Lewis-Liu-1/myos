#pragma once

struct gdt
{
    /* data */
    unsigned limit : 16;
    unsigned base_low : 16;
    unsigned base_hi : 8;
    unsigned type : 4;
    unsigned privilege : 2;
    unsigned present : 1;
    unsigned limit_hi : 4;
    unsigned attribuites : 2;
    unsigned granularity : 1;
    unsigned base_hi_hi : 8;
};
