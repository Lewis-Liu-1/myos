
kernel.out:     file format elf32-i386


Disassembly of section .text:

0000c400 <start>:

  


  # Set up the important data segment registers (DS, ES, SS).
  xorw    %ax,%ax             # Segment number zero
    c400:	31 c0                	xor    %eax,%eax
  movw    %ax,%ds             # -> Data Segment
    c402:	8e d8                	mov    %eax,%ds
  movw    %ax,%es             # -> Extra Segment
    c404:	8e c0                	mov    %eax,%es
  movw    %ax,%ss             # -> Stack Segment
    c406:	8e d0                	mov    %eax,%ss
  
  movb $0x13,%al  # ;vga 320x200x8 位,color mode 
    c408:	b0 13                	mov    $0x13,%al
  movb $0x00,%ah
    c40a:	b4 00                	mov    $0x0,%ah
  int $0x10
    c40c:	cd 10                	int    $0x10
  
#save color mode in ram 0x0ff0
 movb $8,(VMODE)
    c40e:	c6 06 f2             	movb   $0xf2,(%esi)
    c411:	0f 08                	invd   
 movw $320,(SCRNX)
    c413:	c7 06 f4 0f 40 01    	movl   $0x1400ff4,(%esi)
 movw $200,(SCRNY)
    c419:	c7 06 f6 0f c8 00    	movl   $0xc80ff6,(%esi)
 movl $0x000a0000,(VRAM)
    c41f:	66 c7 06 f8 0f       	movw   $0xff8,(%esi)
    c424:	00 00                	add    %al,(%eax)
    c426:	0a 00                	or     (%eax),%al

 #get keyboard led status
 movb	$0x02,%ah 
    c428:	b4 02                	mov    $0x2,%ah
 int     $0x16			#keyboard interrupts
    c42a:	cd 16                	int    $0x16
 movb   %al,(LEDS)
    c42c:	a2 f1 0f be 65       	mov    %al,0x65be0ff1
		
		
		
#diplay something
  movw $msg,%si
    c431:	c4                   	(bad)  
  call puts
    c432:	e8 5d 00 be 81       	call   81bec494 <_GLOBAL_OFFSET_TABLE_+0x81bdfc04>
  
  movw $try,%si
    c437:	c4                   	(bad)  
  call puts
    c438:	e8 57 00 fa fc       	call   fcfac494 <_GLOBAL_OFFSET_TABLE_+0xfcf9fc04>

0000c43d <seta20.1>:
  # Enable A20:
  #   For backwards compatibility with the earliest PCs, physical
  #   address line 20 is tied low, so that addresses higher than
  #   1MB wrap around to zero by default.  This code undoes this. 
seta20.1:
  inb     $0x64,%al               # Wait for not busy
    c43d:	e4 64                	in     $0x64,%al
  testb   $0x2,%al
    c43f:	a8 02                	test   $0x2,%al
  jnz     seta20.1
    c441:	75 fa                	jne    c43d <seta20.1>

  movb    $0xd1,%al               # 0xd1 -> port 0x64
    c443:	b0 d1                	mov    $0xd1,%al
  outb    %al,$0x64
    c445:	e6 64                	out    %al,$0x64

0000c447 <seta20.2>:

