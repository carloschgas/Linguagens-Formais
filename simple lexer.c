//Nesse trabalho admiti que uma vez que o erro fosse encontrado, o lexer deveria parar de ler.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 256
#define MAX_NAME 16

typedef enum
{
    INT,
    DOUBLE,
    NAME,
    LEQ,
    GEQ,
    EQ,
    NEQ,
    AND,
    OR,
    LSHIFT,
    RSHIFT,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    LE,
    GE,
    NOT,
    BNOT,
    BAND,
    BOR,
    BXOR,
    LPAREN,
    RPAREN,
    ATRIB,
    NL,
    SQRT,
    POW,
    COMMA,
    ERROR
} tag;

typedef struct
{
    tag tipo;
    union
    {
        int inteiro;
        double duplo;
        char nome[MAX_NAME];
    } u;
} token;


// Protótipos e Comentários das funções
//---------------------------------------

token gettoken();            // verifica se o próximo char é nulo, se nao passa pro próximo
int verifyNumber(char p);           // verifica se o caractere é digito 0-9
int verifyName(char p);             // verifica se o caractere é letra a-z ou A-Z
int verifyOperator(char p);         // verifica se o caractere é algum operador do enunciado do trabalho
int verifyNewLine(char p);          // verifica se é '\n'
int verifySpecialNames(char name[MAX_NAME]); // verifica se é palavra palavra reservada é == sqrt ou == pow (maiusculas inclusas)
int verifyDouble(char *p);          // verifica se é um numero float a partir da existencia de um '.' alem de validacoes com numeros antes e depois dele. admite um caractere 'e' 'E' opcional
int verifySize(char *p, int MAX);   // verifica se o strlen < MAX
int verifyComment(char p);          // verifica se tem '#'
int verifyInvalid(char p);          // verifica se é caractere invalido
                                    //      considerei como invalido os digitos que nao estavam no exemplo de entrada e saida ou nem na tabela de operadores
                                    //      estes sao --> ! @ $ [ ] { } ; : ' " " `


//---------------------------------------

void consumeNumbers(char **p, char number[MAX_LINE], int *numIndex,token*t);

int consumeNames(char **p, char name[MAX_NAME], int *nameIndex, token *t);  // enquanto existirem letras [menor que len 16], por conta de verifyName armazena todos esses em um vetor
                                                                            // quando encontrar o final '\n', ele verifica o tamanho com verifySize
                                                                            // se validar, verifica se é uma palavra reservada, strcmp == pow || POW || sqrt || SQRT
                                                                            // se for, atribui --> tipo(tag) correspondente
                                                                            // se nao, atribui --> valor pra union e tipo(tag)

void consumeOperators(char **p, token *t);                                  // quando o p for igual a um operador da lista, ele ira consumir automaticamente o caractere com *p+
                                                                            // e ira atribuir ao tipo(tag) correspondente.
                                                                            // sao cuidados operadores de len(2) [tais como <=], consumindo 2 caracteres com *p +=2 

void consumeComment (char **p);                                             // consome o '#' e todo o resto ate encontrar uma quebra de linha ou \0.

void print_token(token t);
//---------------------------------------


int main() {
    static char linha[MAX_LINE];
    char *p;
    token t;

    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        p = linha;

        while (*p != '\0') {
            t = gettoken(&p);
            if (t.tipo == ERROR) {
                printf("Erro na linha: %s\n", linha);
                return 1; // Interromper ao encontrar erro
            }
            print_token(t);
        }
    }

    return 0;
}


