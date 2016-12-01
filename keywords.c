#include <string.h>
#include <keywords.h>

char *keywords[] = {
  "begin",
  "if",
  "then",
  "else",
  "while",
  "do",
  "repeat",
  "until",
  "var",
  "boolean",
  "integer",
  "real",
  "double",
  "div",
  "mod",
  "and",
  "or",
  "not",
  "true",
  "false",
  "end"};

int iskeyword(const char *identifier)
{
  int token;
  for(token = BEGIN;token < END+1;token++) {
    if(strcmp(keywords[token-BEGIN],identifier) == 0)
      return token;
  }
  return 0;
}
