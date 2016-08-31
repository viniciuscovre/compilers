/**@<mybc.c>::**/

/*
1: Tue Aug 16 20:50:47 BRT 2016
*/

#include <stdio.h>
#include <stdlib.h>

FILE *source;

extern int lookahead; // @ parser.c
extern int checa_parenteses = 0;

main (int argc, char *argv[], char *envp[])
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

        expr ();

	match(EOF);

  // Just to print quantity of parenthesis error
  unsigned int quantity;
  quantity = checa_parenteses;
  quantity = (checa_parenteses >= 0 ? checa_parenteses : -checa_parenteses);

	if(lookahead==EOF)
	{
		if (checa_parenteses!=0)
      printf("Error! ");

    if (checa_parenteses < 0)
      printf("Missing %d left parenthesis '('\n", quantity);
    else if (checa_parenteses > 0)
      printf("Missing %d right parenthesis ')'\n", quantity);
	}
  exit (0);
}
