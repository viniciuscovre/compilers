/**@<parser.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
#include <parser.h>
#include <lexer.h>
#include <string.h>

#define MAXSYMTAB_SIZE	0x10000
char cmdsep  = ';';
int symtab_nextentry = 0;
char symtab[MAXSYMTAB_SIZE][MAXID_SIZE+1];
double memtab[MAXSYMTAB_SIZE];
int tab_aux[MAXSYMTAB_SIZE];
int sp = -1;//stack pointer - pilha
//-1 vazia , estorada se sp> MAXSTACK_SIZE
#define MAXSTACK_SIZE 64
double stack[MAXSTACK_SIZE];
double accumulator;
double operation(int op,int operando1,int operando2);
int is_available();
void printa_tabela();
void printa_pilha();
double pop();
void push(int op);
double symtab_lookup(char *nome);
int symtab_lookup_error = 0;

/*************************** LL(1) grammar definition ******************************
*
* mybc -> cmd { ; cmd} \n|<eof>
*
* cmd - > expr
*
* expr -> term {addop term}
*bkplexeme
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
* OCTAL = 0 [1-7][0-7]*
**********************************************************************************/

/***************************** LL(1) grammar emulation *****************************
*
*      source language        ||          object language
* -----------------------------------------------------------------------
* expr -> term { addop term } || expr.pf := term.pf { term.pf addop.pf }
* ----------------------------symtab_lookup_error-------------------------------------------
* expr -> term { addop term [[ printf(addop.pf); ]] }
*/
void mybc(void){
  expr();
  while(lookahead==cmdsep){
    match(lookahead);
    expr(); 
  }
  match(EOF);
}




void expr (void)
{
  /**/int op, neg = 0 /**/;
  if(lookahead == '-'){
    match('-');
    /**/neg = '-';/**/
  }

  term();
  /**/if(neg){ printf("<+/-> "); }/**/

  while( (op = addop()) ) {
    /*printf("<enter> ")*/;
    /**/printf("expr:op<%c> ",op)/**/;
    push(op);
    
    term();
    
  }
}
/*
* term -> fact { mulop fact } || term.pf := fact.pf { fact.pf mulop.pf }
*/
void term (void)
{
  /**/int op/**/;
  fact();
  while( (op = mulop()) )
  {
    /**/printf("<enter> ")/**/;
    fact();
    
    /**/printf("term:op<%c> ",op)/**/;
    push(op);
  }
}
/*
* PREVIOUS:
* fact -> vrbl | cons | ( expr ) || fact.pf := expr.pf
* lookahead
*
* NEW ONE:
* fact -> ID [ = expr ] |  DEC | ( expr )
* variavel = 3 + 4
*/
void fact (void)
{
  /**/ char bkplexeme[MAXID_SIZE+1],operando1,operando2,operador;/**/

  int i = -1, a=0;
  switch (lookahead)
  {
    case ID:
      printf("ID RECONHECIDO: %s",lexeme);
    strcpy(bkplexeme, lexeme);
    match (ID);
    
    if(lookahead == '=') {
      match('=');
      expr();
      /*printf("%s:<store> ",bkplexeme);*/    
      
      strcpy(symtab[i],bkplexeme);
      tab_aux[i] = 1;

      /*Parte Operation: essa parte vai ser removida ou modificada no futuro pra abranger todo os casos de operacao,
      por enquanto apenas resolve operacoes do tipo numero + numero ~William*/
      if(sp > 1) {
        operador = pop();
        operando2 = pop();
        operando1 = pop();
        printf("Operador: %d, op1: %d, op2: %d\n",operador,operando1,operando2);
        memtab[i] = operation(operador,operando1,operando2);
      }
      /*Fim da parte Operation*/

      
    } else{
     
      //double symtab_lookup(char *nome)lookhead
      double foundID = symtab_lookup(bkplexeme);
      
      if(foundID == -1 && symtab_lookup_error==1){
	 i = is_available();

	if(i == -1) {
	  printf("ERROR:ALOCATION_ERR\n");
	  break;
	}
	
	match(ID);
	memtab[i] = 0;
	tab_aux[i] = 1;
	strcpy(symtab[i],lexeme);
	
	
      }else{
	//foundID contem valor da variavel que ja esta na tabela
	i = is_available();
	memtab[i] = foundID;
	strcpy(symtab[i],bkplexeme);
	tab_aux[i] = 1;
	
	/**/printf("id:%s ",bkplexeme);/**/
      } 
    }

    break;

    case DEC:
    /**/printf("dec:%s ", lexeme)/**/;
    /*Atoi eh necessario para inclusao na pilha ~William*/
    printf("atoi %d\n",atoi(lexeme));
    push(atoi(lexeme));
    /*Fim da inclusao  de DEC na pilha~William*/
    match (DEC);
    break;

    case HEX:
    /**/printf("hexadecimlookheadal ")/**/;
    match (HEX);
    break;

    case FLOAT:
    /**/printf("float ")/**/;
    match (FLOAT);
    break;

    case OCTAL:
    /**/printf("octal ")/**/;
    match (OCTAL);
    break;

    default:
    match ('(');
    expr();
    match (')');
  }
}
/*
void resolve()
{
int operando1,operando2,operador;
if(sp>1)
{
operador = pop();
operando2 = pop();
operando1 = pop();
printf("Operador: %d, op1: %d, op2: %d\n",operador,operando1,operando2);
memtab[i] = operation(operador,operando1,operando2);
}
}
*/

