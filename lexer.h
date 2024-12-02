#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definições de constantes
#define MAX_LINE 256
#define MAX_NAME 16

// Estrutura para armazenar o valor do token
typedef struct {
    int tipo; // Tipo do token (definido em parser.tab.h)
    union {
        int inteiro; // Valor inteiro
        double duplo; // Valor double
        char nome[MAX_NAME]; // Nome (variável)
    } u; // União para armazenar diferentes tipos de valor
} token;


// Funções do lexer
int yylex(void);
void consumeNumbers(token *t);
void consumeNames(token *t);
void consumeOperators(token *t);
void consumeComment(void);
int verifyNumber(char c);
int verifyName(char c);
int verifyOperator(char c);
int verifySpecialNames(const char *name);
int verifyComment(char c);
int verifyInvalid(char c);

#endif // LEXER_H