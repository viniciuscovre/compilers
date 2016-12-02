#include <stdio.h>
#include <lexer.h>

 #define MAXSYMTAB_ENTRIES 0x10000
 #define MAXSTACK_SIZE     0x40
 #define MAX_ARG_NUM 1024

/* smpexpr -> term rest */
int expr();
/* term -> fact quoc */
/* rest -> addop term rest | <> */
/* quoc -> mulop fact quoc | <> */
/* fact -> variable | constant | ( smpexpr ) */
/* addop -> + | - */
int addop (void);
/* mulop -> * | / */
int mulop (void);
/* constant -> DEC | OCT | HEX | FLT */
/* variable -> ID */

void mypas(void);
void body(void);
void declarative(void);
int vartype(void);
void imperative(void);
void stmtlist(void);
void stmt(void);
void beginblock(void);
void ifstmt(void);
void whilestmt(void);
void repeatstmt(void);
int isrelop(void);

/******************************* lexer-to-parser interface *****************************************/

extern int lookahead; /** @ parser.c **/

extern int gettoken (FILE *); /** @ lexer.c **/

void match (int expected_token);

extern FILE *source;

extern char lexeme[]; /** @ lexer.c **/
