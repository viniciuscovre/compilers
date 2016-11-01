/**@<lexer.c>::**/

/*

Vina - Modificado 31 de Outubro de 2016

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <lexer.h>
#include <keywords.c>

void skipspaces (FILE *dish)
{
  int cake;

  while ( isspace ( cake = getc (dish) ) && cake != '\n' );
  // while ( isblank ( cake = getc (dish) ) );
  ungetc ( cake, dish );
}

char lexeme[MAXID_SIZE+1];//@ lexer.c

int is_assign(FILE * tape){

  if((lexeme[0] = getc(tape)) == ':'){
    if((lexeme[1] = getc(tape)) == '='){
      lexeme[2] = 0;
      return ASGN;
    }
    ungetc(lexeme[1], tape);

  }
  ungetc(lexeme[0], tape);
  return 0;
}

// ID = [A-Za-z][A-Za-z0-9]*
int is_identifier(FILE *tape)
{
  int token = 0;
  lexeme[token] = getc(tape);
  if (isalpha (lexeme[token]) ) {
    for(token = 1; isalnum(lexeme[token] = getc(tape)); token ++) {
      if(token == MAXID_SIZE)
        break;
    }
    ungetc (lexeme[token], tape);
    lexeme[token] = 0;

    if((token = iskeyword(lexeme)))
      return token;

    return ID;
  }
  ungetc (lexeme[token], tape);
  return 0;
}

// DEC = [1-9][0-9]* | 0
int is_decimal(FILE *dish)
{
  int i = 0;
  if (isdigit (lexeme[i] = getc(dish))) {
    if (lexeme[i] == '0') {
      if( (lexeme[++i] = getc(dish)) == '0' || lexeme[i] == EOF || lexeme[i] == EOL ) {
        // ungetc (lexeme[i], dish);
        return DEC;
      } else if (lexeme[i] == '.' || tolower(lexeme[i]) == 'e') {
        //for later float verification ~vina
        ungetc (lexeme[i], dish);
        lexeme[i] = 0;
        return DEC;
      } else {
        ungetc (lexeme[i], dish);
        ungetc (lexeme[i-1], dish);
        return 0;
      }
    }
    // [0-9]*
    for (i=1; isdigit (lexeme[i] = getc(dish)); i++);
    // if(lexeme[i] == '.' || tolower(lexeme[i]) == 'e') {
    // //for later float verification ~vina
    //   ungetc (lexeme[i], dish);
    //   lexeme[i] = 0;
    //   return DEC;
    // }
    ungetc (lexeme[i], dish);
    lexeme[i] = 0;
    return DEC;
  }
  ungetc (lexeme[i], dish);
  return 0;
}

// OCTAL =  0[1-7][0-7]*
int is_octal(FILE *dish)
{
  int i = 0;
  lexeme[i] = getc(dish);
  if (lexeme[i] == '0') {
    lexeme[++i] = getc(dish);
    if ( lexeme[i] >= '1' && lexeme[i] <= '7') {
      while ( (lexeme[++i] = getc(dish)) >= '0' && lexeme[i] <= '7');
      ungetc (lexeme[i], dish);
      return OCTAL;
    } else {
      ungetc (lexeme[i], dish);
      ungetc (lexeme[i-1], dish);
      return 0;
    }
  }
  ungetc (lexeme[i], dish);
  return 0;
}

// HEX = 0[xX][0-9a-fA-F]+
int is_hexadecimal(FILE *dish)
{
  int i = 0;
  lexeme[i] = getc(dish);
  if (lexeme[i] == '0') {
    if ( tolower((lexeme[++i] = getc(dish))) == 'x'){
      lexeme[++i] = getc(dish);
      if ( isdigit(lexeme[i]) || (tolower(lexeme[i]) >= 'a' && tolower(lexeme[i]) <= 'f') ) {
        while ( isdigit((lexeme[++i] = getc(dish))) || (tolower(lexeme[i]) >= 'a' && tolower(lexeme[i]) <= 'f') );
        ungetc(lexeme[i],dish);
        return HEX;

      } else {
        ungetc(lexeme[i],dish);
        ungetc(lexeme[i-1],dish);
        ungetc(lexeme[i-2],dish);
        return 0;
      }

    } else{
      ungetc(lexeme[i],dish);
      ungetc(lexeme[i-1],dish);
      return 0;
    }

  } else {
    ungetc(lexeme[i],dish);
  }
  return 0;
}

/* FLOAT = ( DEC ‘.’ DIGIT* | ‘.’ DIGIT+ ) EXP? | DEC EXP

DIGIT = [0-9]
EXP =  ('E'|'e') (‘+’|‘-’)? DIGIT+  */
int is_float(FILE *tape) {

  int i;
  if (is_decimal(tape)) { //inicia com dec

    i = strlen(lexeme);
    lexeme[i] = getc(tape);

    if (lexeme[i] == '.'){
      for(i++; isdigit(lexeme[i] = getc(tape)); i++);
      ungetc(lexeme[i], tape);
      lexeme[i] = 0;
      return FLOAT;
    } //else if ()  //verificar se é EXP

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return DEC;
  }

  lexeme[0] = getc(tape);
  if (lexeme[0] == '.') {

    i = 1;
    if (isdigit(lexeme[i] = getc(tape))) {
      for(i++; isdigit(lexeme[i] = getc(tape)); i++);
      ungetc(lexeme[i], tape);
      lexeme[i] = 0;
      return FLOAT;
    }
    ungetc(lexeme[1], tape);
  }
  ungetc(lexeme[0], tape);
  return 0;
}

int gettoken (FILE *tokenstream)
{
  int token;
  skipspaces (tokenstream);

  token = is_assign(tokenstream);
  if (token) return ASGN;

  token = is_identifier(tokenstream);
  if (token) return ID;

  token = is_float(tokenstream);
  if (token) return FLOAT;

  // token = is_decimal (tokenstream);
  // if (token) return DEC;

  token = is_octal(tokenstream);
  if (token) return OCTAL;

  token = is_hexadecimal (tokenstream);
  if (token) return HEX;

  token = getc (tokenstream);
  return token;
}
