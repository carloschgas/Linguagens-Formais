/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_INT = 258,
     TOKEN_DOUBLE = 259,
     TOKEN_NAME = 260,
     TOKEN_ATRIB = 261,
     TOKEN_ADD = 262,
     TOKEN_SUB = 263,
     TOKEN_MUL = 264,
     TOKEN_DIV = 265,
     TOKEN_MOD = 266,
     TOKEN_OR = 267,
     TOKEN_AND = 268,
     TOKEN_EQ = 269,
     TOKEN_NEQ = 270,
     TOKEN_LT = 271,
     TOKEN_LTE = 272,
     TOKEN_GT = 273,
     TOKEN_GTE = 274,
     TOKEN_SHL = 275,
     TOKEN_SHR = 276,
     TOKEN_BITOR = 277,
     TOKEN_BITXOR = 278,
     TOKEN_BITAND = 279,
     TOKEN_LPAREN = 280,
     TOKEN_RPAREN = 281,
     UNARY_OP = 282,
     TOKEN_SQRT = 283,
     TOKEN_POW = 284,
     TOKEN_LEQ = 285,
     TOKEN_GEQ = 286,
     TOKEN_LSHIFT = 287,
     TOKEN_RSHIFT = 288,
     TOKEN_LE = 289,
     TOKEN_GE = 290,
     TOKEN_NOT = 291,
     TOKEN_BNOT = 292,
     TOKEN_BAND = 293,
     TOKEN_BOR = 294,
     TOKEN_BXOR = 295,
     TOKEN_COMMA = 296,
     TOKEN_ERROR = 297,
     TOKEN_NL = 298
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


