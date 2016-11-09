#include <stdio.h>
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

/* expr -> term rest */
int expr(int inherited_type);
int superexpr(int inherited_type);
/* term -> fact quoc */
void term (void);
/* rest -> addop term rest | <> */
void rest (void);
/* quoc -> mulop fact quoc | <> */
void quoc(void);
/* fact -> variable | constant | ( expr ) */
void fact (void);
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
int octalToInt(char octalToConvert[]);
int hexToInt(char hexToConvert[]);

/******************************* lexer-to-parser interface *****************************************/

extern int lookahead; /** @ parser.c **/

extern int gettoken (FILE *); /** @ lexer.c **/

void match (int expected_token);

extern FILE *source_code;

extern char lexeme[]; /** @ lexer.c **/
