	.file	"comandos_em_c.c"
	.text
	.globl	main
	.type	main, @function
;CODIGO ASSEMBLY DO comandos_em_c.c. Nossa saida deve ser proxima disso
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
;ESPECIALMETNE DAQUI
	movl	$5, -8(%rbp)
	movl	$7, -4(%rbp)
	jmp	.L2
.L3:
	movl	-4(%rbp), %eax
	movl	%eax, -8(%rbp)
	addl	$45, -8(%rbp)
.L2:
	cmpl	$-1, -4(%rbp)
	jl	.L3
	cmpl	$3, -8(%rbp)
	jle	.L4
	movl	-4(%rbp), %eax
	addl	$2, %eax
	addl	%eax, %eax
	movl	%eax, -8(%rbp)
	movl	$7, -4(%rbp)
	jmp	.L5
.L4:
	movl	$5, -4(%rbp)
	movl	$4, -8(%rbp)
.L5:
	movl	$0, %eax
	popq	%rbp
;ATE AQUI	
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
