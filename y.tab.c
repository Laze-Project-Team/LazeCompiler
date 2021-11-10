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

    #ifdef __cplusplus
    extern "C" {
    #endif
    #include <stdio.h>
    #include "util.h"
    #include "errormsg.h"
    #include "symbol.h"
    #include "absyn.h"
    #include "y.tab.h"
    #define YYDEBUG 1

    int yylex(void);
    int yyparse(void);
    A_decList absyn_root;

    void yyerror(const char *s)
    {
        EM_error(EM_tokPos, "%s", s);
    }
    #ifdef __cplusplus
    }
    #endif

#line 95 "y.tab.c"

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
#line 27 "lang.y"

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

#line 299 "y.tab.c"

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
typedef yytype_int16 yy_state_t;

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
#define YYFINAL  29
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1165

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  66
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  296

#define YYUNDEFTOK  2
#define YYMAXUTOK   320


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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    89,    89,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   102,   103,   105,   106,   107,   108,   109,
     110,   111,   113,   114,   115,   116,   117,   119,   120,   121,
     122,   123,   124,   125,   127,   128,   129,   130,   132,   133,
     134,   135,   136,   138,   139,   140,   141,   143,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     162,   163,   165,   166,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   178,   179,   180,   182,   206,   207,   230,
     232,   235,   236,   238,   240,   242,   243,   245,   246,   247,
     248,   250,   251,   253,   254,   257,   258,   260,   261,   262,
     263,   264,   266,   267,   268,   269,   270,   272,   273,   275,
     277,   278,   281,   283,   284,   285,   287,   289,   290,   291,
     292,   293
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
  "PROTECTED", "RIGHTARROW", "REPEAT", "JSEXPORT", "UMINUS",
  "LOWER_THAN_ELSE", "$accept", "program", "decs", "funcAndVar", "exp",
  "varExp", "funcExp", "stm", "stmlist", "return", "type", "tyfield",
  "tyfield1", "funcDec", "funcCall", "jsLoad", "jsExport", "funcvarlist",
  "memlist", "mems", "classDec", "templateDec", "assign", "declare", "if",
  "while", "for", "loop", "explist", "id", "lvalue", YY_NULLPTR
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
     315,   316,   317,   318,   319,   320
};
# endif