token gettoken(char **p) {
    token t;
    char number[MAX_LINE] = {0}, name[MAX_NAME] = {0};
    int numIndex = 0, nameIndex = 0;

    //whitespace
    while (isspace(**p) && **p != '\n') (*p)++;

    //eof
    if (**p == '\0' || **p == '\n') {
        t.tipo = NL;
        if (**p == '\n') (*p)++;
        return t;
    }

    //comentários
    if (verifyComment(**p)) {
        consumeComment(p);
        return gettoken(p);
    }

    //números
    if (verifyNumber(**p)) {
        consumeNumbers(p, number, &numIndex, &t);
        return t;
    }

    //nomes
    if (verifyName(**p)) {
        if (consumeNames(p, name, &nameIndex, &t)) {
            return t;
        } else {
            t.tipo = ERROR;
            return t;
        }
    }

    //operadores
    if (verifyOperator(**p)) {
        consumeOperators(p, &t);
        return t;
    }

    //invalido
    if (verifyInvalid(**p)) {
        t.tipo = ERROR;
        return t;
    }


    t.tipo = ERROR;
    return t;
}


int verifyNumber(char p){
    return (p >= '0' && p <= '9');
}

int verifyName(char p){
    return ((p >= 'a' && p <= 'z') || (p >= 'A' && p <= 'Z'));
}

int verifyNewLine(char p){
    return (p == '\n');
}

int verifyOperator(char p){
    switch (p){
    case '+':
    case '-':
    case ',':
    case '*':
    case '/':
    case '%':
    case '<':
    case '>':
    case '=':
    case '!':
    case '&':
    case '|':
    case '^':
    case '~':
    case '(':
    case ')':
        return 1;
    default:
        return 0;
    }
}

int verifySpecialNames(char name[MAX_NAME]){
    if ((strcmp(name, "sqrt") == 0) || (strcmp(name, "SQRT") == 0)) return 1;
    else if ((strcmp(name, "pow") == 0) || (strcmp(name, "POW") == 0)) return 2;
    else return -1;
}

int verifyDouble(char *p){
    int antes = 0, depois = 0;

    while (verifyNumber(*p)){
        antes = 1;
        printf("numero antes verificado\n");
        p++;
    }

    if (*p == '.'){
        p++;
        while (verifyNumber(*p)){
            depois = 1;
            printf("numero depois verificado\n");
            p++;
        }
    }

    if (*p == 'e' || *p == 'E'){
        p++;
        if (*p == '+' || *p == '-')
            p++;
        while (verifyNumber(*p))
            p++;
    }

    return (antes && depois);
}

int verifySize(char *p, int MAX){
    int len = strlen(p);
    return len <= MAX;
}

int verifyComment (char p){
    return (p == '#');
}

int verifyInvalid (char p){
    return (p == '!' || p == '@' || p == '$' || 
            p == ';' || p == ':' || p == '{' || 
            p == '}' || p == '[' || p == ']' || 
            p == '`' || p == '"');
}
void consumeNumbers(char **p, char number[MAX_LINE], int *numIndex, token *t) {
    while (verifyNumber(**p)) {
        number[(*numIndex)++] = **p;
        (*p)++;
    }

    // Verificar se é double
    if (**p == '.' || **p == 'e' || **p == 'E') {
        printf("debug1\n");
        number[(*numIndex)++] = **p;
        (*p)++;
        if (verifyDouble(*p)) {

            printf("debug2\n");
            t->tipo = DOUBLE;
            t->u.duplo = strtod(number, NULL);
            return;
        }
    }

    t->tipo = INT;
    t->u.inteiro = strtol(number, NULL, 10);
}


int consumeNames(char **p, char name[MAX_NAME], int *nameIndex, token *t)
{
    *nameIndex = 0;
    
    while (verifyName(**p)){
        if (*nameIndex < MAX_NAME - 1){
            name[(*nameIndex)++] = **p;
            (*p)++;
        }
        
        else return -1;
        
    }

    name[*nameIndex] = '\0';

    if (verifySize(name, MAX_NAME)){
        
       if (verifySpecialNames(name) == 1){
        t->tipo = SQRT;
       }

       else if (verifySpecialNames(name) == 2){
        t->tipo = POW;
       }

        else {
            t->tipo = NAME;
            strncpy(t->u.nome, name, MAX_NAME);
        }
        
        return 1;
    }
    return -1;
}

