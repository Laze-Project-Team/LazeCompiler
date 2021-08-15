/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "lang.y"

    #include <stdio.h>
    #include "util.h"
    #include "errormsg.h"
    #include "symbol.h"
    #include "absyn.h"
    #include "y.tab.h"
    #define YYDEBUG 1

    int yylex(void);
    A_decList absyn_root;

    void yyerror(const char *s)
    {
        EM_error(EM_tokPos, "%s", s);
    }

#line 88 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
    UMINUS = 318,
    LOWER_THAN_ELSE = 319
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
#define UMINUS 318
#define LOWER_THAN_ELSE 319

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

#line 290 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  26
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   885

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  114
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  254

#define YYUNDEFTOK  2
#define YYMAXUTOK   319


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    82,    82,    84,    85,    86,    87,    88,    89,    90,
      91,    93,    94,    96,    97,    98,    99,   100,   101,   102,
     104,   105,   106,   107,   108,   110,   111,   112,   113,   114,
     115,   116,   118,   119,   120,   121,   123,   124,   125,   126,
     128,   129,   130,   131,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   150,   151,   153,   154,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   166,
     167,   168,   170,   194,   218,   221,   222,   224,   226,   227,
     229,   230,   231,   232,   234,   235,   237,   238,   241,   242,
     244,   245,   246,   247,   248,   250,   251,   252,   253,   254,
     256,   257,   259,   261,   262,   265,   267,   268,   269,   271,
     273,   274,   275,   276,   277
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CHAR", "ID", "STRING", "INT", "REAL",
  "COMMA", "COLON", "SEMICOLON", "LPAREN", "RPAREN", "LBRACK", "RBRACK",
  "LBRACE", "RBRACE", "DOT", "PLUS", "MINUS", "TIMES", "DIVIDE", "EQ",
  "NEQ", "LT", "LE", "GT", "GE", "AND", "OR", "ASSIGN", "IF", "THEN",
  "ELSE", "FROM", "TO", "BREAK", "INTTYPE", "REALTYPE", "CONTINUE",
  "RETURN", "TYPE", "VOID", "NUL", "TRUEE", "FALSEE", "BOOLEAN",
  "CHARTYPE", "MOD", "AMPERSAND", "SHORTTYPE", "FUNCTION", "ARROW", "LOOP",
  "JSLOAD", "SIZEOF", "CLASS", "LEFTARROW", "PRIVATE", "PUBLIC",
  "PROTECTED", "RIGHTARROW", "REPEAT", "UMINUS", "LOWER_THAN_ELSE",
  "$accept", "program", "decs", "funcAndVar", "exp", "varExp", "stm",
  "stmlist", "return", "type", "tyfield", "tyfield1", "funcDec",
  "funcCall", "jsLoad", "funcvarlist", "memlist", "mems", "classDec",
  "templateDec", "assign", "declare", "if", "while", "for", "loop",
  "explist", "id", "lvalue", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319
};
# endif

