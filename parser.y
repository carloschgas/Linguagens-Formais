%{
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.tab.h"
#include "exprnode.h" // Incluindo o cabeçalho do ExprNode

%union {
        int intval;
        double doubleval;
        char *nome;
};

void yyerror(const char *s);
%}

// Tokens definidos em lexer.h

%token <intval> TOKEN_INT
%token <doubleval> TOKEN_DOUBLE
%token <nome> TOKEN_NAME
%token <u>  TOKEN_ATRIB TOKEN_ADD TOKEN_SUB TOKEN_MUL TOKEN_DIV TOKEN_MOD
%token <u> TOKEN_OR TOKEN_AND TOKEN_EQ TOKEN_NEQ TOKEN_LT TOKEN_LTE TOKEN_GT TOKEN_GTE
%token <u> TOKEN_SHL TOKEN_SHR TOKEN_BITOR TOKEN_BITXOR TOKEN_BITAND TOKEN_LPAREN TOKEN_RPAREN
%token <u> UNARY_OP TOKEN_SQRT TOKEN_POW TOKEN_LEQ TOKEN_GEQ TOKEN_LSHIFT TOKEN_RSHIFT
%token <u> TOKEN_LE TOKEN_GE TOKEN_NOT TOKEN_BNOT TOKEN_BAND TOKEN_BOR TOKEN_BXOR TOKEN_COMMA
%token <u> TOKEN_ERROR TOKEN_NL

// Definições de precedência
%left TOKEN_OR TOKEN_AND
%left TOKEN_EQ TOKEN_NEQ TOKEN_LT TOKEN_LTE TOKEN_GT TOKEN_GTE
%left TOKEN_SHL TOKEN_SHR
%left TOKEN_ADD TOKEN_SUB
%left TOKEN_MUL TOKEN_DIV TOKEN_MOD
%left TOKEN_BITOR TOKEN_BITXOR TOKEN_BITAND
%right UNARY_OP // Para operadores unários
%right TOKEN_NOT TOKEN_BNOT // Operadores unários
%right TOKEN_SQRT TOKEN_POW // Funções

%type <node> expression assignment

%%

// Regras de gramática
program:
    | program assignment
    ;

assignment:
    TOKEN_NAME TOKEN_ATRIB expression {
        printf("Atribuição: %s = ", $1.u.nome);
        printExpression($3);
        printf("\n");
        freeExpression($3);
    }
    ;

expression:
    TOKEN_INT {
        $$ = createNode(TOKEN_INT, $1, NULL, NULL);
    }
    | TOKEN_DOUBLE {
        $$ = createNode(TOKEN_DOUBLE, $1, NULL, NULL);
    }
    | TOKEN_NAME {
        $$ = createNode(TOKEN_NAME, $1, NULL, NULL);
    }
    | TOKEN_LPAREN expression TOKEN_RPAREN {
        $$ = $2; // Retorna a expressão dentro dos parênteses
    }
    | expression TOKEN_ADD expression {
        token op;
        op.tipo = TOKEN_ADD;
        $$ = createNode(TOKEN_ADD, op, $1, $3);
    }
    | expression TOKEN_SUB expression {
        token op;
        op.tipo = TOKEN_SUB;
        $$ = createNode(TOKEN_SUB, op, $1, $3);
    }
    | expression TOKEN_MUL expression {
        token op;
        op.tipo = TOKEN_MUL;
        $$ = createNode(TOKEN_MUL, op, $1, $3);
    }
    | expression TOKEN_DIV expression {
        token op;
        op.tipo = TOKEN_DIV;
        $$ = createNode(TOKEN_DIV, op, $1, $3);
    }
    | expression TOKEN_MOD expression {
        token op;
        op.tipo = TOKEN_MOD;
        $$ = createNode(TOKEN_MOD, op, $1, $3);
    }
    | expression TOKEN_OR expression {
        token op;
        op.tipo = TOKEN_OR;
        $$ = createNode(TOKEN_OR, op, $1, $3);
    }
    | expression TOKEN_AND expression {
        token op;
        op.tipo = TOKEN_AND;
        $$ = createNode(TOKEN_AND, op, $1, $3);
    }
    | expression TOKEN_EQ expression {
        token op;
        op.tipo = TOKEN_EQ;
        $$ = createNode(TOKEN_EQ, op, $1, $3);
    }
    | expression TOKEN_NEQ expression {
        token op;
        op.tipo = TOKEN_NEQ;
        $$ = createNode(TOKEN_NEQ, op, $1, $3);
    }
    | expression TOKEN_LT expression {
        token op;
        op.tipo = TOKEN_LT;
        $$ = createNode(TOKEN_LT, op, $1, $3);
    }
    | expression TOKEN_LTE expression {
        token op;
        op.tipo = TOKEN_LTE;
        $$ = createNode(TOKEN_LTE, op, $1, $3);
    }
    | expression TOKEN_GT expression {
        token op;
        op.tipo = TOKEN_GT;
        $$ = createNode(TOKEN_GT , op, $1, $3);
    }
    | expression TOKEN_GTE expression {
        token op;
        op.tipo = TOKEN_GTE;
        $$ = createNode(TOKEN_GTE, op, $1, $3);
    }
    | TOKEN_NOT expression {
        token op;
        op.tipo = TOKEN_NOT;
        $$ = createNode(TOKEN_NOT, op, $2, NULL);
    }
    | TOKEN_BNOT expression {
        token op;
        op.tipo = TOKEN_BNOT;
        $$ = createNode(TOKEN_BNOT, op, $2, NULL);
    }
    | expression TOKEN_SHL expression {
        token op;
        op.tipo = TOKEN_SHL;
        $$ = createNode(TOKEN_SHL, op, $1, $3);
    }
    | expression TOKEN_SHR expression {
        token op;
        op.tipo = TOKEN_SHR;
        $$ = createNode(TOKEN_SHR, op, $1, $3);
    }
    | expression TOKEN_BITOR expression {
        token op;
        op.tipo = TOKEN_BITOR;
        $$ = createNode(TOKEN_BITOR, op, $1, $3);
    }
    | expression TOKEN_BITXOR expression {
        token op;
        op.tipo = TOKEN_BITXOR;
        $$ = createNode(TOKEN_BITXOR, op, $1, $3);
    }
    | expression TOKEN_BITAND expression {
        token op;
        op.tipo = TOKEN_BITAND;
        $$ = createNode(TOKEN_BITAND, op, $1, $3);
    }
    | TOKEN_SQRT expression {
        token op;
        op.tipo = TOKEN_SQRT;
        $$ = createNode(TOKEN_SQRT, op, $2, NULL);
    }
    | TOKEN_POW expression expression {
        token op;
        op.tipo = TOKEN_POW;
        $$ = createNode(TOKEN_POW, op, $2, $3);
    }
    ;

%% 

int main() {
    printf("Digite suas expressões (CTRL+D para sair):\n");
    while (1) {
        if (yyparse() != 0) {
            fprintf(stderr, "Erro ao analisar a expressão.\n");
        }
    }
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}