void consumeOperators(char **p, token *t) {
    if (**p == '<' && *(*p + 1) == '=') {
        t->tipo = LEQ;
        (*p) += 2;
    } else if (**p == '>' && *(*p + 1) == '=') {
        t->tipo = GEQ;
        (*p) += 2;
    } else if (**p == '=' && *(*p + 1) == '=') {
        t->tipo = EQ;
        (*p) += 2;
    } else if (**p == '!' && *(*p + 1) == '=') {
        t->tipo = NEQ;
        (*p) += 2;
    } else if (**p == '&' && *(*p + 1) == '&') {
        t->tipo = AND;
        (*p) += 2;
    } else if (**p == '|' && *(*p + 1) == '|') {
        t->tipo = OR;
        (*p) += 2;
    } else if (**p == '<' && *(*p + 1) == '<') {
        t->tipo = LSHIFT;
        (*p) += 2;
    } else if (**p == '>' && *(*p + 1) == '>') {
        t->tipo = RSHIFT;
        (*p) += 2;
    } else {
        switch (**p) {
            case '+':
                t->tipo = ADD;
                break;
            case '-':
                t->tipo = SUB;
                break;
            case '*':
                t->tipo = MUL;
                break;
            case '/':
                t->tipo = DIV;
                break;
            case '%':
                t->tipo = MOD;
                break;
            case '<':
                t->tipo = LE;
                break;
            case '>':
                t->tipo = GE;
                break;
            case '!':
                t->tipo = NOT;
                break;
            case '~':
                t->tipo = BNOT;
                break;
            case '&':
                t->tipo = BAND;
                break;
            case '|':
                t->tipo = BOR;
                break;
            case '^':
                t->tipo = BXOR;
                break;
            case '(':
                t->tipo = LPAREN;
                break;
            case ')':
                t->tipo = RPAREN;
                break;
            case ',':
                t->tipo = COMMA;
                break;
            case '=':
                t->tipo = ATRIB;
            default:
                break;
        }
        (*p)++;
    }
}

void consumeComment(char **p){
    (*p)++;
    while (**p != '\n' && **p != '\0') (*p)++;
}



void print_token(token t) {
    switch (t.tipo) {
        case NL: printf("NL\n"); break;
        case NAME: printf("NAME\t%s\n", t.u.nome); break;
        case SQRT: printf("SQRT\n"); break;
        case POW: printf("POW\n"); break;
        case INT: printf("INT\t%ld\n", t.u.inteiro); break;
        case DOUBLE: printf("DOUBLE\t%f\n", t.u.duplo); break;
        case ADD: printf("ADD\n"); break;
        case SUB: printf("SUB\n"); break;
        case MUL: printf("MUL\n"); break;
        case DIV: printf("DIV\n"); break;
        case MOD: printf("MOD\n"); break;
        case LE: printf("LE\n"); break;
        case LEQ: printf("LEQ\n"); break;
        case GE: printf("GE\n"); break;
        case GEQ: printf("GEQ\n"); break;
        case EQ: printf("EQ\n"); break;
        case NEQ: printf("NEQ\n"); break;
        case NOT: printf("NOT\n"); break;
        case AND: printf("AND\n"); break;
        case OR: printf("OR\n"); break;
        case BNOT: printf("BNOT\n"); break;
        case BAND: printf("BAND\n"); break;
        case BOR: printf("BOR\n"); break;
        case BXOR: printf("BXOR\n"); break;
        case LSHIFT: printf("LSHIFT\n"); break;
        case RSHIFT: printf("RSHIFT\n"); break;
        case LPAREN: printf("LPAREN\n"); break;
        case RPAREN: printf("RPAREN\n"); break;
        case ATRIB: printf("ATRIB\n"); break;
        case COMMA: printf("COMMA\n"); break;
        default: break; 
    }
}
