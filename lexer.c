#include "parser.tab.h" // Incluindo o arquivo gerado pelo Bison
#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *p; // Ponteiro global para a posição atual no buffer


// Funções de verificação
int verifyNumber(char c) { return (c >= '0' && c <= '9'); }
int verifyName(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }
int verifyOperator(char c) { return strchr("+-*/%<>=!&|^~(),", c) != NULL; }
int verifySpecialNames(const char *name) {
    if (strcasecmp(name, "sqrt") == 0) return TOKEN_SQRT;
    if (strcasecmp(name, "pow") == 0) return TOKEN_POW;
    return -1;
}
int verifyComment(char c) { return (c == '#'); }
int verifyInvalid(char c) {
    return strchr("!@$;:[]{}`\"'", c) != NULL;
}

// Consome números
void consumeNumbers(token *t) {
    char buffer[MAX_LINE] = {0};
    int index = 0, hasDot = 0, hasExp = 0;

    while (verifyNumber(*p) || *p == '.' || *p == 'e' || *p == 'E' || *p == '+' || *p == '-') {
        if (*p == '.') hasDot++;
        if (*p == 'e' || *p == 'E') hasExp++;
        buffer[index++] = *p++;
    }

    buffer[index] = '\0';
    if (hasDot || hasExp) {
        t->tipo = TOKEN_DOUBLE;
        t->u.duplo = strtod(buffer, NULL);
    } else {
        t->tipo = TOKEN_INT;
        t->u.inteiro = strtol(buffer, NULL, 10);
    }
}

// Consome nomes
void consumeNames(token *t) {
    char buffer[MAX_NAME] = {0};
    int index = 0;

    while (verifyName(*p) && index < MAX_NAME - 1) {
        buffer[index++] = *p++;
    }
    buffer[index] = '\0';

    int special = verifySpecialNames(buffer);
    if (special != -1) {
        t->tipo = special;
    } else {
        t->tipo = TOKEN_NAME;
        strncpy(t->u.nome, buffer, MAX_NAME);
    }
}

// Consome operadores
void consumeOperators(token *t) {
    if (*p == '<' && *(p + 1) == '=') { t->tipo = TOKEN_LEQ; p += 2; }
    else if (*p == '>' && *(p + 1) == '=') { t->tipo = TOKEN_GEQ; p += 2; }
    else if (*p == '=' && *(p + 1) == '=') { t->tipo = TOKEN_EQ; p += 2; }
    else if (*p == '!' && *(p + 1) == '=') { t->tipo = TOKEN_NEQ; p += 2; }
    else if (*p == '&' && *(p + 1) == '&') { t->tipo = TOKEN_AND; p += 2; }
    else if (*p == '|' && *(p + 1) == '|') { t->tipo = TOKEN_OR; p += 2; }
    else if (*p == '<' && *(p + 1) == '<') { t->tipo = TOKEN_LSHIFT; p += 2; }
    else if (*p == '>' && *(p + 1) == '>') { t->tipo = TOKEN_RSHIFT; p += 2; }
    else {
        switch (*p) {
            case '+': t->tipo = TOKEN_ADD; break;
            case '-': t->tipo = TOKEN_SUB; break;
            case '*': t->tipo = TOKEN_MUL; break;
            case '/': t->tipo = TOKEN_DIV; break;
            case '%': t->tipo = TOKEN_MOD; break;
            case '<': t->tipo = TOKEN_LE; break;
            case '>': t->tipo = TOKEN_GE; break;
            case '!': t->tipo = TOKEN_NOT; break;
            case '~': t->tipo = TOKEN_BNOT; break;
            case '&': t->tipo = TOKEN_BAND; break;
            case '|': t->tipo = TOKEN_BOR; break;
            case '^': t->tipo = TOKEN_BXOR; break;
            case '(': t->tipo = TOKEN_LPAREN; break;
            case ')': t->tipo = TOKEN_RPAREN; break;
            case ',': t->tipo = TOKEN_COMMA; break;
            case '=': t->tipo = TOKEN_ATRIB; break;
            default: t->tipo = TOKEN_ERROR; break;
        }
        p++;
    }
}

// Consome comentários
void consumeComment() {
    while (*p != '\n' && *p != '\0') p++;
}

int yylex() {
    token t;

    while (isspace(*p)) p++;

    if (*p == '\0') return TOKEN_NL;

    // Comentário
    if (verifyComment(*p)) {
        consumeComment();
        return yylex();
    }

    // Número
    if (verifyNumber(*p)) {
        consumeNumbers(&t);
        if (t.tipo == TOKEN_INT) {
            yylval.intval = t.u.inteiro; // Atribui valor inteiro ao yylval
        } else if (t.tipo == TOKEN_DOUBLE) {
            yylval.doubleval = t.u.duplo; // Atribui valor double ao yylval
        }
        return t.tipo;
    }

    // Nome
    if (verifyName(*p)) {
        consumeNames(&t);
        if (t.tipo == TOKEN_NAME){yylval.nome = strdup(t.u.nome);}
        return t.tipo;
    }

    // Operador
    if (verifyOperator(*p)) {
        consumeOperators(&t);
        return t.tipo;
    }

    // Caractere inválido
    if (verifyInvalid(*p)) {
        return TOKEN_ERROR;
    }

    return TOKEN_ERROR;
}

int main() {
    static char linha[MAX_LINE];

    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        p = linha;
        while (*p != '\0') {
            int token = yylex();
            if (token == TOKEN_ERROR) {
                printf("Erro encontrado na entrada!\n");
                return 1;
            }
        }
    }
    return 0;
}