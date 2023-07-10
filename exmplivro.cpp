/* front.c - um analisador l�xico e analisador sint�tico 
 simples para express�es aritm�ticas simples */ 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
/* Declara��es globais */ 
/* Vari�veis */
int charClass; 
char lexeme [100]; 
char nextChar; 
int lexLen; 
int token;

int nextToken; 
FILE *in_fp, *fopen(); 
/* Declara��es de Fun��es */ 
void addChar(); 
void getChar(); 
void getNonBlank(); 
int lex();
void error();
void expr();
void term();
void factor();

/* Classes de caracteres */ 
#define LETTER 0 
#define DIGIT 1 
#define UNKNOWN 99 
/* C�digos de tokens */ 
#define INT_LIT 10 
#define IDENT 11 
#define ASSIGN_OP 20 
#define ADD_OP 21 
#define SUB_OP 22 
#define MULT_OP 23 
#define DIV_OP 24 
#define LEFT_PAREN 25 
#define RIGHT_PAREN 26 
/******************************************************/ 
/* fun��o principal */ 
main() { 
/* Abrir o arquivo de dados de entrada e processar seu 
conte�do */ 
 if ((in_fp = fopen("front.in", "r")) == NULL) 
 printf("ERROR - cannot open front.in \n"); 
 else { 
 getChar(); 
 do { 
 lex(); 
 } while (nextToken != EOF); 
 } 
}
/******************************************************/ 
/* lookup - uma fun��o para processar operadores e par�nteses 
 e retornar o token */ 
int lookup(char ch) { 
 switch (ch) { 
 case '(': 
 addChar(); 
 nextToken = LEFT_PAREN; 
 break; 
 case ')':
 	
 	addChar(); 
 nextToken = RIGHT_PAREN; 
 break; 
 case '+': 
 addChar(); 
 nextToken = ADD_OP; 
 break; 
 case '-': 
 addChar(); 
 nextToken = SUB_OP; 
 break; 
 case '*': 
 addChar(); 
 nextToken = MULT_OP; 
 break; 
 case '/': 
 addChar(); 
 nextToken = DIV_OP; 
 break; 
 default: 
 addChar(); 
 nextToken = EOF; 
 break; 
 } 
 
 return nextToken; 
} 
/*****************************************************/ 
/* addChar - uma fun��o para adicionar nextChar ao 
vetor lexeme */ 
void addChar() { 
 if (lexLen <= 98) { 
 lexeme[lexLen++] = nextChar; 
 lexeme[lexLen] = 0; 
 } 
 else 
 printf("Error - lexeme is too long \n"); 
} 
/*******************************************************/ 
/* getChar - uma fun��o para obter o pr�ximo caractere da entrada e determinar sua classe de caracteres */ 
void getChar() { 
 if ((nextChar = getc(in_fp)) != EOF) { 
 if (isalpha(nextChar)) 
 charClass = LETTER; 


else if (isdigit(nextChar)) 
 charClass = DIGIT; 
 else charClass = UNKNOWN; 
 } 
 else 
 charClass = EOF; 
} 
/*******************************************************/ 
/* getNonBlank - uma fun��o para chamar getChar at� que ela 
retorne um caractere diferente de espa�o em 
branco */ 
void getNonBlank() { 
 while (isspace(nextChar)) 
 getChar(); 
} 
/********************************************************/ 
/* lex - um analisador l�xico simples para express�es 
aritm�ticas */ 
int lex() { 
 lexLen = 0; 
 getNonBlank(); 
 switch (charClass) { 
/* Reconhecer identificadores */ 
 case LETTER: 
 addChar(); 
 getChar(); 
 while (charClass == LETTER || charClass == DIGIT) { 
 addChar(); 
 getChar(); 
 } 
 nextToken = IDENT; 
 break; 
/* Reconhecer literais inteiros */ 
 case DIGIT: 
 addChar(); 
 getChar(); 
 while (charClass == DIGIT) { 
 addChar(); 
 getChar(); 
 } 
 nextToken = INT_LIT; 
 break; 
/* Par�nteses e operadores */ 
 case UNKNOWN: 
 lookup(nextChar);
 
 getChar(); 
 break;
/* Fim do arquivo */ 
 case EOF: 
 nextToken = EOF; 
 lexeme[0] = 'E'; 
 lexeme[1] = 'O'; 
 lexeme[2] = 'F'; 
 lexeme[3] = 0; 
 break; 
 } /* Fim do switch */ 
 printf("Next token is: %d, Next lexeme is %s\n", 
 nextToken, lexeme); 
 return nextToken; 
} /* Fim da fun��o lex */


void error() {
	printf("Error - ocorreu um erro na analise sintatica\n");
	exit(1);
}
/* expr 
 Analisa sintaticamente cadeias na linguagem gerada pela 
regra: 
 <expr> -> <term> {(+ | -) <term>} 
 */ 
void expr() { 
 printf("Enter <expr>\n"); 
/* Analisa sintaticamente o primeiro termo */ 
 term(); 
/* Desde que o pr�ximo token seja + ou -, obtenha o pr�ximo
 token e analise sintaticamente o pr�ximo termo */ 
 while (nextToken == ADD_OP || nextToken == SUB_OP) { 
 lex(); 
 term(); 
 } 
 printf("Exit <expr>\n"); 
} /* Fim da fun��o expr */

/* term 
 Analisa sintaticamente cadeias na linguagem gerada pela 
regra: 
 <term> -> <factor> {(* | /) <factor>) 
 */ 
void term() { 
 printf("Enter <term>\n"); 
/* Analisa sintaticamente o primeiro termo */ 
 factor(); 
/* Desde que o pr�ximo token seja + ou -, obtenha o pr�ximo 
 token e analise sintaticamente o pr�ximo termo */ 
 while (nextToken == MULT_OP || nextToken == DIV_OP) { 
 lex(); 
 factor(); 
 } 
 printf("Exit <term>\n"); 
} /* Fim da fun��o term */ 

/* factor 
 Analisa sintaticamente cadeias na linguagem gerada pela 
regra: 
 <factor> -> id | int_constant | (<expr) 
 */ 
void factor() { 
 printf("Enter <factor>\n"); 
/* Determina qual RHS */ 

if (nextToken == IDENT || nextToken == INT_LIT) 
/* Obt�m o pr�ximo token */ 
 lex(); 
/* Se a RHS � (<expr>), chame lex para passar o par�ntese 
 esquerdo, chame expr e verifique pelo par�ntese 
 direito */ 
 else { 
 if (nextToken == LEFT_PAREN) { 
 lex(); 
 expr(); 
 if (nextToken == RIGHT_PAREN) 
 lex(); 
 else 
 error(); 
 } /* Fim do if (nextToken == ... */ 
/* N�o era um identificador, um literal inteiro ou um 
 par�ntese esquerdo */ 
 else 
 error(); 
 } /* Fim do else */ 
 printf("Exit <factor>\n");; 
} /* Fim da fun��o factor */ 
