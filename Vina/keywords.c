#include <keywords.h>
#include <string.h>

char *keywords[] = {"begin","if","then","else","while","for","do","to",
"downto","repeat","until","var","procedure","function","integer","real",
"boolean","div","mod","and", "or","not", "end"};


int iskeyword(const char *identifier)
{
  // int token;
  printf("%lu", sizeof(keywords)/sizeof(*keywords));
  int token = BEGIN;
  while(token<=END) {
      if(strcmp(keywords[END-token],identifier) == 0)
        return token;
      token++;
  }
  // for(token = BEGIN;token <= END;token++) {
  //   if(strcmp(keywords[END-token],identifier) == 0)
  //     return token;
  // }
  return 0;
}
