/**@<mypas.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>
#include <string.h>
#include <mypas.h>

void leitura_comandos();
FILE *source_code, *object;
char palavra_de_saida[] = "done";

int main (int argc, char *argv[], char *envp[])
{
  object = stdout;
  if (argc == 0) {
    fprintf(stderr, "cannot compile without an input file... exiting\n");
  } else if (argc == 1) {
    //TODO: verificar extensão do arquivo.
    // char *extension;
    // extension = strtok(argv[0], '.', )
    // source_code = stdin;
  } else {
    source_code = fopen (argv[1], "r");
    if (source_code == NULL) {
      fprintf (stderr, "%s: cannot open %s... exiting\n", argv[0], argv[1]);
      exit (-1);
    }
  }
  mypas();
  printf("\n");
  exit (0);
}