seta20.2:
  inb     $0x64,%al               # Wait for not busy
    c447:	e4 64                	in     $0x64,%al
  testb   $02,%al
    c449:	a8 02                	test   $0x2,%al
  jnz     seta20.2
    c44b:	75 fa                	jne    c447 <seta20.2>

  movb    $0xdf,%al               # 0xdf -> port 0x60
    c44d:	b0 df                	mov    $0xdf,%al
  outb    %al,$0x60
    c44f:	e6 60                	out    %al,$0x60

  # Switch from real to protected mode, using a bootstrap GDT       this is vip ,but i don`t know it clearly now
  # and segment translation that makes virtual addresses 
  # identical to their physical addresses, so that the 
  # effective memory map does not change during the switch.
  lgdt    gdtdesc
    c451:	0f 01 16             	lgdtl  (%esi)
    c454:	d8 c4                	fadd   %st(4),%st
  movl    %cr0, %eax
    c456:	0f 20 c0             	mov    %cr0,%eax
  orl     $CR0_PE_ON, %eax
    c459:	66 83 c8 01          	or     $0x1,%ax
  movl    %eax, %cr0
    c45d:	0f 22 c0             	mov    %eax,%cr0
  
  # Jump to next instruction, but in 32-bit code segment.
  # Switches processor into 32-bit mode.
  ljmp    $PROT_MODE_CSEG, $protcseg
    c460:	ea                   	.byte 0xea
    c461:	a5                   	movsl  %ds:(%esi),%es:(%edi)
    c462:	c4 08                	les    (%eax),%ecx
	...

0000c465 <msg>:
    c465:	0d 0a 0a 0d 6d       	or     $0x6d0d0a0a,%eax
    c46a:	79 20                	jns    c48c <try+0xb>
    c46c:	6b 65 72 6e          	imul   $0x6e,0x72(%ebp),%esp
    c470:	65 6c                	gs insb (%dx),%es:(%edi)
    c472:	20 69 73             	and    %ch,0x73(%ecx)
    c475:	20 72 75             	and    %dh,0x75(%edx)
    c478:	6e                   	outsb  %ds:(%esi),(%dx)
    c479:	69 6e 67 20 6a 6f 73 	imul   $0x736f6a20,0x67(%esi),%ebp
	...

0000c481 <try>:
    c481:	0d 0a 0a 0d 74       	or     $0x740d0a0a,%eax
    c486:	72 79                	jb     c501 <set_palette+0x23>
    c488:	20 69 74             	and    %ch,0x74(%ecx)
    c48b:	20 61 67             	and    %ah,0x67(%ecx)
    c48e:	61                   	popa   
    c48f:	69                   	.byte 0x69
    c490:	6e                   	outsb  %ds:(%esi),(%dx)
	...

0000c492 <puts>:
 try:
  .asciz "\r\n\n\rtry it again"

puts:

	movb (%si),%al
    c492:	8a 04 83             	mov    (%ebx,%eax,4),%al
	add $1,%si
    c495:	c6 01 3c             	movb   $0x3c,(%ecx)
	cmp $0,%al
    c498:	00 74 09 b4          	add    %dh,-0x4c(%ecx,%ecx,1)
	je over
	movb $0x0e,%ah
    c49c:	0e                   	push   %cs
	movw $15,%bx
    c49d:	bb 0f 00 cd 10       	mov    $0x10cd000f,%ebx
	int $0x10
	jmp puts
    c4a2:	eb ee                	jmp    c492 <puts>

0000c4a4 <over>:
over:
	ret	
    c4a4:	c3                   	ret    

0000c4a5 <protcseg>:
	
	
  .code32                     # Assemble for 32-bit mode
protcseg:
  # Set up the protected-mode data segment registers
  movw    $PROT_MODE_DSEG, %ax    # Our data segment selector
    c4a5:	66 b8 10 00          	mov    $0x10,%ax
  movw    %ax, %ds                # -> DS: Data Segment
    c4a9:	8e d8                	mov    %eax,%ds
  movw    %ax, %es                # -> ES: Extra Segment
    c4ab:	8e c0                	mov    %eax,%es
  movw    %ax, %fs                # -> FS
    c4ad:	8e e0                	mov    %eax,%fs
  movw    %ax, %gs                # -> GS
    c4af:	8e e8                	mov    %eax,%gs
  movw    %ax, %ss                # -> SS: Stack Segment
    c4b1:	8e d0                	mov    %eax,%ss
  
  # Set up the stack pointer and call into C.
  movl    $start, %esp
    c4b3:	bc 00 c4 00 00       	mov    $0xc400,%esp
  call bootmain
    c4b8:	e8 c1 00 00 00       	call   c57e <bootmain>

