/**@<pseudoassembly.c>::**/
#include <pseudoassembly.h>

/*unified label counter*/

int labelcounter = 1;

/*control pseudo instructions*/

int gofalse(int label)
{
  fprintf(object, "\tjz .L%d\n", label);
  return label;
}

int jump (int label)
{
  fprintf(object, "\tjmp .L%d\n", label);
  return label;
}

int mklabel(int label)
{
  fprintf(object, ".L%d\n", label);
  return label;
}

int lmovel (char const *variable) // copy of 32 bits
{
  fprintf(object, "\tmovl %%eax,%s\n",variable);
  return 0;
}

int lmoveq (char const *variable) // copy of 64 bits
{
  fprintf(object, "\tmovq %%rax,%s\n",variable);
  return 0;
}

int rmovel (char const *variable) // copy of 32 bits
{
  fprintf(object, "\tpushl %%eax\n");
  fprintf(object, "\tmovl %s, %%eax\n",variable);
  return 0;
}

int rmoveq (char const *variable) // copy of 64 bits
{
  fprintf(object, "\tpushl %%rax\n");
  fprintf(object, "\tmovq %s, %%rax\n",variable);
  return 0;
}

/*ULA pseudo-instructions*/

/*unary*/

int neglog(void)
{
  return 0;
}

int negint(void)
{
  return 0;
}

int negflt(void)
{
  return 0;
}

int negdbl(void)
{
  return 0;
}

/*binary addition and inversion*/

int addlog(int VAR1,int VAR2)
{
/*
Assembly parcial de um codigo c,de uma simples operacao c = 1 OR 0;
pushq	%rbp
.cfi_def_cfa_offset 16
.cfi_offset 6, -16
movq	%rsp, %rbp
.cfi_def_cfa_register 6
subq	$16, %rsp
movl	$1, -12(%rbp)
movl	$0, -8(%rbp)
cmpl	$0, -12(%rbp)
jne	.L2
cmpl	$0, -8(%rbp)
je	.L3
.L2:
movl	$1, %eax
jmp	.L4
.L3:
movl	$0, %eax
.L4:
movl	%eax, -4(%rbp)
movl	-4(%rbp), %eax
movl	%eax, %esi
movl	$.LC0, %edi
movl	$0, %eax
*/
fprintf(object, "\tpushq	%%rbp\n");
fprintf(object, "\tmovq	%%rsp, %%rbp\n");
fprintf(object, "\tsubq	$16, %%rsp\n");
fprintf(object, "\tmovl	$%d, -12(%%rbp)\n",VAR1);
fprintf(object, "\tmovl	$%d, -8(%%rbp)\n",VAR2);
fprintf(object, "\tcmpl	$0, -12(%%rbp)\n");
labelcounter++;
fprintf(object, "\tjne	.L%d\n",labelcounter);
fprintf(object, "\tcmpl	$0, -8(%%rbp)\n");
fprintf(object, "\tje	.L%d\n",labelcounter);
fprintf(object, ".L%d:\n",labelcounter);
fprintf(object, "\tmovl	$1, %%eax\n");
labelcounter++;
fprintf(object, "\tjmp	.L%d\n",labelcounter + 1);
fprintf(object, ".L%d:\n",labelcounter);
fprintf(object, "\tmovl	$0, %%eax\n");
labelcounter++;
fprintf(object, ".L%d:\n",labelcounter);
fprintf(object, "\tmovl	%%eax, -4(%%rbp)\n");
fprintf(object, "\tmovl	-4(%%rbp), %%eax\n");
fprintf(object, "\tmovl	%%eax, %%esi\n");
return 0;
}

int addint(int VAR1,int VAR2)
{
/*
  addl %eax, (%rsp)
  popl %eax
 */
 fprintf(object, "\taddl %%eax, (%%esp)\n");
 fprintf(object, "\tpopl %%eax\n\n");
 return 0;
}

