#include <stdio.h>
#include <lexer.h>

 #define MAXSYMTAB_ENTRIES 0x10000
 #define MAXSTACK_SIZE     0x40

/* expr -> term { addop term }
 * term -> fact { mulop fact }
 * fact -> variable | constant | ( expr )
 */
int expr(void);
/* addop -> + | - */
int addop (void);
/* mulop -> * | / */
int mulop (void);
/* constant -> DEC | OCT | HEX | FLT */
void constant (void);
/* variable -> ID */
void variable (void);

void arith (void);

int arithmetic_op (void);

void mypas(void);
void body(void);
void declarative(void);
void fnctype(void);
void parmdef(void);
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

extern FILE *source_code;

extern char lexeme[]; /** @ lexer.c **/
