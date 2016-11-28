/**@<symtab.c>::**/
#include <string.h>
#include <symtab.h>
#include <parser.h>
#include <lexer.h>

// col 0 of symtab: location of the symbol name(symtab[i][1]) in the symtab_stream
// col 1 of symtab: the type of the symbol
int symtab[MAX_SYMTAB_ENTRIES][2];
int symtab_nextentry = 0; // position of next entry in symtab

// symtab_stream: array with all the declared symbols
// the right position of a symbol in the symtab_stream is stored in symtab
char symtab_stream[MAX_SYMTAB_ENTRIES*(MAXID_SIZE+1)];
// position of next item in the symtab_stream
int symtab_stream_next_descriptor = 0;

int symtab_lookup(char const *name)
{

  int i;
  for(i = symtab_nextentry - 1; i > -1; i--){
   if( strcmp(symtab_stream + symtab[i][0],name) == 0)
    break;
  }
  // at this point 'i' is either the value we want or -1, when didn't find
  return i;
}

int symtab_append(char const *name, int type)
{
  int location = symtab_lookup(name);

  if(symtab_nextentry == MAX_SYMTAB_ENTRIES)
    return -2; // no more space in symtab
  if(location > -1 )
    return -3; // 'name' does not exist in symtab

  strcpy(symtab_stream + symtab_stream_next_descriptor, name);

  // stroe the stream position in the symtab array
  symtab[symtab_nextentry][0] = symtab_stream_next_descriptor;
  // preview next stream entry position
  symtab_stream_next_descriptor += strlen(name) +1;

  return symtab_nextentry++;
}
