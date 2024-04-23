OUTPUT_FORMAT("elf32-powerpc", "elf32-powerpc", "elf32-powerpc");
OUTPUT_ARCH(powerpc:common);
ENTRY(None)

MEMORY
{
	code (rwx) : ORIGIN = ORIGIN_ADDRESS, LENGTH = 0x01800000
}

SECTIONS
{
	.text : {
        . = ALIGN(4);
        KEEP(*(.apply_patches))

        . = ALIGN(4);
        _G_WiiRoomPatchStart = .;
        KEEP(*(.room_patch))
        _G_WiiRoomPatchEnd = .;

		*(.text*)
		*(.rodata*)
		*(.data*)
		*(.bss*)
		*(.sdata*)
		*(.sbss*)
		_G_End = .;
	} > code

	/DISCARD/ : {
		*(.comment*)
		*(.eh_frame*)
		*(.eh_frame_hdr*)
	}
}