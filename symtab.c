/**@<symtab.c>::**/


#include <string.h>
#include <symtab.h>
#include <parser.h>
#include <lexer.h>

int symtab[MAX_SYMTAB_ENTRIES][2];
int symtab_nextentry = 0;

char symtab_stream[MAX_SYMTAB_ENTRIES*(MAXID_SIZE+1)];
int symtab_stream_next_descriptor = 0;

int symtab_lookup(char const *name)
{

  int i;
  for(i=symtab_nextentry-1;i>-1;i--){
   if( strcmp(symtab_stream + symtab[i][0],name)==0){
    break;
   }
  }
  return i;// at this point 'i' is either the value that we want, or -1 that is not found.
}

int symtab_append(char const *name, int type)
{
  int location = symtab_lookup(name);

  if(symtab_nextentry == MAX_SYMTAB_ENTRIES) 
    return -2; //symtab cheio
  if(location > -1 )
    return -3; // 'name' existe dentro do symtab

  strcpy(symtab_stream + symtab_stream_next_descriptor, name);


  /* stroe the stream position in the symtab array*/
  symtab[symtab_nextentry][0]=symtab_stream_next_descriptor;
  /* preview next stream entry position */
  symtab_stream_next_descriptor += strlen(name) +1;

  return symtab_nextentry++;


}
