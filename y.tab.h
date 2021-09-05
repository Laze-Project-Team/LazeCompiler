/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CHAR = 258,
    ID = 259,
    STRING = 260,
    INT = 261,
    REAL = 262,
    COMMA = 263,
    COLON = 264,
    SEMICOLON = 265,
    LPAREN = 266,
    RPAREN = 267,
    LBRACK = 268,
    RBRACK = 269,
    LBRACE = 270,
    RBRACE = 271,
    DOT = 272,
    PLUS = 273,
    MINUS = 274,
    TIMES = 275,
    DIVIDE = 276,
    EQ = 277,
    NEQ = 278,
    LT = 279,
    LE = 280,
    GT = 281,
    GE = 282,
    AND = 283,
    OR = 284,
    ASSIGN = 285,
    IF = 286,
    THEN = 287,
    ELSE = 288,
    FROM = 289,
    TO = 290,
    BREAK = 291,
    INTTYPE = 292,
    REALTYPE = 293,
    CONTINUE = 294,
    RETURN = 295,
    TYPE = 296,
    VOID = 297,
    NUL = 298,
    TRUEE = 299,
    FALSEE = 300,
    BOOLEAN = 301,
    CHARTYPE = 302,
    MOD = 303,
    AMPERSAND = 304,
    SHORTTYPE = 305,
    FUNCTION = 306,
    ARROW = 307,
    LOOP = 308,
    JSLOAD = 309,
    SIZEOF = 310,
    CLASS = 311,
    LEFTARROW = 312,
    PRIVATE = 313,
    PUBLIC = 314,
    PROTECTED = 315,
    RIGHTARROW = 316,
    REPEAT = 317,
    JSEXPORT = 318,
    UMINUS = 319,
    LOWER_THAN_ELSE = 320
  };
#endif
/* Tokens.  */
#define CHAR 258
#define ID 259
#define STRING 260
#define INT 261
#define REAL 262
#define COMMA 263
#define COLON 264
#define SEMICOLON 265
#define LPAREN 266
#define RPAREN 267
#define LBRACK 268
#define RBRACK 269
#define LBRACE 270
#define RBRACE 271
#define DOT 272
#define PLUS 273
#define MINUS 274
#define TIMES 275
#define DIVIDE 276
#define EQ 277
#define NEQ 278
#define LT 279
#define LE 280
#define GT 281
#define GE 282
#define AND 283
#define OR 284
#define ASSIGN 285
#define IF 286
#define THEN 287
#define ELSE 288
#define FROM 289
#define TO 290
#define BREAK 291
#define INTTYPE 292
#define REALTYPE 293
#define CONTINUE 294
#define RETURN 295
#define TYPE 296
#define VOID 297
#define NUL 298
#define TRUEE 299
#define FALSEE 300
#define BOOLEAN 301
#define CHARTYPE 302
#define MOD 303
#define AMPERSAND 304
#define SHORTTYPE 305
#define FUNCTION 306
#define ARROW 307
#define LOOP 308
#define JSLOAD 309
#define SIZEOF 310
#define CLASS 311
#define LEFTARROW 312
#define PRIVATE 313
#define PUBLIC 314
#define PROTECTED 315
#define RIGHTARROW 316
#define REPEAT 317
#define JSEXPORT 318
#define UMINUS 319
#define LOWER_THAN_ELSE 320

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 20 "lang.y"

    int pos;
    int shortt;
    char cval[3];
    long long ival;
    double fval;
    string sval;
    A_var var;
    A_exp exp;
    S_symbol sym;
    S_symbolList symlist;
    A_expList expList;
    A_stm stm;
    A_stmList stmList;
    A_ty type;
    A_dec dec;
    A_fundec funcdec;
    A_fieldList fieldList;
    A_decList decList;
    A_classMemberList memList;

#line 209 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