#define YYPACT_NINF (-181)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-83)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     390,  -181,  1095,  -181,  -181,    -7,  -181,  -181,  -181,  -181,
      23,    26,    34,    59,  -181,   390,    52,  -181,   390,   390,
     390,   390,  -181,    44,  -181,    65,    65,    65,    65,  -181,
    -181,     4,  -181,  -181,  -181,  -181,  1095,    45,    61,    -1,
      67,     4,    69,    42,   148,    51,    72,  1055,   434,    65,
      77,  -181,    47,  -181,  -181,   789,  -181,   169,    65,    21,
      35,   789,    65,  -181,   -40,    71,    75,    82,    80,  -181,
      90,    92,    93,    94,  1074,  -181,   434,    91,    95,    99,
     789,  -181,  -181,  -181,  -181,   722,   789,   789,  -181,  -181,
    -181,     4,   105,   483,   107,  -181,   108,    47,   112,   964,
    -181,  -181,   789,  -181,   789,  1026,  -181,  -181,  -181,    65,
       4,    73,    65,  1074,  1074,  1074,  -181,  -181,  -181,   434,
     120,  1115,   793,   121,    14,   118,  1026,    47,     4,   789,
     789,    65,   789,   789,   789,   789,   789,   789,   789,   789,
     789,   789,   789,   789,   789,    65,   789,   124,  -181,  -181,
    1026,  1026,   128,    -4,   131,   132,  -181,  -181,  -181,   130,
    -181,  -181,    98,  -181,    17,  -181,   981,  -181,  1043,  1043,
     564,   564,   215,   215,   215,   215,   215,   215,   327,   327,
     564,  -181,   142,  -181,  1055,  1076,  1055,  1055,  -181,   133,
    -181,  -181,  -181,   143,  -181,   151,   152,  1055,   116,   510,
     117,   159,   149,   616,   669,   125,   170,   180,   186,   187,
     669,  1026,   189,  -181,   191,  -181,   192,   196,  -181,  -181,
    -181,  -181,  -181,   203,   204,   669,  1055,   198,   838,   205,
     214,   669,   211,   218,   789,  -181,  -181,   789,  -181,   742,
    -181,  -181,  -181,  1055,  -181,   224,    65,   -29,   210,     4,
    -181,  -181,   225,   856,   901,  -181,   233,   235,   242,   244,
     669,   669,   245,    87,   249,   230,  -181,   248,   669,  1076,
    1055,  -181,  -181,   789,   258,   669,  -181,   253,   919,   254,
     234,   219,   237,   263,   669,   268,   271,  -181,  -181,  1055,
     669,   273,   274,   563,   669,  -181
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,   116,     0,    65,    66,     0,    64,    67,    68,    69,
       0,     0,     0,     0,     2,     3,     0,    14,     4,     7,
       5,     6,    13,    70,    71,     0,     0,     0,     0,     1,
      11,     0,    10,    12,     9,     8,     0,     0,     0,     0,
       0,     0,     0,   117,     0,     0,     0,    73,     0,     0,
       0,   117,   119,   102,   106,   115,   103,     0,     0,     0,
       0,     0,     0,    72,     0,    64,     0,     0,     0,    75,
       0,     0,     0,     0,    85,    87,    91,     0,     0,     0,
       0,    19,    16,    15,    17,    73,   115,     0,    18,    20,
      21,     0,     0,   114,    41,    42,     0,    43,     0,     0,
     118,    98,     0,    99,     0,    97,   121,    95,    96,     0,
       0,     0,     0,     0,     0,     0,    86,    92,    93,     0,
       0,     0,     0,     0,   117,     0,    27,    38,     0,   115,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,     0,   104,   120,
     100,   101,     0,    76,     0,     0,    90,    88,    89,     0,
      84,    35,     0,    40,     0,   113,     0,    44,    22,    23,
      24,    25,    28,    29,    33,    32,    31,    30,    36,    37,
      26,    46,     0,   105,    73,     0,    73,    73,    94,     0,
      39,    45,    34,     0,    78,     0,     0,    73,     0,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,    62,
       0,     0,    41,    80,     0,    48,     0,     0,    51,    52,
      53,    54,    56,    43,     0,     0,    73,     0,     0,     0,
       0,    60,     0,     0,     0,    57,    58,     0,   112,     0,
      55,    49,    50,    73,    47,     0,     0,    35,     0,     0,
      61,    59,     0,     0,     0,    81,     0,     0,    77,     0,
       0,     0,     0,    76,     0,     0,    63,    34,     0,     0,
      73,   109,   111,     0,     0,     0,    79,     0,     0,     0,
     107,     0,     0,     0,     0,     0,     0,    83,   108,    73,
       0,     0,     0,    77,     0,   110
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -181,  -181,   284,   316,   226,   174,  -181,  -137,    57,  -181,
       8,   -35,  -180,    22,  -181,  -181,  -181,    13,  -181,   -69,
     227,  -181,   -30,   300,  -181,  -181,  -181,  -181,   -83,     0,
      49
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    74,   211,    94,    95,   213,   232,   214,
      16,   123,    69,   215,   216,    18,    19,    75,    76,    77,
      20,    21,   217,   218,   219,   220,   221,   222,    96,    51,
      97
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,    42,    23,   125,   185,   194,   260,   117,     1,    80,
      24,    70,    68,    58,    48,    23,    11,    25,    23,    23,
      23,    23,    17,   -70,    41,    37,    38,    39,    40,   190,
      80,    43,    26,   261,    58,    27,    23,    17,    36,   101,
      17,    17,    17,    17,    45,    28,   165,    23,    23,    78,
     159,   102,    54,    55,   103,    67,    49,    62,   100,    29,
      80,    31,   106,   182,    58,   104,   229,   231,    36,     1,
      17,    46,    47,   238,    23,    50,    23,    63,    62,    53,
      44,    64,    79,   -74,   109,   124,   107,   116,   244,   276,
      52,   110,   111,    67,   231,   185,    17,    56,    17,   112,
      57,   113,   114,   115,    58,    59,    60,   118,    62,   152,
     119,   120,   155,    23,    23,    23,   128,    61,   146,    23,
     147,   124,   148,   271,   272,   154,   156,   157,   158,    24,
     160,   167,   163,   162,   183,    17,    17,    17,   280,   184,
     127,    17,   186,   187,   197,   181,   188,   288,    62,   193,
     189,   195,   196,   292,   192,   198,   256,   295,    56,   153,
     226,    57,   201,   199,   200,    58,    59,    60,   202,   224,
      52,   225,    81,     1,    82,    83,    84,   164,    61,   233,
      85,   234,    86,    98,    23,    23,    23,    23,    87,    41,
     235,   245,    67,    67,    67,    67,   236,    23,   237,   124,
     239,   240,   241,   124,   124,    67,   242,   246,   257,    62,
     124,   230,    88,    89,    90,   243,    80,   248,    91,    42,
      58,    59,    60,   249,    92,   124,    23,   251,   130,   252,
     264,   124,   131,    61,    67,   277,   258,   -83,   -83,   -83,
     -83,   -83,   -83,    23,   262,   267,   259,   268,   223,    43,
     269,    67,   223,   223,   291,   270,   273,   274,   -82,   223,
     124,   124,   275,   279,    62,   281,   283,   284,   124,    23,
      23,   285,   286,   287,   223,   124,   145,    67,    67,   289,
     223,    93,   290,    99,   124,   293,   294,   105,   250,    23,
     124,   108,     0,   124,   124,     0,     0,    67,   263,    30,
      22,     0,    32,    33,    34,    35,    99,     0,     0,   223,
     223,   122,    93,   126,     0,    22,    15,   223,    22,    22,
      22,    22,     0,     0,   223,     0,     0,     0,   150,     0,
     151,    15,     0,   223,    15,    15,    15,    15,     0,   223,
     130,     0,   223,   223,   131,     0,     0,     0,    22,   136,
     137,   138,   139,   140,   141,    93,   166,     0,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,    93,   212,    22,     0,    22,   212,   212,     0,
       0,     0,     0,     0,   212,     0,     0,     0,   145,     0,
       0,     0,     0,     0,     1,     0,     0,     0,     0,   212,
       0,     0,     0,     0,     0,   212,     0,     0,     0,     0,
       2,     0,     0,    22,    22,    22,     0,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     3,     4,   228,
       0,     5,     6,     0,   212,   212,     7,     8,     1,     0,
       9,    10,   212,     0,     0,     0,    11,     0,     0,   212,
       0,     0,     0,    12,     2,     0,     0,     0,   212,     0,
     253,     0,     0,   254,   212,    93,     0,   212,   212,     0,
       0,     3,     4,     0,     0,     0,     6,     0,     0,     0,
       7,     8,     0,     0,     9,    70,     0,     0,     0,     0,
       0,   129,    71,    72,    73,     0,   130,     0,     0,   278,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,    81,     1,    82,    83,    84,     0,     0,
       0,   203,     0,    86,     0,   204,     0,     0,     0,    87,
     121,   144,     0,     0,     0,     0,     0,     0,     0,     0,
     205,   206,     0,     0,   145,     0,   207,     3,     4,   208,
     209,     0,     6,    88,    89,    90,     7,     8,     0,    91,
       9,    70,     0,   210,     0,    92,    81,     1,    82,    83,
      84,   269,     0,     0,   203,     0,    86,   130,   204,     0,
       0,   131,    87,   121,     0,     0,   136,   137,   138,   139,
     140,   141,   142,   143,   206,     0,     0,     0,     0,   207,
       3,     4,   208,   209,     0,     6,    88,    89,    90,     7,
       8,     0,    91,     9,    70,     0,   210,     0,    92,    81,
       1,    82,    83,    84,     0,   145,     0,   203,     0,    86,
       0,   204,     0,     0,     0,    87,   121,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   206,     0,     0,
       0,     0,   207,     3,     4,   208,   209,     0,    65,    88,
      89,    90,     7,     8,     0,    91,     9,   227,     0,   210,
       0,    92,    81,     1,    82,    83,    84,     0,     0,     0,
     203,     0,    86,     0,   204,     0,     0,     0,    87,   121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     206,     0,     0,     0,     0,   207,     3,     4,   208,   209,
       0,     6,    88,    89,    90,     7,     8,     0,    91,     9,
      70,     0,   210,     0,    92,    81,     1,    82,    83,    84,
       0,     0,     0,    85,     0,    86,     0,     0,     0,     0,
       0,    87,   121,     0,     0,    81,     1,    82,    83,    84,
       0,     0,     0,    85,   255,    86,     0,     0,     0,     3,
       4,    87,    41,     0,    65,    88,    89,    90,     7,     8,
       0,    91,     9,    66,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,     0,     0,
       0,    91,    81,     1,    82,    83,    84,    92,     0,     0,
      85,     0,    86,     0,     0,   161,   130,     0,    87,    41,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,     0,     0,     0,    91,     0,
       0,   144,     0,     0,    92,     0,     0,     0,     0,     0,
     247,   130,     0,     0,   145,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   265,   130,
       0,     0,     0,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,     0,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,   266,   130,     0,     0,   145,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   282,   130,     0,     0,     0,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,   149,     0,
     145,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   130,   191,     0,     0,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,     0,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   130,
       0,     0,   145,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   130,     0,     0,     1,
     131,     0,     0,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,     0,   144,     2,     0,     0,     1,     0,
       1,     0,     0,     0,     0,     0,     0,   145,     0,     0,
       0,   144,     3,     4,     2,     0,     2,    65,     0,     1,
       0,     7,     8,     0,   145,     9,    66,     0,     0,     0,
       0,     3,     4,     3,     4,     2,     6,     0,     6,     1,
       7,     8,     7,     8,     9,    70,     9,    66,     0,     0,
       0,     0,     3,     4,     0,   121,     0,     6,     0,     0,
       0,     7,     8,     0,     0,     9,     0,     0,     0,     0,
       0,     0,     3,     4,     0,     0,     0,     6,     0,     0,
       0,     7,     8,     0,     0,     9
};

