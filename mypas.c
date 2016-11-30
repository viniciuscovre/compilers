/**@<mypas.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>
#include <string.h>
#include <mypas.h>

FILE *source_code, *object;

int main (int argc, char *argv[], char *envp[])
{
  //TODO: ciar arquivo.s para gerar o assembly, não apenas em STDOUT
  object = stdout;

  if (argc == 1) {
    fprintf(stderr, "%s: cannot compile without an input file... exiting\n", argv[0]);
    exit (-1);
  } else if (argc == 2) {
    char *extension;
    //getting extension (dividing argv[1] by '.'). Right one should be '.pas'
    extension = strchr(argv[1], '.');
    if(strcmp(extension, ".pas")) {
      fprintf(stderr, "%s: cannot open '%s'. Extension '%s' is not compatible... exiting\n", argv[0], argv[1], extension);
      exit (-1);
    }
    source_code = fopen(argv[1], "r");
  } else {
    source_code = fopen (argv[1], "r");
    if (source_code == NULL) {
      fprintf (stderr, "%s: cannot open '%s'... exiting\n", argv[0], argv[1]);
      exit (-1);
    }
  }
  mypas();
  printf("\n");
  exit (0);
}
