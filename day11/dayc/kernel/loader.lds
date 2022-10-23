ENTRY(_start)
SECTIONS
{
	. = 0xc400;
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