;
; A simple boot sector program that demonstrates segment offsetting
;
mov ah , 0x0e
; int 10/ ah = 0 eh -> scrolling teletype BIOS routine
mov al , [ the_secret ]
int 0x10

MOV bx , 0x7c0
MOV ds , bx
MOV al , [ the_secret ]
INT 0x10
; Does this print an X?
; Can ’t set ds directly , so set bx
; then copy bx to ds.
; Does this print an X?

mov al , [ es : the_secret ] ; Tell the CPU to use the es ( not ds ) segment.
int 0x10
; Does this print an X?




MOV bx , 0x7c0
MOV es , bx
MOV al , [ es : the_secret ]
INT 0x10
; Does this print an X?
jmp $
; Jump forever.
the_secret :
db "X"
; Padding and magic BIOS number.
times 510 -( $ - $$ ) db 0
dw 0xaa55