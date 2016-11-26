	.file	"negdbl.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -16(%rbp)
	pxor	%xmm0, %xmm0
	ucomisd	-16(%rbp), %xmm0
	jp	.L2
	pxor	%xmm0, %xmm0
	ucomisd	-16(%rbp), %xmm0
	jne	.L2
	movsd	.LC2(%rip), %xmm0
	jmp	.L4
.L2:
	pxor	%xmm0, %xmm0
.L4:
	movsd	%xmm0, -8(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	605830907
	.long	1072822596
	.align 8
.LC2:
	.long	0
	.long	1072693248
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
