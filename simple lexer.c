//Carlos Chagas Oliveira Filho - 122178456

//Nesse trabalho admiti que uma vez que o erro fosse encontrado, o lexer deveria parar de ler.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    COMMA
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

void gettoken(char **p);            // verifica se o próximo char é nulo, se nao passa pro próximo
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

void consumeNumbers(char **p, char number[256], int *numIndex,token*t);     // enquanto existirem algarismos, por conta da verifyNumbers armazena todos esses em um vetor
                                                                            // depois verifica se tem ponto ou 'e' por conta de doubles, quando encontrar '\n'
                                                                            // faz a verificacao se realmente pode ser double (numeros antes e depois do .)
                                                                            // e faz as devidas atribuicoes ao token --> tipo (tag) e valor pra union

int consumeNames(char **p, char name[MAX_NAME], int *nameIndex, token *t);  // enquanto existirem letras [menor que len 16], por conta de verifyName armazena todos esses em um vetor
                                                                            // quando encontrar o final '\n', ele verifica o tamanho com verifySize
                                                                            // se validar, verifica se é uma palavra reservada, strcmp == pow || POW || sqrt || SQRT
                                                                            // se for, atribui --> tipo(tag) correspondente
                                                                            // se nao, atribui --> valor pra union e tipo(tag)

void consumeOperators(char **p, token *t);                                  // quando o p for igual a um operador da lista, ele ira consumir automaticamente o caractere com *p+
                                                                            // e ira atribuir ao tipo(tag) correspondente.
                                                                            // sao cuidados operadores de len(2) [tais como <=], consumindo 2 caracteres com *p +=2 

void consumeComment (char **p);                                             // consome o '#' e todo o resto ate encontrar uma quebra de linha ou \0.

//---------------------------------------

const char* tagToString(tag t); // converte o valor das tags do enum para uma string, utilizado para printar em consumeNumbers, operators e names, e na funcao main.

int main(int argc, char *argv[]){
    
    token t;
    static char linha[2048];

    char *p = NULL;
    char number[256], name[256];
    int numIndex = 0, nameIndex = 0;
    int numLinha = 0;

    while (fgets(linha, sizeof(linha), stdin) != NULL){
        numLinha++;

        if (!verifySize(linha, MAX_LINE)){
            printf("Erros\n-------\nLinha %d Excede 256\n", numLinha);
            exit(1);
        }

        p = linha;

        while (*p != '\0'){
            if (verifyNewLine(*p)){
                t.tipo = NL;
                printf("%s", tagToString(t.tipo));
                p++;
                continue;
            }

            if (verifyNumber(*p)) consumeNumbers(&p, number, &numIndex, &t);
            
            else if (verifyName(*p)){
                if (consumeNames(&p, name, &nameIndex, &t) == -1){
                    printf("Erros\n-------\nLinha %d - NAME excede 16\n", numLinha);
                    exit(1);
                }
            }
            else if (verifyOperator(*p)) consumeOperators(&p, &t);
            else if (verifyComment(*p)) consumeComment(&p);
            else if (verifyInvalid(*p)){
                printf("Erros\n-------\nLinha %d - Digito Invalido\n", numLinha);
                exit(1);
            }
            else gettoken(&p);
        }

        printf("\n");
    }

    return 0;
}

void gettoken(char **p){
    if (p == NULL || *p == NULL) return;
    (*p)++;
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
        p++;
    }

    if (*p == '.'){
        p++;
        while (verifyNumber(*p)){
            depois = 1;
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

void consumeNumbers(char **p, char number[256], int *numIndex, token *t){
    *numIndex = 0;

    while (verifyNumber(**p)){
        number[(*numIndex)++] = **p;
        (*p)++;
    }
    
    number[*numIndex] = '\0';

    if (**p == '.'){
        number[(*numIndex)++] = **p;
        (*p)++;

        while (verifyNumber(**p)){
            number[(*numIndex)++] = **p;
            (*p)++;
        }
    }

    if (**p == 'e' || **p == 'E'){
        number[(*numIndex)++] = **p;
        (*p)++;
        
        if (**p == '+' || **p == '-'){
            number[(*numIndex)++] = **p;
            (*p)++;
        }

        while (verifyNumber(**p)){
            number[(*numIndex)++] = **p;
            (*p)++;
        }
    }

    number[*numIndex] = '\0';

    if (verifyDouble(number)){
        t->u.duplo = atof(number);
        t->tipo = DOUBLE;
        
        printf("%s\t%.6f\n",tagToString(t->tipo), t->u.duplo);
    }
    
    else {
        t->tipo = INT;
        t->u.inteiro = atoi(number);
        
        printf("%s\t%d\n", tagToString(t->tipo), t->u.inteiro);
        
    }
}

int consumeNames(char **p, char name[MAX_NAME], int *nameIndex, token *t){
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
        printf("%s\n", tagToString(SQRT));
       }

       else if (verifySpecialNames(name) == 2){
        t->tipo = POW;
        printf("%s\n", tagToString(POW));
       }

        else {
            t->tipo = NAME;
            strncpy(t->u.nome, name, MAX_NAME);
            printf("%s\t%s\n", tagToString(NAME),name);
        }
        
        return 1;
    }
    return -1;
}

