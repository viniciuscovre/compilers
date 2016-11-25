/* system include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* local include */
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <keywords.h>
#include <mypas.h>

#define MAX_ARG_NUM 1024

void namelist(void);

/*
* cmdsep -> ';' | '\n'
*/
int iscmdsep(void)
{
  switch(lookahead){
    case ';': case '\n':
    match(lookahead);
    return 1;
  }
  return 0;
}

/*
*
* mypas -> prgbody '.'
*
* prgbody -> declarative imperative
*
**************************************************************************
*
* declarative ->[ vardef ] { sbpmod sbpname parmdef  [ : fnctype ]; body }
*
* sbmod -> PROCEDURE | FUNCTION
*
* sbpname -> ID
*
* namelist -> ID { , ID }
*
* vartype -> INTEGER | REAL | BOOLEAN
*
* fnctype -> INTEGER | REAL | BOOLEAN
*
* parmdef -> [( [VAR] namelist ':' { ';' [VAR] namelist ':' vartype }) ]
*
* vardef -> VAR namelist ':' vartype ';' { namelist ':' vartype ';'}
*           || vardef.symtab <- forall symbol in namelist.name do
*						     symtab_append(symbol,vartype.type)
*						   end do
*
* imperative BEGIN stmtlist END
*
*/

// mypas -> prgbody '.'
void mypas(void)
{
  body();
  match('.');
}

// prgbody -> declarative imperative
void body(void)
{
  declarative(); // symbols will be declared here
  imperative(); // symbols will be used here
}

// declarative ->[ vardef ] { sbpmod sbpname parmdef  [ : fnctype ]; body }
void declarative(void)
{
  if (lookahead == VAR) {
    match(VAR);
    do {
      namelist();
      match(':');
      vartype();
      match(';');
    } while(lookahead == ID);
  }
}

// fnctype -> INTEGER | REAL | BOOLEAN
void fnctype(void)
{
  switch(lookahead) {
    case INTEGER:
      match(INTEGER);
      break;

    case REAL:
      match(REAL);
      break;

    default:
      match(BOOLEAN);
  }
}

// parmdef -> [ '(' [VAR] namelist ':' { ';' [VAR] namelist ':' vartype } ')' ]
void parmdef(void)
{
  if(lookahead == '(') {
    match('(');
    if(lookahead == VAR){
      match(VAR);
    }
    namelist();
    match(':');
    while(lookahead == ';') {
      match(';');
      if(lookahead == VAR) {
        match(VAR);
      }
      namelist();
      match(':');
      vartype();
    }
  }
  match(')');
}

//namelist -> ID { , ID }
void namelist(void)
{
  match(ID);
  while(lookahead == ',') {
    match(',');
    match(ID);
  }
}

// vartype -> INTEGER | REAL | BOOLEAN
int vartype(void)
{
  switch(lookahead) {
    case INTEGER:
      match(INTEGER);
      return INTEGER;

    case REAL:
      match(REAL);
      return REAL;

    default:
      match(BOOLEAN);
      return BOOLEAN;
  }
}

// imperative BEGIN stmtlist END
void imperative(void)
{
  match(BEGIN);
  stmtlist();
  match(END);
}

//stmtlist -> stmt { ';' stmt }
void stmtlist(void)
{
  stmt();
  while (lookahead == ';') {
    match(';');
    stmt();
  }
}

/* stmt -> beginblock
          | ifstmt
          | whilestmt
          | repeatstmt
          | forstmt
          | fact
*/
void stmt(void)
{
  switch (lookahead) {
    case BEGIN:
      beginblock();
      break;

    case IF:
      ifstmt();
      break;

    case WHILE:
      whilestmt();
      break;

    case REPEAT:
      repeatstmt();
      break;

    case ID: //tokens.h
    case FLTCONST: //tokens.h
    case INTCONST: //tokens.h
    case TRUE: //keywords.h
    case FALSE: //keywords.h
    case NOT: //keywords.h
    case '-':
    case '(':
      expr();
      break;

    default:
      /*<epsilon>*/
      ;
  }
}

