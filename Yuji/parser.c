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
#include <math.h>

/* local include */
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <vmachine.h>
#include <keywords.h>
#include <symtab.h>


#define MAX_ARG_NUM 1024

char **namelist(void);

/*
* cmdsep -> ';' | '\n'
*/
int iscmdsep(void)
{
  switch(lookahead){
    case ';': case '\n':
    match(lookahead); return 1;
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
 * 
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
void mypas(void){
  body();
  match('.');
}

//prgbody -> declarative imperative
void body(void){
  declarative();
  imperative();
}

/*declarative ->
 * 		[ 
 * 		  VAR namelist : vartype ;   || 
 * 		  { namelist : vartype ;} 
 * 		] 
 * 		{ sbpmod sbpname parmdef  [ : fnctype ]; body } 

*/
void declarative(void){
  
  /*
   * vardef -> VAR namelist ':' vartype ';' || vardef.symtab <- 
   * 						forall symbol in namelist.name do
   *							symtab_append(symbol,vartype.type)
   *						end do 
   * 
   * 
   */
  
  
  if(lookahead == VAR){
    match(VAR);
    do{
      /*[[*/ int type , i; /*]]*/
     /*[[*/ char **namev = /*]]*/ namelist();
      match(':');
      /*[[*/ type =  /*]]*/ vartype();
       /*[[*/ for(i=0;namev[i];i++)symtab_append(namev[i],type); /*]]*/
      match(';');
    }while(lookahead == ID);
    
  }
  while(lookahead == PROCEDURE || lookahead == FUNCTION){
    match(lookahead);
    match(ID);
    parmdef();
    if(lookahead == ':'){
      match(':');
      fnctype();
      match(';');
      body();
      match(';');
      
    }
  }
}




void fnctype(void){
  
    switch(lookahead){
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
void parmdef(void){
 
  if(lookahead == '('){
      match('(');
      if(lookahead == VAR){
	match(VAR);
      }
      namelist();
      match(':');
      while(lookahead == ';'){
	match(';');
	if(lookahead == VAR){
	  match(VAR);
	}
	namelist();
	match(':');
	vartype();
      }
  }
  match(')');
}

void vartype(void){

  switch(lookahead){
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


// imperative BEGIN stmtlist END
void imperative(void){
  match(BEGIN);
  stmtlist();
  match(END);
}



//namelist -> ID { , ID }
char **namelist(void)
{
 /*[[*/ char **symbolvec = (char **)calloc(MAX_ARG_NUM, sizeof(char **));
  int i = 0;/*]]*/
  _namelist_begin:
    /*[[*/ strcpy(symbolvec[i] = malloc(sizeof lexeme +1), lexeme);
    i++;/*]]*/
    match(ID);
    while(lookahead == ','){
      match(',');
      goto _namelist_begin;
    }
    
   /*[[*/ return symbolvec; /*]]*/
}

void stmtlist(void){
  stmt();
  
    while (lookahead == ';') {
        match(';');
        stmt();
    }
}


void stmt(void) {

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
	case ID: /*hereafter we expect FIRST(expr):*/ 
            break;
	    
	case DEC:
	    break;
	case  '(':
	  expr();
	  break;
            /*                     | ""
             */
        default:
	  /*<empty>*/
            ;
    }
}



void beginblock(void){
  match(BEGIN);
  printf("BEGIN RECONHECIDO - IMPLEMENTAR AQUI");
}

void ifstmt(void){
   printf("IF RECONHECIDO - IMPLEMENTAR AQUI");
}

void whilestmt(void){
   printf("WHILE RECONHECIDO - IMPLEMENTAR AQUI");
}

/*
 * repeatstmt -> REPEAT 
 * 
 */
void repeatstmt(void){
   printf("REPEAT RECONHECIDO - IMPLEMENTAR AQUI");
}

/* mypas -> expr { cmdsep expr } <eof> */

void mypas_old(void)
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

/* addop -> + | - */
int addop (void)
{
  switch (lookahead)
  {
    case '+':
    match('+'); return '+';
    case '-':
    match('-'); return '-';
    case OR:
    match(OR); return OR;
    
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
    case AND:
      match(AND); return AND;
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

    case DEC:
    /*[[*/cp2acc(atof(lexeme))/*]]*/;
    match(DEC);
    // /*[[*/printf("decimal: ")/*]]*/;
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
  /*symbol must be declared*/
    if(symtab_lookup(lexeme) == -1){
	exit(-1);
    }
  
  /*[[*/char varname[MAXID_SIZE]/*]]*/;
  /*[[*/strcpy(varname, lexeme)/*]]*/;
  match(ID);
  if (lookahead == ASGN) {// L-VALUE:
    match(ASGN);// ASGN = ':='
    expr();
    /*[[*/store(varname)/*]]*/;
  } else { // R-VALUE:
    /*[[*/recall(varname)/*]]*/;
  }
}

int octalToInt(char octalToConvert[]){

  int n = atoi(octalToConvert);
  int octal = 0, i = 0, rem;
  while (n!=0)
  {
    rem = n%10;
    n/=10;
    //octal += rem*pow(8,i);
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
    fprintf (stderr, "parser: token mismatch error.\n");
    fprintf (stderr, "expecting %d but seen %d... exting\n",
    expected_token, lookahead);
    exit (SYNTAX_ERR);
  }
}
