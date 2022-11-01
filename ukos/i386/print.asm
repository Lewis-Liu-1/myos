
[bits 16]
print_string:
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
