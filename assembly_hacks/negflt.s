	.file	"negflt.c"
	.section	.rodata
.LC3:
	.string	"b = not(a) =%f\n"
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
	subq	$16, %rsp
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -8(%rbp)
	pxor	%xmm0, %xmm0
	ucomiss	-8(%rbp), %xmm0
	jp	.L2
	pxor	%xmm0, %xmm0
	ucomiss	-8(%rbp), %xmm0
	jne	.L2
	movss	.LC2(%rip), %xmm0
	jmp	.L4
.L2:
	pxor	%xmm0, %xmm0
.L4:
	movss	%xmm0, -4(%rbp)
	cvtss2sd	-4(%rbp), %xmm0
	movl	$.LC3, %edi
	movl	$1, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1066359849
	.align 4
.LC2:
	.long	1065353216
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
