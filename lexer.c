/**@<lexer.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <keywords.h>
#include <lexer.h>

void skipspaces (FILE *tape)
{
  int cake;

  while ( isspace ( cake = getc (tape) ) && cake != (' ' | '\n' | '\t') );
  ungetc ( cake, tape );
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

    token = iskeyword(lexeme);
    if(token)
      return token;

    return ID;
  }
  ungetc (lexeme[token], tape);
  return 0;
}

// DEC = [1-9][0-9]* | 0
int is_decimal(FILE *tape)
{
  int i = 0;
  if (isdigit (lexeme[i] = getc(tape))) {
    if (lexeme[i] == '0') {
      if( (lexeme[++i] = getc(tape)) == '0' || lexeme[i] == EOF || lexeme[i] == EOL ) {
        // ungetc (lexeme[i], tape);
        return INTCONST;
      } else if (lexeme[i] == '.' || tolower(lexeme[i]) == 'e') {
        //for later float verification ~vina
        ungetc (lexeme[i], tape);
        lexeme[i] = 0;
        return INTCONST;
      } else {
        ungetc (lexeme[i], tape);
        ungetc (lexeme[i-1], tape);
        return 0;
      }
    }
    // [0-9]*
    for (i=1; isdigit (lexeme[i] = getc(tape)); i++);
    // if(lexeme[i] == '.' || tolower(lexeme[i]) == 'e') {
    // //for later float verification ~vina
    //   ungetc (lexeme[i], tape);
    //   lexeme[i] = 0;
    //   return DEC;
    // }
    ungetc (lexeme[i], tape);
    lexeme[i] = 0;
    return INTCONST;
  }
  ungetc (lexeme[i], tape);
  return 0;
}

// OCTAL =  0[1-7][0-7]*
int is_octal(FILE *tape)
{
  int i = 0;
  lexeme[i] = getc(tape);
  if (lexeme[i] == '0') {
    lexeme[++i] = getc(tape);
    if ( lexeme[i] >= '1' && lexeme[i] <= '7') {
      while ( (lexeme[++i] = getc(tape)) >= '0' && lexeme[i] <= '7');
      ungetc (lexeme[i], tape);
      return OCTAL;
    } else {
      ungetc (lexeme[i], tape);
      ungetc (lexeme[i-1], tape);
      return 0;
    }
  }
  ungetc (lexeme[i], tape);
  return 0;
}

// HEX = 0[xX][0-9a-fA-F]+
int is_hexadecimal(FILE *tape)
{
  int i = 0;
  lexeme[i] = getc(tape);
  if (lexeme[i] == '0') {
    if ( tolower((lexeme[++i] = getc(tape))) == 'x'){
      lexeme[++i] = getc(tape);
      if ( isdigit(lexeme[i]) || (tolower(lexeme[i]) >= 'a' && tolower(lexeme[i]) <= 'f') ) {
        while ( isdigit((lexeme[++i] = getc(tape))) || (tolower(lexeme[i]) >= 'a' && tolower(lexeme[i]) <= 'f') );
        ungetc(lexeme[i],tape);
        return HEX;

      } else {
        ungetc(lexeme[i],tape);
        ungetc(lexeme[i-1],tape);
        ungetc(lexeme[i-2],tape);
        return 0;
      }

    } else{
      ungetc(lexeme[i],tape);
      ungetc(lexeme[i-1],tape);
      return 0;
    }

  } else {
    ungetc(lexeme[i],tape);
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
      return FLTCONST;
    } else if (tolower(lexeme[i]) == 'e') {
      lexeme[++i] = getc(tape);

      if (isdigit(lexeme[i]) || lexeme[i] == '+' || lexeme[i] == '-') {
        lexeme[++i] = getc(tape);
        if (isdigit(lexeme[i])) {
          for(i++; isdigit(lexeme[i] = getc(tape)); i++);
          ungetc(lexeme[i], tape);
          lexeme[i] = 0;
          return FLTCONST;
        }
      }

      ungetc(lexeme[i], tape);
      lexeme[i] = 0;

      return 0;
    }

    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return INTCONST;
  }

  lexeme[0] = getc(tape);
  if (lexeme[0] == '.') {

    i = 1;
    if (isdigit(lexeme[i] = getc(tape))) {
      for(i++; isdigit(lexeme[i] = getc(tape)); i++);
      ungetc(lexeme[i], tape);
      lexeme[i] = 0;
      return FLTCONST;
    }
    ungetc(lexeme[1], tape);
  }
  ungetc(lexeme[0], tape);
  return 0;
}

// gettoken verifies token by token of the given input
int gettoken (FILE *tokenstream)
{
  int token;
  skipspaces (tokenstream);

  token = is_assign(tokenstream);
  if (token) return token;

  token = is_identifier(tokenstream);
  if (token) return token;

  token = is_decimal (tokenstream);
  if (token) return token;

  token = is_float(tokenstream);
  if (token) return token;

  token = is_octal(tokenstream);
  if (token) return token;

  token = is_hexadecimal (tokenstream);
  if (token) return token;

  token = getc (tokenstream);
  return token;
}
