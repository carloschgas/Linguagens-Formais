#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include "exprnode.h"

ExprNode* createNode(int tipo, token value, ExprNode* left, ExprNode* right) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    node->tipo = tipo;
    node->value = value;
    node->left = left;
    node->right = right;
    return node;
}

void printExpression(ExprNode* node) {
    if (!node) return;
    if (node->tipo == TOKEN_NAME || node->tipo == TOKEN_INT || node->tipo == TOKEN_DOUBLE) {
        if (node->tipo == TOKEN_NAME) {
            printf("%s", node->value.u.nome);
        } else if (node->tipo == TOKEN_INT) {
            printf("%d", node->value.u.inteiro);
        } else if (node->tipo == TOKEN_DOUBLE) {
            printf("%f", node->value.u.duplo);
        }
    } else {
        printf("(");
        printExpression(node->left);
        printf(" %s ", node->value.u.nome);
        printExpression(node->right);
        printf(")");
    }
}

void freeExpression(ExprNode* node) {
    if (node) {
        freeExpression(node->left);
        freeExpression(node->right);
        free(node);
    }
}