static const yytype_int16 yycheck[] =
{
       0,    31,     2,    86,     8,   185,    35,    76,     4,    13,
       2,    51,    47,    17,    15,    15,    56,    24,    18,    19,
      20,    21,     0,     9,    20,    25,    26,    27,    28,    12,
      13,    31,     9,    62,    17,     9,    36,    15,    24,    18,
      18,    19,    20,    21,    36,    11,   129,    47,    48,    49,
     119,    30,    10,    11,    19,    47,    57,    61,    58,     0,
      13,     9,    62,   146,    17,    30,   203,   204,    24,     4,
      48,    26,    11,   210,    74,     8,    76,    26,    61,    10,
      31,     9,     5,    12,     9,    85,    64,    74,   225,   269,
      41,     9,    12,    85,   231,     8,    74,    10,    76,     9,
      13,     9,     9,     9,    17,    18,    19,    16,    61,   109,
      15,    12,   112,   113,   114,   115,    11,    30,    11,   119,
      12,   121,    10,   260,   261,    52,   113,   114,   115,   121,
      10,   131,    14,    12,    10,   113,   114,   115,   275,    11,
      91,   119,    11,    11,    11,   145,    16,   284,    61,   184,
      52,   186,   187,   290,    12,    12,   239,   294,    10,   110,
      11,    13,   197,    12,    12,    17,    18,    19,    52,    52,
     121,    12,     3,     4,     5,     6,     7,   128,    30,    54,
      11,    11,    13,    14,   184,   185,   186,   187,    19,    20,
      10,   226,   184,   185,   186,   187,    10,   197,    11,   199,
      11,    10,    10,   203,   204,   197,    10,     9,   243,    61,
     210,   203,    43,    44,    45,    11,    13,    12,    49,   249,
      17,    18,    19,     9,    55,   225,   226,    16,    13,    11,
       5,   231,    17,    30,   226,   270,    12,    22,    23,    24,
      25,    26,    27,   243,    34,    12,   246,    12,   199,   249,
       8,   243,   203,   204,   289,    11,    11,     8,    10,   210,
     260,   261,    32,     5,    61,    12,    12,    33,   268,   269,
     270,    52,    35,    10,   225,   275,    61,   269,   270,    11,
     231,    55,    11,    57,   284,    12,    12,    61,   231,   289,
     290,    64,    -1,   293,   294,    -1,    -1,   289,   249,    15,
       0,    -1,    18,    19,    20,    21,    80,    -1,    -1,   260,
     261,    85,    86,    87,    -1,    15,     0,   268,    18,    19,
      20,    21,    -1,    -1,   275,    -1,    -1,    -1,   102,    -1,
     104,    15,    -1,   284,    18,    19,    20,    21,    -1,   290,
      13,    -1,   293,   294,    17,    -1,    -1,    -1,    48,    22,
      23,    24,    25,    26,    27,   129,   130,    -1,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,    -1,   146,   199,    74,    -1,    76,   203,   204,    -1,
      -1,    -1,    -1,    -1,   210,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,   225,
      -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,
      20,    -1,    -1,   113,   114,   115,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,   203,
      -1,    41,    42,    -1,   260,   261,    46,    47,     4,    -1,
      50,    51,   268,    -1,    -1,    -1,    56,    -1,    -1,   275,
      -1,    -1,    -1,    63,    20,    -1,    -1,    -1,   284,    -1,
     234,    -1,    -1,   237,   290,   239,    -1,   293,   294,    -1,
      -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      46,    47,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,
      -1,     8,    58,    59,    60,    -1,    13,    -1,    -1,   273,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    -1,    13,    -1,    15,    -1,    -1,    -1,    19,
      20,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    31,    -1,    -1,    61,    -1,    36,    37,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    -1,    55,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    -1,    13,    13,    15,    -1,
      -1,    17,    19,    20,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    31,    -1,    -1,    -1,    -1,    36,
      37,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    -1,    55,     3,
       4,     5,     6,     7,    -1,    61,    -1,    11,    -1,    13,
      -1,    15,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    37,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      -1,    55,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    -1,    13,    -1,    15,    -1,    -1,    -1,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    -1,    55,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    19,    20,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    -1,    -1,    -1,    37,
      38,    19,    20,    -1,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    43,    44,    45,    -1,    -1,
      -1,    49,     3,     4,     5,     6,     7,    55,    -1,    -1,
      11,    -1,    13,    -1,    -1,    12,    13,    -1,    19,    20,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    48,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,    61,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    12,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    -1,    -1,    61,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    12,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    -1,
      61,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    61,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    13,    -1,    -1,     4,
      17,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    48,    20,    -1,    -1,     4,    -1,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    48,    37,    38,    20,    -1,    20,    42,    -1,     4,
      -1,    46,    47,    -1,    61,    50,    51,    -1,    -1,    -1,
      -1,    37,    38,    37,    38,    20,    42,    -1,    42,     4,
      46,    47,    46,    47,    50,    51,    50,    51,    -1,    -1,
      -1,    -1,    37,    38,    -1,    20,    -1,    42,    -1,    -1,
      -1,    46,    47,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    46,    47,    -1,    -1,    50
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    20,    37,    38,    41,    42,    46,    47,    50,
      51,    56,    63,    67,    68,    69,    76,    79,    81,    82,
      86,    87,    89,    95,    76,    24,     9,     9,    11,     0,
      68,     9,    68,    68,    68,    68,    24,    95,    95,    95,
      95,    20,    88,    95,    96,    76,    26,    11,    15,    57,
       8,    95,    96,    10,    10,    11,    10,    13,    17,    18,
      19,    30,    61,    26,     9,    42,    51,    76,    77,    78,
      51,    58,    59,    60,    69,    83,    84,    85,    95,     5,
      13,     3,     5,     6,     7,    11,    13,    19,    43,    44,
      45,    49,    55,    70,    71,    72,    94,    96,    14,    70,
      95,    18,    30,    19,    30,    70,    95,    79,    86,     9,
       9,    12,     9,     9,     9,     9,    83,    85,    16,    15,
      12,    20,    70,    77,    95,    94,    70,    96,    11,     8,
      13,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    48,    61,    11,    12,    10,    14,
      70,    70,    95,    96,    52,    95,    83,    83,    83,    85,
      10,    12,    12,    14,    96,    94,    70,    95,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    95,    94,    10,    11,     8,    11,    11,    16,    52,
      12,    14,    12,    77,    78,    77,    77,    11,    12,    12,
      12,    77,    52,    11,    15,    30,    31,    36,    39,    40,
      53,    70,    71,    73,    75,    79,    80,    88,    89,    90,
      91,    92,    93,    96,    52,    12,    11,    51,    70,    73,
      76,    73,    74,    54,    11,    10,    10,    11,    73,    11,
      10,    10,    10,    11,    73,    77,     9,    12,    12,     9,
      74,    16,    11,    70,    70,    12,    94,    77,    12,    95,
      35,    62,    34,    96,     5,    12,    12,    12,    12,     8,
      11,    73,    73,    11,     8,    32,    78,    77,    70,     5,
      73,    12,    12,    12,    33,    52,    35,    10,    73,    11,
      11,    77,    73,    12,    12,    73
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    66,    67,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    69,    69,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    71,    71,    71,    71,    72,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      74,    74,    75,    75,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    77,    77,    77,    78,    78,    78,    78,
      79,    80,    80,    81,    82,    83,    83,    84,    84,    84,
      84,    85,    85,    86,    86,    87,    87,    88,    88,    88,
      88,    88,    89,    89,    89,    89,    89,    90,    90,    91,
      92,    92,    93,    94,    94,    94,    95,    96,    96,    96,
      96,    96
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     4,     3,     3,     3,     2,     4,
       3,     1,     1,     1,     3,     4,     3,     8,     1,     2,
       2,     1,     1,     1,     1,     2,     1,     2,     2,     3,
       1,     2,     1,     4,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     0,     1,     1,     3,    10,     5,    12,
      11,     3,     4,    18,     7,     1,     2,     1,     3,     3,
       3,     1,     2,     6,     8,     6,     6,     3,     3,     3,
       4,     4,     4,     4,     6,     7,     4,     6,     8,     5,
      12,     5,     2,     3,     1,     0,     1,     1,     3,     2,
       4,     3
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
#line 89 "lang.y"
              {absyn_root = (yyvsp[0].decList);}
#line 1847 "y.tab.c"
    break;

  case 3:
#line 91 "lang.y"
                       {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 1853 "y.tab.c"
    break;

  case 4:
#line 92 "lang.y"
                     {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 1859 "y.tab.c"
    break;

  case 5:
#line 93 "lang.y"
                       {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 1865 "y.tab.c"
    break;

  case 6:
#line 94 "lang.y"
                          {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 1871 "y.tab.c"
    break;

  case 7:
#line 95 "lang.y"
                       {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 1877 "y.tab.c"
    break;

  case 8:
#line 96 "lang.y"
                               {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 1883 "y.tab.c"
    break;

  case 9:
#line 97 "lang.y"
                            {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 1889 "y.tab.c"
    break;

  case 10:
#line 98 "lang.y"
                          {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 1895 "y.tab.c"
    break;

  case 11:
#line 99 "lang.y"
                              {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 1901 "y.tab.c"
    break;

  case 12:
#line 100 "lang.y"
                            {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 1907 "y.tab.c"
    break;

  case 13:
#line 102 "lang.y"
                    {(yyval.dec) = (yyvsp[0].dec);}
#line 1913 "y.tab.c"
    break;

  case 14:
#line 103 "lang.y"
                      {(yyval.dec) = (yyvsp[0].dec);}
#line 1919 "y.tab.c"
    break;

  case 15:
#line 105 "lang.y"
                {(yyval.exp) = A_IntExp(EM_tokPos, (yyvsp[0].ival));}
#line 1925 "y.tab.c"
    break;

  case 16:
#line 106 "lang.y"
                     { (yyval.exp) = A_StringExp(EM_tokPos, (yyvsp[0].sval));}
#line 1931 "y.tab.c"
    break;

  case 17:
#line 107 "lang.y"
                   { (yyval.exp) = A_RealExp(EM_tokPos, (yyvsp[0].fval));}
#line 1937 "y.tab.c"
    break;

  case 18:
#line 108 "lang.y"
                  {(yyval.exp) = A_NilExp(EM_tokPos);}
#line 1943 "y.tab.c"
    break;

  case 19:
#line 109 "lang.y"
                   {(yyval.exp) = A_CharExp(EM_tokPos, (yyvsp[0].cval));}
#line 1949 "y.tab.c"
    break;

  case 20:
#line 110 "lang.y"
                    {(yyval.exp) = A_BoolExp(EM_tokPos, TRUE);}
#line 1955 "y.tab.c"
    break;

  case 21:
#line 111 "lang.y"
                     {(yyval.exp) = A_BoolExp(EM_tokPos, FALSE);}
#line 1961 "y.tab.c"
    break;

  case 22:
#line 113 "lang.y"
                           {(yyval.exp) = A_OpExp(EM_tokPos, A_plusOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1967 "y.tab.c"
    break;

  case 23:
#line 114 "lang.y"
                            {(yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1973 "y.tab.c"
    break;

  case 24:
#line 115 "lang.y"
                            {(yyval.exp) = A_OpExp(EM_tokPos, A_timesOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1979 "y.tab.c"
    break;

  case 25:
#line 116 "lang.y"
                             {(yyval.exp) = A_OpExp(EM_tokPos, A_divideOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1985 "y.tab.c"
    break;

  case 26:
#line 117 "lang.y"
                          {(yyval.exp) = A_OpExp(EM_tokPos, A_modOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1991 "y.tab.c"
    break;

  case 27:
#line 119 "lang.y"
                                     {(yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, A_IntExp(0, 0), (yyvsp[0].exp));}
#line 1997 "y.tab.c"
    break;

  case 28:
#line 120 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_eqOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2003 "y.tab.c"
    break;

  case 29:
#line 121 "lang.y"
                          {(yyval.exp) = A_OpExp(EM_tokPos, A_neqOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2009 "y.tab.c"
    break;

  case 30:
#line 122 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_geOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2015 "y.tab.c"
    break;

  case 31:
#line 123 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_gtOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2021 "y.tab.c"
    break;

  case 32:
#line 124 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_leOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2027 "y.tab.c"
    break;

  case 33:
#line 125 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_ltOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2033 "y.tab.c"
    break;

  case 34:
#line 127 "lang.y"
                                           {(yyval.exp) = A_CallExp(EM_tokPos, (yyvsp[-3].exp), (yyvsp[-1].expList));}
#line 2039 "y.tab.c"
    break;

  case 35:
#line 128 "lang.y"
                                {(yyval.exp) = (yyvsp[-1].exp);}
#line 2045 "y.tab.c"
    break;

  case 36:
#line 129 "lang.y"
                          {(yyval.exp) = A_OpExp(EM_tokPos, A_andOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2051 "y.tab.c"
    break;

  case 37:
#line 130 "lang.y"
                         {(yyval.exp) = A_OpExp(EM_tokPos, A_orOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2057 "y.tab.c"
    break;

  case 38:
#line 132 "lang.y"
                               {(yyval.exp) = A_AddressExp(EM_tokPos, (yyvsp[0].var));}
#line 2063 "y.tab.c"
    break;

  case 39:
#line 133 "lang.y"
                                          {(yyval.exp) = A_SizeofExp(EM_tokPos, (yyvsp[-1].var));}
#line 2069 "y.tab.c"
    break;

  case 40:
#line 134 "lang.y"
                                    {(yyval.exp) = A_ArrayExp(EM_tokPos, (yyvsp[-1].expList));}
#line 2075 "y.tab.c"
    break;

  case 41:
#line 135 "lang.y"
                     {(yyval.exp) = (yyvsp[0].exp);}
#line 2081 "y.tab.c"
    break;

  case 42:
#line 136 "lang.y"
                      {(yyval.exp) = (yyvsp[0].exp);}
#line 2087 "y.tab.c"
    break;

  case 43:
#line 138 "lang.y"
                   { (yyval.exp) = A_VarExp(EM_tokPos, (yyvsp[0].var)); }
#line 2093 "y.tab.c"
    break;

  case 44:
#line 139 "lang.y"
                         {(yyval.exp) = A_FieldExp(EM_tokPos, (yyvsp[-2].exp), (yyvsp[0].sym));}
#line 2099 "y.tab.c"
    break;

  case 45:
#line 140 "lang.y"
                                    {(yyval.exp) = A_SubscriptExp(EM_tokPos, (yyvsp[-3].exp), (yyvsp[-1].exp));}
#line 2105 "y.tab.c"
    break;

  case 46:
#line 141 "lang.y"
                                {(yyval.exp) = A_ArrowFieldExp(EM_tokPos, (yyvsp[-2].exp), (yyvsp[0].sym));}
#line 2111 "y.tab.c"
    break;

  case 47:
#line 143 "lang.y"
                                                                  {(yyval.exp) = A_FuncExp(EM_tokPos, (yyvsp[-6].fieldList), (yyvsp[-2].fieldList), (yyvsp[0].stm));}
#line 2117 "y.tab.c"
    break;

  case 48:
#line 147 "lang.y"
                    {(yyval.stm) = A_DeclarationStm(EM_tokPos, (yyvsp[0].dec));}
#line 2123 "y.tab.c"
    break;

  case 50:
#line 149 "lang.y"
                               {(yyval.stm) = (yyvsp[-1].stm);}
#line 2129 "y.tab.c"
    break;

  case 51:
#line 150 "lang.y"
                      {(yyval.stm) = A_DeclarationStm(EM_tokPos, (yyvsp[0].dec));}
#line 2135 "y.tab.c"
    break;

  case 52:
#line 151 "lang.y"
                 {(yyval.stm) = (yyvsp[0].stm);}
#line 2141 "y.tab.c"
    break;

  case 53:
#line 152 "lang.y"
                    {(yyval.stm) = (yyvsp[0].stm);}
#line 2147 "y.tab.c"
    break;

  case 54:
#line 153 "lang.y"
                  {(yyval.stm) = (yyvsp[0].stm);}
#line 2153 "y.tab.c"
    break;

  case 55:
#line 154 "lang.y"
                              {(yyval.stm) = (yyvsp[-1].stm);}
#line 2159 "y.tab.c"
    break;

  case 56:
#line 155 "lang.y"
                   {(yyval.stm) = A_DeclarationStm(EM_tokPos, A_FunctionDec(EM_tokPos, A_FundecList(A_Fundec(EM_tokPos, S_Symbol("loop"), A_FieldList(NULL, NULL), A_FieldList(NULL, NULL), (yyvsp[0].stm) -> u.loop.body), NULL)));}
#line 2165 "y.tab.c"
    break;

  case 57:
#line 156 "lang.y"
                              {(yyval.stm) = A_BreakStm(EM_tokPos);}
#line 2171 "y.tab.c"
    break;

  case 58:
#line 157 "lang.y"
                                 {(yyval.stm) = A_ContinueStm(EM_tokPos);}
#line 2177 "y.tab.c"
    break;

  case 59:
#line 158 "lang.y"
                                    {(yyval.stm) = A_CompoundStm(EM_tokPos, (yyvsp[-1].stmList));}
#line 2183 "y.tab.c"
    break;

  case 60:
#line 162 "lang.y"
                {(yyval.stmList) = A_StmList((yyvsp[0].stm), NULL);}
#line 2189 "y.tab.c"
    break;

  case 61:
#line 163 "lang.y"
                        {(yyval.stmList) = A_StmList((yyvsp[-1].stm), (yyvsp[0].stmList)); }
#line 2195 "y.tab.c"
    break;

  case 62:
#line 165 "lang.y"
                   {(yyval.stm) = A_ReturnStm(EM_tokPos, NULL);}
#line 2201 "y.tab.c"
    break;

  case 63:
#line 166 "lang.y"
                                       {(yyval.stm) = A_ReturnStm(EM_tokPos, (yyvsp[-1].exp));}
#line 2207 "y.tab.c"
    break;

  case 64:
#line 168 "lang.y"
                 {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("void"));}
#line 2213 "y.tab.c"
    break;

  case 65:
#line 169 "lang.y"
                      {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("int"));}
#line 2219 "y.tab.c"
    break;

  case 66:
#line 170 "lang.y"
                       {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("real"));}
#line 2225 "y.tab.c"
    break;

  case 67:
#line 171 "lang.y"
                      {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("int"));}
#line 2231 "y.tab.c"
    break;

  case 68:
#line 172 "lang.y"
                       {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("char"));}
#line 2237 "y.tab.c"
    break;

  case 69:
#line 173 "lang.y"
                        {(yyval.type) = A_NameTy(EM_tokPos, S_Symbol("short"));}
#line 2243 "y.tab.c"
    break;

  case 70:
#line 174 "lang.y"
                 {(yyval.type) = A_NameTy(EM_tokPos, (yyvsp[0].sym));}
#line 2249 "y.tab.c"
    break;

  case 71:
#line 175 "lang.y"
                         {(yyval.type) = A_PointerTy(EM_tokPos, (yyvsp[0].type));}
#line 2255 "y.tab.c"
    break;

  case 72:
#line 176 "lang.y"
                            {(yyval.type) = A_PolyTy(EM_tokPos, (yyvsp[-3].sym), (yyvsp[-1].type));}
#line 2261 "y.tab.c"
    break;

  case 73:
#line 178 "lang.y"
                        {(yyval.fieldList) = A_FieldList(NULL, NULL);}
#line 2267 "y.tab.c"
    break;

  case 74:
#line 179 "lang.y"
                   {(yyval.fieldList) = A_FieldList(NULL, NULL);}
#line 2273 "y.tab.c"
    break;

  case 75:
#line 180 "lang.y"
                       {(yyval.fieldList) = (yyvsp[0].fieldList);}
#line 2279 "y.tab.c"
    break;

  case 76:
#line 182 "lang.y"
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
#line 2308 "y.tab.c"
    break;

  case 77:
#line 206 "lang.y"
                                                                                  {(yyval.fieldList) = A_FieldList(A_Field(EM_tokPos, (yyvsp[-7].sym), A_FuncTy(EM_tokPos, (yyvsp[-5].fieldList), (yyvsp[-1].fieldList))), NULL);}
#line 2314 "y.tab.c"
    break;

  case 78:
#line 207 "lang.y"
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
#line 2342 "y.tab.c"
    break;

  case 79:
#line 230 "lang.y"
                                                                                                 {(yyval.fieldList) = A_FieldList(A_Field(EM_tokPos, (yyvsp[-9].sym), A_FuncTy(EM_tokPos, (yyvsp[-7].fieldList), (yyvsp[-3].fieldList))), (yyvsp[0].fieldList));}
#line 2348 "y.tab.c"
    break;

  case 80:
#line 232 "lang.y"
                                                                                    {(yyval.dec) = A_FunctionDec(EM_tokPos, A_FundecList(A_Fundec(EM_tokPos, (yyvsp[-8].sym), (yyvsp[-6].fieldList), (yyvsp[-2].fieldList), (yyvsp[0].stm)), NULL));}
#line 2354 "y.tab.c"
    break;

  case 81:
#line 235 "lang.y"
                                 {(yyval.stm) = A_CallStm(EM_tokPos, (yyvsp[-2].exp), NULL);}
#line 2360 "y.tab.c"
    break;

  case 82:
#line 236 "lang.y"
                                           {(yyval.stm) = A_CallStm(EM_tokPos, (yyvsp[-3].exp), (yyvsp[-1].expList));}
#line 2366 "y.tab.c"
    break;

  case 83:
#line 238 "lang.y"
                                                                                                                                          {(yyval.dec) = A_FuncImport(EM_tokPos, (yyvsp[-15].sym), (yyvsp[-13].fieldList), (yyvsp[-9].fieldList), (yyvsp[-4].sval), (yyvsp[-2].sval));}
#line 2372 "y.tab.c"
    break;

  case 84:
#line 240 "lang.y"
                                                             {(yyval.dec) = A_FuncExport(EM_tokPos, (yyvsp[-4].sym), (yyvsp[-2].sval));}
#line 2378 "y.tab.c"
    break;

  case 85:
#line 242 "lang.y"
                        {(yyval.decList) = A_DecList((yyvsp[0].dec), NULL);}
#line 2384 "y.tab.c"
    break;

  case 86:
#line 243 "lang.y"
                                     {(yyval.decList) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decList));}
#line 2390 "y.tab.c"
    break;

  case 87:
#line 245 "lang.y"
                        {(yyval.memList) = A_ClassMemFromDecList((yyvsp[0].decList), A_public);}
#line 2396 "y.tab.c"
    break;

  case 88:
#line 246 "lang.y"
                                       {(yyval.memList) = A_ClassMemFromDecList((yyvsp[0].decList), A_public);}
#line 2402 "y.tab.c"
    break;

  case 89:
#line 247 "lang.y"
                                          {(yyval.memList) = A_ClassMemFromDecList((yyvsp[0].decList), A_protected);}
#line 2408 "y.tab.c"
    break;

  case 90:
#line 248 "lang.y"
                                        {(yyval.memList) = A_ClassMemFromDecList((yyvsp[0].decList), A_private);}
#line 2414 "y.tab.c"
    break;

  case 91:
#line 250 "lang.y"
                    {(yyval.memList) = A_ClassMemFromTwoList((yyvsp[0].memList), NULL);}
#line 2420 "y.tab.c"
    break;

  case 92:
#line 251 "lang.y"
                           {(yyval.memList) = A_ClassMemFromTwoList((yyvsp[-1].memList), (yyvsp[0].memList));}
#line 2426 "y.tab.c"
    break;

  case 93:
#line 253 "lang.y"
                                              {(yyval.dec) = A_ClassDec(EM_tokPos, (yyvsp[-3].sym), (yyvsp[-1].memList), NULL);}
#line 2432 "y.tab.c"
    break;

  case 94:
#line 254 "lang.y"
                                                             {(yyval.dec) = A_ClassDec(EM_tokPos, (yyvsp[-5].sym), (yyvsp[-1].memList), S_SymbolList((yyvsp[-3].sym), NULL));}
#line 2438 "y.tab.c"
    break;

  case 95:
#line 257 "lang.y"
                                        {(yyval.dec) = A_TemplateDec(EM_tokPos, (yyvsp[-3].sym), (yyvsp[0].dec));}
#line 2444 "y.tab.c"
    break;

  case 96:
#line 258 "lang.y"
                                           {(yyval.dec) = A_TemplateDec(EM_tokPos, (yyvsp[-3].sym), (yyvsp[0].dec));}
#line 2450 "y.tab.c"
    break;

  case 97:
#line 260 "lang.y"
                              {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-2].var), (yyvsp[0].exp), FALSE);}
#line 2456 "y.tab.c"
    break;

  case 98:
#line 261 "lang.y"
                               {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-2].var), A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, (yyvsp[-2].var)), A_IntExp(EM_tokPos, 1)), FALSE);}
#line 2462 "y.tab.c"
    break;

  case 99:
#line 262 "lang.y"
                                 {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-2].var), A_OpExp(EM_tokPos, A_minusOp, A_VarExp(EM_tokPos, (yyvsp[-2].var)), A_IntExp(EM_tokPos, 1)), FALSE);}
#line 2468 "y.tab.c"
    break;

  case 100:
#line 263 "lang.y"
                                     {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-3].var), A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, (yyvsp[-3].var)), (yyvsp[0].exp)), FALSE);}
#line 2474 "y.tab.c"
    break;

  case 101:
#line 264 "lang.y"
                                      {(yyval.stm) = A_AssignStm(EM_tokPos, (yyvsp[-3].var), A_OpExp(EM_tokPos, A_minusOp, A_VarExp(EM_tokPos, (yyvsp[-3].var)), (yyvsp[0].exp)), FALSE);}
#line 2480 "y.tab.c"
    break;

  case 102:
#line 266 "lang.y"
                                        {(yyval.dec) = A_VarDec(EM_tokPos, (yyvsp[-1].stm), (yyvsp[-3].type));}
#line 2486 "y.tab.c"
    break;

  case 103:
#line 267 "lang.y"
                                         {(yyval.dec) = A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, (yyvsp[-1].var), NULL, TRUE), (yyvsp[-3].type));}
#line 2492 "y.tab.c"
    break;

  case 104:
#line 268 "lang.y"
                                                        {(yyval.dec) = A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, (yyvsp[-3].var), NULL, TRUE), (yyvsp[-5].type));}
