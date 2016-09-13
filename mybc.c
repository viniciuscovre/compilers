/**@<mybc.c>::**/

/*
1: Tue Aug 16 20:50:47 BRT 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>
#include <mybc.h>
#include <vmachine.h>

FILE *source_code;

extern int lookahead; // @ parser.c

main (int argc, char *argv[], char *envp[])
{
        if (argc == 1) {
                source_code = stdin;
        } else {
                source_code = fopen (argv[1], "r");
                if (source_code == NULL) {
                        fprintf (stderr, "%s: cannot open %s... exiting\n",
                                argv[0], argv[1]);
                        exit (-1);
                }
        }

        lookahead = gettoken (source_code);

        mybc ();

        printf("\n");

        exit (0);
}