//beginblock -> BEGIN stmt { ; stmt } END
void beginblock(void)
{
  match(BEGIN);
  stmt();
  while(lookahead == ';') {
    match(';');
    stmt();
  }
  match(END);
}

//ifstmt -> IF expr THEN stmt [ ELSE stmt ] | other
void ifstmt(void)
{
  match(IF);
  expr();
  if(isrelop()) {
    expr();
  }
  match(THEN);
  stmt();
  if(lookahead == ELSE) {
    match(ELSE);
    stmt();
  }
}

//whilestmt -> WHILE expr DO stmt
void whilestmt(void)
{
  match(WHILE);
  expr();
  if(isrelop()) {
    expr();
  }
  match(DO);
  stmt();
}

//repeatstmt -> REPEAT stmt { ; stmt } UNTIL expr
void repeatstmt(void)
{
  match(REPEAT);
  stmt();
  while(lookahead == ';') {
    match(';');
    stmt();
  }
  match(UNTIL);
  expr();
  if(isrelop()) {
    expr();
  }
}

int iscompatible(int ltype, int rtype)
{
  switch(ltype) {

    case BOOLEAN: case INTEGER:
      if(rtype == ltype)
	      return ltype;
      break;

    case REAL:
      switch(rtype) {
	       case INTEGER: case REAL:
	         return ltype;
      }
      break;

    case DOUBLE:
      switch(rtype) {
	       case INTEGER: case REAL: case DOUBLE:
	         return ltype;
      }
  }
  return 0;
}

int isrelop(void)
{
  switch(lookahead) {
    case '>':
      match('>');
      if(lookahead == '=') {
	       match('=');
         return GEQ; //greater or equal
      }
      return '>';

    case '<':
      match('<');
      if(lookahead == '>') {
	       match('>');
         return NEQ; //not equal
      }
      if(lookahead == '=') {
	       match('=');
         return LEQ; //less or equal
      }
      return '<';

    case '=':
      match('=');
      return '=';
  }
  return 0;
}

int expr()
{
  if(lookahead == '-'){
    match('-');
  } else if (lookahead == NOT) {
    match(NOT);
  }

  T_entry:
  F_entry:
    switch(lookahead){

      case ID:
        match(ID);
        if (lookahead == ASGN) {
          match(ASGN);
          expr();
	      }
        break;

      case FLTCONST:
        match(FLTCONST);
        break;

      case INTCONST:
        match(INTCONST);
        break;

      default:
        match('(');
        expr();
        match(')');
    }

    if(mulop())
      goto F_entry;
    if(addop())
      goto T_entry;
    return 0;
}

/* addop -> + | - | OR */
int addop (void)
{
  switch (lookahead)
  {
    case '+':
      match('+');
      return '+';

    case '-':
      match('-');
      return '-';

    case OR:
      match(OR);
      return OR;
  }
  return 0;
}

/* mulop -> * | / | AND */
int mulop (void)
{
  switch (lookahead)
  {
    case '*':
      match('*');
      return '*';

    case '/':
      match('/');
      return '/';

    case AND:
      match(AND);
      return AND;
  }
  return 0;
}

/* arithmetic_op -> > | < */
int arithmetic_op (void)
{
  switch (lookahead)
  {
    case '>':
    match('>'); return '>';
    case '<':
    match('<'); return '<';
  }
  return 0;
}

/* variable -> [[ print ID ]] ID */
void variable (void)
{
  /* symbol must be declared */
  match(ID);
  if (lookahead == ASGN) {
    match(ASGN); // ASGN = ':='
    expr();
  }
}

/******************************* lexer-to-parser interface *****************************************/

int lookahead; /** @ parser **/

extern int gettoken (FILE *); /** @ lexer **/

void match (int expected_token)
{
  if (expected_token == lookahead) {
    lookahead = gettoken (source_code);
  } else {
    fprintf (stderr, "\nparser: token mismatch error.\n");
    fprintf (stderr, "expecting %d but seen %d. Exting...\n",
    expected_token, lookahead);
    exit (SYNTAX_ERR);
  }
}
