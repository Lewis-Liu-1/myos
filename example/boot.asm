%include "pm.inc"

org 0x7C00
jmp _main_16

[SECTION .gdt]
DESC_GDT: Descriptor 0, 0, 0
DESC_VIDEO: Descriptor 0xB8000, 0xFFFF, DA_DRW
DESC_DATA: Descriptor 0, STRING_LEN - 1, DA_DR
DESC_CODE: Descriptor 0, CODE32_LEN - 1, DA_C + DA_32

GdtLen equ $ - DESC_GDT
GdtPtr dw GdtLen
       dd 0

DataSelector equ DESC_DATA - DESC_GDT
CodeSelector equ DESC_CODE - DESC_GDT
VideoSelector equ DESC_VIDEO - DESC_GDT

[SECTION .s16]
[BITS 16]
_main_16:
	mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

	call near _init_desc

	; 设置段基址
	xor eax, eax
    mov ax, ds
    shl eax, 4
    add eax, DESC_GDT
    mov dword [GdtPtr + 2], eax

	lgdt [GdtPtr]

	cli
	in al, 0x92
    or al, 00000010b
    out 0x92, al

	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp dword CodeSelector:0

_init_desc:
	xor eax, eax
	mov ax, cs
	shl eax, 4
	add eax, _main_32

	mov di, DESC_CODE
	call near _init_desc_base_address

	xor eax, eax
	mov ax, cs
	shl eax, 4
	add eax, STRING

	mov di, DESC_DATA
	call near _init_desc_base_address
	ret


_init_desc_base_address:
	mov word [di + 2], ax
	shr eax, 16
	mov byte [di + 4], al
    mov byte [di + 7], ah
	ret

[SECTION .s32]
[BITS 32]
_main_32:
    mov ax, VideoSelector
    mov gs, ax
    mov esi, 0xA0
 
    mov ax, DataSelector
    mov ds, ax
    mov edi, 0

    mov ecx, STRING_LEN

    print_loop:
        mov al, ds:[edi]
        mov ah, 0xC  ;设置成红色
        mov word gs:[esi], ax
        add esi, 2
        inc edi
        loop print_loop

    jmp $

CODE32_LEN equ $ - _main_32

[SECTION .s32]
[BITS 32]
STRING: db 'Hello, world'
STRING_LEN equ $ - STRING

times 290 db 0
dw 0xAA55