#define YYPACT_NINF (-141)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-77)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     527,  -141,    16,  -141,  -141,   -16,  -141,  -141,  -141,  -141,
      17,    24,    12,  -141,   527,    37,  -141,   527,   527,   527,
    -141,    51,  -141,    18,    18,    18,  -141,  -141,    30,  -141,
    -141,  -141,    16,    45,    73,    -9,    30,    76,    81,    64,
      67,    88,   819,   280,    18,  -141,   -10,  -141,  -141,   537,
    -141,   472,    18,    46,    49,   537,    18,  -141,    34,    87,
      92,    95,  -141,    99,   102,   109,   110,   516,  -141,   280,
     104,   111,   537,  -141,  -141,  -141,  -141,   537,   537,   537,
    -141,  -141,  -141,    30,   113,   324,   118,   120,   -10,   124,
     702,  -141,  -141,   537,  -141,   537,   764,  -141,  -141,  -141,
      30,    78,    18,   516,   516,   516,  -141,  -141,  -141,   280,
     268,   121,   764,   -10,    30,   537,   537,    18,   537,   537,
     537,   537,   537,   537,   537,   537,   537,   537,   537,   537,
     537,    18,   537,   127,  -141,  -141,   764,   764,    -4,   131,
     133,  -141,  -141,  -141,   122,  -141,  -141,    -2,  -141,   719,
    -141,   781,   781,   420,   420,   809,   809,   809,   809,   809,
     809,   824,   824,   420,  -141,   135,  -141,    16,   819,   819,
    -141,  -141,  -141,  -141,  -141,   141,   143,   362,   108,   419,
     419,   834,   107,   151,   153,   156,   162,   419,   764,   164,
    -141,   168,  -141,   173,   175,  -141,  -141,  -141,  -141,  -141,
       7,   193,   178,   576,   179,   419,   180,   184,   537,  -141,
    -141,   537,  -141,   490,  -141,  -141,  -141,   819,    25,   165,
    -141,  -141,   196,   594,   639,  -141,   190,   191,   419,   419,
     197,   199,   181,  -141,   204,   419,  -141,  -141,   537,   211,
     419,   657,   212,   198,   195,   223,   419,   225,  -141,  -141,
     419,   227,   419,  -141
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,   109,     0,    62,    63,     0,    61,    64,    65,    66,
       0,     0,     0,     2,     3,     0,    12,     4,     5,     6,
      11,    60,    67,     0,     0,     0,     1,    10,     0,     9,
       8,     7,     0,     0,     0,     0,     0,     0,   110,     0,
       0,     0,    69,     0,     0,   110,   112,    95,    99,   108,
      96,     0,     0,     0,     0,     0,     0,    68,     0,    61,
       0,     0,    71,     0,     0,     0,     0,    78,    80,    84,
       0,     0,     0,    17,    14,    13,    15,     0,   108,     0,
      16,    18,    19,     0,     0,   107,    39,     0,    40,     0,
       0,   111,    91,     0,    92,     0,    90,   114,    88,    89,
       0,     0,     0,     0,     0,     0,    79,    85,    86,     0,
       0,     0,    25,    36,     0,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   108,     0,    97,   113,    93,    94,    72,     0,
       0,    83,    81,    82,     0,    33,    38,     0,   106,     0,
      41,    20,    21,    22,    23,    26,    27,    31,    30,    29,
      28,    34,    35,    24,    43,     0,    98,     0,    69,    69,
      87,    37,    42,    32,    73,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,     0,    39,
      74,     0,    44,     0,     0,    47,    48,    49,    50,    52,
     110,    40,     0,     0,     0,    56,     0,     0,     0,    53,
      54,     0,   105,     0,    51,    45,    46,    69,    33,     0,
      57,    55,     0,     0,     0,    75,     0,     0,     0,     0,
       0,     0,     0,    59,    32,     0,   102,   104,     0,     0,
       0,     0,     0,   100,     0,     0,     0,     0,    77,   101,
       0,     0,     0,   103
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -141,  -141,   208,   140,   183,  -140,   -64,    32,  -141,     3,
    -139,    77,   -31,  -141,  -141,    47,  -141,   -48,   185,  -141,
     217,  -107,  -141,  -141,  -141,  -141,   -77,     0,    13
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    67,   188,    86,   190,   206,   191,    15,
      61,    62,    16,   193,    17,    68,    69,    70,    18,    19,
     194,    20,   196,   197,   198,   199,    87,    45,    88
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      21,   111,    21,    72,   167,    22,    43,    52,    23,    72,
     171,    72,    26,    52,    21,    52,   -60,    21,    21,    21,
       1,   107,     1,    33,    34,    35,    24,    98,    38,   175,
     176,    32,    21,    25,     1,    40,     2,   189,   148,   189,
     189,    39,    21,    21,    71,    60,    28,   189,    44,    46,
      36,    56,    91,     3,     4,   165,    97,    56,     6,    56,
     228,   144,     7,     8,    92,   189,     9,    21,    94,    21,
     195,    41,   195,   195,    50,    32,    93,    51,   227,    95,
     195,    52,    53,    54,    42,    63,    47,   229,   189,   189,
      11,    48,    49,    57,    55,   189,   113,    58,   195,   -70,
     189,   100,   140,    21,    21,    21,   189,   101,   102,    21,
     189,   103,   189,   138,   106,   204,   205,   150,   104,   105,
     108,   195,   195,   212,   114,    56,   109,   147,   195,   132,
     139,   164,   133,   195,   134,   146,   226,   166,   170,   195,
      14,   205,   168,   195,   169,   195,   192,   173,   192,   192,
     141,   142,   143,   177,    14,   178,   192,    14,    14,    14,
     202,   207,   208,   209,   236,   237,   210,    21,    21,    21,
      60,    60,    60,   211,   192,   213,   243,   200,   214,   200,
     200,   200,   249,   215,    22,   216,   251,   200,   253,   217,
     201,   219,   201,   201,    46,   222,   221,   192,   192,   230,
     201,   231,   234,   235,   192,   200,    72,   239,   238,   192,
      52,    53,    54,   240,   -76,   192,   242,    21,   201,   192,
      60,   192,    27,    55,   245,    29,    30,    31,   200,   200,
     247,   246,    85,   248,    90,   200,   250,   220,    96,   252,
     200,   201,   201,    99,   174,    37,   200,     0,   201,     0,
     200,     0,   200,   201,    56,    90,     0,     0,     0,   201,
     110,    85,   112,   201,     0,   201,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   136,     0,   137,     0,
     145,   116,     0,     0,     1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,    85,   149,
       2,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,     0,    85,   130,     3,     4,     0,
       0,     0,     6,     0,     0,     0,     7,     8,     0,   131,
       9,    63,   115,     0,     0,     0,     0,   116,    64,    65,
      66,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,     0,     0,     0,     0,     0,     0,
       0,     0,   203,     0,     0,    73,     1,    74,    75,    76,
       0,     0,   130,   179,     0,    78,     0,   180,     0,     0,
       0,    79,   181,     0,     0,   131,     0,     0,     0,     0,
       0,   223,   182,   183,   224,     0,    85,     0,   184,     3,
       4,   185,   186,     0,     6,    80,    81,    82,     7,     8,
       0,    83,     9,    63,     0,   187,     0,    84,     0,     0,
       0,   241,    73,     1,    74,    75,    76,     0,     0,     0,
     179,     0,    78,   116,   180,     0,     0,   117,    79,   181,
       0,     0,   122,   123,   124,   125,   126,   127,   128,   129,
     183,     0,     0,     0,     0,   184,     3,     4,   185,   186,
       0,     6,    80,    81,    82,     7,     8,     0,    83,     9,
      63,     0,   187,     0,    84,    73,     1,    74,    75,    76,
       0,   131,     0,    77,     0,    78,    89,     0,     0,     0,
       0,    79,    36,    73,     1,    74,    75,    76,     0,     0,
       0,    77,   225,    78,     0,     0,     0,     0,     0,    79,
      36,     0,     0,     0,     0,    80,    81,    82,     0,     0,
       1,    83,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     1,     0,    80,    81,    82,     2,     0,     0,    83,
      73,     1,    74,    75,    76,    84,     0,     2,    77,     0,
      78,     0,     0,     3,     4,     0,    79,    36,     6,     0,
       0,     0,     7,     8,     3,     4,     9,    63,     5,     6,
       0,     0,     0,     7,     8,     0,     0,     9,    10,     0,
      80,    81,    82,    11,     0,     0,    83,     0,   218,   116,
       0,     0,    84,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   232,   116,     0,     0,
       0,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   131,     0,     0,
       0,     0,   130,     0,     0,     0,     0,     0,     0,     0,
       0,   233,   116,     0,     0,   131,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   244,
     116,     0,     0,     0,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     131,     0,     0,     0,     0,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   135,     0,   131,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   116,   172,     0,     0,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,     0,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131,     0,     0,     0,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,     0,     0,
     131,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   116,     0,     0,     0,   117,     0,
       0,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,     0,   130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,     1,     0,   131,   117,     0,     0,   130,
       0,   -77,   -77,   -77,   -77,   -77,   -77,   116,     1,     2,
       0,   117,   131,     0,     0,     0,   122,   123,   124,   125,
     126,   127,     0,     0,   181,     0,     3,     4,     0,     0,
       0,    59,     0,     0,     0,     7,     8,     0,     0,     9,
     131,     3,     4,     0,     0,     0,     6,     0,     0,     0,
       7,     8,     0,     0,     9,   131
};

