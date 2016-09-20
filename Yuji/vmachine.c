/**@<vmachine.c>::**/
#include <lexer.h>
#include <vmachine.h>
#include <stdio.h>
#include <string.h>

char symtab[MAXSYMTAB_ENTRIES][MAXID_SIZE+1];
int symtab_nextentry = 0;
double vmem[MAXSYMTAB_ENTRIES];
double stack[MAXSTACK_SIZE];
int sp = -1;
double acc;

void accpush(void)
{
  sp++;
  stack[sp] = acc;
}

void accpop(void)
{
  acc = stack[sp];
  sp--;
}

void cp2acc(double constant)
{
  acc = constant;
}

/* Used when the entry is not an id...
Is about to recall the entry in order to validate id ~vina */
void recall(char const *symbol)
{
  int symtab_pos = symtab_lookup(symbol);

  if(symtab_pos == -1) { /* Place the symbol in the available position */
    strcpy(symtab[symtab_nextentry], symbol);
    symtab_pos = symtab_nextentry;
    symtab_nextentry++;
  }

  acc = vmem[symtab_pos];
}

/* Looks for the symbol in the table...
If it is in there, return the position
Otherwise, place it (done in the caller function, ie: recall) ~vina */
int symtab_lookup(char const *symbol)
{
  int i;
  for(i = 0; i < symtab_nextentry && strcmp(symtab[i], symbol); i++);
  if (i == symtab_nextentry) return -1; /* The symbol is not in the table. Needs to insert it ~vina */
  return i; /* Return the position where the symbol is ~vina */
}

void store(char const *symbol)
{
  int symtab_pos = symtab_lookup(symbol);

  if(symtab_pos == -1) {
    strcpy(symtab[symtab_nextentry], symbol);
    symtab_pos = symtab_nextentry;
    symtab_nextentry++;
  }

  vmem[symtab_pos] = acc;
}

void operationlib(int op)
{
  //int temp;
  switch(op) {
    case '+':
    acc += stack[sp];
    sp--;
    break;

    case '*':
    acc *= stack[sp];
    sp--;
    break;

    case '-':
    stack[sp] -= acc;
    accpop();
    break;

    case '/':
    stack[sp] /= acc;
    accpop();
    break;

    case '>':
    if (acc > stack[sp]) acc = 1;
    else acc = 0;
    break;

  }
}