int addflt(float VAR1, float VAR2)
{
/*
  movss (%esp), %xmm1
  movss %eax, %xmm0
  addss xmm1, %xmm0
  movss %xmm0, x(%rip)
  addl $4, %esp
*/
  fprintf(object, "\tmovss (%%esp), %%xmm1\n");
  fprintf(object, "\tmovss %%eax, %%xmm0\n");
  fprintf(object, "\taddss %%xmm1, %%xmm0\n");
  fprintf(object, "\tmovss %%xmm0, x(%%rip)\n");
  fprintf(object, "\taddl $4, %%esp\n");
  return 0;
}

int adddbl(double VAR1, double VAR2)
{
/*
  movsd   xx(%rsp), %xmm1
  movsd   %rax, %xmm0
  addsd   %xmm1, xmm0
  movsd   %xmm0, %rax
  addq    $8, %rsp
*/
  fprintf(object, "\tmovsd xx(%%rsp), %%xmm1\n");
  fprintf(object, "\tmovsd %%rax, %%xmm0\n");
  fprintf(object, "\taddsd %%xmm1, %%xmm0\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8, %%rsp\n");
  return 0;
}

int subint(int VAR1,int VAR2)
{
  /*
  Assembly parcial de um codigo c,de uma simples operacao c = 25 - 7;
  pushq	%rbp
  .cfi_def_cfa_offset 16
  .cfi_offset 6, -16
  movq	%rsp, %rbp
  .cfi_def_cfa_register 6
  subq	$16, %rsp
  movl	$25, -12(%rbp)
  movl	$7, -8(%rbp)
  movl	-12(%rbp), %eax
  subl	-8(%rbp), %eax
  movl	%eax, -4(%rbp)
  movl	-4(%rbp), %eax
  movl	%eax, %esi
  movl	$.LC0, %edi
  movl	$0, %eax
  */
  fprintf(object, "\tpushq	%%rbp\n");
  fprintf(object, "\tmovq	%%rsp, %%rbp\n");
  fprintf(object, "\tsubq	$16, %%rsp\n");
  fprintf(object, "\tmovl	$%d, -12(%%rbp)\n",VAR1);
  fprintf(object, "\tmovl	$%d, -8(%%rbp)\n",VAR2);
  fprintf(object, "\tmovl	-12(%%rbp), %%eax\n");
  fprintf(object, "\tsubl	-8(%%rbp), %%eax\n");
  fprintf(object, "\tmovl	%%eax, -4(%%rbp)\n");
  fprintf(object, "\tmovl	-4(%%rbp), %%eax\n");
  fprintf(object, "\tmovl	%%eax, %%esi\n");
  fprintf(object, "\tmovl	$.LC0, %%edi\n");
  fprintf(object, "\tmovl	$0, %%eax\n");
  return 0;
}

int subflt(float VAR1,float VAR2)
{
  /*
  Assembly parcial de um codigo c,de uma simples operacao c = 10.20 - 2.34
  pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -12(%rbp)
	movss	.LC1(%rip), %xmm0
	movss	%xmm0, -8(%rbp)
	movss	-12(%rbp), %xmm0
	subss	-8(%rbp), %xmm0
	movss	%xmm0, -4(%rbp)
	cvtss2sd	-4(%rbp), %xmm0
  ;;;
  .align 4
.LC0:
  .long	1092825907
  .align 4
.LC1:
  .long	1075167887
  */
  fprintf(object, "\tpushq	%%rbp\n");
  fprintf(object, "\tmovq	%%rsp, %%rbp\n");
  fprintf(object, "\tsubq	$16, %%rsp\n");
  labelcounter++;
  fprintf(object, "\tmovss	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovss	%%xmm0, -12(%%rbp)\n");
  labelcounter++;
  fprintf(object, "\tmovss	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovss	%%xmm0, -8(%%rbp)\n");
  fprintf(object, "\tmovss	-12(%%rbp), %%xmm0\n");
  fprintf(object, "\tsubss	-8(%%rbp), %%xmm0\n");
  fprintf(object, "\tmovss	%%xmm0, -4(%%rbp)\n");
  fprintf(object, "\tcvtss2sd	-4(%%rbp), %%xmm0\n");
  fprintf(object, "\t.align 4\n");
  fprintf(object, ".LC%d\n",labelcounter - 1);
  //nao eh pra ler var como float, eh pra ler como long REMOVER ESSE COMENTARIO DEPOIS
  fprintf(object, "\t.long	$%f",VAR1);
  fprintf(object, "\t.align 4\n");
  fprintf(object, ".LC%d\n",labelcounter);
  //nao eh pra ler var como float, eh pra ler como long REMOVER ESSE COMENTARIO DEPOIS
  fprintf(object, "\t.long	$%f",VAR2);
  return 0;
}