static const yytype_int16 yycheck[] =
{
       0,    78,     2,    13,     8,     2,    15,    17,    24,    13,
      12,    13,     0,    17,    14,    17,     9,    17,    18,    19,
       4,    69,     4,    23,    24,    25,     9,    58,    28,   168,
     169,    24,    32,     9,     4,    32,    20,   177,   115,   179,
     180,    28,    42,    43,    44,    42,     9,   187,    57,    36,
      20,    61,    52,    37,    38,   132,    56,    61,    42,    61,
      35,   109,    46,    47,    18,   205,    50,    67,    19,    69,
     177,    26,   179,   180,    10,    24,    30,    13,   217,    30,
     187,    17,    18,    19,    11,    51,    10,    62,   228,   229,
      56,    10,    11,    26,    30,   235,    83,     9,   205,    12,
     240,     9,   102,   103,   104,   105,   246,    12,     9,   109,
     250,     9,   252,   100,    67,   179,   180,   117,     9,     9,
      16,   228,   229,   187,    11,    61,    15,   114,   235,    11,
      52,   131,    12,   240,    10,    14,   213,    10,    16,   246,
       0,   205,    11,   250,    11,   252,   177,    12,   179,   180,
     103,   104,   105,    12,    14,    12,   187,    17,    18,    19,
      52,    54,    11,    10,   228,   229,    10,   167,   168,   169,
     167,   168,   169,    11,   205,    11,   240,   177,    10,   179,
     180,   181,   246,    10,   181,    10,   250,   187,   252,    11,
     177,    12,   179,   180,   181,    11,    16,   228,   229,    34,
     187,     5,    12,    12,   235,   205,    13,     8,    11,   240,
      17,    18,    19,    32,    10,   246,     5,   217,   205,   250,
     217,   252,    14,    30,    12,    17,    18,    19,   228,   229,
      35,    33,    49,    10,    51,   235,    11,   205,    55,    12,
     240,   228,   229,    58,   167,    28,   246,    -1,   235,    -1,
     250,    -1,   252,   240,    61,    72,    -1,    -1,    -1,   246,
      77,    78,    79,   250,    -1,   252,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    95,    -1,
      12,    13,    -1,    -1,     4,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,   115,   116,
      20,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,    48,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    46,    47,    -1,    61,
      50,    51,     8,    -1,    -1,    -1,    -1,    13,    58,    59,
      60,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    48,    11,    -1,    13,    -1,    15,    -1,    -1,
      -1,    19,    20,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,   208,    30,    31,   211,    -1,   213,    -1,    36,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    -1,    55,    -1,    -1,
      -1,   238,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    -1,    13,    13,    15,    -1,    -1,    17,    19,    20,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      31,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    -1,    55,     3,     4,     5,     6,     7,
      -1,    61,    -1,    11,    -1,    13,    14,    -1,    -1,    -1,
      -1,    19,    20,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      20,    -1,    -1,    -1,    -1,    43,    44,    45,    -1,    -1,
       4,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,     4,    -1,    43,    44,    45,    20,    -1,    -1,    49,
       3,     4,     5,     6,     7,    55,    -1,    20,    11,    -1,
      13,    -1,    -1,    37,    38,    -1,    19,    20,    42,    -1,
      -1,    -1,    46,    47,    37,    38,    50,    51,    41,    42,
      -1,    -1,    -1,    46,    47,    -1,    -1,    50,    51,    -1,
      43,    44,    45,    56,    -1,    -1,    49,    -1,    12,    13,
      -1,    -1,    55,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    12,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    13,    -1,    -1,    61,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    12,
      13,    -1,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    14,    -1,    61,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,
      61,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    13,    -1,    -1,    -1,    17,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    13,     4,    -1,    61,    17,    -1,    -1,    48,
      -1,    22,    23,    24,    25,    26,    27,    13,     4,    20,
      -1,    17,    61,    -1,    -1,    -1,    22,    23,    24,    25,
      26,    27,    -1,    -1,    20,    -1,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    46,    47,    -1,    -1,    50,
      61,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      46,    47,    -1,    -1,    50,    61
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    20,    37,    38,    41,    42,    46,    47,    50,
      51,    56,    66,    67,    68,    74,    77,    79,    83,    84,
      86,    92,    74,    24,     9,     9,     0,    67,     9,    67,
      67,    67,    24,    92,    92,    92,    20,    85,    92,    93,
      74,    26,    11,    15,    57,    92,    93,    10,    10,    11,
      10,    13,    17,    18,    19,    30,    61,    26,     9,    42,
      74,    75,    76,    51,    58,    59,    60,    68,    80,    81,
      82,    92,    13,     3,     5,     6,     7,    11,    13,    19,
      43,    44,    45,    49,    55,    69,    70,    91,    93,    14,
      69,    92,    18,    30,    19,    30,    69,    92,    77,    83,
       9,    12,     9,     9,     9,     9,    80,    82,    16,    15,
      69,    91,    69,    93,    11,     8,    13,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      48,    61,    11,    12,    10,    14,    69,    69,    93,    52,
      92,    80,    80,    80,    82,    12,    14,    93,    91,    69,
      92,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    92,    91,    10,     8,    11,    11,
      16,    12,    14,    12,    76,    75,    75,    12,    12,    11,
      15,    20,    30,    31,    36,    39,    40,    53,    69,    70,
      71,    73,    77,    78,    85,    86,    87,    88,    89,    90,
      92,    93,    52,    69,    71,    71,    72,    54,    11,    10,
      10,    11,    71,    11,    10,    10,    10,    11,    12,    12,
      72,    16,    11,    69,    69,    12,    91,    75,    35,    62,
      34,     5,    12,    12,    12,    12,    71,    71,    11,     8,
      32,    69,     5,    71,    12,    12,    33,    35,    10,    71,
      11,    71,    12,    71
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    65,    66,    67,    67,    67,    67,    67,    67,    67,
      67,    68,    68,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      70,    70,    70,    70,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    72,    72,    73,    73,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    75,
      75,    75,    76,    76,    77,    78,    78,    79,    80,    80,
      81,    81,    81,    81,    82,    82,    83,    83,    84,    84,
      85,    85,    85,    85,    85,    86,    86,    86,    86,    86,
      87,    87,    88,    89,    89,    90,    91,    91,    91,    92,
      93,    93,    93,    93,    93
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     2,     3,     3,     3,     3,
       3,     3,     4,     3,     3,     3,     2,     4,     3,     1,
       1,     3,     4,     3,     1,     2,     2,     1,     1,     1,
       1,     2,     1,     2,     2,     3,     1,     2,     1,     4,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     0,
       1,     1,     3,     5,    11,     3,     4,    18,     1,     2,
       1,     3,     3,     3,     1,     2,     6,     8,     6,     6,
       3,     3,     3,     4,     4,     4,     4,     6,     7,     4,
       6,     8,     5,    12,     5,     2,     3,     1,     0,     1,
       1,     3,     2,     4,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 82 "lang.y"
              {absyn_root = (yyvsp[0].decList);}
#line 1764 "y.tab.c"
    break;

  case 3:
#line 84 "lang.y"
                       {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 1770 "y.tab.c"
    break;

  case 4:
#line 85 "lang.y"
                     {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 1776 "y.tab.c"
    break;

  case 5:
#line 86 "lang.y"
                       {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 1782 "y.tab.c"
    break;

  case 6:
#line 87 "lang.y"
                          {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 1788 "y.tab.c"
    break;

  case 7:
#line 88 "lang.y"
                               {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 1794 "y.tab.c"
    break;

  case 8:
#line 89 "lang.y"
                            {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 1800 "y.tab.c"
    break;

  case 9:
#line 90 "lang.y"
                          {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 1806 "y.tab.c"
    break;

  case 10:
#line 91 "lang.y"
                              {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 1812 "y.tab.c"
    break;

  case 11:
#line 93 "lang.y"
                    {(yyval.dec) = (yyvsp[0].dec);}
#line 1818 "y.tab.c"
    break;

  case 12:
#line 94 "lang.y"
                      {(yyval.dec) = (yyvsp[0].dec);}
#line 1824 "y.tab.c"
    break;

  case 13:
#line 96 "lang.y"
                {(yyval.exp) = A_IntExp(EM_tokPos, (yyvsp[0].ival));}
#line 1830 "y.tab.c"
    break;

  case 14:
#line 97 "lang.y"
                     { (yyval.exp) = A_StringExp(EM_tokPos, (yyvsp[0].sval));}
#line 1836 "y.tab.c"
    break;

  case 15:
#line 98 "lang.y"
                   { (yyval.exp) = A_RealExp(EM_tokPos, (yyvsp[0].fval));}
#line 1842 "y.tab.c"
    break;

  case 16:
#line 99 "lang.y"
                  {(yyval.exp) = A_NilExp(EM_tokPos);}
#line 1848 "y.tab.c"
    break;

  case 17:
#line 100 "lang.y"
                   {(yyval.exp) = A_CharExp(EM_tokPos, (yyvsp[0].cval));}
#line 1854 "y.tab.c"
    break;

  case 18:
#line 101 "lang.y"
                    {(yyval.exp) = A_BoolExp(EM_tokPos, TRUE);}
#line 1860 "y.tab.c"
    break;

  case 19:
#line 102 "lang.y"
                     {(yyval.exp) = A_BoolExp(EM_tokPos, FALSE);}
#line 1866 "y.tab.c"
    break;

  case 20:
#line 104 "lang.y"
                           {(yyval.exp) = A_OpExp(EM_tokPos, A_plusOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1872 "y.tab.c"
    break;

  case 21:
#line 105 "lang.y"
                            {(yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1878 "y.tab.c"
    break;

  case 22:
#line 106 "lang.y"
                            {(yyval.exp) = A_OpExp(EM_tokPos, A_timesOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1884 "y.tab.c"
    break;

  case 23:
#line 107 "lang.y"
                             {(yyval.exp) = A_OpExp(EM_tokPos, A_divideOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1890 "y.tab.c"
    break;

  case 24:
#line 108 "lang.y"
                          {(yyval.exp) = A_OpExp(EM_tokPos, A_modOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1896 "y.tab.c"
    break;

  case 25:
#line 110 "lang.y"
                                     {(yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, A_IntExp(0, 0), (yyvsp[0].exp));}
#line 1902 "y.tab.c"
    break;

  case 26:
#line 111 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_eqOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1908 "y.tab.c"
    break;

  case 27:
#line 112 "lang.y"
                          {(yyval.exp) = A_OpExp(EM_tokPos, A_neqOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1914 "y.tab.c"
    break;

  case 28:
#line 113 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_geOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1920 "y.tab.c"
    break;

  case 29:
#line 114 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_gtOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1926 "y.tab.c"
    break;

  case 30:
#line 115 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_leOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1932 "y.tab.c"
    break;

  case 31:
#line 116 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_ltOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1938 "y.tab.c"
    break;

  case 32:
#line 118 "lang.y"
                                           {(yyval.exp) = A_CallExp(EM_tokPos, (yyvsp[-3].exp), (yyvsp[-1].expList));}
#line 1944 "y.tab.c"
    break;

  case 33:
#line 119 "lang.y"
                                {(yyval.exp) = (yyvsp[-1].exp);}
#line 1950 "y.tab.c"
    break;

  case 34:
#line 120 "lang.y"
                          {(yyval.exp) = A_OpExp(EM_tokPos, A_andOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1956 "y.tab.c"
    break;

  case 35:
#line 121 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_orOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1962 "y.tab.c"
    break;

  case 36:
#line 123 "lang.y"
                               {(yyval.exp) = A_AddressExp(EM_tokPos, (yyvsp[0].var));}
#line 1968 "y.tab.c"
    break;

  case 37:
#line 124 "lang.y"
                                          {(yyval.exp) = A_SizeofExp(EM_tokPos, (yyvsp[-1].var));}
#line 1974 "y.tab.c"
    break;

  case 38:
#line 125 "lang.y"
                                    {(yyval.exp) = A_ArrayExp(EM_tokPos, (yyvsp[-1].expList));}
#line 1980 "y.tab.c"
    break;

  case 39:
#line 126 "lang.y"
                     {(yyval.exp) = (yyvsp[0].exp);}
#line 1986 "y.tab.c"
    break;

  case 40:
#line 128 "lang.y"
                   { (yyval.exp) = A_VarExp(EM_tokPos, (yyvsp[0].var)); }
#line 1992 "y.tab.c"
    break;

  case 41:
#line 129 "lang.y"
                         {(yyval.exp) = A_FieldExp(EM_tokPos, (yyvsp[-2].exp), (yyvsp[0].sym));}
#line 1998 "y.tab.c"
    break;

  case 42:
#line 130 "lang.y"
                                    {(yyval.exp) = A_SubscriptExp(EM_tokPos, (yyvsp[-3].exp), (yyvsp[-1].exp));}
#line 2004 "y.tab.c"
    break;

  case 43:
#line 131 "lang.y"
                                {(yyval.exp) = A_ArrowFieldExp(EM_tokPos, (yyvsp[-2].exp), (yyvsp[0].sym));}
#line 2010 "y.tab.c"
    break;

  case 44:
#line 135 "lang.y"
                    {(yyval.stm) = A_DeclarationStm(EM_tokPos, (yyvsp[0].dec));}
#line 2016 "y.tab.c"
    break;

  case 46:
#line 137 "lang.y"
                               {(yyval.stm) = (yyvsp[-1].stm);}
#line 2022 "y.tab.c"
    break;

  case 47:
#line 138 "lang.y"
                      {(yyval.stm) = A_DeclarationStm(EM_tokPos, (yyvsp[0].dec));}
#line 2028 "y.tab.c"
    break;

  case 48:
#line 139 "lang.y"
                 {(yyval.stm) = (yyvsp[0].stm);}
#line 2034 "y.tab.c"
    break;

  case 49:
#line 140 "lang.y"
                    {(yyval.stm) = (yyvsp[0].stm);}
#line 2040 "y.tab.c"
    break;

  case 50:
#line 141 "lang.y"
                  {(yyval.stm) = (yyvsp[0].stm);}
#line 2046 "y.tab.c"
    break;

  case 51:
#line 142 "lang.y"
                              {(yyval.stm) = (yyvsp[-1].stm);}
#line 2052 "y.tab.c"
    break;

  case 52:
#line 143 "lang.y"
                   {(yyval.stm) = A_DeclarationStm(EM_tokPos, A_FunctionDec(EM_tokPos, A_FundecList(A_Fundec(EM_tokPos, S_Symbol("loop"), A_FieldList(NULL, NULL), A_FieldList(NULL, NULL), (yyvsp[0].stm) -> u.loop.body), NULL)));}
#line 2058 "y.tab.c"
    break;

  case 53:
#line 144 "lang.y"
                              {(yyval.stm) = A_BreakStm(EM_tokPos);}
#line 2064 "y.tab.c"
    break;

  case 54:
#line 145 "lang.y"
                                 {(yyval.stm) = A_ContinueStm(EM_tokPos);}
#line 2070 "y.tab.c"
    break;

  case 55:
#line 146 "lang.y"
                                    {(yyval.stm) = A_CompoundStm(EM_tokPos, (yyvsp[-1].stmList));}
#line 2076 "y.tab.c"
    break;

  case 56:
#line 150 "lang.y"
                {(yyval.stmList) = A_StmList((yyvsp[0].stm), NULL);}
#line 2082 "y.tab.c"
    break;

  case 57:
#line 151 "lang.y"
                        {(yyval.stmList) = A_StmList((yyvsp[-1].stm), (yyvsp[0].stmList)); }
#line 2088 "y.tab.c"
    break;

  case 58:
#line 153 "lang.y"
                   {(yyval.stm) = A_ReturnStm(EM_tokPos, NULL);}
#line 2094 "y.tab.c"
    break;

  case 59:
#line 154 "lang.y"
                                       {(yyval.stm) = A_ReturnStm(EM_tokPos, (yyvsp[-1].exp));}
#line 2100 "y.tab.c"
    break;

  case 60:
#line 156 "lang.y"
               {(yyval.type) = A_NameTy(EM_tokPos, (yyvsp[0].sym));}
#line 2106 "y.tab.c"
    break;

  case 61:
#line 157 "lang.y"
                   {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("void"));}
#line 2112 "y.tab.c"
    break;

  case 62:
#line 158 "lang.y"
                      {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("int"));}
#line 2118 "y.tab.c"
    break;

  case 63:
#line 159 "lang.y"
                       {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("real"));}
#line 2124 "y.tab.c"
    break;

  case 64:
#line 160 "lang.y"
                      {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("int"));}
#line 2130 "y.tab.c"
    break;

  case 65:
#line 161 "lang.y"
                       {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("char"));}
#line 2136 "y.tab.c"
    break;

  case 66:
#line 162 "lang.y"
                        {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("short"));}
#line 2142 "y.tab.c"
    break;

  case 67:
#line 163 "lang.y"
                         {(yyval.type) = A_PointerTy(EM_tokPos, (yyvsp[0].type));}
#line 2148 "y.tab.c"
    break;

  case 68:
#line 164 "lang.y"
                            {(yyval.type) = A_PolyTy(EM_tokPos, (yyvsp[-3].sym), (yyvsp[-1].type));}
#line 2154 "y.tab.c"
    break;

  case 69:
#line 166 "lang.y"
                        {(yyval.fieldList) = A_FieldList(NULL, NULL);}
#line 2160 "y.tab.c"
    break;

  case 70:
#line 167 "lang.y"
                   {(yyval.fieldList) = A_FieldList(NULL, NULL);}
#line 2166 "y.tab.c"
    break;

  case 71:
#line 168 "lang.y"
                       {(yyval.fieldList) = (yyvsp[0].fieldList);}
#line 2172 "y.tab.c"
    break;

  case 72:
#line 170 "lang.y"
                              {
                if((yyvsp[0].var) -> kind == A_simpleVar)
                {
                    // printf("%s fasdlkfjlaj;dfklja;dklf\n", S_name($3 -> u.simple));
                    (yyval.fieldList) = A_FieldList(A_Field(EM_tokPos, (yyvsp[0].var) -> u.simple, (yyvsp[-2].type)), NULL);
                }
                else if((yyvsp[0].var) -> kind == A_subscriptVar)
                {
                    A_ty type = (yyvsp[-2].type);
                    for(A_var var = (yyvsp[0].var); var -> kind == A_subscriptVar; var = var -> u.subscript.var)
                    {
                        if(var -> u.subscript.exp -> kind!= A_intExp)
                        {
                            EM_error(EM_tokPos, "Cannot declare array with an unfixed size.");
                        }
                        type = A_ArrayTy(EM_tokPos, type, var -> u.subscript.exp -> u.intt);
                    }
                    (yyval.fieldList) = A_FieldList(A_Field(EM_tokPos, (yyvsp[0].var) -> u.subscript.name, type), NULL);
                }
                else
                {
                    EM_error(EM_tokPos, "Not supported.");
                }
            }
#line 2201 "y.tab.c"
    break;

  case 73:
#line 194 "lang.y"
                                               {
                if((yyvsp[-2].var) -> kind == A_simpleVar)
                {
                    (yyval.fieldList) = A_FieldList(A_Field(EM_tokPos, (yyvsp[-2].var) -> u.simple, (yyvsp[-4].type)), (yyvsp[0].fieldList));
                }
                else if((yyvsp[-2].var) -> kind == A_subscriptVar)
                {
                    A_ty type = (yyvsp[-4].type);
                    for(A_var var = (yyvsp[-2].var); var -> kind == A_subscriptVar; var = var -> u.subscript.var)
                    {
                        if(var -> u.subscript.exp -> kind!= A_intExp)
                        {
                            EM_error(EM_tokPos, "Cannot declare array with an unfixed size.");
                        }
                        type = A_ArrayTy(EM_tokPos, type, var -> u.subscript.exp -> u.intt);
                    }
                    (yyval.fieldList) = A_FieldList(A_Field(EM_tokPos, (yyvsp[-2].var) -> u.subscript.name, type), (yyvsp[0].fieldList));
                }
                else
                {
                    EM_error(EM_tokPos, "Not supported.");
                }
            }
#line 2229 "y.tab.c"
    break;

  case 74:
#line 218 "lang.y"
                                                                                    {(yyval.dec) = A_FunctionDec(EM_tokPos, A_FundecList(A_Fundec(EM_tokPos, (yyvsp[-8].sym), (yyvsp[-6].fieldList), (yyvsp[-2].fieldList), (yyvsp[0].stm)), NULL));}
#line 2235 "y.tab.c"
    break;

  case 75:
#line 221 "lang.y"
                                 {(yyval.stm) = A_CallStm(EM_tokPos, (yyvsp[-2].exp), NULL);}
#line 2241 "y.tab.c"
    break;

  case 76:
#line 222 "lang.y"
                                           {(yyval.stm) = A_CallStm(EM_tokPos, (yyvsp[-3].exp), (yyvsp[-1].expList));}
#line 2247 "y.tab.c"
    break;

  case 77:
#line 224 "lang.y"
                                                                                                                                          {(yyval.dec) = A_FuncImport(EM_tokPos, (yyvsp[-15].sym), (yyvsp[-13].fieldList), (yyvsp[-9].fieldList), (yyvsp[-4].sval), (yyvsp[-2].sval));}
#line 2253 "y.tab.c"
    break;

  case 78:
#line 226 "lang.y"
                        {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 2259 "y.tab.c"
    break;

  case 79:
#line 227 "lang.y"
                                     {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 2265 "y.tab.c"
    break;

  case 80:
#line 229 "lang.y"
                        {(yyval.memList) = A_ClassMemFromDecList((yyvsp[0].decList), A_public);}
#line 2271 "y.tab.c"
    break;

  case 81:
#line 230 "lang.y"
                                       {(yyval.memList) = A_ClassMemFromDecList((yyvsp[0].decList), A_public);}
#line 2277 "y.tab.c"
    break;

  case 82:
#line 231 "lang.y"
                                          {(yyval.memList) = A_ClassMemFromDecList((yyvsp[0].decList), A_protected);}
#line 2283 "y.tab.c"
    break;

  case 83:
#line 232 "lang.y"
                                        {(yyval.memList) = A_ClassMemFromDecList((yyvsp[0].decList), A_private);}
#line 2289 "y.tab.c"
    break;

  case 84:
#line 234 "lang.y"
                    {(yyval.memList) = A_ClassMemFromTwoList((yyvsp[0].memList), NULL);}
#line 2295 "y.tab.c"
    break;

  case 85:
#line 235 "lang.y"
                           {(yyval.memList) = A_ClassMemFromTwoList((yyvsp[-1].memList), (yyvsp[0].memList));}
#line 2301 "y.tab.c"
    break;

  case 86:
#line 237 "lang.y"
                                              {(yyval.dec) = A_ClassDec(EM_tokPos, (yyvsp[-3].sym), (yyvsp[-1].memList), NULL);}
#line 2307 "y.tab.c"
    break;

  case 87:
#line 238 "lang.y"
                                                             {(yyval.dec) = A_ClassDec(EM_tokPos, (yyvsp[-5].sym), (yyvsp[-1].memList), S_SymbolList((yyvsp[-3].sym), NULL));}
#line 2313 "y.tab.c"
    break;

  case 88:
#line 241 "lang.y"
                                        {(yyval.dec) = A_TemplateDec(EM_tokPos, (yyvsp[-3].sym), (yyvsp[0].dec));}
#line 2319 "y.tab.c"
    break;

  case 89:
#line 242 "lang.y"
                                           {(yyval.dec) = A_TemplateDec(EM_tokPos, (yyvsp[-3].sym), (yyvsp[0].dec));}
#line 2325 "y.tab.c"
    break;

  case 90:
#line 244 "lang.y"
                              {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-2].var), (yyvsp[0].exp), FALSE);}
#line 2331 "y.tab.c"
    break;

  case 91:
#line 245 "lang.y"
                               {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-2].var), A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, (yyvsp[-2].var)), A_IntExp(EM_tokPos, 1)), FALSE);}
#line 2337 "y.tab.c"
    break;

  case 92:
#line 246 "lang.y"
                                 {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-2].var), A_OpExp(EM_tokPos, A_minusOp, A_VarExp(EM_tokPos, (yyvsp[-2].var)), A_IntExp(EM_tokPos, 1)), FALSE);}
#line 2343 "y.tab.c"
    break;

  case 93:
#line 247 "lang.y"
                                     {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-3].var), A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, (yyvsp[-3].var)), (yyvsp[0].exp)), FALSE);}
#line 2349 "y.tab.c"
    break;

  case 94:
#line 248 "lang.y"
                                      {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-3].var), A_OpExp(EM_tokPos, A_minusOp, A_VarExp(EM_tokPos, (yyvsp[-3].var)), (yyvsp[0].exp)), FALSE);}
