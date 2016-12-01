#define MAX_SYMTAB_ENTRIES  32

extern int symtab[MAX_SYMTAB_ENTRIES][2];
extern int symtab_append(char const *name, int type);
void print_symtab_stream(void);

extern int symtab_lookup(char const *name);
extern char symtab_stream[];
