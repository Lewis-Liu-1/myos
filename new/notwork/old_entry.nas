CYLS	EQU		0x0ff0			; u[gZN^ªÝè·é
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; FÉÖ·éîñBœrbgJ[©H
SCRNX	EQU		0x0ff4			; ðxÌX
SCRNY	EQU		0x0ff6			; ðxÌY
VRAM	EQU		0x0ff8			; OtBbNobt@ÌJnÔn

PROT_MODE_CSEG equ 0x8         ; kernel code segment selector
PROT_MODE_DSEG equ 0x10        ; kernel data segment selector
CR0_PE_ON equ      0x1         ;# protected mode enable flag

;ORG		0xc400			; ±ÌvOªÇ±ÉÇÝÜêéÌ©
global start;
start:
    MOV		BYTE [VMODE],8	; æÊ[hð·éiCŸêªQÆ·éj
    MOV		WORD [SCRNX],320
    MOV		WORD [SCRNY],200
    MOV		DWORD [VRAM],0x000a0000

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    MOV		AL,0x13			; VGAOtBbNXA320x200x8bitJ[
    MOV		AH,0x00
    INT		0x10
    
    mov ah, 2
    int 0x16
    mov [LEDS], al

    mov si, msg
    call puts

    mov si, try
    call puts

    jmp $
    cli
    cld
    
setad20.1:
    in al, 0x64
    and al, 2
    jnz setad20.1

    mov al, 0xd1
    out 0x64, al

setad20.2:
    in  al, 0x64
    and al, 2
    jnz  setad20.2

    mov al, 0xdf
    out 0x60, al

    lgdt    [gdtdesc]
    mov eax, CR0
    and eax, 0x7fffffff
    or eax, 0x00000001
    mov cr0, eax

    jmp  pipelineflush

pipelineflush:
    mov ax, 2*8
    MOV DS, AX
    MOV ES, AX
    MOV FS, AX
    MOV GS, AX
    MOV SS, AX

    MOV ESP, start

    [extern bootmain]
    call bootmain

    ;mov si, try
    ;call puts
    ;jmp $


fin:
HLT
JMP		fin

puts:
    mov al, [si]
    add si, 1
    cmp al, 0
    je over
    mov ah, 0xe
    mov bx, 15
    int 0x10
    jmp puts
over:
    ret

msg:
    db "kernel is running", 0xd,0xa, 0
try:
    db "try it again", 0xd,0xa, 0    

    ALIGNB	16

GDT0:
    RESB	8				; kZN^
    DW		0xffff,0x0000,0x9200,0x00cf	; ÇÝ«Â\ZOg32bit
    DW		0xffff,0x0000,0x9a28,0x0047	; ÀsÂ\ZOg32bitibootpackpj

    DW		0
  
gdtdesc:
    DW		8*3-1
    DD		GDT0

    ALIGNB	16
  