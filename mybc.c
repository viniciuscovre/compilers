/**@<mybc.c>::**/

/*
1: Tue Aug 16 20:50:47 BRT 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <mybc.h>

FILE *source;

extern int lookahead; // @ parser.c
extern int parentheses_check = 0;

int main (int argc, char *argv[], char *envp[])
{
  if (argc == 1) {
    source = stdin;
  } else {
    source = fopen (argv[1], "r");
    if (source == NULL) {
      fprintf (stderr, "%s: cannot open %s... exiting\n",
      argv[0], argv[1]);
      exit (-1);
    }
  }

  lookahead = gettoken (source);

  // init();

  expr ();
  printf("\n");

  match(EOF);

  // Just to print quantity of parenthesis error
  unsigned int quantity;
  quantity = parentheses_check;
  quantity = (parentheses_check >= 0 ? parentheses_check : -parentheses_check);

  if(lookahead==EOF)
  {
    if (parentheses_check!=0)
    printf("Error! ");

    if (parentheses_check < 0)
    printf("Missing %d left parenthesis '('\n", quantity);
    else if (parentheses_check > 0)
    printf("Missing %d right parenthesis ')'\n", quantity);
  }
  exit (0);
}
