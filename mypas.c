/**@<mypas.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>
#include <string.h>
#include <mypas.h>
#define MAX_NUM_DE_COMANDOS 64 //para leitura_comandos
#define TAMANHO_MAX_DE_CADA_COMANDO 256 //para leitura_comandos

void leitura_comandos();
FILE *source_code;
char palavra_de_saida[] = "done";
extern int lookahead; // @ parser.c

FILE *source, *object;

int main (int argc, char *argv[], char *envp[])
{
  object = stdout;
  if (argc == 1) {
    /* para remover o modo de leitura de comandos direto do terminal
    comente as 2 linhas seguintes e descomente a terceira
    linha seguinte */
    leitura_comandos();
    source_code = fopen("comandos","r");
    //source_code = stdin;
  } else {
    source_code = fopen (argv[1], "r");
    if (source_code == NULL) {
      fprintf (stderr, "%s: cannot open %s... exiting\n",
      argv[0], argv[1]);
      exit (-1);
    }
  }
    // int c;
    // if (tape) {
    //   while ((c = getc(tape)) != EOF)
    //       putchar(c);
    //   fclose(tape);
    // }
  lookahead = gettoken (source_code);
  // printf("||| lookahead: %d |||\n", lookahead);
  mypas ();

  printf("\n");

  exit (0);
}
void leitura_comandos()
{
  FILE* comandos;
  comandos = fopen("comandos","w+");
  int i=0, numero_de_comandos=-1;
  char entrada[MAX_NUM_DE_COMANDOS][TAMANHO_MAX_DE_CADA_COMANDO];
  printf("\nBem vindo ao MyPas!\n");
  printf("Digite os comandos e aperte enter.\nDigite '%s' para sair...\n\n",palavra_de_saida);

  do {
    numero_de_comandos++;
    scanf("%s",entrada[numero_de_comandos]);
  } while (strcmp(palavra_de_saida,entrada[numero_de_comandos]));

  // insere os comandos escritos no arquivo "comandos"
  do {
    fprintf(comandos,"%s",entrada[i]);
    if((i+1)!=numero_de_comandos)
      fprintf(comandos,"\n");
    i++;
  } while (i<numero_de_comandos);

  fclose(comandos);
  printf("____________________________________\n\n");
}