#line 2498 "y.tab.c"
    break;

  case 105:
#line 269 "lang.y"
                                                            {(yyval.dec) = A_ObjectDec(EM_tokPos, (yyvsp[-6].type), (yyvsp[-4].sym), (yyvsp[-2].expList));}
#line 2504 "y.tab.c"
    break;

  case 106:
#line 270 "lang.y"
                                      {(yyval.dec) = A_ObjectDec(EM_tokPos, (yyvsp[-3].type), (yyvsp[-1].sym), NULL);}
#line 2510 "y.tab.c"
    break;

  case 107:
#line 272 "lang.y"
                                                                 {(yyval.stm) = A_IfStm(EM_tokPos, (yyvsp[-3].exp), (yyvsp[0].stm), NULL);}
#line 2516 "y.tab.c"
    break;

  case 108:
#line 273 "lang.y"
                                                     {(yyval.stm) = A_IfStm(EM_tokPos, (yyvsp[-5].exp), (yyvsp[-2].stm), (yyvsp[0].stm));}
#line 2522 "y.tab.c"
    break;

  case 109:
#line 275 "lang.y"
                                     {(yyval.stm) = A_WhileStm(EM_tokPos, (yyvsp[-3].exp), (yyvsp[0].stm));}
#line 2528 "y.tab.c"
    break;

  case 110:
#line 277 "lang.y"
                                                                              {(yyval.stm) = A_ForStm(EM_tokPos, (yyvsp[-10].stm), (yyvsp[-6].exp), (yyvsp[-2].stm), (yyvsp[0].stm));}
#line 2534 "y.tab.c"
    break;

  case 111:
#line 278 "lang.y"
                                           {(yyval.stm) = A_ForStm(EM_tokPos, A_DeclarationStm(EM_tokPos, A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ")), A_IntExp(EM_tokPos, 0), TRUE), A_NameTy(EM_tokPos, S_Symbol("int")))), A_OpExp(EM_tokPos, A_eqOp, A_VarExp(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ"))), (yyvsp[-3].exp)), A_AssignStm(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ")), A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ"))), A_IntExp(EM_tokPos, 1)), FALSE), (yyvsp[0].stm));}
#line 2540 "y.tab.c"
    break;

  case 112:
#line 281 "lang.y"
                     {(yyval.stm) = A_LoopStm(EM_tokPos, (yyvsp[0].stm)); }
#line 2546 "y.tab.c"
    break;

  case 113:
#line 283 "lang.y"
                              {(yyval.expList) = A_ExpList((yyvsp[-2].exp), (yyvsp[0].expList));}
#line 2552 "y.tab.c"
    break;

  case 114:
