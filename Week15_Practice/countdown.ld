ENTRY(reset_handler)

MEMORY{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 64K
	RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
	.text : {
		KEEP(*(.isr_vector))
		*(.text)
		*(.text.*)
		*(.rodata)
		_sidata = .;
	} >FLASH

	.data : AT(_sidata){
		_sdata = .;
		*(.data)
		*(.data*)
		_edata = .;
	} >RAM

	.bss : {
		_sbss = .;
		*(.bss)
		_ebss = .;
	} >RAM

	_estack = ORIGIN(RAM) + LENGTH(RAM);
}
