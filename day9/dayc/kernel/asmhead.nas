; haribote-os boot asm
; TAB=4

BOTPAK	EQU		0x00280000		; bootpack LOADED location
DSKCAC	EQU		0x00100000		; disk cache location
DSKCAC0	EQU		0x00008000		; disk cache real mode location

; BOOT_INFO�֌W
CYLS	EQU		0x0ff0			; boot sector
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; color info
SCRNX	EQU		0x0ff4			; resolution X
SCRNY	EQU		0x0ff6			; resolution Y
VRAM	EQU		0x0ff8			; picture cache start position

		ORG		0xc200			; ���̃v���O�������ǂ��ɓǂݍ��܂��̂�

; display setting

		MOV		AL,0x13			; VGA display card 320*200*8 bit
        MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	; screen mode
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

; �L�[�{�[�h��LED��Ԃ�BIOS�ɋ����Ă��炤

		MOV		AH,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],AL

; PIC close all interrupt
;	according to AT compatible, if init PIC,
;	must done before CLI, or halt happens
;	PIC intialise

		MOV		AL,0xff
		OUT		0x21,AL
		NOP						; if continue OUT, some machines can't run
		OUT		0xa1,AL

		CLI						; stop CPU interrupts

; if during mode switch, interrupt happens, big trouble

; 

		CALL	waitkbdout ; equals wait_KBC_sendready
		MOV		AL,0xd1     ;  key command port write output
		OUT		0x64,AL     ;PORT_KEYCMD
		CALL	waitkbdout
		MOV		AL,0xdf			; enable A20
		OUT		0x60,AL         ; PORT_KEYDAT, key data port 
		CALL	waitkbdout
        ; Above process is to ask keybord connected circuit output 0XDF,
        ; WHICH will let A20GATE signal line ON

; switch to protect mode

[INSTRSET "i486p"]				; want to use 486 instructions

		LGDT	[GDTR0]			; setup temporary GDT
		MOV		EAX,CR0
		AND		EAX,0x7fffffff	; bit31 TO 0, in order to stop
		OR		EAX,0x00000001	; bit0 to 1, switch to protect mode
		MOV		CR0,EAX
		JMP		pipelineflush
pipelineflush:
		MOV		AX,1*8			;  R/W segment to be 32bit; GDT+1
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX

; bootpack's transmit

		MOV		ESI,bootpack	; source 
		MOV		EDI,BOTPAK		; destination
		MOV		ECX,512*1024/4
		CALL	memcpy
        ;memcpy(bootpack, BOTPAK, 512*1024/4)

; disk data finnally sent to it orignal location

; first start from boot sector

		MOV		ESI,0x7c00		; source 
		MOV		EDI,DSKCAC		; destination; copy 512 bytes to RAM after 1MB
		MOV		ECX,512/4
		CALL	memcpy

; all left
        ; copy 0x8200 disk content to 0x100200
		MOV		ESI,DSKCAC0+512	; source 
		MOV		EDI,DSKCAC+512	; destination
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4	; cylinders to bytes /4
		SUB		ECX,512/4		; minus IPL
		CALL	memcpy

; WORKS that must done by asmhead has finished
;	now all rest is bootpack to finish

; bootpack startup

		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3			; ECX += 3;
		SHR		ECX,2			; ECX /= 4;
		JZ		skip			; no more to transmit
		MOV		ESI,[EBX+20]	; transmit source
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	; transmit destination
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]	; stack initial value
		JMP		DWORD 2*8:0x0000001b ; CS=2*8, 0x280000+0x1b
        ; now jump to 0x28001b to run
        ;memory info
        ;0-0xfffff 1MB
        ;0x100000 - 0x267fff       store floppy disk content
        ;0x268000 - 0x26f7ff       empty
        ;0x26f800 - 0x26ffff       idt
        ;0x270000 - 0x27ffff       gdt
        ;0x280000 - 0x2fffff       bootpack.hrb(512kb)
        ;0x300000 - 0x3fffff        stack

waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout		; AND�̌��ʂ�0�łȂ����waitkbdout��
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy			; �����Z�������ʂ�0�łȂ����memcpy��
		RET
; memcpy�̓A�h���X�T�C�Y�v���t�B�N�X�����Y��Ȃ���΁A�X�g�����O���߂ł�������

		ALIGNB	16              ; continue add DB0
                                ; until address can be divided by 16
GDT0:
		RESB	8				; �k���Z���N�^
		DW		0xffff,0x0000,0x9200,0x00cf	; �ǂݏ����\�Z�O�����g32bit
		DW		0xffff,0x0000,0x9a28,0x0047	; ���s�\�Z�O�����g32bit�ibootpack�p�j

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

		ALIGNB	16
bootpack:
