/*
 *  linux/arch/armnommu/boot/compressed/head-p2001.S
 *
 *  Copyright (C) 2004 Tobias Lorenz
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>

/*
 * Debugging stuff
 *
 * Note that these macros must not contain any code which is not
 * 100% relocatable.  Any attempt to do so will result in a crash.
 * Please select one of the following when turning on debugging.
 */
#ifdef DEBUG
		.macro	loadsp, rb
		mov	\rb, #0x00140000	@ Adr_UART_OLD_BASE
		.endm

		.macro	writeb, rb
		str	\rb, [r3, #0x00]	@ REL_Adr_Uart_TX1
		.endm
#endif

		.macro	kputc,val
		mov	r0, \val
		bl	putc
		.endm

		.macro	kphex,val,len
		mov	r0, \val
		mov	r1, #\len
		bl	phex
		.endm

		.macro	debug_reloc_start
#ifdef DEBUG
		kputc	#'\n'
		kphex	r6, 8		/* processor id */
		kputc	#':'
		kphex	r7, 8		/* architecture id */
		kputc	#'\n'
		kphex	r5, 8		/* decompressed kernel start */
		kputc	#'-'
		kphex	r8, 8		/* decompressed kernel end  */
		kputc	#'>'
		kphex	r4, 8		/* kernel execution address */
		kputc	#'\n'
#endif
		.endm

		.macro	debug_reloc_end
#ifdef DEBUG
		kphex	r5, 8		/* end of kernel */
		kputc	#'\n'
		mov	r0, r4
		bl	memdump		/* dump 256 bytes at start of kernel */
#endif
		.endm


		.section ".start", #alloc, #execinstr
/*
 * sort out different calling conventions
 */
		.align
start:
		.type	start,#function
		.rept	8
		mov	r0, r0
		.endr

		b	1f
		.word	0x016f2818		@ Magic numbers to help the loader
		.word	start			@ absolute load/run zImage address
		.word	_edata			@ zImage end address
1:		mov	r7, r1			@ save architecture ID
		mov	r8, #0			@ save r0

		/*
		 * Booting from P2001 - need to enter SVC mode and disable
		 * FIQs/IRQs (numeric definitions from angel arm.h source).
		 * We only do this if we were in user mode on entry.
		 */
		mrs	r2, cpsr		@ get current mode
		orr	r2, r2, #0xc0		@ turn off interrupts
		msr	cpsr_c, r2

		/*
		 * Note that some cache flushing and other stuff may
		 * be needed here - is there an Angel SWI call for this?
		 */

		/*
		 * some architecture specific code can be inserted
		 * by the linker here, but it should preserve r7 and r8.
		 */

		.text
		adr	r0, LC0
		ldmia	r0, {r1, r2, r3, r4, r5, r6, ip, sp}
		subs	r0, r0, r1		@ calculate the delta offset

						@ if delta is zero, we're
		beq	not_relocated		@ running at the address we
						@ were linked at.

		/*
		 * We're running at a different address.  We need to fix
		 * up various pointers:
		 *   r5 - zImage base address
		 *   r6 - GOT start
		 *   ip - GOT end
		 */
		add	r5, r5, r0
		add	r6, r6, r0
		add	ip, ip, r0

#ifndef CONFIG_ZBOOT_ROM
		/*
		 * If we're running fully PIC === CONFIG_ZBOOT_ROM = n,
		 * we need to fix up pointers into the BSS region.
		 *   r2 - BSS start
		 *   r3 - BSS end
		 *   sp - stack pointer
		 */
		add	r2, r2, r0
		add	r3, r3, r0
		add	sp, sp, r0

		/*
		 * Relocate all entries in the GOT table.
		 */
1:		ldr	r1, [r6, #0]		@ relocate entries in the GOT
		add	r1, r1, r0		@ table.  This fixes up the
		str	r1, [r6], #4		@ C references.
		cmp	r6, ip
		blo	1b
#else

		/*
		 * Relocate entries in the GOT table.  We only relocate
		 * the entries that are outside the (relocated) BSS region.
		 */
1:		ldr	r1, [r6, #0]		@ relocate entries in the GOT
		cmp	r1, r2			@ entry < bss_start ||
		cmphs	r3, r1			@ _end < entry
		addlo	r1, r1, r0		@ table.  This fixes up the
		str	r1, [r6], #4		@ C references.
		cmp	r6, ip
		blo	1b
#endif

not_relocated:	mov	r0, #0
1:		str	r0, [r2], #4		@ clear bss
		str	r0, [r2], #4
		str	r0, [r2], #4
		str	r0, [r2], #4
		cmp	r2, r3
		blo	1b

		/*
		 * The C runtime environment should now be setup
		 * sufficiently.  Set up some pointers, and start decompressing.
		 */
		mov	r1, sp			@ malloc space above stack
		add	r2, sp, #0x10000	@ 64k max

/*
 * Check to see if we will overwrite ourselves.
 *   r4 = final kernel address
 *   r5 = start of this image
 *   r2 = end of malloc space (and therefore this image)
 * We basically want:
 *   r4 >= r2 -> OK
 *   r4 + image length <= r5 -> OK
 */
		cmp	r4, r2
		bhs	wont_overwrite
		add	r0, r4, #4096*1024	@ 4MB largest kernel size
		cmp	r0, r5
		bls	wont_overwrite

		mov	r5, r2			@ decompress after malloc space
		mov	r0, r5
		mov	r3, r7
		bl	decompress_kernel

		add	r0, r0, #127
		bic	r0, r0, #127		@ align the kernel length
/*
 * r0     = decompressed kernel length
 * r1-r3  = unused
 * r4     = kernel execution address
 * r5     = decompressed kernel start
 * r6     = processor ID
 * r7     = architecture ID
 * r8-r14 = unused
 */
		add	r1, r5, r0		@ end of decompressed kernel
		adr	r2, reloc_start
		ldr	r3, LC1
		add	r3, r2, r3
1:		ldmia	r2!, {r8 - r13}		@ copy relocation code
		stmia	r1!, {r8 - r13}
		ldmia	r2!, {r8 - r13}
		stmia	r1!, {r8 - r13}
		cmp	r2, r3
		blo	1b

		add	pc, r5, r0		@ call relocation code

/*
 * We're not in danger of overwriting ourselves.  Do this the simple way.
 *
 * r4     = kernel execution address
 * r7     = architecture ID
 */
wont_overwrite:	mov	r0, r4
		mov	r3, r7
		bl	decompress_kernel
		b	call_kernel

		.type	LC0, #object
LC0:		.word	LC0			@ r1
		.word	__bss_start		@ r2
		.word	_end			@ r3
		.word	_load_addr		@ r4
		.word	_start			@ r5
		.word	_got_start		@ r6
		.word	_got_end		@ ip
		.word	user_stack+4096		@ sp
LC1:		.word	reloc_end - reloc_start
		.size	LC0, . - LC0

/*
 * Initialise the page tables, turning on the cacheable and bufferable
 * bits for the RAM area only.
 */
		mov	r0, r3
		mov	r8, r0, lsr #18
		mov	r8, r8, lsl #18		@ start of RAM
		add	r9, r8, #0x10000000	@ a reasonable RAM size
		mov	r1, #0x12
		orr	r1, r1, #3 << 10
		add	r2, r3, #16384
1:		cmp	r1, r8			@ if virt > start of RAM
		orrhs	r1, r1, #0x0c		@ set cacheable, bufferable
		cmp	r1, r9			@ if virt > end of RAM
		bichs	r1, r1, #0x0c		@ clear cacheable, bufferable
		str	r1, [r0], #4		@ 1:1 mapping
		add	r1, r1, #1048576
		teq	r0, r2
		bne	1b
/*
 * If ever we are running from Flash, then we surely want the cache
 * to be enabled also for our execution instance...  We map 2MB of it
 * so there is no map overlap problem for up to 1 MB compressed kernel.
 * If the execution is in RAM then we would only be duplicating the above.
 */
		mov	r1, #0x1e
		orr	r1, r1, #3 << 10
		mov	r2, pc, lsr #20
		orr	r1, r1, r2, lsl #20
		add	r0, r3, r2, lsl #2
		str	r1, [r0], #4
		add	r1, r1, #1048576
		str	r1, [r0]
		mov	pc, lr

/*
 * All code following this line is relocatable.  It is relocated by
 * the above code to the end of the decompressed kernel image and
 * executed there.  During this time, we have no stacks.
 *
 * r0     = decompressed kernel length
 * r1-r3  = unused
 * r4     = kernel execution address
 * r5     = decompressed kernel start
 * r6     = processor ID
 * r7     = architecture ID
 * r8-r14 = unused
 */
		.align	5
reloc_start:	add	r8, r5, r0
		debug_reloc_start
		mov	r1, r4
1:
		.rept	4
		ldmia	r5!, {r0, r2, r3, r9 - r13}	@ relocate kernel
		stmia	r1!, {r0, r2, r3, r9 - r13}
		.endr

		cmp	r5, r8
		blo	1b
		debug_reloc_end

call_kernel: 	mov	r0, #0
		mov	r1, r7			@ restore architecture number
		mov	pc, r4			@ call kernel

/*
 * Various debugging routines for printing hex characters and
 * memory, which again must be relocatable.
 */
#ifdef DEBUG
		.type	phexbuf,#object
phexbuf:	.space	12
		.size	phexbuf, . - phexbuf

phex:		adr	r3, phexbuf
		mov	r2, #0
		strb	r2, [r3, r1]
1:		subs	r1, r1, #1
		movmi	r0, r3
		bmi	puts
		and	r2, r0, #15
		mov	r0, r0, lsr #4
		cmp	r2, #10
		addge	r2, r2, #7
		add	r2, r2, #'0'
		strb	r2, [r3, r1]
		b	1b

puts:		loadsp	r3
1:		ldrb	r2, [r0], #1
		teq	r2, #0
		moveq	pc, lr
2:		writeb	r2
		mov	r1, #0x00020000
3:		subs	r1, r1, #1
		bne	3b
		teq	r2, #'\n'
		moveq	r2, #'\r'
		beq	2b
		teq	r0, #0
		bne	1b
		mov	pc, lr
putc:
		mov	r2, r0
		mov	r0, #0
		loadsp	r3
		b	2b

memdump:	mov	r12, r0
		mov	r10, lr
		mov	r11, #0
2:		mov	r0, r11, lsl #2
		add	r0, r0, r12
		mov	r1, #8
		bl	phex
		mov	r0, #':'
		bl	putc
1:		mov	r0, #' '
		bl	putc
		ldr	r0, [r12, r11, lsl #2]
		mov	r1, #8
		bl	phex
		and	r0, r11, #7
		teq	r0, #3
		moveq	r0, #' '
		bleq	putc
		and	r0, r11, #7
		add	r11, r11, #1
		teq	r0, #7
		bne	1b
		mov	r0, #'\n'
		bl	putc
		cmp	r11, #64
		blt	2b
		mov	pc, r10
#endif

reloc_end:

		.align
		.section ".stack", "w"
user_stack:	.space	4096