double symtab_lookup(char *nome){
  int contador = 0;
  
  for(contador = 0; contador < MAXSYMTAB_SIZE; contador++){
    if(strcmp(symtab[contador], nome)){
      return memtab[contador];
      symtab_lookup_error = 0;
      
    }
  }
  symtab_lookup_error = 1; 
  return -1;
}


/*Funcoes push e pop para lidar com a pilha.
Os erros mencionados aqui ja estao em tokens.h, porem ainda nao tem uso real ~William*/
void push(int op)//op pode ser operador ou operando
{
  printf("push\n");
  if(sp>=MAXSTACK_SIZE)
  printf("ERROR:STACK OVERFLOW\n");
  else
  {
    sp = sp + 1;
    stack[sp] = op;
  }
}

double pop()
{
  printf("pop\n");
  if(sp<=-1)
  {
    printf("ERROR:PUSH ON EMPTY LIST\n");
    return PUSH_ERR;
  }
  else
  {
    sp = sp - 1;
    return stack[sp+1];
  }
}
/*Fim push e pop ~William*/
/*Modifiquei a funcao operation para ter o return. Isso pode mudar futuramente ~William*/
double operation(int op, int operando1, int operando2)
{

  double result;

  printf("Op: %d, op1: %d, op2: %d\n",op,operando1,operando2);
  switch(op){
    
    case '+':
    result = (double) (operando1 + operando2);
    return result;
    break;

    case '-':
    result = (double) (operando1 - operando2);
    return result;
    break;

    case '*':
    result = (double) (operando1 * operando2);
    return result;
    break;

    case '/':
    if(operando2 == 0){
      printf("ERROR, DIVISION 0");

    }else{
      result = (double) (operando1 / operando2);
      return result;
    }
    break;
    default:
    printf("ERROR - COMMAND NOT VALID");
    break;
  }
  return 0;
}
/*Funcao para debug e estetica. Printa a tabela de memoria ~William*/
void printa_tabela()
{
  int i =0;
  printf("\n###########Tabela:##############\n");
  while(tab_aux[i]==1)
  {
    printf("|| Linha %d: [nome]%s|[conteudo]%g||\n",i,symtab[i],memtab[i]);
    i = i +1;
  }
  printf("#################################\n");
}
/*Mesmo que acima, porem printa a stack ~William*/
void printa_pilha()
{
  int sp_local = sp;
  printf("\n============Pilha:===========\n");
  while(sp_local>0)
  {
    printf("=== Linha %d: [nome]%g",sp_local,stack[sp_local]);
    sp_local = sp_local - 1;
  }
  printf("\n==============================\n");
}

int is_available(){

  int i = 0;
  for(i = 0; i < MAXSYMTAB_SIZE; i++){
    if(tab_aux[i] == 0){
      return i;
    }
  }

  return -1;
}

void init(){
  int i = 0;
  while( i< MAXSYMTAB_SIZE){
    tab_aux[i] = 0;
  }
}

/*
* vrbl -> IDis_identifier
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