int subdbl(double VAR1,double VAR2)
{
  /*
  Assembly parcial de um codigo c,de uma simples operacao c = 10.20 - 2.34,sendo as variaveis envolvidas double
  pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -16(%rbp)
	movsd	-24(%rbp), %xmm0
	subsd	-16(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	movl	$.LC2, %edi
	movl	$1, %eax
  .align 8
.LC0:
  .long	1717986918
  .long	1076127334
  .align 8
.LC1:
  .long	3951369912
  .long	1073920081
  */
  fprintf(object, "\tpushq	%%rbp\n");
  fprintf(object, "\tmovq	%%rsp, %%rbp\n");
  fprintf(object, "\tsubq	$48, %%rsp\n");
  labelcounter++;
  fprintf(object, "\tmovsd	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovsd	%%xmm0, -24(%%rbp)\n");
  labelcounter++;
  fprintf(object, "\tmovsd	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovsd	%%xmm0, -16(%%rbp)\n");
  fprintf(object, "\tmovsd	-24(%%rbp), %%xmm0\n");
  fprintf(object, "\tsubsd	-16(%%rbp), %%xmm0\n");
  fprintf(object, "\tmovsd	%%xmm0, -8(%%rbp)\n");
  fprintf(object, "\tmovsd	%%xmm0, -8(%%rbp)\n");
  fprintf(object, "\tmovq	-8(%%rbp), %%rax\n");
  fprintf(object, "\tmovq	%%rax, -40(%%rbp)\n");
  fprintf(object, "\tmovsd	-40(%%rbp), %%xmm0\n");
  fprintf(object, ".LC%d:\n",labelcounter-1);
  fprintf(object, "\t.long	VAR1_parteAlta\n"/*VAR1 parte alta*/);
  fprintf(object, "\t.long	VAR1_parteBaixa\n"/*VAR1 parte baixa*/);
  fprintf(object, "\t.align 8\n");
  fprintf(object, ".LC%d:\n",labelcounter);
  fprintf(object, "\t.long	VAR2_parteAlta\n"/*,VAR2 parte alta*/);
  fprintf(object, "\t.long	VAR2_parteBaixa\n"/*,VAR2 parte baixa*/);
  return 0;
}

/*binary multiplication and inverse*/

