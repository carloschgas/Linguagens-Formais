# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -g

# Arquivos de origem
SRC = lexer.c exprnode.c main.c

# Arquivo do parser
PARSER_SRC = parser.y

# Arquivo objeto do parser
PARSER_OBJ = parser.o

# Arquivos objeto
OBJ = $(SRC:.c=.o) $(PARSER_OBJ)

# Nome do executável
EXEC = my_program

# Regra padrão
all: $(EXEC)

# Regra para criar o executável
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para compilar o parser.y

# Regra para compilar os arquivos .c
%.o: %.c parser.tab.h
	$(CC) $(CFLAGS) -c $<

# Limpar arquivos gerados
clean:
	rm -f $(OBJ) $(EXEC) parser.tab.c parser.tab.h