void consumeOperators(char **p, token *t) {
    if (**p == '<' && *(*p + 1) == '=') {
        t->tipo = LEQ;
        printf("%s\n", tagToString(t->tipo));
        (*p) += 2;
    } else if (**p == '>' && *(*p + 1) == '=') {
        t->tipo = GEQ;
        printf("%s\n", tagToString(t->tipo));
        (*p) += 2;
    } else if (**p == '=' && *(*p + 1) == '=') {
        t->tipo = EQ;
        printf("%s\n", tagToString(t->tipo));
        (*p) += 2;
    } else if (**p == '!' && *(*p + 1) == '=') {
        t->tipo = NEQ;
        printf("%s\n", tagToString(t->tipo));
        (*p) += 2;
    } else if (**p == '&' && *(*p + 1) == '&') {
        t->tipo = AND;
        printf("%s\n", tagToString(t->tipo));
        (*p) += 2;
    } else if (**p == '|' && *(*p + 1) == '|') {
        t->tipo = OR;
        printf("%s\n", tagToString(t->tipo));
        (*p) += 2;
    } else if (**p == '<' && *(*p + 1) == '<') {
        t->tipo = LSHIFT;
        printf("%s\n", tagToString(t->tipo));
        (*p) += 2;
    } else if (**p == '>' && *(*p + 1) == '>') {
        t->tipo = RSHIFT;
        printf("%s\n", tagToString(t->tipo));
        (*p) += 2;
    } else {
        switch (**p) {
            case '+':
                t->tipo = ADD;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '-':
                t->tipo = SUB;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '*':
                t->tipo = MUL;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '/':
                t->tipo = DIV;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '%':
                t->tipo = MOD;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '<':
                t->tipo = LE;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '>':
                t->tipo = GE;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '!':
                t->tipo = NOT;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '~':
                t->tipo = BNOT;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '&':
                t->tipo = BAND;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '|':
                t->tipo = BOR;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '^':
                t->tipo = BXOR;
                printf("%s\n", tagToString(t->tipo));
                break;
            case '(':
                t->tipo = LPAREN;
                printf("%s\n", tagToString(t->tipo));
                break;
            case ')':
                t->tipo = RPAREN;
                printf("%s\n", tagToString(t->tipo));
                break;
            case ',':
                t->tipo = COMMA;
                printf("%s\n", tagToString (t->tipo));
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

const char* tagToString(tag t) {
    switch (t) {
        case INT: return "INT";
        case DOUBLE: return "DOUBLE";
        case NAME: return "NAME";
        case LEQ: return "LEQ";
        case GEQ: return "GEQ";
        case EQ: return "EQ";
        case NEQ: return "NEQ";
        case AND: return "AND";
        case OR: return "OR";
        case LSHIFT: return "LSHIFT";
        case RSHIFT: return "RSHIFT";
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case MOD: return "MOD";
        case LE: return "LE";
        case GE: return "GE";
        case NOT: return "NOT";
        case BNOT: return "BNOT";
        case BAND: return "BAND";
        case BOR: return "BOR";
        case BXOR: return "BXOR";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case ATRIB: return "ATRIB";
        case NL: return "NL";
        case SQRT: return "SQRT";
        case POW: return "POW";
        case COMMA: return "COMMA";
        default: 
            "ERRO - TAG INEXISTENTE NO ENUM";
            exit(1);
    }
}