#line 284 "lang.y"
                  { (yyval.expList) = A_ExpList((yyvsp[0].exp), 0); }
#line 2558 "y.tab.c"
    break;

  case 115:
#line 285 "lang.y"
              {(yyval.expList) = NULL;}
#line 2564 "y.tab.c"
    break;

  case 116:
#line 287 "lang.y"
               { (yyval.sym) = S_Symbol((yyvsp[0].sval)); }
#line 2570 "y.tab.c"
    break;

  case 117:
#line 289 "lang.y"
               {(yyval.var) = A_SimpleVar(EM_tokPos, (yyvsp[0].sym));}
#line 2576 "y.tab.c"
    break;

  case 118:
#line 290 "lang.y"
                            {(yyval.var) = A_FieldVar(EM_tokPos, (yyvsp[-2].var), (yyvsp[0].sym));}
#line 2582 "y.tab.c"
    break;

  case 119:
#line 291 "lang.y"
                           {(yyval.var) = A_DerefVar(EM_tokPos, (yyvsp[0].var));}
#line 2588 "y.tab.c"
    break;

  case 120:
#line 292 "lang.y"
                                       { (yyval.var) = A_SubscriptVar(EM_tokPos, (yyvsp[-3].var), (yyvsp[-1].exp)); }
#line 2594 "y.tab.c"
    break;

  case 121:
#line 293 "lang.y"
                                   {(yyval.var) = A_ArrowFieldVar(EM_tokPos, (yyvsp[-2].var), (yyvsp[0].sym));}
#line 2600 "y.tab.c"
    break;


#line 2604 "y.tab.c"

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
