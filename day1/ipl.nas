; haribote-ipl
; 选项卡=4

ORG 0x7c00 ; 加载此程序的位置

; 以下为标准 FAT12 格式软盘

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
MOV AX,0                ; 寄存器初始化
MOV SS,AX
MOV SP,0x8c00
MOV DS,AX               ; DS=0

; 读取磁盘

MOV AX,0x0820           ; AX=0x0820
MOV ES,AX               ; ES=0x0820
MOV CH,0                ; cylinder 0
MOV DH,0                ; 头 0
MOV CL,2                ; 扇区 2

MOV AH,0x02             ; AH=0x02 : 磁盘读取
MOV AL,1                ; 1 个扇区
MOV BX,0
MOV DL,0x00             ; 驱动器 A
INT 0x13                ; 磁盘 BIOS 调用
JC err                  ; jump if carry

; 看完了,不过暂时无事可做,先去睡觉了
CALL ok

fin:
HLT                     ; 停止 CPU 直到有事发生
JMP fin                 ; 无限循环

err:
MOV SI,msg
putloop:
MOV AL,[SI]             ; [SI]=[DS:SI]
ADD SI,1                ; SI 加 1
CMP AL,0
JE  fin                 ;

MOV AH,0x0e             ; 单字符显示函数
MOV BX,15               ; 颜色代码
INT 0x10                ; 视频 BIOS 调用
JMP putloop

msg:
DB 0x0a, 0x0a           ; 两个换行符
DB "LOAD ERROR"         ; 
DB 0x0a                 ; 换行
DB 0

ok:
MOV SI,msg1
putloop1:
MOV AL,[SI]             ; [SI]=[DS:SI]
ADD SI,1                ; SI 加 1
CMP AL,0
JE  fin1                 ;

MOV AH,0x0e             ; 单字符显示函数
MOV BX,15               ; 颜色代码
INT 0x10                ; 视频 BIOS 调用
JMP putloop1

fin1:
  RET
  
msg1:
DB 0x0a, 0x0a           ; 两个换行符
DB "HELLO WORLD"         ; 
DB 0x0a                 ; 换行
DB 0

;RESB 0x7dfe-($-$$)           ; 用 0x00 填充到 0x7dfe 的指令
RESB    0x1fe-($-$$)

DB 0x55, 0xaa
