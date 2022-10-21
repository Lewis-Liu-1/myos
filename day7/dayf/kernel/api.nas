
[BITS 32]						; 32�r�b�g���[�h�p�̋@�B�����点��


		GLOBAL	load_gdtr, load_idtr1, load_idtr2,  asm_inthandler27, asm_inthandler21,asm_inthandler2c, io_stihlt 
		EXTERN	inthandler21, inthandler27, inthandler2c, keyboard_handler

;	load_gdtr(0x0000ffff, 0x00270000);
load_gdtr:		; void load_gdtr(int limit, int addr);
		;ff ff 00 00  00 00 27 00
		MOV		AX,[ESP+4]		; limit
		
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

load_idtr2:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET

load_idtr1:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	keyboard_handler; inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler27
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD
	