##Description

This project implements the lexical analyzer (lexer) for a simple calculator language inspired by the operators of the C programming language.

The goal of lexical analysis is to read the input and break it into tokens, which are the basic “words” of the program.
At this stage, only tokenization is required — no syntax or semantic validation of expressions is performed.


#Comment

x = 1 + 20 # another comment

2 >> x

sqrt(1.0e-2)

## Output Format

For each token, print one line containing:

CATEGORY<TAB>VALUE


Separate category and value using a tab character (\t)

Do not print anything for spaces or comments

Example Output

NAME	x

ATRIB

INT	1

ADD

INT	20

NL

INT	2

RSHIFT

NAME	x

NL

SQRT

LPAREN

DOUBLE	0.010000

RPAREN

NL
