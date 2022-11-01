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
        mov ah, 0xC
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
