/**@<mypas.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>
#include <string.h>
#include <symtab.h>
#include <mypas.h>

FILE *source_code, *object;

int main (int argc, char *argv[], char *envp[])
{
  //TODO: ciar arquivo.s para gerar o assembly, não apenas em STDOUT
  //object = fopen("arquivo.s", "ab+");
  
  printf("%d \n",argc);
  
  if (argc == 1) {
    fprintf(stderr, "%s: cannot compile without an input file... exiting\n", argv[0]);
    exit (-1);
  } else if (argc == 2 || argc == 3) {
    char *extension;
    //getting extension (dividing argv[1] by '.'). Right one should be '.pas'
    extension = strchr(argv[1], '.');
    if(strcmp(extension, ".pas")) {
      fprintf(stderr, "%s: cannot open '%s'. Extension '%s' is not compatible... exiting\n", argv[0], argv[1], extension);
      exit (-1);
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
    //printf("%s -- filename: %s\n", argv[1], filename);
      
      
       char *parameter = strchr(argv[2], '-');
       //printf("%s\n",parameter);
	if(strcmp(parameter, "-S") == 0){
	  object = fopen(strcat(filename,".s"), "w+");
	}else{
	   fprintf(stderr, "%s: cannot understand parameter... exiting\n",argv[0], argv[2]);
	   exit (-1);
	}
    }else{
      object = stdout;
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
  print_symtab_stream();
  printf("\n");
  exit (0);
}
