#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Declarações globais */
/* Variáveis */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
int inter = 0;
int *pinter;
int num1, num2, result;
int *pnum1, *pnum2, *presult;
char operator;
char *poperator;
FILE* in_fp, * fopen();

/* Declarações de Funções */
void addChar();
void getChar();
void getNonBlank();
int lex();
void imprimirNomeToken(int token);
void error();
void expr(int level, int anterior);
void term(int level, int anterior);
void factor(int level, int anterior);
void computeExpression();

/* Classes de caracteres */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Códigos de tokens */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define STD_RESV 30
#define COUT_RESV 31
#define OUTPUT_OP 32

/******************************************************/
/* função principal */
int main() {

	pnum1 = &num1;
	pnum2 = &num2;
	presult = &result;
	poperator = &operator;

	pinter = &inter;
	char resposta[10];
	int i = 1;
	int *pi;
	pi = &i;

	while (i) {
		printf("Escolha uma opcao:\n");
		printf("1. Abrir arquivo de Exemplo.\n");
		printf("2. Escrever expressao.\n");
		printf("0. Sair\n");
		printf("Opcao: ");

		fgets(resposta, sizeof(resposta), stdin);

		// Remover o caractere de nova linha da resposta
		resposta[strcspn(resposta, "\n")] = '\0';

		// Verificar a opção escolhida
		if (strcmp(resposta, "1") == 0) {
			printf("Opcao selecionada: Abrir arquivo de Exemplo.\n");
			if ((in_fp = fopen("exemplo.txt", "r")) == NULL)
				printf("ERROR - cannot open front.in \n");
			else {
				getChar();
				do {
					lex();
					expr(0,0);
				} while (nextToken != EOF);
			}
		} else if (strcmp(resposta, "2") == 0) {
			printf("Opcao selecionada: Escrever expressao.\n");
			char input[100];
			printf("Digite uma expressao: ");
			scanf("%s", input);
			getchar();
			in_fp = fopen("temp.txt", "w+");
			fprintf(in_fp, "%s", input);
			fclose(in_fp);

			/* Abrir o arquivo e dados de entrada e processar seu
			conteúdo */
			if ((in_fp = fopen("temp.txt", "r")) == NULL)
				printf("ERROR - cannot open temp.txt \n");
			else {
				getChar();
				do {
					lex();
					expr(0,0);
				} while (nextToken != EOF);
			}
		} else if (strcmp(resposta, "0") == 0) {
			printf("Saindo...\n");
			*pi = 0;
		} else {
			printf("Opcao invalida! Tente novamente.\n");
		}
	}

	return 0;
}
/******************************************************/

/* lookup - uma função para processar operadores e parênteses
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
			lexeme[0] = '+';
			lexeme[1] = '\0';
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
		case ':':
			addChar();
			getChar();
			if (nextChar == ':') {
				addChar();
				getChar();
				nextToken = STD_RESV;
			} else {
				nextToken = UNKNOWN;
			}
			break;
		case '<':
			addChar();
			getChar();
			if (nextChar == '<') {
				addChar();
				getChar();
				nextToken = OUTPUT_OP;
			} else {
				nextToken = UNKNOWN;
			}
			break;
		default:
			addChar();
			nextToken = EOF;
			break;
	}

	return nextToken;
}
/*****************************************************/

/* addChar - uma função para adicionar nextChar ao
vetor lexeme */
void addChar() {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	} else
		printf("Error - lexeme is too long \n");
}
/*******************************************************/

/* getChar - uma função para obter o próximo caractere da entrada e determinar sua classe de caracteres */
void getChar() {
	if ((nextChar = getc(in_fp)) != EOF) {
		if (isalpha(nextChar))
			charClass = LETTER;

		else if (isdigit(nextChar))
			charClass = DIGIT;
		else
			charClass = UNKNOWN;
	} else
		charClass = EOF;
}
/*******************************************************/

/* getNonBlank - uma função para chamar getChar até que ela
retorne um caractere diferente de espaço em
branco */
void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}
/********************************************************/

/* lex - um analisador léxico simples para expressões
aritméticas */
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
			if (strcmp(lexeme, "std") == 0) {
				nextToken = STD_RESV;
			} else if (strcmp(lexeme, "cout") == 0) {
				nextToken = COUT_RESV;
			} else {
				nextToken = IDENT;
			}
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
			/* Parênteses e operadores */
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
	if (inter > 0) {
		imprimirNomeToken(nextToken);
		printf(" --> %s\n", lexeme);
	}
	return nextToken;
}
//
//Imprime o nome do Token ao inves de seu codigo
void imprimirNomeToken(int token) {
	switch (token) {
		case INT_LIT:
			printf("INT_LIT");
			break;
		case IDENT:
			printf("IDENT");
			break;
		case ASSIGN_OP:
			printf("ASSIGN_OP");
			break;
		case ADD_OP:
			printf("ADD_OP");
			break;
		case SUB_OP:
			printf("SUB_OP");
			break;
		case MULT_OP:
			printf("MULT_OP");
			break;
		case DIV_OP:
			printf("DIV_OP");
			break;
		case LEFT_PAREN:
			printf("LEFT_PAREN");
			break;
		case RIGHT_PAREN:
			printf("RIGHT_PAREN");
			break;
		case STD_RESV:
			printf("STD_RESV");
			break;
		case COUT_RESV:
			printf("COUT_RESV");
			break;
		case OUTPUT_OP:
			printf("OUTPUT_OP");
			break;
		default:
			printf("Token desconhecido");
			break;
	}
}

