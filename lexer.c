/**@<lexer.c>::**/

/*

TODO: ALTERAR TODOS OS METODOS PARA UTILIZAR O LEXEME, PARA O PARSER.C IMPRIMIR O VALOR

TODO: ADICIONAR CODIGO NO IS_FLOAT

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <lexer.h>

void skipspaces (FILE *dish)
{
  int cake;

  while ( isspace ( cake = getc (dish) ) );

  ungetc ( cake, dish );
}

char lexeme[MAXID_SIZE+1];//@ lexer.c

int is_identifier(FILE *dish)
{
  int i = 0;
  lexeme[i] = getc(dish);
  if (isalpha (lexeme[i]) ) {
    for (i++; isalnum (lexeme[i] = getc(dish)); i++);
    ungetc (lexeme[i], dish);
    lexeme[i] = 0;
    return ID;
  }
  if (lexeme[MAXID_SIZE] != 0)
  lexeme[MAXID_SIZE] = 0;
  ungetc (lexeme[i], dish);
  return 0;
}

int is_decimal(FILE *dish)
{
  int i = 0;
  if (isdigit (lexeme[0] = getc(dish))) {
    if (lexeme[0] == '0') {
      lexeme[1] = 0;
      return DEC;
    }
    // [0-9]*
    for (i=1; isdigit (lexeme[i] = getc(dish)); i++);
    ungetc (lexeme[i], dish);
    lexeme[i] = 0;
    return DEC;
  }
  ungetc (lexeme[0], dish);
  return 0;
}

int is_octal(FILE *dish)
{
  int octpref = getc(dish);
  if (octpref == '0') {
    int cake = getc(dish);
    if ( cake >= '0' && cake <= '7') {
      while ( (cake = getc(dish)) >= '0' && cake <= '7');
      ungetc (cake, dish);
      return OCTAL;
    } else {
      ungetc (cake, dish);
      ungetc (octpref, dish);
      return 0;
    }
  }
  ungetc (octpref, dish);
  return 0;
}

int is_hexadecimal(FILE *dish)
{
  int head = getc(dish);
  int head_aux = head;
  if (head == '0') {
    head = getc(dish);

    if (head =='x'){

      if ( ((head = getc(dish)) >= '0' && head <= '9') || (head >= 'a' && head <= 'f') || (head >= 'A' && head <= 'F') ) {

        while ( ((head = getc(dish)) >= '0' && head <= '9') || (head >= 'a' && head <= 'f') || (head >= 'A' && head <= 'F') );

        if((head = getc(dish)!=EOF))
        return 0;
        else {
          ungetc(head,dish);
          return HEX;
        }

      } else {
        ungetc(head,dish);
      }

    } else{
      ungetc(head,dish);
      ungetc(head_aux,dish);
    }

  } else {
    ungetc(head,dish);
  }

  return 0;
}

/* int is_float(FILE *dish)
{

} */

int gettoken (FILE *tokenstream)
{
  int token;
  skipspaces (tokenstream);

  token = is_hexadecimal (tokenstream);
  if (token)
  return HEX;

  token = is_octal(tokenstream);
  if (token)
  return OCTAL;

  token = is_decimal (tokenstream);
  if (token)
  return DEC;

  token = is_identifier(tokenstream);
  if (token)
  return ID;

  token = getc (tokenstream);
  return token;
}
