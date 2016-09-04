/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <parser.h>

int parentheses_check;
/*************************** LL(1) grammar definition ******************************
*
* expr -> term {addop term}
*
* term -> fact {mulop fact}
*
* fact -> vrbl | cons | ( expr )
*
* vrbl -> ID
*
* cons -> DEC
*
* addop -> '+' | '-'
*
* mulop -> '*' | '/'
*
* ID = [A-Za-z][A-Za-z0-9]*
*
* DEC = [1-9][0-9]* | 0
*
**********************************************************************************/

/***************************** LL(1) grammar emulation *****************************
*
* expr -> term { addop term } */


void expr (void)
{
	E_entry:
	T_entry:
	F_entry:
	switch (lookahead)
	{
		case ID:
			match (ID);
			break;

		case DEC:
			match (DEC);
			break;

		default:
			break;
	}
	if(mulop())
		goto F_entry;
	else if(addop())
		goto T_entry;
	else
	{
		if(lookahead=='(')
		{
			match ('(');
			parentheses_check++;
			goto E_entry;
		}
		if(lookahead==')')
		{	//printf("look:%d",lookahead);
			match (')');
			parentheses_check--;
			goto E_entry;
		}
	}
}


/*
* vrbl -> ID
*
* cons -> DEC
*
* ID = [A-Za-z][A-Za-z0-9]*
*
* DEC = [1-9][0-9]* | 0
*
* addop -> '+' | '-' */
int addop (void)
{
	switch(lookahead){
		case '+':
		match('+'); return '+';
		case '-':
		match('-'); return '-';
	}
	return 0;
}

/*
* mulop -> '*' | '/' */
int mulop (void)
{
	switch(lookahead){
		case '*':
		match('*'); return '*';
		case '/':
		match('/'); return '/';
	}
	return 0;
}
/***************************** lexer-to-parser interface **************************/

int lookahead; // @ local

void match (int expected)
{
	if ( expected == lookahead) {
		lookahead = gettoken (source);
	} else {
		fprintf(stderr,"parser: token mismatch error. found # %d ",
		lookahead);
		fprintf(stderr,"whereas expected # %d\n",
		expected);
		exit (SYNTAX_ERR);
	}
}
