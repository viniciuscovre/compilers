#include <keywords.h>
#include <string.h>


// BEGIN, IF , THEN, ELSE, WHILE, DO, REPEAT, UNTIL,VAR, PROCEDURE, FUNCTION, INTEGER, REAL, BOOLEAN, DIV, MOD, AND, OR, NOT, END};
//colocar as palavras do keyword.h aqui
char *keywords[] = {"begin","if","then","else","while","do","repeat","until","var","procedure","function",
  "integer","real","boolean","div","mod","and", "or","not", "end"};


int iskeyword(char const *identifier)
{
  int token;
  for(token = BEGIN;token <= END;token++)
  {
    if(strcmp(keywords[token-IF],identifier) == 0)
      return token;
  }
  return 0;
}