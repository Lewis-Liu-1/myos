; haribote-ipl
; TAB=4

CYLS EQU 10 ; How far to read

ORG 0x7c00 ; where this program is loaded

; the following is for a standard FAT12 formatted floppy disk

JMP entry
DB 0x90
DB "HARIBOTE" ; You can freely write the boot sector name (8 bytes)
DW 512 ; one sector size (must be 512)
DB 1 ; cluster size (must be 1 sector)
DW 1 ; where the FAT starts (usually sector 1)
DB 2 ; Number of FATs (must be 2)
DW 224 ; Size of root directory area (usually 224 entries)
DW 2880 ; Size of this drive (must be 2880 sectors)
DB 0xf0 ; Media type (must be 0xf0)
DW 9 ; Length of FAT area (must be 9 sectors)
DW 18 ; how many sectors per track (must be 18)
DW 2 ; number of heads (must be 2)
DD 0 ; Always 0 here because no partition is used
DD 2880 ; write this drive size again
DB 0,0,0x29 ;
DD 0xffffffff ; probably volume serial number
DB "HARIBOTEOS " ; Disk name (11 bytes)
DB "FAT12 " ; Name of format (8 bytes)
RESB 18 ; Reserve 18 bytes for now

; program body

entry:
MOV AX,0 ; Register initialization
MOV SS,AX
MOV SP, 0x7c00
MOV DS,AX

; read disk

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

error:
MOV SI, msg
putloop:
MOV AL,[SI]
ADD SI,1 ; Add 1 to SI
CMP AL,0
JE fin
MOV AH,0x0e ; Single character display function
MOV BX,15 ; color code
INT 0x10 ; Video BIOS call
JMP putloop

fin:
HLT ; Halt the CPU until something happens
JMP fin ; infinite loop
msg:
DB 0x0a, 0x0a ; two newlines
DB "load error"
DB 0x0a ; newline
DB 0

;RESB 0x7dfe-$ ; Instruction to fill up to 0x7dfe with 0x00
;RESB 0x7dfe-($-$$)           ; 用 0x00 填充到 0x7dfe 的指令
RESB    0x1fe-($-$$)

DB 0x55, 0xaa
