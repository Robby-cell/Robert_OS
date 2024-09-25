bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002              ;magic
        dd 0x00                    ;flags
        dd - (0x1BADB002 + 0x00)   ;checksum. m+f+c should be zero

global start
global Krnl_keyboard_handler
global Krnl_read_port
global Krnl_write_port
global Krnl_load_idt

extern kmain 		;this is defined in the c file
extern Krnl_keyboard_handler_main

Krnl_read_port:
	mov edx, [esp + 4]
			;al is the lower 8 bits of eax
	in al, dx	;dx is the lower 16 bits of edx
	ret

Krnl_write_port:
	mov   edx, [esp + 4]    
	mov   al, [esp + 4 + 4]  
	out   dx, al  
	ret

Krnl_load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti 				;turn on interrupts
	ret

Krnl_keyboard_handler:                 
	call    Krnl_keyboard_handler_main
	iretd

start:
	cli 				;block interrupts
	mov esp, stack_space
	call kmain
	hlt 				;halt the CPU

section .bss
resb 8192; 8KB for stack
stack_space:
