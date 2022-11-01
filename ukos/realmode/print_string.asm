[ bits 32]
; Define some constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f
; prints a null - terminated string pointed to by EDX
print_string:
    pusha
    mov edx , VIDEO_MEMORY ; Set edx to the start of vid mem.
    print_string_pm_loop :
    mov al , [ ebx ]
    ; Store the char at EBX in AL
    mov ah , WHITE_ON_BLACK ; Store the attributes in AH
    cmp al , 0
    je print_string_pm_done ; if ( al == 0) , at end of string , so
    ; jump to done
    mov [ edx ] , ax ;

    add ebx , 1
    add edx , 2
    ;Store char and attributes at current
    ;character cell.
    ;Increment EBX to the next char in string.
    ;Move to next character cell in vid mem.
    jmp print_string_pm_loop

    ; loop around to print the next char.
    print_string_pm_done :
    popa
    ret
    ; Return from the function

[bits 16]
print_string1:
    pusha
    MOV AH, 0xe

    print_me:
    MOV AL, [BX]
    CMP AL, 0
    JE END_ME
    INT 10H
    ADD BX, 1
    JMP print_me

END_ME:
    popa
    ret

print_hex:
    PUSHA
    MOV CX, 7
MOV_PRINT:
    SHL CX, 2

    MOV AX, DX
    shr AX, CL
    AND AX, 0XF
    MOV BX, HEX_PATTERN
    ADD BX, AX
    MOV AX, [BX]
    MOV AH, 0XE
    INT 10H
    SHR CX, 2
    DEC CX
    CMP CX, 0
    JE  END_NOW
    JMP MOV_PRINT
 END_NOW:
    MOV AX, DX
    AND AX, 0XF
    MOV BX, HEX_PATTERN
    ADD BX, AX
    MOV AX, [BX]
    MOV AH, 0XE
    INT 10H    
    popa
    ret

    HEX_PATTERN:
     db '0123456789ABCDEF',0
