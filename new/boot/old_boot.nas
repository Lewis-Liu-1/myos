CYLS EQU 10 ; How far to read

ORG 0X7C00

JMP entry
DB 0x90
DB "HARIBOTE"           ; 可以自由写入引导扇区名称(8字节)
DW 512                  ; 一个扇区大小(必须为 512)
DB 1                    ; 簇大小(必须是 1 个扇区)
DW 1                    ; FAT 开始的地方(通常是扇区 1)
DB 2                    ; FAT 数量(必须为 2)
DW 224                  ; 根目录区域的大小(通常为 224 个条目)
DW 2880                 ; 该驱动器的大小(必须是 2880 个扇区)
DB 0xf0                 ; 媒体类型(必须为 0xf0)
DW 9                    ; FAT 区域的长度(必须是 9 个扇区)
DW 18                   ; 每个磁道有多少个扇区(必须是 18)
DW 2                    ; 正面数(必须为 2)
DD 0                    ; 这里总是 0 因为没有使ESB用分区
DD 2880                 ; 再次写入此驱动器大小
DB 0,0,0x29             ;
DD 0xffffffff           ; 可能是卷序列号
DB "HARIBOTEOS"         ; 磁盘名称(11 个字节)
DB "FAT12"              ; 格式名称(8 字节)
RESB 18                 ; 暂时保留 18 个字节

; 程序体

entry:
    MOV ax, 0
    mov ss, ax
    mov sp, 0x8c00
    mov ds, ax

    MOV AX, 0x0820
    MOV ES,AX
    MOV CH,0 ; Cylinder 0
    MOV DH,0 ; head 0
    MOV CL,2 ; sector 2
    readloop:
    MOV SI,0 ; register to count the number of failures
    retry:
    MOV AH,0x02 ; AH=0x02 : Disk read
    MOV AL,1 ; 1 sector
    MOV BX,0
    MOV DL,0x00 ; Drive A
    INT 0x13 ; Disk BIOS call
    JNC next ; To next if no error occurs
    ADD SI,1 ; Add 1 to SI
    CMP SI,5 ; Compare SI and 5
    JAE error ; If SI >= 5, go to error
    MOV AH,0x00
    MOV DL,0x00 ; Drive A
    INT 0x13 ; Drive reset
    JMP retry
    next:
    MOV AX,ES ; Advance address by 0x200
    ADD AX, 0x0020
    MOV ES,AX ; ADD ES,0x020 This is because there is no instruction
    ADD CL,1 ; add 1 to CL
    CMP CL,18 ; Compare CL with 18
    JBE readloop ; If CL <= 18, go to readloop
    MOV CL,1
    ADD DH,1
    CMP DH,2
    JB readloop ; If DH < 2, go to readloop
    MOV DH,0
    ADD CH,1
    CMP CH, CYLS
    JB readloop ; If CH < CYLS, go to readloop

    ; Now that you've read it, run haribote.sys!

    JMP 0xc400


fin:
    jmp fin
    
error:
    mov si, msg
putloop:
    mov al, [si]
    add si, 1
    cmp al, 0
    je fin

    mov ah, 0xe
    mov bx, 15
    int 0x10
    jmp putloop

msg:
    db 0xa,0xa
    db "load error"
    db 0xa,0
ok:
    mov si, msg1
putloop1:
    mov al, [si]
    add si, 1
    cmp al, 0
    je fin1
    mov ah, 0xe
    mov bx, 15
    int 0x10
    jmp putloop1
fin1:
    ret

msg1:
    db 0xa,0xa
    db "hello world"
    db 0xd, 0xa,0

    resb 0x1fe-($-$$)

    db 0x55, 0xaa