int mullog(int VAR1,int VAR2)
{
  /*
  Assembly parcial de um codigo c,de uma simples operacao c = 1 && 0;
  movl	$1, -12(%rbp)
  movl	$0, -8(%rbp)
  cmpl	$0, -12(%rbp)
  je	.L2
  cmpl	$0, -8(%rbp)
  je	.L2
  movl	$1, %eax
  jmp	.L3
.L2:
  movl	$0, %eax
.L3:
  movl	%eax, -4(%rbp)
  movl	-4(%rbp), %eax
  movl	%eax, %esi
  movl	$.LC0, %edi
  movl	$0, %eax
  */
  fprintf(object, "\tmovl $%d, -12(%%rbp)\n",VAR1);
  fprintf(object, "\tmovl $%d, -8(%%rbp)\n",VAR2);
  fprintf(object, "\tcmpl	$0, -12(%%rbp)\n");
  labelcounter++;
  fprintf(object, "\tje	.L%d\n",labelcounter);
  fprintf(object, "\tcmpl	$0, -8(%%rbp)\n");
  fprintf(object, "\tje	.L%d\n",labelcounter);
  fprintf(object, "\tmovl	$1, %%eax\n");
  labelcounter++;
  fprintf(object, "\tjmp	.L%d\n",labelcounter);
  fprintf(object, ".L%d\n",labelcounter - 1);
  fprintf(object, "\tmovl	$0, %%eax\n");
  fprintf(object, ".L%d\n",labelcounter);
  fprintf(object, "\tmovl	%%eax, -4(%%rbp)\n");
  fprintf(object, "\tmovl	-4(%%rbp), %%eax\n");
  fprintf(object, "\tmovl	%%eax, %%esi\n");
  return 0;
}

int mulint(int VAR1,int VAR2)
{
  /*
  Assembly parcial de um codigo c,de uma simples operacao c = 7 * 8;
  pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$7, -12(%rbp)
	movl	$8, -8(%rbp)
	movl	-12(%rbp), %eax
	imull	-8(%rbp), %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
  */
  fprintf(object, "\tpushq	%%rbp\n");
  fprintf(object, "\tmovq	%%rsp, %%rbp\n");
  fprintf(object, "\tsubq	$16, %%rsp\n");
  fprintf(object, "\tmovl	$%d, -12(%%rbp)\n",VAR1);
  fprintf(object, "\tmovl	$%d, -8(%%rbp)\n",VAR2);
  fprintf(object, "\tmovl	-12(%%rbp), %%eax\n");
  fprintf(object, "\timull	-8(%%rbp), %%eax\n");
  fprintf(object, "\tmovl	%%eax, -4(%%rbp)\n");
  fprintf(object, "\tmovl	-4(%%rbp), %%eax\n");
  fprintf(object, "\tmovl	%%eax, %%esi\n");
  return 0;
}

int mulflt(float VAR1,float VAR2)
{
  /*
  Assembly parcial de um codigo c,de uma simples operacao c = 7 * 8, sendo 7 e 8 double
  pushq	%rbp
  movq	%rsp, %rbp
  subq	$16, %rsp
  movss	.LC0(%rip), %xmm0
  movss	%xmm0, -12(%rbp)
  movss	.LC1(%rip), %xmm0
  movss	%xmm0, -8(%rbp)
  movss	-12(%rbp), %xmm0
  mulss	-8(%rbp), %xmm0
  movss	%xmm0, -4(%rbp)
  cvtss2sd	-4(%rbp), %xmm0
  movl	$.LC2, %edi
  movl	$1, %eax
  .align 4
  .LC0:
  .long	1088421888
  .align 4
  .LC1:
  .long	1090519040
  */
  fprintf(object, "\tpushq	%%rbp\n");
  fprintf(object, "\tmovq	%%rsp, %%rbp\n");
  fprintf(object, "\tsubq	$16, %%rsp\n");
  labelcounter++;
  fprintf(object, "\tmovss	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovss	%%xmm0, -12(%%rbp)\n");
  labelcounter++;
  fprintf(object, "\tmovss	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovss	%%xmm0, -8(%%rbp)\n");
  fprintf(object, "\tmovss	-12(%%rbp), %%xmm0\n");
  fprintf(object, "\tmulss	-8(%%rbp), %%xmm0\n");
  fprintf(object, "\tmovss	%%xmm0, -4(%%rbp)\n");
  fprintf(object, "\tcvtss2sd	-4(%%rbp), %%xmm0\n");
  labelcounter++;
  fprintf(object, "\tmmovl	$.LC%d, %%edi\n",labelcounter);
  fprintf(object, "\tmovl	$1, %%eax\n");
  fprintf(object, "\t.align 4\n");
  fprintf(object, "LC%d:\n",labelcounter-3);
  //encaixar VAR1 e 2 \/
  fprintf(object, "\t.long	VAR1\n");
  fprintf(object, "\t.align 4n");
  fprintf(object, "LC%d:\n",labelcounter-2);
  fprintf(object, "\t.long	VAR2\n");
  return 0;
}

