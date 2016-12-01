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

int jle(int label){
  fprintf(object, "\tjle .L%d\n", label);
  return 0;
}

int jlt(int label){
  fprintf(object, "\tjlt .L%d\n", label);
  return 0;
}

int jge(int label){
  fprintf(object, "\tjge .L%d\n", label);
  return 0;
}

int jgt(int label){
  fprintf(object, "\tjgt .L%d\n", label);
  return 0;
}

int jeq(int label){
  fprintf(object, "\tjeq .L%d\n", label);
  return 0;
}

int jne(int label){
  fprintf(object, "\tjne .L%d\n", label);
  return 0;
}

int cmpl() {
  fprintf(object, "\tcmpl (%%esp), %%eax\n");
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
  fprintf(object, "\tnot %%eax\n");
  return 0;
}

int negint(void)
{
  fprintf(object, "\tnot %%eax\n");
  return 0;
}

int negflt(void)
{
  fprintf(object, "\taddss %%xmm1, %%xmm0\n");
  fprintf(object, "\tmovss %%xmm0, x(%%rip)\n");
  fprintf(object, "\tnot %%esp\n");
  return 0;
}

int negdbl(void)
{
  fprintf(object, "\taddsd %%xmm1, %%xmm0\n");
  fprintf(object, "\tmovsd %%xmm0, x(%%rip)\n");
  fprintf(object, "\tnot %%esp\n");
  return 0;
}

/*binary addition and inversion*/

/*dbl and flt functions expects the value in the last label
then move the value from the label to register
then apply operation (value on register with value on top of stack)
  int functions expects value in register then just apply operation
*/

int addlog(void)
{
  fprintf(object, "\tor %%eax, (%%esp)\n");
  fprintf(object, "\tpopl %%eax\n");
  return 0;
}

int addint(void)
{
  fprintf(object, "\tmovl (%%rsp), %%ebx\n");
  fprintf(object, "\tmovl %%rax, %%eax\n");  
  fprintf(object, "\taddl %%eax, (%%esp)\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
 return 0;
}

int addflt(void)
{
  fprintf(object, "\tmovss (%%rsp), %%xmm0\n");
  fprintf(object, "\tmovss %%rax, %%xmm1\n");
  fprintf(object, "\taddss %%xmm1, %%xmm0\n");
  fprintf(object, "\tmovss %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}

int adddbl(void)
{
  fprintf(object, "\tmovsd (%%rsp), %%xmm0\n");
  fprintf(object, "\tmovsd %%rax, %%xmm1\n");
  fprintf(object, "\tmulsd %%rbp, %%xmm0\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}

int subint(void)
{
  fprintf(object, "\tmovl (%%rsp), %%ebx\n");
  fprintf(object, "\tmovl %%rax, %%eax\n");  
  fprintf(object, "\tsubl %%esp,%%eax\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}

int subflt(void)
{
  fprintf(object, "\tmovsd (%%rsp), %%xmm0\n");
  fprintf(object, "\tmovsd %%rax, %%xmm1\n"); 
  fprintf(object, "\tsubss %%rbp, %%xmm0\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}
int subdbl(void)
{
  fprintf(object, "\tmovsd (%%rsp), %%xmm0\n");
  fprintf(object, "\tmovsd %%rax, %%xmm1\n");  
  fprintf(object, "\tsubsd %%rbp, %%xmm0\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}

/*binary multiplication and inverse*/

int mullog(void)
{
  fprintf(object, "\tand %%eax, (%%esp)\n");
  fprintf(object, "\tpopl %%eax\n");
  return 0;
}

int mulint(void)
{
  fprintf(object, "\tmovl (%%rsp), %%ebx\n");
  fprintf(object, "\tmovl %%rax, %%eax\n");  
  fprintf(object, "\timull %%esp, %%eax\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}

int mulflt(void)
{
  fprintf(object, "\tmovsd (%%rsp), %%xmm0\n");
  fprintf(object, "\tmovsd %%rax, %%xmm1\n");  
  fprintf(object, "\tmulss %%rbp), %%xmm0\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}

int muldbl(void)
{
  fprintf(object, "\tmovsd (%%rsp), %%xmm0\n");
  fprintf(object, "\tmovsd %%rax, %%xmm1\n");  
  fprintf(object, "\tmulsd %%rbp, %%xmm0\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
 
  return 0;
}

int divint(void)
{
  fprintf(object, "\tmovsd (%%rsp), %%xmm0\n");
  fprintf(object, "\tmovsd %%rax, %%xmm1\n");  
  fprintf(object, "\tcltd\n");
  fprintf(object, "\tidivl %%rbp\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}

int divflt(void)
{
  fprintf(object, "\tmovsd (%%rsp), %%xmm0\n");
  fprintf(object, "\tmovsd %%rax, %%xmm1\n");  
  fprintf(object, "\tdivss %%rbp, %%xmm0;\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}

int divdbl(void)
{
  fprintf(object, "\tmovsd (%%rsp), %%xmm0\n");
  fprintf(object, "\tmovsd %%rax, %%xmm1\n");  
  fprintf(object, "\tdivss %%rbp), %%xmm0;\n");
  fprintf(object, "\tmovsd %%xmm0, %%rax\n");
  fprintf(object, "\taddq $8,%%rsp\n");
  return 0;
}