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
* smpexpr -> term rest
*
* rest -> addop term rest | <>
*
* term -> fact quoc
*
* quoc -> mulop fact quoc | <>
*
* fact -> variable | constant | ( smpexpr )
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
// #include <malloc.h>©
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* local include */
#include <tokens.h>
#include <lexer.h>
#include <keywords.h>
#include <symtab.h>
#include <mypas.h>
#include <macros.h>
#include <pseudoassembly.h>
#include <parser.h>

int ERROR_COUNTER = 0; // semantic errors counter

char **namelist(void);

/* function to increment semantic error counter (ERROR_COUNTER) and print
error number in error file (via fprintf) */
int semanticErrorNum()
{
  ERROR_COUNTER++;
  return ERROR_COUNTER;
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
  lookahead = gettoken (source);
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
      /*[[*/ int type , i /*]]*/;
      // get the names of the declared variables
      /*[[*/ char **namev = /*]]*/ namelist();
      match(':');
      // get the type of the declared variables
      /*[[*/ type =  /*]]*/ vartype();

      // insert name values and types of the variables in the symtab
      /*[[*/
      for(i=0; namev[i]; i++) {
        if(symtab_append(namev[i], type) == -2)
          fprintf(stderr,"%d: FATAL ERROR -2: no more space in symtab", semanticErrorNum());
      }
      /*]]*/
      match(';');
    } while(lookahead == ID);

  }
}

//namelist -> ID { , ID }
// array of symbols (symbolvec) with names of variables (IDs)
char **namelist(void)
{
  /*[[*/ char **symbolvec = (char **)calloc(MAX_ARG_NUM, sizeof(char **)); /*]]*/
  /*[[*/ int i = 0; /*]]*/

  _namelist_begin:
  /*[[*/ strcpy(symbolvec[i] = malloc(sizeof lexeme +1), lexeme); i++; /*]]*/
  match(ID);
  while(lookahead == ',') {
    match(',');
    goto _namelist_begin;
  }

  /*[[*/ return symbolvec /*]]*/;
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

    /*hereafter we expect FIRST(smpexpr):*/
    case ID: //tokens.h
    case FLTCONST: //tokens.h
    case INTCONST: //tokens.h
    case TRUE: //keywords.h
    case FALSE: //keywords.h
    case NOT: //keywords.h
    case '-':
    case '(':
      smpexpr(0);
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
  /*[[*/int _endif, _else;/*]]*/
  match(IF);
  expr(BOOLEAN);
  // syntype = expr(BOOLEAN); //-> <expr>asm
  /*[[*/fprintf(object, "\tjz .L%d\n", _endif = _else = labelcounter++);/*]]*/
  match(THEN);
  stmt();
  if(lookahead == ELSE) {
    match(ELSE);
    /*[[*/_endif = jump(labelcounter++);/*]]*/
    /*[[*/mklabel(_else);/*]]*/
    /*[[*/mklabel(_endif);/*]]*/
    stmt();
  }
  /*[[*/mklabel(_endif);/*]]*/

}

//whilestmt -> WHILE smpexpr DO stmt
void whilestmt(void)
{
  /*[[*/int while_head, while_tail/*]]*/;
  match(WHILE);
  /*[[*/mklabel(while_head = labelcounter++)/*]]*/;
  expr(BOOLEAN);
  /*[[*/gofalse(while_tail = labelcounter++)/*]]*/;
  match(DO);
  stmt();
  /*[[*/jump(while_head)/*]]*/;
  /*[[*/mklabel(while_tail)/*]]*/;

}

//repeatstmt -> REPEAT stmt { ; stmt } UNTIL smpexpr
void repeatstmt(void)
{
  match(REPEAT);
  stmt();
  while(lookahead == ';') {
    match(';');
    stmt();
  }
  match(UNTIL);
  expr(BOOLEAN);
}

