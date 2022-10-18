
[BITS 32]						; 32�r�b�g���[�h�p�̋@�B�����点��

		GLOBAL	load_gdtr, load_idtr

;	load_gdtr(0x0000ffff, 0x00270000);
load_gdtr:		; void load_gdtr(int limit, int addr);
		;ff ff 00 00  00 00 27 00
		MOV		AX,[ESP+4]		; limit
		
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET
