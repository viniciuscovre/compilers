/********************** Recursive LL(1) Pareser **********************
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
#include <math.h>
/* local include */
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <vmachine.h>
#include <keywords.h>
#include <symtab.h>
#include <mypas.h>

#define MAX_ARG_NUM 1024

char **namelist(void);

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
*
* declarative ->[ VAR namelist : vartype ; { namelist : vartype } ]
* 		{ sbpmod sbpname parmdef  [ : fnctype ]; body }
*
* sbmod -> PROCEDURE | FUNCTION
*
* sbpname -> ID
*
* namelist -> ID { , ID }
*
* parmdef -> [( [VAR] namelist ':' { ';' [VAR] namelist ':' vartype }) ]
*
*
* imperative BEGIN stmtlist END
*
*/

// mypas -> prgbody '.'
void mypas(void)
{
  body();
  match('.');
  // match(EOF); // VERIFY IF REALLY NECESSARY ~vina
}

//prgbody -> declarative imperative
void body(void)
{
  declarative(); // simbolos serao declarados aqui
  imperative(); // simbolos serao usados aqui
}

/*declarative ->
* 		[
* 		  VAR namelist : vartype ;   ||
* 		  { namelist : vartype ;}
* 		]
* 		{ sbpmod sbpname parmdef  [ : fnctype ]; body }

*/
void declarative(void)
{
  /*
  * vardef -> VAR namelist ':' vartype ';' || vardef.symtab <-
  * 						forall symbol in namelist.name do
  *							symtab_append(symbol,vartype.type)
  *						end do
  *
  *
  */

  if(lookahead == VAR) {
    match(VAR);
    do {
      /*[[*/ int type , i /*]]*/;
      /*[[*/ char **namev = /*]]*/ namelist();
      match(':');
      /*[[*/ type =  /*]]*/ vartype();
      /*[[*/ for(i=0; namev[i]; i++) symtab_append(namev[i], type); /*]]*/
      match(';');
    } while(lookahead == ID);

  }

  while(lookahead == PROCEDURE || lookahead == FUNCTION) {
    match(lookahead);
    match(ID);
    parmdef();
    if(lookahead == ':') {
      match(':');
      fnctype();
      match(';');
      body();
      match(';');
    }
  }

}

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

// * parmdef -> [( [VAR] namelist ':' { ';' [VAR] namelist ':' vartype }) ]
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
  printf("antes de match begin: %d\n", lookahead);
  match(BEGIN);
  printf("\ndepois de match begin: %d\n", lookahead);
  stmtlist();
  match(END);
}

//namelist -> ID { , ID }
char **namelist(void)
{
  /*[[*/ char **symbolvec = (char **)calloc(MAX_ARG_NUM, sizeof(char **));
  int i = 0;/*]]*/
  _namelist_begin:
  /*[[*/ strcpy(symbolvec[i] = malloc(sizeof lexeme +1), lexeme); i++;/*]]*/
  match(ID);
  while(lookahead == ',') {
    match(',');
    goto _namelist_begin;
  }

  /*[[*/ return symbolvec /*]]*/;
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

    // see if FOR will be implemented (already in keywrods) ~vina
    case FOR:
      forstmt();
      break;

    case ID: /*hereafter we expect FIRST(expr):*/
      expr(ID);
      break;

    case DEC:
      expr(DEC);
      break;

    case  '(':
      expr(DEC); //DEC??
      break;
    /*                     | ""
    */

    default:
      /*<empty>*/
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
  expr(IF);
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
  expr(WHILE);
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
  expr(DEC); //??
}

//forstmt -> FOR variable ASGN expr (TO | DOWNTO) expr DO stmt
void forstmt(void)
{
  match(FOR);
  // printf("RECONHECE O FOR | IMPLEMENTAR AQUI /~vina");
  variable();
  match(ASGN);
  expr(DEC);

  // see if TO and DOWNTO will be implemented (already in keywrods) ~vina
  if(lookahead == TO)
    match(TO);
  else
    match(DOWNTO);

  expr(DEC);
  match(DO);
  stmt();
}

/* mypas -> expr { cmdsep expr } <eof> */
void mypas_old(void)
{
  expr(DEC);
  /*[[*/printf("%g\n", acc)/*]]*/;

  while ( iscmdsep() ) {
    if(lookahead!=-1) {
      expr(DEC);
      /*[[*/printf("%g\n", acc)/*]]*/;
    }
  }

  match(EOF);
}

/* expr -> term { addop [[<enter>]] term [[ print addop.pf ]] } */
/* expr -> ['-'] term { addop term } */ //como ta na do eraldo
/*
 * regras de checagem de tipos (e de herança de tipos)...
 *
 *  OP      |  BOOLEAN    |   NUMERIC
 * =====================================
 *  NOT     |     X       |     N/A
 *  OR      |     X       |     N/A
 *  AND     |     X       |     N/A
 *  NEG     |    N/A      |      X     // mudança de sinal
 *  '+''-'  |    N/A      |      X
 *  '*''/'  |    N/A      |      X
 *  DIV     |    N/A      |   INTEGER
 *  MOD     |    N/A      |   INTEGER
 *  RELOP   | BOOL x BOOL |  NUM x NUM
 *
 * _EXPRESSIONS_  ||  INTEGER   |     REAL    |   DOUBLE
 * ========================================================
 *  INTEGER       |   INTEGER   |     REAL    |   DOUBLE
 *  REAL          |    REAL     |     REAL    |   DOUBLE
 *  DOUBLE        |   DOUBLE    |    DOUBLE   |   DOUBLE
 *
 *
 *  _LVALUE_  || BOOLEAN | INTEGER |   REAL  |   DOUBLE
 * =======================================================
 *  BOOLEAN   |  BOOLEAN |   N/A   |   N/A   |    N/A
 *  INTEGER   |    N/A   | INTEGER |   N/A   |    N/A
 *  REAL      |    N/A   |   REAL  |   REAL  |    N/A
 *  DOUBLE    |    N/A   |  DOUBLE |  DOUBLE |   DOUBLE
 *
 *  TUDO ISSO TEM Q VIRAR IF DENTRO DA EXPRESSAO
 */
int expr(int inherited_type)
{
  /*[[*/int varlocality, lvalue = 0, acctype = inherited_type/*]]*/;

  if(lookahead == '-'){
    match('-');
    /*[[*/
    if(acctype == BOOLEAN) {
      fprintf(stderr, "incompatible unary operator: fatal error.\n");
    } else if (acctype == 0) {
      acctype = INTEGER;
    }
    /*]]*/
  } else if (lookahead == NOT){
    match(NOT);
    /*[[*/
    if(acctype > BOOLEAN) {
      fprintf(stderr, "incompatible unary operator: fatal error.\n");
    }
    acctype = BOOLEAN;
    /*]]*/
  }

  T_entry:
  F_entry:
    switch(lookahead){

      case ID:
        /*[[*/
        varlocality = symtab_lookup(lexeme);
        if(varlocality < 0) {
          fprintf(stderr, "parser: %s not declared... fatal error!\n",
            lexeme);
        }
        /*]]*/
        match(ID);
        if (lookahead == ASGN) {
          /* located variable is LVALUE */
          /*]]*/ lvalue = 1 /*]]*/;
          match(ASGN);
          expr(DEC);
        }
        /*[[*/
        else if(varlocality > -1) {
          fprintf(object, "\tpushl %%eax\n\tmovl %s,%%eax\n",
            symtab_stream + symtab[varlocality][0]);
        }
        /*]]*/
        break;

      case FLOAT:
        match(FLOAT);
        break;

      case DEC:
        match(DEC);
        break;

      default:
        match('(');
        expr(DEC);
        match(')');
    }

    if(mulop())
      goto F_entry;
    if(addop())
      goto T_entry;
    /* expression ends down here */
    /*[[*/
    if(lvalue && varlocality > -1) {
      fprintf(object, "\tmovl %%eax,%s\n",symtab_stream + symtab[varlocality][0]);
    }
    /*]]*/
}

/* expr -> term { addop [[<enter>]] term [[ print addop.pf ]] } */
// void expr (void)
// {/*[[*/int opsymbol;/*]]*/
//   term();
//   while((opsymbol=addop())) {
//     /*[[*/accpush()/*]]*/;
//     term();
//     /*[[*/operationlib(opsymbol)/*]]*/;
//   }
// }

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

/* fact -> variable | constant | ( expr )

variable -> ID
constant -> DEC */
void fact (void)
{
  switch (lookahead) {

    case ID:
    variable(); break;

    case '(':
    match('('); expr(DEC); match(')');break;

    case '>':
    match('>'); arith(); match(')');break;

    default:
    constant();
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
/* constant -> [[ print DEC ]] DEC
*           | [[ print OCT ]] OCT
*           | [[ print HEX ]] HEX
*           | [[ print FLT ]] FLT */
void constant (void)
{
  int value = -1; //starts with -1 to detect a possible further error
  switch(lookahead) {

    // case DEC:
    // /*[[*/cp2acc(atof(lexeme))/*]]*/;
    // match(DEC);
    // // /*[[*/printf("decimal: ")/*]]*/;
    // break;

    case FLOAT:
      /*[[*/cp2acc(atof(lexeme))/*]]*/;
      match(FLOAT);
      // /*[[*/printf("float: ")/*]]*/;
      break;

    case OCTAL:
      value = octalToInt(lexeme);
      /*[[*/cp2acc((float)value)/*]]*/;
      match(OCTAL);
      // /*[[*/printf("octal value in decimal: ")/*]]*/;
      break;

    case HEX:
      value = hexToInt(lexeme);
      /*[[*/cp2acc((float)value)/*]]*/;
      match(HEX);
      // /*[[*/printf("hexadecimal value in decimal: ")/*]]*/;
      break;

    /* VRIFY DEFAULT CASE ~vina */
  }
}

/* variable -> [[ print ID ]] ID */
void variable (void)
{
  /* symbol must be declared */
  if(symtab_lookup(lexeme) == -1) {
    exit(-1);
  }

  /*[[*/char varname[MAXID_SIZE]/*]]*/;
  /*[[*/strcpy(varname, lexeme)/*]]*/;
  match(ID);
  if (lookahead == ASGN) {// L-VALUE:
    match(ASGN); // ASGN = ':='
    expr(DEC);
    /*[[*/store(varname)/*]]*/;
  } else { // R-VALUE:
    /*[[*/recall(varname)/*]]*/;
  }
}

int octalToInt(char octalToConvert[])
{

  int n = atoi(octalToConvert);
  int octal = 0, i = 0, rem;
  while (n!=0)
  {
    rem = n%10;
    n/=10;
    octal += rem*pow(8,i);
    ++i;
  }
  return octal;
}

int hexToInt(char hexToConvert[])
{
  int j;
  int len = strlen(hexToConvert);
  char aux[len-2];

  for(j = 2; j < len; j++) //removing 2 prefixes (0x) of hexadecimal ~vina
  {
    aux[j-2] = hexToConvert[j];
  }
  aux[(len-2)-1] = 0; //closing string with '/0' ~vina


  int hex = (int)strtol(aux, NULL, 16); //hex --> decimal ~vina
  return hex;
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