#line 2355 "y.tab.c"
    break;

  case 95:
#line 250 "lang.y"
                                        {(yyval.dec) = A_VarDec(EM_tokPos, (yyvsp[-1].stm), (yyvsp[-3].type));}
#line 2361 "y.tab.c"
    break;

  case 96:
#line 251 "lang.y"
                                         {(yyval.dec) = A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, (yyvsp[-1].var), NULL, TRUE), (yyvsp[-3].type));}
#line 2367 "y.tab.c"
    break;

  case 97:
#line 252 "lang.y"
                                                        {(yyval.dec) = A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, (yyvsp[-3].var), NULL, TRUE), (yyvsp[-5].type));}
#line 2373 "y.tab.c"
    break;

  case 98:
#line 253 "lang.y"
                                                            {(yyval.dec) = A_ObjectDec(EM_tokPos, (yyvsp[-6].type), (yyvsp[-4].sym), (yyvsp[-2].expList));}
#line 2379 "y.tab.c"
    break;

  case 99:
#line 254 "lang.y"
                                      {(yyval.dec) = A_ObjectDec(EM_tokPos, (yyvsp[-3].type), (yyvsp[-1].sym), NULL);}
#line 2385 "y.tab.c"
    break;

  case 100:
#line 256 "lang.y"
                                                                 {(yyval.stm) = A_IfStm(EM_tokPos, (yyvsp[-3].exp), (yyvsp[0].stm), NULL);}
