ENTRY(start)
SECTIONS
{
	/*. = 0xc400;*/
	. = 0x280000;
	.text :{
		*(.text)
	}	
	. = ALIGN(32);
	.data :{
		*(.data)
	}
	. = ALIGN(32);
	.bss :{
		*(.bss)
	}
}