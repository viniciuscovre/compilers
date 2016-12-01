/**@<mypas.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <lexer.h>
#include <string.h>
#include <symtab.h>
#include <mypas.h>

FILE *source, *object;

int main (int argc, char *argv[], char *envp[])
{
  if (argc == 1) {
    fprintf(stderr, "%s: cannot compile without an input file... exiting\n", argv[0]);
    exit (FILE_NOT_FOUND);
  } else if (argc == 2 || argc == 3) {
    
    char *extension;
    //getting extension (dividing argv[1] by '.'). Right one should be '.pas'
    extension = strchr(argv[1], '.');
    if(extension == NULL) {
      fprintf(stderr, "%s: cannot open '%s'. File has no extension... exiting\n", argv[0], argv[1]);
      exit (EMPTY_FILE_EXTENSION);
    }
    if(strcmp(extension, ".pas")) {
      fprintf(stderr, "%s: cannot open '%s'. Extension '%s' is not compatible... exiting\n", argv[0], argv[1], extension);
      exit (INCOMPATIBLE_FILE_EXTENSION);
    }

    //verify parameter -S to generate file
    if( argc == 3){
      //get filename
      char filename[255];
      int i;
      for (i=0; i < strlen(argv[1]) - strlen(extension); i++){
        filename[i] = argv[1][i];
      }
      filename[i] = '\0';

      // verify if assembly code is asked ('-S' typed in terminal after .pas file)
  	  if(strcmp(argv[2], "-S") == 0){
  	     object = fopen(strcat(filename,".s"), "w+");
  	  } else {
  	   fprintf(stderr, "%s: cannot understand parameter '%s'... exiting\n",argv[0], argv[2]);
  	   exit (INCOMPATIBLE_PARAMETER);
  	  }

    } else {
      object = stdout;
    }

    source = fopen(argv[1], "r");
  } else {
    source = fopen (argv[1], "r");
    if (source == NULL) {
      fprintf (stderr, "%s: cannot open '%s'... exiting\n", argv[0], argv[1]);
      exit (PARAMETERS_SURPLUS);
    }
  }
  mypas();
  //print_symtab_stream(); //this is a function for debug purposes, prints the entire symtab_stream
  printf("\n");
  exit (END_OF_COMPILATION);
}