#line 2391 "y.tab.c"
    break;

  case 101:
#line 257 "lang.y"
                                                     {(yyval.stm) = A_IfStm(EM_tokPos, (yyvsp[-5].exp), (yyvsp[-2].stm), (yyvsp[0].stm));}
#line 2397 "y.tab.c"
    break;

  case 102:
#line 259 "lang.y"
                                     {(yyval.stm) = A_WhileStm(EM_tokPos, (yyvsp[-3].exp), (yyvsp[0].stm));}
#line 2403 "y.tab.c"
    break;

  case 103:
#line 261 "lang.y"
                                                                              {(yyval.stm) = A_ForStm(EM_tokPos, (yyvsp[-10].stm), (yyvsp[-6].exp), (yyvsp[-2].stm), (yyvsp[0].stm));}
#line 2409 "y.tab.c"
    break;

  case 104:
#line 262 "lang.y"
                                           {(yyval.stm) = A_ForStm(EM_tokPos, A_DeclarationStm(EM_tokPos, A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ")), A_IntExp(EM_tokPos, 0), TRUE), A_NameTy(EM_tokPos, S_Symbol("int")))), A_OpExp(EM_tokPos, A_eqOp, A_VarExp(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ"))), (yyvsp[-3].exp)), A_AssignStm(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ")), A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ"))), A_IntExp(EM_tokPos, 1)), FALSE), (yyvsp[0].stm));}
