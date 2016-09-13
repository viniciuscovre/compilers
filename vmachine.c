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

void recall(char const *symbol)
{
        int symtab_pos = symtab_lookup(symbol);

        if(symtab_pos == -1) {
                strcpy(symtab[symtab_nextentry], symbol);
                symtab_pos = symtab_nextentry;
                symtab_nextentry++;
        }

        acc = vmem[symtab_pos];
}

int symtab_lookup(char const *symbol)
{
    int i;
    for(i = 0; i < symtab_nextentry && strcmp(symtab[i], symbol); i++);
    if (i == symtab_nextentry) return -1;
    return i;
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
    }
}