/* smpexpr -> term { addop [[<enter>]] term [[ print addop.pf ]] } */
/* smpexpr -> ['-'] term { addop term } */ //como ta na do eraldo
/*
 * regras de checagem de tipos (e de herança de tipos)...
 *
 *  OP      |  BOOLEAN    |   NUMERIC
 * =====================================
 *  NOT     |     X       |     N/A
 *  OR      |     X       |     N/A
 *  AND     |     X       |     N/A
 *  NEG     |    N/A      |      X     // signal changes
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
 * iscompatible...
 *
 *  _LVALUE_  || BOOLEAN | INTEGER |   REAL  |   DOUBLE
 * =======================================================
 *  BOOLEAN   |  BOOLEAN |   N/A   |   N/A   |    N/A
 *  INTEGER   |    N/A   | INTEGER |   N/A   |    N/A
 *  REAL      |    N/A   |   REAL  |   REAL  |    N/A
 *  DOUBLE    |    N/A   |  DOUBLE |  DOUBLE |   DOUBLE
 *
 */

 int iscompatible(int ltype, int rtype)
 {
   switch(ltype) {

     case BOOLEAN:
     case INTEGER:
       if(rtype == ltype)
         return ltype;
       break;

     case REAL:
       switch(rtype) {
         case INTEGER:
         case REAL:
         return ltype;
       }
       break;

     case DOUBLE:
       switch(rtype) {
         case INTEGER:
         case REAL:
         case DOUBLE:
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

/* syntax: expr -> smpexpr [ relop smpexpr ] */
int expr(int inherited_type)
{
  int t1;
  t1 = smpexpr(0); // t1 is for the right side of the smpexpression
  int t2 = 0;


  if(isrelop()) { // verifies only when it comes a relational operator
     t2 = smpexpr(t1);

    if(iscompatible(t1,t2)) {
      //	cmpl();
    } else {
       fprintf(stderr, "%d: incompatible operation %d with %d: fatal error.\n",semanticErrorNum(),t1,t2);
       return -1;
    }
  }

  if(t2){
    if(t1 == t2 && t1 == BOOLEAN || t1 > BOOLEAN && t2 > BOOLEAN){
      return BOOLEAN;
    } else {
     if((inherited_type == BOOLEAN && t1 > BOOLEAN) || (t1 == BOOLEAN && inherited_type > BOOLEAN)){
       fprintf(stderr, "%d: incompatible operation %d with %d: fatal error.\n",semanticErrorNum(),t1,t2);
       return -1;
     } else {
      return max(t1,inherited_type);
     }

    }
  }
  return -1;
}

int smpexpr(int inherited_type)
{
  /*[[*/int
	add_flag = 0,
	mul_flag = 0,
	varlocality,             // position of a variable in symtab
	lvalue_seen = 0,         // flag is 1 when see LVALUE [before]
	acctype = inherited_type,// accumulated type [after]
	syntype,                 // symbol type declared in symtab [before]
	ltype,           // syntype but for later compatibility verification [before]
	rtype/*]]*/;     // updated type (with or without promotion) [after]

  if(lookahead == '-'){
    match('-');
    /*[[*/
    if(acctype == BOOLEAN) { // "minus" isn't compatible with boolean operation
      fprintf(stderr, "%d: incompatible unary operator: fatal error.\n",semanticErrorNum());
    } else if (acctype == 0) {
      acctype = INTEGER;
    }
    /*]]*/
  } else if (lookahead == NOT) {
    match(NOT);
    /*[[*/
    if(acctype > BOOLEAN) { // "not" isn't compatible with non-boolean operation
      fprintf(stderr, "%d: incompatible unary operator: fatal error.\n", semanticErrorNum());
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
          fprintf(stderr, "%d: parser: %s not declared... fatal error!\n", semanticErrorNum(),lexeme);
	        syntype = -1;
        } else {
	        syntype = symtab[varlocality][1];
	      }
        /*]]*/
	if (acctype == 0){
	  acctype = syntype;
	}
        match(ID);
        if (lookahead == ASGN) {
	    /* located variable is LVALUE */
	    /*[[*/
		  lvalue_seen = 1;
		  ltype = syntype;
		  /*]]*/
	    match(ASGN);
	    rtype = expr(/*[[*/ltype/*]]*/);

	    /*[[*/
	    if(iscompatible(ltype, rtype)) {
	      acctype = max(rtype,acctype);
	    } else {
	      acctype = -1;
	    }
	    /*]]*/
	} /*[[*/ else if(varlocality > -1) {
          fprintf(object, "\tpushl %%eax\n\tmovl %s,%%eax\n",
            symtab_stream + symtab[varlocality][0]);
        }
        /*]]*/
        break;

      case FLTCONST:
        {
          float lexval = atof(lexeme);
          char *fltIEEE = malloc(sizeof(lexeme) + 1);
          /*[[*/sprintf(fltIEEE, "$%i", ((int *)&lexval) );/*]]*/
          /*[[*/rmovel(fltIEEE);/*]]*/
        }
        match(FLTCONST);
	syntype = REAL;
	if (acctype > BOOLEAN || acctype == 0) {
	    acctype = max(acctype, syntype);
	}
        break;

      case INTCONST:
	     /*[[*/rmovel((char*)lexeme);/*]]*/
        match(INTCONST);
	syntype = INTEGER;
	if (acctype > BOOLEAN || acctype == 0) {
	    acctype = max(acctype, syntype);
	}

        break;

      default:
        match('(');
	      /*[[*/syntype = /*]]*/ expr(0);

	      /*[[*/
	      if(iscompatible(syntype, acctype)) {
	         acctype = max(acctype,syntype);
	      } else {
		 printf("default");
	         fprintf(stderr, "%d: incompatible unary operator: fatal error.\n", semanticErrorNum());
		 acctype = -1;
	      }
	      /*]]*/
        match(')');
    }

     if(mul_flag){
       mulint();
    }

    if(mul_flag = mulop())
      goto F_entry;

    if(add_flag){
       addint();
    }

    if(add_flag = addop())
      goto T_entry;
    /* smpexpression ends down here */

    /*[[*/
    if(lvalue_seen && varlocality > -1) {
      switch(ltype) {
        // verify which kind of instructions will be worked
        case INTEGER: case REAL: case BOOLEAN:
          lmovel(symtab_stream + symtab[varlocality][0]); // when 32-bit operation
          break;

        case DOUBLE:
          lmoveq(symtab_stream + symtab[varlocality][0]); // when 64-bit operation
          break;

        default: //case  BOOLEAN
        //TODO: decide what to do with BOOLEAN
          break;
      }
    }
    /*]]*/
    return acctype;
}

/* addop -> + | - | OR */
int addop (void)
{
  switch (lookahead)
  {
    case '+':
      match('+');
      /*[[*/addint();/*]]*/
      return '+';

    case '-':
      match('-');
      /*[[*/subint();/*]]*/
      return '-';

    case OR:
      match(OR);
      /*[[*/mullog();/*]]*/
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
      /*[[*/mulint();/*]]*/
      return '*';

    case '/':
      match('/');
      /*[[*/divint();/*]]*/
      return '/';

    case AND:
      match(AND);
      /*[[*/addlog();/*]]*/
      return AND;
  }
  return 0;
}

/******************************* lexer-to-parser interface *****************************************/

int lookahead;

void match (int expected_token)
{
  if (expected_token == lookahead) {
    lookahead = gettoken (source);
  } else {
    fprintf (stderr, "\nparser: token mismatch error.\n");
    fprintf (stderr, "expecting %d but seen %d. Exting...\n",
    expected_token, lookahead);
    //exit (SYNTAX_ERR);
    fprintf(stderr,"FATAL ERROR %d",SYNTAX_ERR);
    return;
  }
}
