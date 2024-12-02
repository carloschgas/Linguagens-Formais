#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"   // Inclua o cabeçalho do lexer
#include "parser.tab.h"  // Inclua o cabeçalho do parser
#include "exprnode.h" // Inclua o cabeçalho do ExprNode

extern int yyparse(); // Declaração da função yyparse
extern void yyerror(const char *s); // Declaração da função de erro

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