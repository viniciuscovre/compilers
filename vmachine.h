/**@<vmachine.h>::**/

#define         MAXSYMTAB_ENTRIES       0x10000
#define         MAXSTACK_SIZE           0x40

extern char symtab[MAXSYMTAB_ENTRIES][MAXID_SIZE+1];
extern int symtab_nextentry;
extern double vmem[MAXSYMTAB_ENTRIES];
extern double stack[MAXSTACK_SIZE];
extern int sp;
extern double acc;

void accpush(void);
void accpop(void);
void cp2acc(double constant);
void recall(char const *symbol);
int symtab_lookup(char const *symbol);//@ vmachine.c
void store(char const *symbol);
void operationlib(int op);