0000c4bd <spin>:

  # If bootmain returns (it shouldn't), loop.
spin:
  jmp spin
    c4bd:	eb fe                	jmp    c4bd <spin>
    c4bf:	90                   	nop

0000c4c0 <gdt>:
	...
    c4c8:	ff                   	(bad)  
    c4c9:	ff 00                	incl   (%eax)
    c4cb:	00 00                	add    %al,(%eax)
    c4cd:	9a cf 00 ff ff 00 00 	lcall  $0x0,$0xffff00cf
    c4d4:	00                   	.byte 0x0
    c4d5:	92                   	xchg   %eax,%edx
    c4d6:	cf                   	iret   
	...

0000c4d8 <gdtdesc>:
    c4d8:	17                   	pop    %ss
    c4d9:	00 c0                	add    %al,%al
    c4db:	c4 00                	les    (%eax),%eax
	...

0000c4de <set_palette>:
extern void init_palette(void);
extern void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void set_palette(int start, int end, unsigned char *rgb)
{
    c4de:	f3 0f 1e fb          	endbr32 
    c4e2:	55                   	push   %ebp
    c4e3:	89 e5                	mov    %esp,%ebp
    c4e5:	56                   	push   %esi
    c4e6:	8b 4d 10             	mov    0x10(%ebp),%ecx
    c4e9:	53                   	push   %ebx
    c4ea:	8b 5d 08             	mov    0x8(%ebp),%ebx
// read eflags and write_eflags
static __inline uint32_t
read_eflags(void)
{
    uint32_t eflags;
    __asm __volatile("pushfl; popl %0"
    c4ed:	9c                   	pushf  
    c4ee:	5e                   	pop    %esi
    int i, eflags;
    eflags = read_eflags(); //替代作者的io_load_eflags()
    io_cli();
    c4ef:	fa                   	cli    
    __asm __volatile("outb %0,%w1"
    c4f0:	ba c8 03 00 00       	mov    $0x3c8,%edx
    c4f5:	89 d8                	mov    %ebx,%eax
    c4f7:	ee                   	out    %al,(%dx)
    c4f8:	ba c9 03 00 00       	mov    $0x3c9,%edx
    outb(0x03c8, start); //替代作者的io_out8()
    for (i = start; i <= end; i++)
    c4fd:	3b 5d 0c             	cmp    0xc(%ebp),%ebx
    c500:	7f 1a                	jg     c51c <set_palette+0x3e>
    {
        outb(0x03c9, rgb[0] / 4);
    c502:	8a 01                	mov    (%ecx),%al
    c504:	c0 e8 02             	shr    $0x2,%al
    c507:	ee                   	out    %al,(%dx)
        outb(0x03c9, rgb[1] / 4);
    c508:	8a 41 01             	mov    0x1(%ecx),%al
    c50b:	c0 e8 02             	shr    $0x2,%al
    c50e:	ee                   	out    %al,(%dx)
        outb(0x03c9, rgb[2] / 4);
    c50f:	8a 41 02             	mov    0x2(%ecx),%al
    c512:	c0 e8 02             	shr    $0x2,%al
    c515:	ee                   	out    %al,(%dx)
        rgb += 3;
    c516:	83 c1 03             	add    $0x3,%ecx
    for (i = start; i <= end; i++)
    c519:	43                   	inc    %ebx
    c51a:	eb e1                	jmp    c4fd <set_palette+0x1f>
}

static __inline void
write_eflags(uint32_t eflags)
{
    __asm __volatile("pushl %0; popfl"
    c51c:	56                   	push   %esi
    c51d:	9d                   	popf   
    }
    write_eflags(eflags); //替代作者的io_store_eflags(eflags)
    return;
}
    c51e:	5b                   	pop    %ebx
    c51f:	5e                   	pop    %esi
    c520:	5d                   	pop    %ebp
    c521:	c3                   	ret    

0000c522 <init_palette>:

void init_palette(void)
{
    c522:	f3 0f 1e fb          	endbr32 
    c526:	e8 63 02 00 00       	call   c78e <__x86.get_pc_thunk.ax>
    c52b:	05 65 03 00 00       	add    $0x365,%eax
    c530:	55                   	push   %ebp
    c531:	89 e5                	mov    %esp,%ebp
    c533:	83 ec 0c             	sub    $0xc,%esp
            0x00, 0x00, 0x84, /*12:dark 青*/
            0x84, 0x00, 0x84, /*13:dark purper*/
            0x00, 0x84, 0x84, /*14:light blue*/
            0x84, 0x84, 0x84, /*15:dark gray*/
        };
    set_palette(0, 15, table_rgb);
    c536:	8d 80 d0 ff ff ff    	lea    -0x30(%eax),%eax
    c53c:	50                   	push   %eax
    c53d:	6a 0f                	push   $0xf
    c53f:	6a 00                	push   $0x0
    c541:	e8 98 ff ff ff       	call   c4de <set_palette>
    return;
    c546:	83 c4 10             	add    $0x10,%esp
}
    c549:	c9                   	leave  
    c54a:	c3                   	ret    

0000c54b <boxfill8>:

void boxfill8(char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    c54b:	f3 0f 1e fb          	endbr32 
    c54f:	55                   	push   %ebp
    c550:	89 e5                	mov    %esp,%ebp
    c552:	53                   	push   %ebx
    c553:	8b 55 18             	mov    0x18(%ebp),%edx
    c556:	8b 45 0c             	mov    0xc(%ebp),%eax
    c559:	8a 5d 10             	mov    0x10(%ebp),%bl
    c55c:	0f af c2             	imul   %edx,%eax
    c55f:	03 45 08             	add    0x8(%ebp),%eax
    int x, y;
    for (y = y0; y <= y1; y++)
    c562:	3b 55 20             	cmp    0x20(%ebp),%edx
    c565:	7f 14                	jg     c57b <boxfill8+0x30>
    {
        for (x = x0; x <= x1; x++)
    c567:	8b 4d 14             	mov    0x14(%ebp),%ecx
    c56a:	3b 4d 1c             	cmp    0x1c(%ebp),%ecx
    c56d:	7f 06                	jg     c575 <boxfill8+0x2a>
            vram[y * xsize + x] = c;
    c56f:	88 1c 08             	mov    %bl,(%eax,%ecx,1)
        for (x = x0; x <= x1; x++)
    c572:	41                   	inc    %ecx
    c573:	eb f5                	jmp    c56a <boxfill8+0x1f>
    for (y = y0; y <= y1; y++)
    c575:	42                   	inc    %edx
    c576:	03 45 0c             	add    0xc(%ebp),%eax
    c579:	eb e7                	jmp    c562 <boxfill8+0x17>
    }
    return;
}
    c57b:	5b                   	pop    %ebx
    c57c:	5d                   	pop    %ebp
    c57d:	c3                   	ret    

0000c57e <bootmain>:

void bootmain(void)
{
    c57e:	f3 0f 1e fb          	endbr32 
    c582:	55                   	push   %ebp
    c583:	89 e5                	mov    %esp,%ebp
    c585:	83 ec 08             	sub    $0x8,%esp
    //注意这里的函数名字为bootmain，因为在entry.S中设定的入口名字也是bootmain，两者要保持一致
	char *vram;
	int xsize, ysize;

	init_palette();
    c588:	e8 95 ff ff ff       	call   c522 <init_palette>
	vram = (char *) 0xa0000;
	xsize = 320;
	ysize = 200;

	boxfill8(vram, xsize, COL8_008484,  0,         0,          xsize -  1, ysize - 29);
    c58d:	52                   	push   %edx
    c58e:	68 ab 00 00 00       	push   $0xab
    c593:	68 3f 01 00 00       	push   $0x13f
    c598:	6a 00                	push   $0x0
    c59a:	6a 00                	push   $0x0
    c59c:	6a 0e                	push   $0xe
    c59e:	68 40 01 00 00       	push   $0x140
    c5a3:	68 00 00 0a 00       	push   $0xa0000
    c5a8:	e8 9e ff ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 28, xsize -  1, ysize - 28);
    c5ad:	83 c4 1c             	add    $0x1c,%esp
    c5b0:	68 ac 00 00 00       	push   $0xac
    c5b5:	68 3f 01 00 00       	push   $0x13f
    c5ba:	68 ac 00 00 00       	push   $0xac
    c5bf:	6a 00                	push   $0x0
    c5c1:	6a 08                	push   $0x8
    c5c3:	68 40 01 00 00       	push   $0x140
    c5c8:	68 00 00 0a 00       	push   $0xa0000
    c5cd:	e8 79 ff ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_FFFFFF,  0,         ysize - 27, xsize -  1, ysize - 27);
    c5d2:	83 c4 1c             	add    $0x1c,%esp
    c5d5:	68 ad 00 00 00       	push   $0xad
    c5da:	68 3f 01 00 00       	push   $0x13f
    c5df:	68 ad 00 00 00       	push   $0xad
    c5e4:	6a 00                	push   $0x0
    c5e6:	6a 07                	push   $0x7
    c5e8:	68 40 01 00 00       	push   $0x140
    c5ed:	68 00 00 0a 00       	push   $0xa0000
    c5f2:	e8 54 ff ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 26, xsize -  1, ysize -  1);
    c5f7:	83 c4 1c             	add    $0x1c,%esp
    c5fa:	68 c7 00 00 00       	push   $0xc7
    c5ff:	68 3f 01 00 00       	push   $0x13f
    c604:	68 ae 00 00 00       	push   $0xae
    c609:	6a 00                	push   $0x0
    c60b:	6a 08                	push   $0x8
    c60d:	68 40 01 00 00       	push   $0x140
    c612:	68 00 00 0a 00       	push   $0xa0000
    c617:	e8 2f ff ff ff       	call   c54b <boxfill8>

	boxfill8(vram, xsize, COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
    c61c:	83 c4 1c             	add    $0x1c,%esp
    c61f:	68 b0 00 00 00       	push   $0xb0
    c624:	6a 3b                	push   $0x3b
    c626:	68 b0 00 00 00       	push   $0xb0
    c62b:	6a 03                	push   $0x3
    c62d:	6a 07                	push   $0x7
    c62f:	68 40 01 00 00       	push   $0x140
    c634:	68 00 00 0a 00       	push   $0xa0000
    c639:	e8 0d ff ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
    c63e:	83 c4 1c             	add    $0x1c,%esp
    c641:	68 c4 00 00 00       	push   $0xc4
    c646:	6a 02                	push   $0x2
    c648:	68 b0 00 00 00       	push   $0xb0
    c64d:	6a 02                	push   $0x2
    c64f:	6a 07                	push   $0x7
    c651:	68 40 01 00 00       	push   $0x140
    c656:	68 00 00 0a 00       	push   $0xa0000
    c65b:	e8 eb fe ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
    c660:	83 c4 1c             	add    $0x1c,%esp
    c663:	68 c4 00 00 00       	push   $0xc4
    c668:	6a 3b                	push   $0x3b
    c66a:	68 c4 00 00 00       	push   $0xc4
    c66f:	6a 03                	push   $0x3
    c671:	6a 0f                	push   $0xf
    c673:	68 40 01 00 00       	push   $0x140
    c678:	68 00 00 0a 00       	push   $0xa0000
    c67d:	e8 c9 fe ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
    c682:	83 c4 1c             	add    $0x1c,%esp
    c685:	68 c3 00 00 00       	push   $0xc3
    c68a:	6a 3b                	push   $0x3b
    c68c:	68 b1 00 00 00       	push   $0xb1
    c691:	6a 3b                	push   $0x3b
    c693:	6a 0f                	push   $0xf
    c695:	68 40 01 00 00       	push   $0x140
    c69a:	68 00 00 0a 00       	push   $0xa0000
    c69f:	e8 a7 fe ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
    c6a4:	83 c4 1c             	add    $0x1c,%esp
    c6a7:	68 c5 00 00 00       	push   $0xc5
    c6ac:	6a 3b                	push   $0x3b
    c6ae:	68 c5 00 00 00       	push   $0xc5
    c6b3:	6a 02                	push   $0x2
    c6b5:	6a 00                	push   $0x0
    c6b7:	68 40 01 00 00       	push   $0x140
    c6bc:	68 00 00 0a 00       	push   $0xa0000
    c6c1:	e8 85 fe ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_000000, 60,         ysize - 24, 60,         ysize -  3);
    c6c6:	83 c4 1c             	add    $0x1c,%esp
    c6c9:	68 c5 00 00 00       	push   $0xc5
    c6ce:	6a 3c                	push   $0x3c
    c6d0:	68 b0 00 00 00       	push   $0xb0
    c6d5:	6a 3c                	push   $0x3c
    c6d7:	6a 00                	push   $0x0
    c6d9:	68 40 01 00 00       	push   $0x140
    c6de:	68 00 00 0a 00       	push   $0xa0000
    c6e3:	e8 63 fe ff ff       	call   c54b <boxfill8>

	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
    c6e8:	83 c4 1c             	add    $0x1c,%esp
    c6eb:	68 b0 00 00 00       	push   $0xb0
    c6f0:	68 3c 01 00 00       	push   $0x13c
    c6f5:	68 b0 00 00 00       	push   $0xb0
    c6fa:	68 11 01 00 00       	push   $0x111
    c6ff:	6a 0f                	push   $0xf
    c701:	68 40 01 00 00       	push   $0x140
    c706:	68 00 00 0a 00       	push   $0xa0000
    c70b:	e8 3b fe ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
    c710:	83 c4 1c             	add    $0x1c,%esp
    c713:	68 c4 00 00 00       	push   $0xc4
    c718:	68 11 01 00 00       	push   $0x111
    c71d:	68 b1 00 00 00       	push   $0xb1
    c722:	68 11 01 00 00       	push   $0x111
    c727:	6a 0f                	push   $0xf
    c729:	68 40 01 00 00       	push   $0x140
    c72e:	68 00 00 0a 00       	push   $0xa0000
    c733:	e8 13 fe ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
    c738:	83 c4 1c             	add    $0x1c,%esp
    c73b:	68 c5 00 00 00       	push   $0xc5
    c740:	68 3c 01 00 00       	push   $0x13c
    c745:	68 c5 00 00 00       	push   $0xc5
    c74a:	68 11 01 00 00       	push   $0x111
    c74f:	6a 07                	push   $0x7
    c751:	68 40 01 00 00       	push   $0x140
    c756:	68 00 00 0a 00       	push   $0xa0000
    c75b:	e8 eb fd ff ff       	call   c54b <boxfill8>
	boxfill8(vram, xsize, COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);
    c760:	83 c4 1c             	add    $0x1c,%esp
    c763:	68 c5 00 00 00       	push   $0xc5
    c768:	68 3d 01 00 00       	push   $0x13d
    c76d:	68 b0 00 00 00       	push   $0xb0
    c772:	68 3d 01 00 00       	push   $0x13d
    c777:	6a 07                	push   $0x7
    c779:	68 40 01 00 00       	push   $0x140
    c77e:	68 00 00 0a 00       	push   $0xa0000
    c783:	e8 c3 fd ff ff       	call   c54b <boxfill8>
    c788:	83 c4 20             	add    $0x20,%esp

    for (;;)
    {
        io_halt();
    c78b:	f4                   	hlt    
    c78c:	eb fd                	jmp    c78b <bootmain+0x20d>

Disassembly of section .text.__x86.get_pc_thunk.ax:

0000c78e <__x86.get_pc_thunk.ax>:
    c78e:	8b 04 24             	mov    (%esp),%eax
    c791:	c3                   	ret    