int muldbl(double VAR1,double VAR2)
{
  /*
  Assembly parcial de um codigo c,de uma simples operacao c = 7.31 * 8,91, sendo 7 e 8 double
  pushq	%rbp
	movq	%rsp, %rbp
	subq	$48, %rsp
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -16(%rbp)
	movsd	-24(%rbp), %xmm0
	mulsd	-16(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	.align 8
.LC0:
	.long	2748779069
	.long	1075658096
	.align 8
.LC1:
	.long	2233382994
	.long	1075958251
  */
  fprintf(object, "\tpushq	%%rbp\n");
  fprintf(object, "\tmovq	%%rsp, %%rbp\n");
  fprintf(object, "\tsubq	$48, %%rsp\n");
  labelcounter++;
  fprintf(object, "\tmovsd	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovsd	%%xmm0, -24(%%rbp)\n");
  labelcounter++;
  fprintf(object, "\tmovsd	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovsd	%%xmm0, -16(%%rbp)\n");
  fprintf(object, "\tmovsd	-24(%%rbp), %%xmm0\n");
  fprintf(object, "\tmulsd	-16(%%rbp), %%xmm0\n");
  fprintf(object, "\tmovsd	%%xmm0, -8(%%rbp)\n");
  fprintf(object, "\tmovq	-8(%%rbp), %%rax\n");
  fprintf(object, "\tmovq	%%rax, -40(%%rbp)\n");
  fprintf(object, "\tmovsd	-40(%%rbp), %%xmm0\n");
  fprintf(object, "\t.align 8\n");
  fprintf(object, "LC%d:\n",labelcounter-1);
  fprintf(object, "\t.long	VAR1_parteAlta\n");
  fprintf(object, "\t.long	VAR1_parteBaixa\n");
  fprintf(object, "\t.align 8\n");
  fprintf(object, "LC%d:\n",labelcounter);
  fprintf(object, "\t.long	VAR2_parteAlta\n");
  fprintf(object, "\t.long	VAR2_parteBaixa\n");
  return 0;
}

int divint(int VAR1,int VAR2)
{
  /*
  Assembly parcial de um codigo c,de uma simples operacao c = 4 / 2, sendo 4 e 2 int
  pushq	%rbp
  .cfi_def_cfa_offset 16
  .cfi_offset 6, -16
  movq	%rsp, %rbp
  .cfi_def_cfa_register 6
  subq	$16, %rsp
  movl	$4, -12(%rbp)
  movl	$2, -8(%rbp)
  movl	-12(%rbp), %eax
  cltd
  idivl	-8(%rbp)
  movl	%eax, -4(%rbp)
  movl	-4(%rbp), %eax
  movl	%eax, %esi
  */
  fprintf(object, "\tpushq	%%rbp\n");
  fprintf(object, "\tmovq	%%rsp, %%rbp\n");
  fprintf(object, "\tsubq	$16, %%rsp\n");
  fprintf(object, "\tmovl	$%d, -12(%%rbp)\n",VAR1);
  fprintf(object, "\tmovl	$%d, -8(%%rbp)\n",VAR2);
  fprintf(object, "\tmovl	-12(%%rbp), %%eax\n");
  fprintf(object, "\tcltd\n");
  fprintf(object, "\tidivl	-8(%%rbp)\n");
  fprintf(object, "\tmovl	%%eax, -4(%%rbp)\n");
  fprintf(object, "\tmovl	-4(%%rbp), %%eax\n");
  fprintf(object, "\tmovl	%%eax, %%esi\n");
  return 0;
}

