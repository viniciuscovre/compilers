/**@<symtab.h>::**/


#define MAX_SYMTAB_ENTRIES  32


extern int symtab_append(char const *name, int type);

extern int symtab_lookup(char const *name);
extern char symtab_stream[];
