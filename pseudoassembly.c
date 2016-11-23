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
  fprintf(object, "\tjz .L%d\n", label);
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

int addlog(void)
{
  return 0;
}

int addint(void)
{
/*
  addl %eax, (%rsp)
  popl %eax
 */
 fprintf(object, "\taddl %%eax, (%%esp)\n");
 fprintf(object, "\tpopl %%eax\n\n");
 return 0;
}

int addflt(void)
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
  fprintf(object, "\taddss xmm1, %%xmm0\n");
  fprintf(object, "\tmovss %%xmm0, x(%%rip)\n");
  fprintf(object, "\taddl $4, %%esp\n");
  return 0;
}

int adddbl(void)
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
  fprintf(object, "\taddsd %%xmm1, xmm0\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8, %%rsp\n");
  return 0;
}

int subint(void)
{
  return 0;
}

int subflt(void)
{
  return 0;
}

int subdbl(void)
{
  return 0;
}

/*binary multiplication and inverse*/

int mullog(void)
{
  return 0;
}

int mulint(void)
{
  return 0;
}

int mulflt(void)
{
  return 0;
}

int muldbl(void)
{
  return 0;
}

int divint(void)
{
  return 0;
}

int divflt(void)
{
  return 0;
}

int divdbl(void)
{
  return 0;
}
