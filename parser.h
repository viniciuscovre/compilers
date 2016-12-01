#include <stdio.h>
#include <lexer.h>
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

 #define MAXSYMTAB_ENTRIES 0x10000
 #define MAXSTACK_SIZE     0x40
 #define MAX_ARG_NUM 1024

/* smpexpr -> term rest */
int smpexpr(int inherited_type);
int expr(int inherited_type);
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

/******************************* lexer-to-parser interface *****************************************/

extern int lookahead; /** @ parser.c **/

extern int gettoken (FILE *); /** @ lexer.c **/

void match (int expected_token);

extern FILE *source;

extern char lexeme[]; /** @ lexer.c **/