int divflt(float VAR1,float VAR2)
{
  /*
  Assembly parcial de um codigo c,de uma simples operacao a = 15.321 e b = 2.1; c = a / b;
  pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -12(%rbp)
	movss	.LC1(%rip), %xmm0
	movss	%xmm0, -8(%rbp)
	movss	-12(%rbp), %xmm0
	divss	-8(%rbp), %xmm0
	movss	%xmm0, -4(%rbp)
	cvtss2sd	-4(%rbp), %xmm0
	movl	$.LC2, %edi
	.align 4
.LC0:
	.long	1098195665
	.align 4
.LC1:
	.long	1074161254
  */
  fprintf(object, "\tpushq	%%rbp\n");
  fprintf(object, "\tmovq	%%rsp, %%rbp\n");
  fprintf(object, "\tsubq	$16, %%rsp\n");
  labelcounter++;
  fprintf(object, "\tmovss	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovss	%%xmm0, -12(%%rbp)\n");
  labelcounter++;
  fprintf(object, "\tmovss	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovss	%%xmm0, -8(%%rbp)\n");
  fprintf(object, "\tmovss	-12(%%rbp), %%xmm0\n");
  fprintf(object, "\tdivss	-8(%%rbp), %%xmm0\n");
  fprintf(object, "\tmovss	%%xmm0, -4(%%rbp)\n");
  fprintf(object, "\tcvtss2sd	-4(%%rbp), %%xmm0\n");
  fprintf(object, "\t.align 4\n");
  fprintf(object, "\t.LC%d\n",labelcounter-1);
  fprintf(object, "\t.long	VAR1\n");
  fprintf(object, "\t.align 4\n");
  fprintf(object, "\t.LC%d\n",labelcounter);
  fprintf(object, "\t.long	VAR1\n");
  return 0;
}

int divdbl(double VAR1,double VAR2)
{

  /*
  pushq	%rbp
	movq	%rsp, %rbp
	subq	$48, %rsp
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -16(%rbp)
	movsd	-24(%rbp), %xmm0
	divsd	-16(%rbp), %xmm0
	movsd	%xmm0, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	movl	$.LC2, %edi
	.align 8
.LC0:
	.long	1133871366
	.long	1079825547
	.align 8
.LC1:
	.long	3402988488
	.long	1077223300
  */
  fprintf(object, "\tpushq	%%rbp\n");
  fprintf(object, "\tmovq	%%rsp, %%rbp\n");
  fprintf(object, "\tsubq	$48, %%rsp\n");
  labelcounter++;
  fprintf(object, "\tmovsd	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovsd	%%xmm0, -24(%%rbp)\n");
  labelcounter++;
  fprintf(object, "\tmovsd	.LC%d(%%rip), %%xmm0\n",labelcounter);
  fprintf(object, "\tmovsd	%%xmm0, -16(%%rbp)\n");
  fprintf(object, "\tmovsd	-24(%%rbp), %%xmm0\n");
  fprintf(object, "\tdivsd	-16(%%rbp), %%xmm0\n");
  fprintf(object, "\tmovsd	%%xmm0, -8(%%rbp)\n");
  fprintf(object, "\tmovq	-8(%%rbp), %%rax\n");
  fprintf(object, "\tmovq	%%rax, -40(%%rbp)\n");
  fprintf(object, "\tmovsd	-40(%%rbp), %%xmm0\n");
  fprintf(object, "\t.align 8\n");
  fprintf(object, "\t.LC0:\n");
  fprintf(object, "\t.long	VAR1_parteAlta\n");
  fprintf(object, "\t.long	VAR1_parteBaixa\n");
  fprintf(object, "\t.align 8\n");
  fprintf(object, "\t.LC1:\n");
  fprintf(object, "\t.long	VAR2_parteAlta\n");
  fprintf(object, "\t.long	VAR2_parteBaixa\n");
  return 0;
}