#line 2415 "y.tab.c"
    break;

  case 105:
#line 265 "lang.y"
                     {(yyval.stm) = A_LoopStm(EM_tokPos, (yyvsp[0].stm)); }
#line 2421 "y.tab.c"
    break;

  case 106:
#line 267 "lang.y"
                              {(yyval.expList) = A_ExpList((yyvsp[-2].exp), (yyvsp[0].expList));}
#line 2427 "y.tab.c"
    break;

  case 107:
#line 268 "lang.y"
                  { (yyval.expList) = A_ExpList((yyvsp[0].exp), 0); }
#line 2433 "y.tab.c"
    break;

  case 108:
#line 269 "lang.y"
              {(yyval.expList) = NULL;}
#line 2439 "y.tab.c"
    break;

  case 109:
#line 271 "lang.y"
               { (yyval.sym) = S_Symbol((yyvsp[0].sval)); }
#line 2445 "y.tab.c"
    break;

  case 110:
#line 273 "lang.y"
               {(yyval.var) = A_SimpleVar(EM_tokPos, (yyvsp[0].sym));}
#line 2451 "y.tab.c"
    break;

  case 111:
#line 274 "lang.y"
                            {(yyval.var) = A_FieldVar(EM_tokPos, (yyvsp[-2].var), (yyvsp[0].sym));}
#line 2457 "y.tab.c"
    break;

  case 112:
#line 275 "lang.y"
                           {(yyval.var) = A_DerefVar(EM_tokPos, (yyvsp[0].var));}
#line 2463 "y.tab.c"
    break;

  case 113:
#line 276 "lang.y"
                                       { (yyval.var) = A_SubscriptVar(EM_tokPos, (yyvsp[-3].var), (yyvsp[-1].exp)); }
#line 2469 "y.tab.c"
    break;

  case 114:
#line 277 "lang.y"
                                   {(yyval.var) = A_ArrowFieldVar(EM_tokPos, (yyvsp[-2].var), (yyvsp[0].sym));}
#line 2475 "y.tab.c"
    break;


#line 2479 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
