; haribote-os
; TAB=4

; BOOT_INFO
CYLS	EQU		0x0ff0			; boot sector
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; clors
SCRNX	EQU		0x0ff4			; 
SCRNY	EQU		0x0ff6			; 
VRAM	EQU		0x0ff8			; display buffer

		ORG		0xc400			; 

		MOV		AL,0x13			; color display 320*200*8
		MOV		AH,0x00
		INT		0x10

		MOV		BYTE [VMODE],8	; color mode
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

; keyboard led status

		MOV		AH,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],AL

fin:
		HLT
		JMP		fin