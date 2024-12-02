#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "lexer.h"
#include "parser.tab.h"

typedef struct ExprNode {
    int tipo; // Tipo do token
    struct ExprNode *left;
    struct ExprNode *right;
    token value; // Valor do token
} ExprNode;

// Funções para manipular ExprNode
ExprNode* createNode(int tipo, token value, ExprNode* left, ExprNode* right);
void printExpression(ExprNode* node);
void freeExpression(ExprNode* node);

#endif // EXPRNODE_H