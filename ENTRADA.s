	pushl %eax
	movl 2, %eax
	movl %eax,x
	pushl %eax
	movl 2, %eax
	movl %eax,y
.L1
	pushl %eax
	movl y,%eax
	pushl %eax
	movl 1, %eax
	jz .L2
	pushl %eax
	movl y,%eax
	movl %eax,x
	pushl %eax
	movl x,%eax
	movl (%rsp), %ebx
	movl %rax, %eax
	addl %eax, (%esp)
	movsd %xmm0, %rax
	addq $8,%rsp
	pushl %eax
	movl 45, %eax
	movl (%rsp), %ebx
	movl %rax, %eax
	addl %eax, (%esp)
	movsd %xmm0, %rax
	addq $8,%rsp
	movl %eax,x
	pushl %eax
	movl y,%eax
	movl (%rsp), %ebx
	movl %rax, %eax
	addl %eax, (%esp)
	movsd %xmm0, %rax
	addq $8,%rsp
	pushl %eax
	movl 1, %eax
	movl (%rsp), %ebx
	movl %rax, %eax
	addl %eax, (%esp)
	movsd %xmm0, %rax
	addq $8,%rsp
	movl %eax,y
	jmp .L1
.L2
	pushl %eax
	movl x,%eax
	pushl %eax
	movl 3, %eax
	jz .L3
	pushl %eax
	movl 2, %eax
	movl (%rsp), %ebx
	movl %rax, %eax
	imull %esp, %eax
	movsd %xmm0, %rax
	addq $8,%rsp
	pushl %eax
	movl y,%eax
	movl (%rsp), %ebx
	movl %rax, %eax
	imull %esp, %eax
	movsd %xmm0, %rax
	addq $8,%rsp
	movl (%rsp), %ebx
	movl %rax, %eax
	addl %eax, (%esp)
	movsd %xmm0, %rax
	addq $8,%rsp
	pushl %eax
	movl 4, %eax
	movl (%rsp), %ebx
	movl %rax, %eax
	addl %eax, (%esp)
	movsd %xmm0, %rax
	addq $8,%rsp
	movl %eax,x
	pushl %eax
	movl 7, %eax
	movl %eax,y
	jmp .L4
.L3
.L4
	pushl %eax
	movl 5, %eax
	movl %eax,y
	pushl %eax
	movl 4, %eax
	movl %eax,x
.L4
