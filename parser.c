/********************************** Recursive LL(1) Pareser *****************************************
*
* Method: assign nonterminal symbols to C-function names
*
* LL(1) grammars have not left recursion:
*
* productions like A =>* A alpha are not allowed.
*
* Left recursive grammars must be renormalized to Greibach normal form
* or at least productions of kind A =>* B alpha, where B not equal to A.
*
* For example: E -> E + T | T could be written as
*
* E -> T R, R-> + T R | <>
*
* Thus, the calculator language becomes:
*
* expr -> term rest
*
* rest -> addop term rest | <>
*
* term -> fact quoc
*
* quoc -> mulop fact quoc | <>
*
* fact -> variable | constant | ( expr )
*
* addop -> + | -
*
* mulop -> * | /
*
* variable -> ID
*
* constant -> DEC | OCT | HEX | FLT
*/

/* system include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* local include */
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <vmachine.h>

/*
* mypas -> expr { cmdsep expr } <eof>
*
* cmdsep -> ';' | '\n'
*/
int iscmdsep(void)
{
  switch(lookahead){
    case ';':case'\n':
    match(lookahead);return 1;
  }
  return 0;
}

void mypas(void)
{
  expr();/*[[*/printf("%g\n", acc)/*]]*/;

  while ( iscmdsep() ) {
    if(lookahead!=-1)
    {
      expr();
      /*[[*/printf("%g\n", acc)/*]]*/;
    }
  }
  match(EOF);
}

/* expr -> term { addop [[<enter>]] term [[ print addop.pf ]] } */
void expr (void)
{/*[[*/int opsymbol;/*]]*/
  term();
  while((opsymbol=addop())) {
    /*[[*/accpush()/*]]*/;
    term();
    /*[[*/operationlib(opsymbol)/*]]*/;
  }
}

/* term -> fact { mulop fact [[ print mulop.pf ]] } */
void term (void)
{/*[[*/int opsymbol/*]]*/;
  fact();
  while((opsymbol=mulop())) {
    /*[[*/accpush()/*]]*/;
    fact();
    /*[[*/operationlib(opsymbol)/*]]*/;
  }
}
/* arith ->  NUMERO arithmetic_op NUMERO , no caso numero sera DEC inicialmente, deve evoluir pra todos os tipos*/
void arith (void)
{/*[[*/int opsymbol/*]]*/;
  fact();
  while((opsymbol=arithmetic_op())) {
    /*[[*/accpush()/*]]*/;
    fact();
    /*[[*/operationlib(opsymbol)/*]]*/;
  }
}

/* fact -> variable | constant | ( expr ) */
void fact (void)
{
  switch (lookahead) {
    case ID:
      variable(); break;
    case '(':
      match('('); expr(); match(')');break;
    case '>':
      match('>'); arith(); match(')');break;
    default:
      constant();
  }
}

/* addop -> + | - */
int addop (void)
{
  switch (lookahead)
  {
    case '+':
    match('+'); return '+';
    case '-':
    match('-'); return '-';
  }
  return 0;
}
/* mulop -> * | / */
int mulop (void)
{
  switch (lookahead)
  {
    case '*':
    match('*'); return '*';
    case '/':
    match('/'); return '/';
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
/* constant -> [[ print DEC ]] DEC
*           | [[ print OCT ]] OCT
*           | [[ print HEX ]] HEX
*           | [[ print FLT ]] FLT */
void constant (void)
{
  /*[[*/cp2acc(atof(lexeme))/*]]*/;match(DEC);
}

/* variable -> [[ print ID ]] ID */
void variable (void)
{/*[[*/char varname[MAXID_SIZE]/*]]*/;
  /*[[*/strcpy(varname, lexeme)/*]]*/;
  match(ID);
  if (lookahead == '=') {// L-VALUE:
    match('=');
    expr();
    /*[[*/store(varname)/*]]*/;
  } else { // R-VALUE:
    /*[[*/recall(varname)/*]]*/;
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
    fprintf (stderr, "parser: token mismatch error.\n");
    fprintf (stderr, "expecting %d but seen %d... exting\n",
    expected_token, lookahead);
    exit (SYNTAX_ERR);
  }
}
