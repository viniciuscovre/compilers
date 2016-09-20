/**@<lexer.c>::**/

/*

1: Tue Aug 16 20:49:40 BRT 2016

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tokens.h>
#include <lexer.h>

void skipspaces (FILE *dish)
{
  int cake;

  // while ( isspace ( cake = getc (dish) ) );
  while ( isblank ( cake = getc (dish) ) );
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
  ungetc (lexeme[i], dish);
  return 0;
}

// int i = 0; //making i global to use in is_float verification ~vina
// int is_decimal(FILE *dish)
// {
//   // int i;
//   if (isdigit (lexeme[0] = getc(dish)) ) {
//     if (lexeme[0] == '0') {
//       lexeme[1] = 0;
//       return DEC;
//     }
//     // [0-9]*
//     for(i=1; isdigit (lexeme[i] = getc(dish)); i++);
//     ungetc (lexeme[i], dish);
//     lexeme[i] = 0;
//     return DEC;
//   }
//   ungetc (lexeme[0], dish);
//   return 0;
// }

// int i = 0; //making i global to use in is_float verification ~vina
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
        return 0;
      } else {
        ungetc (lexeme[i], dish);
        ungetc (lexeme[i-1], dish);
        return 0;
      }
    }
    // [0-9]*
    for (i=1; isdigit (lexeme[i] = getc(dish)); i++);
    if(lexeme[i] == '.' || tolower(lexeme[i]) == 'e') { //for later float verification ~vina
      ungetc (lexeme[i], dish);
      return 0;
    }
    ungetc (lexeme[i], dish);
    lexeme[i] = 0;
    return DEC;
  }
  ungetc (lexeme[i], dish);
  return 0;
}

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

int is_hexadecimal(FILE *dish)
{
  int i = 0;
  lexeme[i] = getc(dish);
  if (lexeme[i] == '0') {
    if ( (lexeme[++i] = getc(dish)) == 'x'){
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

int is_float(FILE *dish)
{
  int i = 0;
  lexeme[i] = getc(dish);
  if (tolower(lexeme[i]) == 'e' || lexeme[i] == '.') {
    if (lexeme[i] == '.') {
      if ( isdigit((lexeme[++i] = getc(dish))) ) {
        while( isdigit(lexeme[++i]=getc(dish)) );
        if ( tolower(lexeme[i]) == 'e') { //VERIFYING EXP ~vina
          lexeme[++i] = getc(dish);
          // if(!isdigit(lexeme[i]) && lexeme[i] != '-' && lexeme[i] != '+' && lexeme[i] != EOF && lexeme[i] != EOL) {
          //   ungetc (lexeme[i], dish);
          //   return 0;
          // }
          if( lexeme[i] == '+' || lexeme[i] == '-' ) {
            lexeme[++i] = getc(dish);
            if (isdigit(lexeme[i])) {
              while( isdigit(lexeme[++i]=getc(dish)) );
              if (isdigit(lexeme[i]) || lexeme[i] == EOF || lexeme[i] == EOL) {
                ungetc (lexeme[i], dish);
                return FLOAT;
              }
              ungetc (lexeme[i], dish);
              return 0;
            }
            ungetc (lexeme[i], dish);
            return 0;
          }
          if (lexeme[i] == EOF || lexeme[i] == EOL) {
            ungetc (lexeme[i], dish);
            return FLOAT;
          }
          if(isdigit(lexeme[i])) {
            while( isdigit(lexeme[++i]=getc(dish)) );
            if (isdigit(lexeme[i]) || lexeme[i] == EOF || lexeme[i] == EOL) {
              ungetc (lexeme[i], dish);
              return FLOAT;
            }
            ungetc (lexeme[i], dish);
            return 0;
          }
          ungetc (lexeme[i], dish);
          return 0;
        } else if (isdigit(lexeme[i]) || lexeme[i] == EOF || lexeme[i] == EOL) {
          ungetc (lexeme[i], dish);
          return FLOAT;
        } else {
          ungetc (lexeme[i], dish);
          return 0;
        }
      }
      ungetc (lexeme[i], dish);
      return 0;
    } else { // (decimal ...) 'e' something ~vina
    lexeme[++i] = getc(dish);
    if(lexeme[i] == EOF || lexeme[i] == EOL) {
      ungetc(lexeme[i], dish);
      return FLOAT;
    }
    if( lexeme[i] == '+' || lexeme [i] == '-' || isdigit(lexeme[i]) ) {
      while( isdigit(lexeme[++i] = getc(dish)) );
      if (isdigit(lexeme[i-1])) {
        ungetc(lexeme[i], dish);
        return FLOAT;
      }
    }
    ungetc(lexeme[i], dish);
    return 0;
  }
}
ungetc (lexeme[i], dish);
return 0;
}

int gettoken (FILE *tokenstream)
{
  int token;
  skipspaces (tokenstream);

  token = is_identifier(tokenstream);
  if (token) return ID;

  token = is_decimal (tokenstream);
  if (token) return DEC;

  token = is_octal(tokenstream);
  if (token) return OCTAL;

  token = is_hexadecimal (tokenstream);
  if (token) return HEX;

  token = is_float(tokenstream);
  if (token) return token;

  token = getc (tokenstream);
  return token;
}