/* Fim da função lex */

/* Inicio da função Syn */

void error() {
	printf("Error - ocorreu um erro na analise sintatica\n");
	exit(1);
}

/* expr
 Analisa sintaticamente cadeias na linguagem gerada pela
regra:
 <expr> -> <term> {(+ | -) <term>}
 */
void expr(int level, int anterior) {
	printf("%*sEnter <expr>\n", level, "");
	level=level+5;

	/* Analisa sintaticamente o primeiro termo */
	term(level,anterior);
	//computeExpression();
	/* Desde que o próximo token seja + ou -, obtenha o próximo
	 token e analise sintaticamente o próximo termo */
	while (nextToken == ADD_OP || nextToken == SUB_OP) {
		*pinter = 1;
		printf("%*s", level, "");
		imprimirNomeToken(nextToken);
		printf(" --> %s\n", lexeme);
		*poperator = lexeme[0];
		anterior++;
		*pinter = 0;
		lex();
		term(level, anterior);
	}

	printf("%*sExit <expr>\n", level-5, "");
} /* Fim da função expr */

/* term
 Analisa sintaticamente cadeias na linguagem gerada pela
regra:
 <term> -> <factor> {(* | /) <factor>)
 */
void term(int level, int anterior) {
	printf("%*sEnter <term>\n", level, "");
	level=level+5;

	/* Analisa sintaticamente o primeiro termo */
	factor(level, anterior);
	/* Desde que o próximo token seja + ou -, obtenha o próximo
	 token e analise sintaticamente o próximo termo */
	while (nextToken == MULT_OP || nextToken == DIV_OP) {
		if (*pinter == 0) {
			*pinter = 1;
			printf("%*s", level, "");
			imprimirNomeToken(nextToken);
			printf(" --> %s\n", lexeme);
			*poperator = lexeme[0];
			anterior++;
			*pinter = 0;
		}
		lex();
		factor(level, anterior);
	}

	printf("%*sExit <term>\n", level-5, "");
} /* Fim da função term */

/* factor
 Analisa sintaticamente cadeias na linguagem gerada pela
regra:
 <factor> -> id | int_constant | (<expr)
 */
void factor(int level, int anterior) {
	printf("%*sEnter <factor>\n", level, "");

	/* Determina qual RHS */
	if (nextToken == IDENT || nextToken == INT_LIT) {
		if (*pinter == 0) {
			*pinter = 1;
			printf("%*s", level, "");
			imprimirNomeToken(nextToken);
			printf(" --> %s\n", lexeme);
			if(anterior == 1) {
				*pnum1 = atoi(lexeme);
			} else if(anterior == 2) {
				*pnum2 = atoi(lexeme);
			}
			*pinter = 0;
		}
		/* Obtém o próximo token */
		lex();
	}
	/* Se a RHS é (<expr>), chame lex para passar o parêntese
	 esquerdo, chame expr e verifique pelo parêntese
	 direito */
	else {
		if (nextToken == LEFT_PAREN) {
			if (*pinter == 0) {
				*pinter = 1;
				printf("%*s", level, "");
				imprimirNomeToken(nextToken);
				printf(" --> %s\n", lexeme);
				*pinter = 0;
			}
			lex();
			expr(level,1);
			if (nextToken == RIGHT_PAREN) {
				if (*pinter == 0) {
					*pinter = 1;
					printf("%*s", level, "");
					imprimirNomeToken(nextToken);
					printf(" --> %s\n", lexeme);
					printf("%*s", level, "");
					int valor = calculaResultado(num1, num2, &operator);
					printf("%d\n", valor);
					*pinter = 0;
				}
				lex();
			} else {
				error();
			}
		} /* Fim do if (nextToken == ... */
		/* Não era um identificador, um literal inteiro ou um
		 parêntese esquerdo */
		else {
			error();
		}
	} /* Fim do else */
	printf("%*sExit <factor>\n", level, "");
} /* Fim da função factor */

int calculaResultado(int num1, int num2, char *poperator) {
	int result;
	switch (operator) {
		case '+':
			result = num1 + num2;
			return result;
		case '-':
			result = num1 - num2;
			return result;
		case '*':
			result = num1 * num2;
			return result;
		case '/':
			if (num2 != 0) {
				result = num1 / num2;
			} else {
				printf("Error - Divisao por zero\n");
				return;
			}
			return result;
		default:
			printf("Error - Operador desconhecido\n");
			return;
	}
}
