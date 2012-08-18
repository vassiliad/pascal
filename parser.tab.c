/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "parser.y"

#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dead_code.h"
#include "constants.h"
#include "bison_union.h"
#include "expressions.h"
#include "symbol_table.h"
#include "statements.h"
#include "tree.h"
#include "printer.h"
#include "register.h"

#define YYERROR_VERBOSE 1


extern FILE* yyin;
int enable_constant_propagation = 0;
int enable_dead_code_elimination = 0;

static const struct option l_opts[] = {
  	{	"help",		no_argument,		NULL,	'h' },
		{ "constant_propagation", no_argument, NULL , 'c' },
		{ "dead_code_elimination", no_argument, NULL , 'd' }
	};

static const char s_opts[] = "hcd";


int yylex(void);
void yyerror(const char *fmt, ...);

extern int yylineno;
scope_t *scope;


/* Line 268 of yacc.c  */
#line 112 "parser.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PROGRAM = 258,
     CONST = 259,
     TYPE = 260,
     ARRAY = 261,
     LIST = 262,
     SET = 263,
     OF = 264,
     RECORD = 265,
     VAR = 266,
     FUNCTION = 267,
     PROCEDURE = 268,
     INTEGER = 269,
     REAL = 270,
     BOOLEAN = 271,
     CHAR = 272,
     FORWARD = 273,
     LENGTH = 274,
     NEW = 275,
     T_BEGIN = 276,
     END = 277,
     IF = 278,
     THEN = 279,
     ELSE = 280,
     WHILE = 281,
     DO = 282,
     CASE = 283,
     OTHERWISE = 284,
     FOR = 285,
     TO = 286,
     DOWNTO = 287,
     WITH = 288,
     READ = 289,
     WRITE = 290,
     LISTFUNC = 291,
     SEMI = 292,
     CCONST = 293,
     BCONST = 294,
     STRING = 295,
     RCONST = 296,
     ICONST = 297,
     ID = 298,
     RBRACK = 299,
     RPAREN = 300,
     COMMA = 301,
     ASSIGN = 302,
     DOTDOT = 303,
     COLON = 304,
     LBRACK = 305,
     INOP = 306,
     RELOP = 307,
     EQU = 308,
     ADDOP = 309,
     OROP = 310,
     MULDIVANDOP = 311,
     NOTOP = 312,
     DOT = 313,
     LPAREN = 314
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 56 "parser.y"

  char *id;
  int  iconst;
  double rconst;
  int bconst;
  char cconst;
  int op;
  char *listfunc;
  char *string;
  int sign;

  data_type_t dataType;
  limit_t limit;
  limits_t limits;
  identifiers_t identifiers;
  variabledefs_t variabledefs;
  declarations_t declarations;
  fields_t fields;
  typedefs_t typedefs;
  typedefs_entry_t type_def;
  expression_t *expr;
  expressions_t expressions;
  constant_t constant;
  variable_t *variable;
  sub_header_t sub_header;
  int pass;
  parameter_list_t params;
  constdefs_t constdefs;
  statement_t *statement;
  iter_space_t iter_space;



/* Line 293 of yacc.c  */
#line 241 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 253 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   354

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  134
/* YYNRULES -- Number of states.  */
#define YYNSTATES  267

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   314

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      55,    56,    57,    58,    59
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     9,    13,    17,    21,    22,    28,    32,
      36,    40,    44,    48,    52,    56,    59,    62,    64,    69,
      74,    79,    81,    85,    87,    89,    93,    98,   103,   107,
     109,   111,   113,   115,   117,   121,   124,   128,   129,   135,
     139,   146,   150,   154,   158,   162,   166,   168,   172,   174,
     177,   179,   181,   184,   186,   188,   189,   191,   193,   195,
     197,   199,   201,   205,   207,   211,   215,   217,   221,   222,
     228,   232,   236,   237,   241,   242,   243,   251,   257,   263,
     267,   270,   274,   275,   282,   287,   289,   290,   294,   298,
     300,   302,   304,   306,   308,   310,   312,   314,   316,   318,
     320,   322,   323,   330,   335,   342,   346,   350,   357,   361,
     363,   367,   368,   372,   374,   377,   380,   385,   386,   391,
     398,   402,   406,   407,   413,   415,   420,   425,   430,   434,
     436,   438,   442,   444,   446
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      61,     0,    -1,    62,    63,    85,    93,    58,    -1,     3,
      43,    37,    -1,    64,    71,    83,    -1,     4,    65,    37,
      -1,    -1,    65,    37,    43,    53,    66,    -1,    43,    53,
      66,    -1,    66,    52,    66,    -1,    66,    53,    66,    -1,
      66,    51,    66,    -1,    66,    55,    66,    -1,    66,    54,
      66,    -1,    66,    56,    66,    -1,    54,    66,    -1,    57,
      66,    -1,    67,    -1,    43,    59,    68,    45,    -1,    19,
      59,    66,    45,    -1,    20,    59,    66,    45,    -1,    69,
      -1,    59,    66,    45,    -1,    70,    -1,    43,    -1,    67,
      58,    43,    -1,    67,    50,    68,    44,    -1,    36,    59,
      66,    45,    -1,    68,    46,    66,    -1,    66,    -1,    42,
      -1,    41,    -1,    39,    -1,    38,    -1,    50,    68,    44,
      -1,    50,    44,    -1,     5,    72,    37,    -1,    -1,    72,
      37,    43,    53,    73,    -1,    43,    53,    73,    -1,     6,
      50,    74,    44,     9,    78,    -1,     7,     9,    78,    -1,
       8,     9,    78,    -1,    10,    80,    22,    -1,    76,    48,
      76,    -1,    74,    46,    75,    -1,    75,    -1,    76,    48,
      76,    -1,    43,    -1,    77,    42,    -1,    38,    -1,    39,
      -1,    54,    43,    -1,    43,    -1,    54,    -1,    -1,    79,
      -1,    43,    -1,    14,    -1,    15,    -1,    16,    -1,    17,
      -1,    80,    37,    81,    -1,    81,    -1,    82,    49,    78,
      -1,    82,    46,    43,    -1,    43,    -1,    11,    84,    37,
      -1,    -1,    84,    37,    82,    49,    78,    -1,    82,    49,
      78,    -1,    85,    86,    37,    -1,    -1,    89,    37,    18,
      -1,    -1,    -1,    89,    37,    87,    63,    88,    85,    93,
      -1,    12,    43,    90,    49,    79,    -1,    12,    43,    90,
      49,     7,    -1,    13,    43,    90,    -1,    12,    43,    -1,
      59,    91,    45,    -1,    -1,    91,    37,    92,    82,    49,
      78,    -1,    92,    82,    49,    78,    -1,    11,    -1,    -1,
      21,    94,    22,    -1,    94,    37,    95,    -1,    95,    -1,
      96,    -1,    98,    -1,    99,    -1,    97,    -1,   100,    -1,
     106,    -1,   107,    -1,   109,    -1,   111,    -1,   112,    -1,
      93,    -1,    -1,    23,    66,    24,    96,    25,    96,    -1,
      23,    66,    24,    95,    -1,    23,    66,    24,    96,    25,
      98,    -1,    67,    47,    66,    -1,    67,    47,    40,    -1,
      28,    66,     9,   101,   105,    22,    -1,   101,    37,   102,
      -1,   102,    -1,   103,    49,    95,    -1,    -1,   103,    46,
     104,    -1,   104,    -1,    77,    69,    -1,    77,    43,    -1,
      37,    29,    49,    95,    -1,    -1,    26,    66,    27,    95,
      -1,    30,    43,    47,   108,    27,    95,    -1,    66,    31,
      66,    -1,    66,    32,    66,    -1,    -1,    33,    67,    27,
     110,    95,    -1,    43,    -1,    43,    59,    68,    45,    -1,
      34,    59,   113,    45,    -1,    35,    59,   115,    45,    -1,
     113,    46,   114,    -1,   114,    -1,    67,    -1,   115,    46,
     116,    -1,   116,    -1,    66,    -1,    40,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   121,   121,   143,   150,   194,   199,   203,   216,   231,
     258,   286,   314,   342,   370,   398,   425,   444,   448,   452,
     457,   462,   466,   470,   479,   484,   494,   502,   510,   521,
     530,   535,   541,   546,   553,   557,   564,   569,   574,   583,
     592,   598,   603,   608,   613,   621,   628,   634,   643,   655,
     664,   669,   674,   681,   689,   697,   701,   705,   712,   716,
     720,   724,   730,   742,   748,   763,   771,   779,   784,   790,
     800,   810,   811,   814,   856,   867,   855,   921,   930,   939,
     948,   959,   964,   969,   977,   987,   992,   996,  1002,  1059,
    1065,  1066,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,
    1078,  1080,  1086,  1097,  1106,  1117,  1126,  1137,  1147,  1148,
    1151,  1152,  1155,  1156,  1159,  1160,  1164,  1165,  1168,  1182,
    1193,  1199,  1209,  1208,  1221,  1233,  1247,  1254,  1262,  1263,
    1266,  1269,  1270,  1273,  1274
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM", "CONST", "TYPE", "ARRAY",
  "LIST", "SET", "OF", "RECORD", "VAR", "FUNCTION", "PROCEDURE", "INTEGER",
  "REAL", "BOOLEAN", "CHAR", "FORWARD", "LENGTH", "NEW", "T_BEGIN", "END",
  "IF", "THEN", "ELSE", "WHILE", "DO", "CASE", "OTHERWISE", "FOR", "TO",
  "DOWNTO", "WITH", "READ", "WRITE", "LISTFUNC", "SEMI", "CCONST",
  "BCONST", "STRING", "RCONST", "ICONST", "ID", "RBRACK", "RPAREN",
  "COMMA", "ASSIGN", "DOTDOT", "COLON", "LBRACK", "INOP", "RELOP", "EQU",
  "ADDOP", "OROP", "MULDIVANDOP", "NOTOP", "DOT", "LPAREN", "$accept",
  "program", "header", "declarations", "constdefs", "constant_defs",
  "expression", "variable", "expressions", "constant", "setlistexpression",
  "typedefs", "type_defs", "type_def", "dims", "limits", "limit", "sign",
  "typename", "standard_type", "fields", "field", "identifiers", "vardefs",
  "variable_defs", "subprograms", "subprogram", "$@1", "$@2", "sub_header",
  "formal_parameters", "parameter_list", "pass", "comp_statement",
  "statements", "statement", "matched", "matched_if_statement",
  "unmatched", "assignment", "case_statement", "cases", "single_case",
  "label_list", "label", "case_tail", "while_statement", "for_statement",
  "iter_space", "with_statement", "@3", "subprogram_call", "io_statement",
  "read_list", "read_item", "write_list", "write_item", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    60,    61,    62,    63,    64,    64,    65,    65,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    67,    67,    67,    67,    68,    68,
      69,    69,    69,    69,    70,    70,    71,    71,    72,    72,
      73,    73,    73,    73,    73,    74,    74,    75,    75,    76,
      76,    76,    76,    76,    77,    77,    78,    78,    79,    79,
      79,    79,    80,    80,    81,    82,    82,    83,    83,    84,
      84,    85,    85,    86,    87,    88,    86,    89,    89,    89,
      89,    90,    90,    91,    91,    92,    92,    93,    94,    94,
      95,    95,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    97,    98,    98,    99,    99,   100,   101,   101,
     102,   102,   103,   103,   104,   104,   105,   105,   106,   107,
     108,   108,   110,   109,   111,   111,   112,   112,   113,   113,
     114,   115,   115,   116,   116
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     3,     3,     3,     0,     5,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     1,     4,     4,
       4,     1,     3,     1,     1,     3,     4,     4,     3,     1,
       1,     1,     1,     1,     3,     2,     3,     0,     5,     3,
       6,     3,     3,     3,     3,     3,     1,     3,     1,     2,
       1,     1,     2,     1,     1,     0,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     3,     1,     3,     0,     5,
       3,     3,     0,     3,     0,     0,     7,     5,     5,     3,
       2,     3,     0,     6,     4,     1,     0,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     6,     4,     6,     3,     3,     6,     3,     1,
       3,     0,     3,     1,     2,     2,     4,     0,     4,     6,
       3,     3,     0,     5,     1,     4,     4,     4,     3,     1,
       1,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     6,     0,     1,     0,    72,    37,     3,
       0,     0,     0,     0,    68,     0,     5,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     4,     0,     0,     0,
      33,    32,    31,    30,    24,     0,     0,     0,     0,     8,
      17,    21,    23,     0,    80,    82,     0,     0,     0,     0,
       0,     0,     0,    24,     0,   100,     0,    89,    90,    93,
      91,    92,    94,    95,    96,    97,    98,    99,    71,    74,
       2,    55,    36,    66,     0,     0,     0,     0,     0,     0,
      35,    29,     0,    15,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,     0,    79,     0,     0,
       0,     0,    24,     0,     0,     0,     0,     0,    87,   101,
      73,     6,     0,     0,     0,     0,    50,    51,    53,    54,
      39,     0,     0,     0,     0,     0,    67,     0,     0,     0,
       0,    34,     0,    22,    11,     9,    10,    13,    12,    14,
       0,    25,     7,    85,     0,     0,     0,   101,   101,    55,
       0,   122,   130,     0,   129,   134,   133,     0,   132,     0,
     106,   105,    88,    75,    55,     0,     0,     0,    63,     0,
      52,    55,    49,    55,    65,    58,    59,    60,    61,    57,
      70,    56,     0,    19,    20,    27,    18,    28,    26,    86,
      81,     0,    78,    77,   103,    90,   118,    54,     0,   117,
     109,     0,   113,     0,     0,   101,   126,     0,   127,     0,
     125,    72,    48,     0,    46,     0,    41,    42,    43,     0,
       0,    44,    38,     0,     0,     0,   101,   115,   114,    55,
       0,    55,   101,     0,     0,   101,   123,   128,   131,     0,
       0,    55,    55,    62,    64,    69,     0,    84,   102,   104,
       0,   108,   107,   112,   110,   120,   121,   119,    76,     0,
      45,    47,     0,   101,    40,    83,   116
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     7,     8,    11,    81,    40,    82,    41,
      42,    14,    24,   120,   213,   214,   121,   122,   180,   181,
     167,   168,   169,    26,    75,    12,    20,   111,   211,    21,
      96,   144,   145,    55,    56,    57,    58,    59,    60,    61,
      62,   199,   200,   201,   202,   230,    63,    64,   204,    65,
     205,    66,    67,   153,   154,   157,   158
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -155
static const yytype_int16 yypact[] =
{
      17,    -4,    48,    54,    52,  -155,    56,  -155,    90,  -155,
      67,    64,    16,    89,   133,   195,   107,   114,   124,   255,
     142,   144,   111,   122,   160,   152,  -155,   147,   150,   159,
    -155,  -155,  -155,  -155,   170,    97,   195,   195,   195,   204,
     -42,  -155,  -155,   164,    11,   173,   195,   195,   195,   176,
      55,   184,   187,   -15,   -20,  -155,    -1,  -155,  -155,  -155,
    -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,   212,
    -155,     7,   192,  -155,    57,   210,   195,   195,   195,   195,
    -155,   204,    13,   197,  -155,   241,   195,   195,   195,   195,
     195,   195,   195,   205,   195,   252,   215,  -155,    58,   171,
       0,   218,  -155,    -9,    55,   123,   195,   151,  -155,   255,
    -155,    54,   217,   260,   270,   152,  -155,  -155,  -155,   237,
    -155,   234,   242,   247,   244,   225,   152,   254,   266,   278,
     138,  -155,   195,  -155,   288,   288,   288,   197,   197,  -155,
     128,  -155,   204,  -155,    -5,   152,   321,   255,   255,    25,
     195,  -155,   -42,   154,  -155,  -155,   204,   158,  -155,   166,
    -155,   204,  -155,  -155,    29,   225,   225,    65,  -155,    69,
    -155,   223,  -155,     7,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,    85,  -155,  -155,  -155,  -155,   204,  -155,   252,
    -155,    99,  -155,  -155,  -155,   276,  -155,  -155,    83,   265,
    -155,   100,  -155,   219,   277,   255,  -155,    55,  -155,   123,
    -155,  -155,   264,   132,  -155,   267,  -155,  -155,  -155,   152,
     225,  -155,  -155,   225,   152,   225,   255,  -155,  -155,   131,
     281,   259,   255,   195,   195,   255,  -155,  -155,  -155,    16,
     305,    29,   223,  -155,  -155,  -155,   106,  -155,  -155,  -155,
     275,  -155,  -155,  -155,  -155,   204,   204,  -155,  -155,   225,
    -155,  -155,   225,   255,  -155,  -155,  -155
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -155,  -155,  -155,   214,  -155,  -155,   -13,   -19,     1,   118,
    -155,  -155,  -155,   153,  -155,    86,  -145,  -143,  -154,   199,
    -155,   127,   -22,  -155,  -155,   136,  -155,  -155,  -155,  -155,
     303,  -155,   161,   -11,  -155,  -105,  -142,  -155,   125,  -155,
    -155,  -155,   120,  -155,   121,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,  -155,  -155,   146,  -155,   145
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -125
static const yytype_int16 yytable[] =
{
      54,    22,    39,    74,   162,   195,   198,  -124,    92,   149,
    -124,   216,   217,   112,   113,   114,    93,   115,   151,   215,
       1,   108,  -124,    83,    84,    85,   221,   107,    17,    18,
      92,   103,   189,    98,    99,   100,   109,    19,    93,     4,
     190,    92,   194,   196,   106,   116,   117,  -111,     5,    93,
     118,    86,    87,    88,    89,    90,    91,   131,     6,   132,
     -82,   119,  -111,   127,   128,   129,   244,   116,   117,   245,
      95,   247,   212,   134,   135,   136,   137,   138,   139,   197,
     130,   142,   147,   119,   248,   152,   198,   218,   198,     9,
      54,    29,   156,   140,   161,    13,   215,   261,   102,    10,
     236,    16,   219,   124,   182,   264,   125,   159,   265,    86,
      87,    88,    89,    90,    91,   124,    27,    28,   220,   187,
      15,    30,    31,   191,    32,    33,   227,   254,    54,    54,
     257,   124,    23,    29,   223,    30,    31,   203,    32,    33,
      34,    80,    27,    28,    25,   124,   231,    35,   225,   232,
      43,    36,   124,  -111,    37,   262,    38,    44,   266,    29,
     250,    30,    31,   155,    32,    33,    34,    45,  -111,    70,
      27,    28,   188,    35,   132,    71,   240,    36,   241,    68,
      37,    69,    38,   186,   132,   197,    54,    29,   152,    30,
      31,   160,    32,    33,    34,    73,   156,    72,   148,   206,
     207,    35,   246,   208,   209,    36,    76,    54,    37,    77,
      38,   210,   132,    54,    27,    28,    54,    94,    78,   101,
     255,   256,    86,    87,    88,    89,    90,    91,   258,    79,
     110,    29,    95,    30,    31,   123,    32,    33,    34,   175,
     176,   177,   178,   104,    54,    35,   105,   126,   141,    36,
     233,   234,    37,    91,    38,    86,    87,    88,    89,    90,
      91,   116,   117,   143,   146,   150,   118,   164,   179,   165,
      86,    87,    88,    89,    90,    91,    19,   119,    46,   166,
     170,    47,   171,    48,   172,    49,   133,   174,    50,    51,
      52,    29,    86,    87,    88,    89,    90,    91,    53,   183,
     173,   226,   229,   252,   235,    86,    87,    88,    89,    90,
      91,   184,   -53,   197,   259,   242,   228,    86,    87,    88,
      89,    90,    91,   185,   263,   163,   222,   260,   192,    86,
      87,    88,    89,    90,    91,   175,   176,   177,   178,  -125,
    -125,  -125,    89,    90,    91,   193,   243,   239,    97,   251,
     224,   249,   253,   237,   238
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-155))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-125))

static const yytype_uint16 yycheck[] =
{
      19,    12,    15,    25,   109,   147,   149,    22,    50,     9,
      25,   165,   166,     6,     7,     8,    58,    10,    27,   164,
       3,    22,    37,    36,    37,    38,   171,    47,    12,    13,
      50,    50,    37,    46,    47,    48,    37,    21,    58,    43,
      45,    50,   147,   148,    59,    38,    39,    22,     0,    58,
      43,    51,    52,    53,    54,    55,    56,    44,     4,    46,
      49,    54,    37,    76,    77,    78,   220,    38,    39,   223,
      59,   225,    43,    86,    87,    88,    89,    90,    91,    54,
      79,    94,    24,    54,   226,   104,   229,    22,   231,    37,
     109,    36,   105,    92,   107,     5,   241,   242,    43,    43,
     205,    37,    37,    46,   126,   259,    49,   106,   262,    51,
      52,    53,    54,    55,    56,    46,    19,    20,    49,   132,
      53,    38,    39,   145,    41,    42,    43,   232,   147,   148,
     235,    46,    43,    36,    49,    38,    39,   150,    41,    42,
      43,    44,    19,    20,    11,    46,    46,    50,    49,    49,
      43,    54,    46,    22,    57,    49,    59,    43,   263,    36,
      29,    38,    39,    40,    41,    42,    43,    43,    37,    58,
      19,    20,    44,    50,    46,    53,    44,    54,    46,    37,
      57,    37,    59,    45,    46,    54,   205,    36,   207,    38,
      39,    40,    41,    42,    43,    43,   209,    37,    27,    45,
      46,    50,   224,    45,    46,    54,    59,   226,    57,    59,
      59,    45,    46,   232,    19,    20,   235,    53,    59,    43,
     233,   234,    51,    52,    53,    54,    55,    56,   239,    59,
      18,    36,    59,    38,    39,    43,    41,    42,    43,    14,
      15,    16,    17,    59,   263,    50,    59,    37,    43,    54,
      31,    32,    57,    56,    59,    51,    52,    53,    54,    55,
      56,    38,    39,    11,    49,    47,    43,    50,    43,     9,
      51,    52,    53,    54,    55,    56,    21,    54,    23,     9,
      43,    26,    48,    28,    42,    30,    45,    43,    33,    34,
      35,    36,    51,    52,    53,    54,    55,    56,    43,    45,
      53,    25,    37,    22,    27,    51,    52,    53,    54,    55,
      56,    45,    48,    54,     9,    48,   198,    51,    52,    53,
      54,    55,    56,    45,    49,   111,   173,   241,     7,    51,
      52,    53,    54,    55,    56,    14,    15,    16,    17,    51,
      52,    53,    54,    55,    56,   146,   219,   211,    45,   229,
     189,   226,   231,   207,   209
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    61,    62,    43,     0,     4,    63,    64,    37,
      43,    65,    85,     5,    71,    53,    37,    12,    13,    21,
      86,    89,    93,    43,    72,    11,    83,    19,    20,    36,
      38,    39,    41,    42,    43,    50,    54,    57,    59,    66,
      67,    69,    70,    43,    43,    43,    23,    26,    28,    30,
      33,    34,    35,    43,    67,    93,    94,    95,    96,    97,
      98,    99,   100,   106,   107,   109,   111,   112,    37,    37,
      58,    53,    37,    43,    82,    84,    59,    59,    59,    59,
      44,    66,    68,    66,    66,    66,    51,    52,    53,    54,
      55,    56,    50,    58,    53,    59,    90,    90,    66,    66,
      66,    43,    43,    67,    59,    59,    59,    47,    22,    37,
      18,    87,     6,     7,     8,    10,    38,    39,    43,    54,
      73,    76,    77,    43,    46,    49,    37,    66,    66,    66,
      68,    44,    46,    45,    66,    66,    66,    66,    66,    66,
      68,    43,    66,    11,    91,    92,    49,    24,    27,     9,
      47,    27,    67,   113,   114,    40,    66,   115,   116,    68,
      40,    66,    95,    63,    50,     9,     9,    80,    81,    82,
      43,    48,    42,    53,    43,    14,    15,    16,    17,    43,
      78,    79,    82,    45,    45,    45,    45,    66,    44,    37,
      45,    82,     7,    79,    95,    96,    95,    54,    77,   101,
     102,   103,   104,    66,   108,   110,    45,    46,    45,    46,
      45,    88,    43,    74,    75,    76,    78,    78,    22,    37,
      49,    76,    73,    49,    92,    49,    25,    43,    69,    37,
     105,    46,    49,    31,    32,    27,    95,   114,   116,    85,
      44,    46,    48,    81,    78,    78,    82,    78,    96,    98,
      29,   102,    22,   104,    95,    66,    66,    95,    93,     9,
      75,    76,    49,    49,    78,    78,    95
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
      int yyn = yypact[*yyssp];
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
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
      yychar = YYLEX;
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

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

/* Line 1806 of yacc.c  */
#line 122 "parser.y"
    {
  node_list_t *main_tree;
	statement_t *body = (yyvsp[(4) - (5)].statement);
	
	if ( enable_dead_code_elimination ) {
		body = dead_code_elimination(body, scope);
	}
	
	main_tree = tree_generate_tree( body, scope );

  if ( main_tree == NULL )
  {
		printf("[-] Failed to generate instruction tree\n");
  } else {
		printf("[+] Printing instruction tree\n");
		givepostnumbers_tree(main_tree);
		print_instruction_tree(main_tree, stdout);
	}
}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 144 "parser.y"
    {
  (yyval.id) = (yyvsp[(2) - (3)].id);
  scope = st_init(NULL); //global scope
}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 151 "parser.y"
    {
  int i =0,j;
  //enum TypedefType { TT_Array, TT_List, TT_Set, TT_Record, TT_Range };
  const char *typedef_types[] = { "Array", "List", "Set", "Record", "Range" };

  (yyval.declarations).vardefs = (yyvsp[(3) - (3)].variabledefs);
  (yyval.declarations).typedefs = (yyvsp[(2) - (3)].typedefs);
  (yyval.declarations).constdefs = (yyvsp[(1) - (3)].constdefs);

  printf("Declarations::constdefs %d\n", (yyvsp[(1) - (3)].constdefs).size);
  for ( i = 0 ; i < (yyvsp[(1) - (3)].constdefs).size; i ++ ) {
    if ( st_const_define((yyvsp[(1) - (3)].constdefs).ids[i], (yyvsp[(1) - (3)].constdefs).constants+i, scope) == NULL ) {
			yyerror("Could not define Constant");
			return 1;
		}
  }

  printf("Declarations::typedefs %d\n", (yyvsp[(2) - (3)].typedefs).size);

  for (i=0; i<(yyvsp[(2) - (3)].typedefs).size; i++ ) {
    printf("%s -> %s\n", (yyvsp[(2) - (3)].typedefs).typedefs[i].name, typedef_types[(yyvsp[(2) - (3)].typedefs).typedefs[i].type]);
    if ( st_typedef_register((yyvsp[(2) - (3)].typedefs).typedefs+i, scope) != Success ) {
			yyerror("Could not define typedef");
			return 1;
		}
  }

  printf("Declarations::vardefs %d\n", (yyvsp[(3) - (3)].variabledefs).size);
  for ( i=0; i<(yyvsp[(3) - (3)].variabledefs).size; i++ ) {
    printf("type: %d %s\n", (yyvsp[(3) - (3)].variabledefs).types[i].dataType, ((yyvsp[(3) - (3)].variabledefs).types[i].dataType==VT_User ? (yyvsp[(3) - (3)].variabledefs).types[i].userType : "standard_type" ));
    for (j=0; j<(yyvsp[(3) - (3)].variabledefs).ids[i].size; j++) {
      printf("\t%s\n", (yyvsp[(3) - (3)].variabledefs).ids[i].ids[j]);
      if ( st_var_define((yyvsp[(3) - (3)].variabledefs).ids[i].ids[j], (yyvsp[(3) - (3)].variabledefs).types[i], scope) == NULL ) {
				printf("Could not define var");
				return 1;
			}
    }
  }


}
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 195 "parser.y"
    {
  (yyval.constdefs) = (yyvsp[(2) - (3)].constdefs);
}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 199 "parser.y"
    {
  (yyval.constdefs).size = 0;
}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 204 "parser.y"
    {
  constant_t temp;
  if ( expression_evaluate((yyvsp[(5) - (5)].expr), &temp, scope) == Failure ) {
    yyerror("constant_defs invalid expression\n");
  } else {
    (yyval.constdefs) = (yyvsp[(1) - (5)].constdefs);
    (yyval.constdefs).constants = ( constant_t* ) realloc((yyval.constdefs).constants, ((yyval.constdefs).size+1) * sizeof(constant_t));
    (yyval.constdefs).ids = ( char** ) realloc((yyval.constdefs).ids, ((yyval.constdefs).size+1) * sizeof(char*));
    (yyval.constdefs).ids[ (yyval.constdefs).size ] = (yyvsp[(3) - (5)].id);
    (yyval.constdefs).constants[ (yyval.constdefs).size ++ ] = temp;
  }
}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 217 "parser.y"
    {
  constant_t temp;
  if ( expression_evaluate((yyvsp[(3) - (3)].expr), &temp, scope) == Failure ) {
    yyerror("constant_defs invalid expression\n");
  } else {
    (yyval.constdefs).size = 1;
    (yyval.constdefs).ids = ( char** ) calloc(1, sizeof(char*));
    (yyval.constdefs).ids[0] = (yyvsp[(1) - (3)].id);
    (yyval.constdefs).constants = ( constant_t* ) calloc(1, sizeof(constant_t));
    (yyval.constdefs).constants[0] = temp;
  }
}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 232 "parser.y"
    {
	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate((yyvsp[(1) - (3)].expr), &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = (yyvsp[(1) - (3)].expr);
		if ( expression_evaluate((yyvsp[(3) - (3)].expr), &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = (yyvsp[(3) - (3)].expr);
	} else {
		l = (yyvsp[(1) - (3)].expr);
		r = (yyvsp[(3) - (3)].expr);
	}
	
  (yyval.expr) = expression_binary(l, r, (yyvsp[(2) - (3)].op));

	if ( enable_constant_propagation )
		if ( expression_evaluate((yyval.expr), &temp, scope) == Success ) {
			(yyval.expr) = expression_constant(temp.type, &(temp.bconst));
		}
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 259 "parser.y"
    {
	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate((yyvsp[(1) - (3)].expr), &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = (yyvsp[(1) - (3)].expr);
		if ( expression_evaluate((yyvsp[(3) - (3)].expr), &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = (yyvsp[(3) - (3)].expr);
	} else {
		l = (yyvsp[(1) - (3)].expr);
		r = (yyvsp[(3) - (3)].expr);
	}

  (yyval.expr) = expression_binary(l, r, (yyvsp[(2) - (3)].op));

	if ( enable_constant_propagation )
		if ( expression_evaluate((yyval.expr), &temp, scope) == Success ) {
			(yyval.expr) = expression_constant(temp.type, &(temp.bconst));
		}
}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 287 "parser.y"
    {
	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate((yyvsp[(1) - (3)].expr), &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = (yyvsp[(1) - (3)].expr);
		if ( expression_evaluate((yyvsp[(3) - (3)].expr), &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = (yyvsp[(3) - (3)].expr);
	} else {
		l = (yyvsp[(1) - (3)].expr);
		r = (yyvsp[(3) - (3)].expr);
	}

  (yyval.expr) = expression_binary(l, r, (yyvsp[(2) - (3)].op));

	if ( enable_constant_propagation )
		if ( expression_evaluate((yyval.expr), &temp, scope) == Success ) {
			(yyval.expr) = expression_constant(temp.type, &(temp.bconst));
		}
}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 315 "parser.y"
    {
	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate((yyvsp[(1) - (3)].expr), &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = (yyvsp[(1) - (3)].expr);
		if ( expression_evaluate((yyvsp[(3) - (3)].expr), &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = (yyvsp[(3) - (3)].expr);
	} else {
		l = (yyvsp[(1) - (3)].expr);
		r = (yyvsp[(3) - (3)].expr);
	}

  (yyval.expr) = expression_binary(l, r, (yyvsp[(2) - (3)].op));

	if ( enable_constant_propagation )
		if ( expression_evaluate((yyval.expr), &temp, scope) == Success ) {
			(yyval.expr) = expression_constant(temp.type, &(temp.bconst));
		}
}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 343 "parser.y"
    {
 	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate((yyvsp[(1) - (3)].expr), &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = (yyvsp[(1) - (3)].expr);
		if ( expression_evaluate((yyvsp[(3) - (3)].expr), &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = (yyvsp[(3) - (3)].expr);
	} else {
		l = (yyvsp[(1) - (3)].expr);
		r = (yyvsp[(3) - (3)].expr);
	}

  (yyval.expr) = expression_binary(l, r, (yyvsp[(2) - (3)].op));

	if ( enable_constant_propagation )
		if ( expression_evaluate((yyval.expr), &temp, scope) == Success ) {
			(yyval.expr) = expression_constant(temp.type, &(temp.bconst));
		}
}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 371 "parser.y"
    {
 	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate((yyvsp[(1) - (3)].expr), &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = (yyvsp[(1) - (3)].expr);
		if ( expression_evaluate((yyvsp[(3) - (3)].expr), &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = (yyvsp[(3) - (3)].expr);
	} else {
		l = (yyvsp[(1) - (3)].expr);
		r = (yyvsp[(3) - (3)].expr);
	}

  (yyval.expr) = expression_binary(l, r, (yyvsp[(2) - (3)].op));

	if ( enable_constant_propagation )
		if ( expression_evaluate((yyval.expr), &temp, scope) == Success ) {
			(yyval.expr) = expression_constant(temp.type, &(temp.bconst));
		}
}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 399 "parser.y"
    {
	constant_t temp;
	expression_t *e;
	
	e = (yyvsp[(2) - (2)].expr);

	if ( enable_constant_propagation ) {
		if ( expression_evaluate((yyvsp[(2) - (2)].expr), &temp, scope) == Success ) 
			e = expression_constant(temp.type, &(temp.bconst));
	}

  if ( (yyvsp[(1) - (2)].op) == AddopP ) 
    (yyval.expr) = e;
  else {
    //TODO negative e ( quick hack )
    expression_t *neg = calloc(1, sizeof(expression_t));
    neg->type = ET_Constant;
    neg->constant.type = VT_Iconst;
    neg->constant.iconst = -1;
    neg->dataType = VT_Integer;
    //neg->next = NULL;

    (yyval.expr) = expression_binary(neg, e, MuldivandopM); 
    //$$ = NULL;
  }
}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 426 "parser.y"
    {
	constant_t temp;
	expression_t *e;
	
	e = (yyvsp[(2) - (2)].expr);

	if ( enable_constant_propagation ) {
		if ( expression_evaluate((yyvsp[(2) - (2)].expr), &temp, scope) == Success ) 
			e = expression_constant(temp.type, &(temp.bconst));
	}

	(yyval.expr)=  expression_not(e);

	if ( enable_constant_propagation )
		if ( expression_evaluate((yyval.expr), &temp, scope) == Success ) {
			(yyval.expr) = expression_constant(temp.type, &(temp.bconst));
		}
}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 445 "parser.y"
    {
  (yyval.expr) = expression_variable((yyvsp[(1) - (1)].variable), scope);
}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 449 "parser.y"
    {
  (yyval.expr) = expression_call((yyvsp[(1) - (4)].id), (yyvsp[(3) - (4)].expressions).exprs, (yyvsp[(3) - (4)].expressions).size, scope);
}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 453 "parser.y"
    {
  printf("%d ", yylineno);
  yyerror("Oxi length lparen expression rparen\n");
}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 458 "parser.y"
    {
  printf("%d ", yylineno);
  yyerror("NEW LPAREN expression RPAREN\n");
}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 463 "parser.y"
    {
  (yyval.expr) = expression_constant((yyvsp[(1) - (1)].constant).type, &((yyvsp[(1) - (1)].constant).iconst));
}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 467 "parser.y"
    {
  (yyval.expr) = (yyvsp[(2) - (3)].expr);
}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 471 "parser.y"
    {
  expressions_t *exprs = ( expressions_t* ) calloc(1, sizeof(expressions_t));
  exprs->exprs = (yyvsp[(1) - (1)].expressions).exprs;
  exprs->size = (yyvsp[(1) - (1)].expressions).size;
  (yyval.expr) = expression_set(exprs);
}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 480 "parser.y"
    {
  (yyval.variable) = ( variable_t* ) calloc(1, sizeof(variable_t));
  (yyval.variable)->id= (yyvsp[(1) - (1)].id);
}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 485 "parser.y"
    {
  variable_t *p;
  p = (yyval.variable) = (yyvsp[(1) - (3)].variable);
  while ( p->child )
    p = p->child;
  p->child = ( variable_t * ) calloc(1, sizeof(variable_t));
  p = p->child;
  p->id = (yyvsp[(3) - (3)].id);
}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 495 "parser.y"
    {
  variable_t *p;
  p = (yyval.variable) = (yyvsp[(1) - (4)].variable);
  while ( p->child )
    p = p->child;
  p->expr = (yyvsp[(3) - (4)].expressions);
}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 503 "parser.y"
    {
  (yyval.variable) = NULL;
  printf("%d) Listfunc is not supported\n", yylineno);
	assert(0 && "ListFunc is not supported");
}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 511 "parser.y"
    {
  (yyval.expressions) = (yyvsp[(1) - (3)].expressions);
  (yyval.expressions).exprs = ( expression_t * ) realloc( (yyval.expressions).exprs, sizeof(expression_t) * ( (yyval.expressions).size+1));

	assert( (yyvsp[(3) - (3)].expr) && "Failed to parse expression in expressions(1)");

	(yyval.expressions).exprs[ (yyval.expressions).size++ ] = *(yyvsp[(3) - (3)].expr);

	free((yyvsp[(3) - (3)].expr));
}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 522 "parser.y"
    {
	assert((yyvsp[(1) - (1)].expr) && "Failed to parse expression in expressions(2)");
	(yyval.expressions).exprs = (yyvsp[(1) - (1)].expr);

  (yyval.expressions).size = 1;
}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 531 "parser.y"
    {
  (yyval.constant).type = VT_Iconst;
  (yyval.constant).iconst = (yyvsp[(1) - (1)].iconst);
}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 537 "parser.y"
    {
  (yyval.constant).type = VT_Rconst;
  (yyval.constant).rconst = (yyvsp[(1) - (1)].rconst);
}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 542 "parser.y"
    {
  (yyval.constant).type = VT_Bconst;
  (yyval.constant).bconst = (yyvsp[(1) - (1)].bconst);
}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 547 "parser.y"
    {
  (yyval.constant).type = VT_Cconst;
  (yyval.constant).cconst = (yyvsp[(1) - (1)].cconst);
}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 554 "parser.y"
    {
  (yyval.expressions) = (yyvsp[(2) - (3)].expressions);
}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 558 "parser.y"
    {
  (yyval.expressions).size = 0;
  (yyval.expressions).exprs = 0;
}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 565 "parser.y"
    {
  (yyval.typedefs) = (yyvsp[(2) - (3)].typedefs);
}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 569 "parser.y"
    {
  (yyval.typedefs).size = 0;
  (yyval.typedefs).typedefs = NULL;
}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 575 "parser.y"
    {
  (yyvsp[(1) - (5)].typedefs).size ++;
  (yyvsp[(1) - (5)].typedefs).typedefs = (typedefs_entry_t*) realloc( (yyvsp[(1) - (5)].typedefs).typedefs, (yyvsp[(1) - (5)].typedefs).size * sizeof(typedefs_entry_t));
  (yyvsp[(1) - (5)].typedefs).typedefs[ (yyvsp[(1) - (5)].typedefs).size - 1 ] = (yyvsp[(5) - (5)].type_def);
  (yyvsp[(1) - (5)].typedefs).typedefs[ (yyvsp[(1) - (5)].typedefs).size - 1 ].name = (yyvsp[(3) - (5)].id);

  (yyval.typedefs) = (yyvsp[(1) - (5)].typedefs);
}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 584 "parser.y"
    {
  (yyval.typedefs).size = 1;
  (yyval.typedefs).typedefs = (typedefs_entry_t*) calloc(1, sizeof(typedefs_entry_t));
  (yyval.typedefs).typedefs[0] = (yyvsp[(3) - (3)].type_def);
  (yyval.typedefs).typedefs[0].name = (yyvsp[(1) - (3)].id); // this has to be done AFTER $$.typedefs[0] = $3;
}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 593 "parser.y"
    {
  (yyval.type_def).type = TT_Array;
  (yyval.type_def).array.dims = (yyvsp[(3) - (6)].limits);
  (yyval.type_def).array.typename = (yyvsp[(6) - (6)].dataType);
}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 599 "parser.y"
    {
  (yyval.type_def).type = TT_List;
  (yyval.type_def).list.typename = (yyvsp[(3) - (3)].dataType);
}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 604 "parser.y"
    {
  (yyval.type_def).type = TT_Set;
  (yyval.type_def).set.typename = (yyvsp[(3) - (3)].dataType);
}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 609 "parser.y"
    {
  (yyval.type_def).type = TT_Record;
  (yyval.type_def).record = (yyvsp[(2) - (3)].fields);
}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 614 "parser.y"
    {
  (yyval.type_def).type = TT_Range;
  (yyval.type_def).range.from = (yyvsp[(1) - (3)].limit);
  (yyval.type_def).range.to = (yyvsp[(3) - (3)].limit);
}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 622 "parser.y"
    {
  (yyvsp[(1) - (3)].limits).size++;
  (yyvsp[(1) - (3)].limits).limits = (limits_entry_t*) realloc( (yyvsp[(1) - (3)].limits).limits, (yyvsp[(1) - (3)].limits).size * sizeof(limits_entry_t));
  (yyvsp[(1) - (3)].limits).limits[ (yyvsp[(1) - (3)].limits).size-1 ] = (yyvsp[(3) - (3)].limits).limits[0];
  (yyval.limits) = (yyvsp[(1) - (3)].limits);
}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 629 "parser.y"
    {
  (yyval.limits) = (yyvsp[(1) - (1)].limits);
}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 635 "parser.y"
    {
  (yyval.limits).limits = (limits_entry_t*) calloc(1, sizeof(limits_entry_t));
  (yyval.limits).limits[0].isRange = 1;
  (yyval.limits).limits[0].range.from = (yyvsp[(1) - (3)].limit);
  (yyval.limits).limits[0].range.to = (yyvsp[(3) - (3)].limit);

  (yyval.limits).size = 1;
}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 644 "parser.y"
    {
  (yyval.limits).limits = (limits_entry_t*) calloc(1, sizeof(limits_entry_t));
  (yyval.limits).limits[0].isRange = 0;
  (yyval.limits).limits[0].limit.type = LT_Id;
  (yyval.limits).limits[0].limit.id.sign = Positive;
  (yyval.limits).limits[0].limit.id.id = (yyvsp[(1) - (1)].id);

  (yyval.limits).size = 1;
}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 656 "parser.y"
    {
  (yyval.limit).type =LT_Iconst;

  if ( (yyvsp[(1) - (2)].sign) == Negative )
    (yyval.limit).iconst = - (yyvsp[(2) - (2)].iconst);
  else
    (yyval.limit).iconst = (yyvsp[(2) - (2)].iconst);
}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 665 "parser.y"
    {
  (yyval.limit).type = LT_Cconst;
  (yyval.limit).cconst = (yyvsp[(1) - (1)].cconst);
}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 670 "parser.y"
    {
  (yyval.limit).type = LT_Bconst;
  (yyval.limit).bconst = (yyvsp[(1) - (1)].bconst);
}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 675 "parser.y"
    {
  (yyval.limit).type = LT_Id;
  (yyval.limit).id.id = (yyvsp[(2) - (2)].id);
  (yyval.limit).id.sign = ( (yyvsp[(1) - (2)].op)==AddopP ? Positive : Negative );
}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 682 "parser.y"
    {
  (yyval.limit).type = LT_Id;
  (yyval.limit).id.id = (yyvsp[(1) - (1)].id);
  (yyval.limit).id.sign = Positive;
}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 690 "parser.y"
    {
  if ( (yyvsp[(1) - (1)].op) == AddopP )
    (yyval.sign) = Positive;
  else
    (yyval.sign) = Negative;
}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 697 "parser.y"
    {
  (yyval.sign) = Positive;
}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 702 "parser.y"
    {
  (yyval.dataType) = (yyvsp[(1) - (1)].dataType);
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 706 "parser.y"
    {
  (yyval.dataType).dataType = VT_User;
  (yyval.dataType).userType = (yyvsp[(1) - (1)].id);
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 713 "parser.y"
    {
  (yyval.dataType).dataType = VT_Integer;
}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 717 "parser.y"
    {
  (yyval.dataType).dataType = VT_Real;
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 721 "parser.y"
    {
  (yyval.dataType).dataType = VT_Boolean;
}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 725 "parser.y"
    {
  (yyval.dataType).dataType = VT_Char;
}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 731 "parser.y"
    {
	int i,j ;
  (yyvsp[(1) - (3)].fields).ids = (char**) realloc( (yyvsp[(1) - (3)].fields).ids, ((yyvsp[(1) - (3)].fields).size+(yyvsp[(3) - (3)].fields).size) * sizeof(char*));
  (yyvsp[(1) - (3)].fields).types = (data_type_t*) realloc( (yyvsp[(1) - (3)].fields).types, ((yyvsp[(1) - (3)].fields).size+(yyvsp[(3) - (3)].fields).size) * sizeof(data_type_t));
	for ( j=0, i=(yyvsp[(1) - (3)].fields).size; i < (yyvsp[(1) - (3)].fields).size + (yyvsp[(3) - (3)].fields).size; i++, j++ ) {
	  (yyvsp[(1) - (3)].fields).ids[ i ] = (yyvsp[(3) - (3)].fields).ids[j];
  	(yyvsp[(1) - (3)].fields).types[ i ] = (yyvsp[(3) - (3)].fields).types[j];
	}
	(yyvsp[(1) - (3)].fields).size += (yyvsp[(3) - (3)].fields).size;
  (yyval.fields) = (yyvsp[(1) - (3)].fields);
}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 743 "parser.y"
    {
  (yyval.fields) = (yyvsp[(1) - (1)].fields);
}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 749 "parser.y"
    {
	int i;

  (yyval.fields).size = (yyvsp[(1) - (3)].identifiers).size;
  (yyval.fields).ids = (char **) calloc(1, (yyvsp[(1) - (3)].identifiers).size * sizeof(char*));
  (yyval.fields).types = (data_type_t*) calloc(1, (yyvsp[(1) - (3)].identifiers).size * sizeof(data_type_t));
	
	for ( i = 0; i < (yyvsp[(1) - (3)].identifiers).size; i ++ ) {
		(yyval.fields).ids[ i ] = (yyvsp[(1) - (3)].identifiers).ids[i];
		(yyval.fields).types[ i ] = (yyvsp[(3) - (3)].dataType);
	}
}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 764 "parser.y"
    {
  (yyvsp[(1) - (3)].identifiers).size++;
  (yyvsp[(1) - (3)].identifiers).ids = (char**) realloc((yyvsp[(1) - (3)].identifiers).ids, sizeof(char*) * (yyvsp[(1) - (3)].identifiers).size);
  (yyvsp[(1) - (3)].identifiers).ids[ (yyvsp[(1) - (3)].identifiers).size - 1 ] = (yyvsp[(3) - (3)].id);

  (yyval.identifiers) = (yyvsp[(1) - (3)].identifiers);
}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 772 "parser.y"
    {
  (yyval.identifiers).size = 1;
  (yyval.identifiers).ids = (char**) calloc(1, sizeof(char*));
  (yyval.identifiers).ids[0] = (yyvsp[(1) - (1)].id);
}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 780 "parser.y"
    {
  (yyval.variabledefs) = (yyvsp[(2) - (3)].variabledefs);
}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 784 "parser.y"
    {
  (yyval.variabledefs).size = 0;
  (yyval.variabledefs).ids = 0;
  (yyval.variabledefs).types = 0;
}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 791 "parser.y"
    {
  (yyvsp[(1) - (5)].variabledefs).size++;
  (yyvsp[(1) - (5)].variabledefs).ids = (identifiers_t*) realloc((yyvsp[(1) - (5)].variabledefs).ids, (yyvsp[(1) - (5)].variabledefs).size * sizeof(identifiers_t));
  (yyvsp[(1) - (5)].variabledefs).types = (data_type_t*) realloc((yyvsp[(1) - (5)].variabledefs).types, (yyvsp[(1) - (5)].variabledefs).size * sizeof(data_type_t));
  (yyvsp[(1) - (5)].variabledefs).ids[ (yyvsp[(1) - (5)].variabledefs).size - 1 ] = (yyvsp[(3) - (5)].identifiers);
  (yyvsp[(1) - (5)].variabledefs).types[ (yyvsp[(1) - (5)].variabledefs).size - 1 ] = (yyvsp[(5) - (5)].dataType);

  (yyval.variabledefs) = (yyvsp[(1) - (5)].variabledefs);
}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 801 "parser.y"
    {
  (yyval.variabledefs).size = 1;
  (yyval.variabledefs).ids = (identifiers_t*) calloc(1, sizeof(identifiers_t));
  (yyval.variabledefs).types = (data_type_t*) calloc(1, sizeof(data_type_t));
  (yyval.variabledefs).ids[0] = (yyvsp[(1) - (3)].identifiers);
  (yyval.variabledefs).types[0] = (yyvsp[(3) - (3)].dataType);
}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 815 "parser.y"
    {
	int i,j;
  var_t *var;
  var_t *params=0;
  int size = 0;
  func_t *func;
	


	if ( (yyvsp[(1) - (3)].sub_header).isForward == 0 ) {
		scope = st_init(scope);
		// This function is declared and specified here

		// first register the function ID as a local variable
		st_var_define((yyvsp[(1) - (3)].sub_header).id, (yyvsp[(1) - (3)].sub_header).type, scope);

		// then register all parameters as local variables so that they are visible
		for ( i = 0; i < (yyvsp[(1) - (3)].sub_header).size; i ++ ) {
			for ( j = 0; j < (yyvsp[(1) - (3)].sub_header).params[i].ids.size; j++ ) {
				var = st_var_define((yyvsp[(1) - (3)].sub_header).params[i].ids.ids[j], (yyvsp[(1) - (3)].sub_header).params[i].type, scope);
				if ( var ) {
					var->pass = (yyvsp[(1) - (3)].sub_header).params[i].pass;
					size++;

					params = ( var_t * ) realloc(params, size * sizeof(var_t));
					params[ size-1 ] = *var;
				} else {
					
				}
			}
		}

		// register the function to the global scope
		func = st_func_define((yyvsp[(1) - (3)].sub_header).id, (yyvsp[(1) - (3)].sub_header).type, params, size, scope->parent);
		func->isProcedure = (yyvsp[(1) - (3)].sub_header).isProcedure;
		func->scope = scope; // store the scope
		scope = scope->parent;
	}
	
}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 856 "parser.y"
    {
	if ( (yyvsp[(1) - (2)].sub_header).isForward ) {
		func_t *func = st_func_find((yyvsp[(1) - (2)].sub_header).id, scope);
		if ( func == NULL ) {
			yyerror("[-] Function %s is not forward-declared.", (yyvsp[(1) - (2)].sub_header).id);
			return 1;
		}
		scope = func->scope;
	} else
		scope = st_init(scope);
}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 867 "parser.y"
    {
  int i,j;
  var_t *var;
  var_t *params=0;
  int size = 0;
  func_t *func;

	if ( (yyvsp[(1) - (4)].sub_header).isForward == 0 ) {
		// This function is declared and specified here

		// first register the function ID as a local variable
		st_var_define((yyvsp[(1) - (4)].sub_header).id, (yyvsp[(1) - (4)].sub_header).type, scope);

		// then register all parameters as local variables so that they are visible
		for ( i = 0; i < (yyvsp[(1) - (4)].sub_header).size; i ++ ) {
			for ( j = 0; j < (yyvsp[(1) - (4)].sub_header).params[i].ids.size; j++ ) {
				var = st_var_define((yyvsp[(1) - (4)].sub_header).params[i].ids.ids[j], (yyvsp[(1) - (4)].sub_header).params[i].type, scope);
				if ( var ) {
					var->pass = (yyvsp[(1) - (4)].sub_header).params[i].pass;
					size++;

					params = ( var_t * ) realloc(params, size * sizeof(var_t));
					params[ size-1 ] = *var;
				} else {
					yyerror("[-] Param %s is already defined.", (yyvsp[(1) - (4)].sub_header).params[i].ids.ids[j]);
					return 1;
				}
			}
		}

		// register the function to the global scope
		func = st_func_define((yyvsp[(1) - (4)].sub_header).id, (yyvsp[(1) - (4)].sub_header).type, params, size, scope->parent);
		func->isProcedure = (yyvsp[(1) - (4)].sub_header).isProcedure;
	} else {
		// This function is already forward-declared
		func = st_func_find((yyvsp[(1) - (4)].sub_header).id, scope->parent);
		if ( func == NULL ) {
			yyerror("[-] Function %s is not forward-declared.");
			return 1;
		} else if ( func->body ) {
			yyerror("[-] Function %s already has a body.");
			return 1;
		}
	}
}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 913 "parser.y"
    {
	func_t *func = st_func_find((yyvsp[(1) - (7)].sub_header).id, scope->parent);
	func->body = (yyvsp[(7) - (7)].statement);
  scope = scope->parent; // TODO should the scope be destroyed here? I think not
}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 922 "parser.y"
    {
	(yyval.sub_header).isForward = 0;
  (yyval.sub_header).id = (yyvsp[(2) - (5)].id);
  (yyval.sub_header).isProcedure = 0;
  (yyval.sub_header).type = (yyvsp[(5) - (5)].dataType);
  (yyval.sub_header).params = (yyvsp[(3) - (5)].params).params;
  (yyval.sub_header).size = (yyvsp[(3) - (5)].params).size;
}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 931 "parser.y"
    {
	(yyval.sub_header).isForward = 0;
  (yyval.sub_header).id = (yyvsp[(2) - (5)].id);
  (yyval.sub_header).isProcedure = 0;
  (yyval.sub_header).type.dataType = 0;
  (yyval.sub_header).params = (yyvsp[(3) - (5)].params).params;
  (yyval.sub_header).size= (yyvsp[(3) - (5)].params).size;
}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 940 "parser.y"
    {
	(yyval.sub_header).isForward = 0;
  (yyval.sub_header).id = (yyvsp[(2) - (3)].id);
  (yyval.sub_header).isProcedure = 1;
  (yyval.sub_header).type.dataType = 0;
  (yyval.sub_header).params = (yyvsp[(3) - (3)].params).params;
  (yyval.sub_header).size= (yyvsp[(3) - (3)].params).size;
}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 949 "parser.y"
    {
	(yyval.sub_header).isForward = 1;
  (yyval.sub_header).id = (yyvsp[(2) - (2)].id);
  (yyval.sub_header).isProcedure = 0;
  (yyval.sub_header).type.dataType = 0;
  (yyval.sub_header).size= 0;
  (yyval.sub_header).params = NULL;
}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 960 "parser.y"
    {
  (yyval.params) = (yyvsp[(2) - (3)].params);
}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 964 "parser.y"
    {
  (yyval.params).size = 0;
}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 970 "parser.y"
    {
  (yyvsp[(1) - (6)].params).params = ( parameters_t* ) realloc((yyvsp[(1) - (6)].params).params, ((yyvsp[(1) - (6)].params).size+1) * sizeof(parameters_t));
  (yyvsp[(1) - (6)].params).params[ (yyvsp[(1) - (6)].params).size ].pass = (yyvsp[(3) - (6)].pass);
  (yyvsp[(1) - (6)].params).params[ (yyvsp[(1) - (6)].params).size ].ids  = (yyvsp[(4) - (6)].identifiers);
  (yyvsp[(1) - (6)].params).params[ (yyvsp[(1) - (6)].params).size++ ].type = (yyvsp[(6) - (6)].dataType);
  (yyval.params) = (yyvsp[(1) - (6)].params);
}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 978 "parser.y"
    {
  (yyval.params).size = 1;
  (yyval.params).params = ( parameters_t * ) calloc(1, sizeof(parameters_t));
  (yyval.params).params->pass = (yyvsp[(1) - (4)].pass);
  (yyval.params).params->ids  = (yyvsp[(2) - (4)].identifiers);
  (yyval.params).params->type = (yyvsp[(4) - (4)].dataType);
}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 988 "parser.y"
    { 
  (yyval.pass) = 1;
}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 992 "parser.y"
    {
  (yyval.pass) = 0;
}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 997 "parser.y"
    {
  (yyval.statement) = (yyvsp[(2) - (3)].statement);
}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 1003 "parser.y"
    {
  statement_t *p, *t;
  (yyval.statement) = (yyvsp[(1) - (3)].statement);

  for ( p = (yyval.statement); p && p->next; p = p->next );
  if ( p  && (yyvsp[(3) - (3)].statement) ) {
    p->next = (yyvsp[(3) - (3)].statement);
		p->join = (yyvsp[(3) - (3)].statement);
 
    if ( (yyvsp[(3) - (3)].statement) ) {
      (yyvsp[(3) - (3)].statement) -> prev = p;
		}
	
    switch ( p->type )
    {
      case ST_If:
        if ( ( t = p->_if._true ) != NULL ) {
          for ( ;  t->next; t = t->next );
          t->join = (yyvsp[(3) - (3)].statement);
        }
        
        if ( ( t = p->_if._false ) != NULL ) {
          for ( ; t->next; t = t->next );
          t->join = (yyvsp[(3) - (3)].statement);
        }
			p->join = (yyvsp[(3) - (3)].statement);
      break;

      case ST_While:
        if ( ( t = p->_while.loop ) != NULL ) {
          for ( ; t && t->next; t = t->next );
          
          t->join = (yyvsp[(3) - (3)].statement);
        }
      break;

      case ST_For:
        if ( ( t = p->_for.loop ) != NULL ) {
          for ( ; t && t->next; t = t->next );
          
          t->join = (yyvsp[(3) - (3)].statement);
        }
			break;

      case ST_With:
        if ( ( t = p->with.statement ) != NULL ) {
          for ( ; t && t->next; t = t->next );
          t->join = (yyvsp[(3) - (3)].statement);
        }
      break;

			default:
			break;
    }
	}
}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 1060 "parser.y"
    {
  (yyval.statement) = (yyvsp[(1) - (1)].statement);
}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 1080 "parser.y"
    {
  (yyval.statement) = NULL;
}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1087 "parser.y"
    {
  (yyval.statement) = statement_if((yyvsp[(2) - (6)].expr), (yyvsp[(4) - (6)].statement), (yyvsp[(6) - (6)].statement));

	if ( (yyval.statement) == NULL ) {
		yyerror("[-] Failed to generate IF statement.");
		return 1;
	}
}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1098 "parser.y"
    { 
  (yyval.statement) = statement_if((yyvsp[(2) - (4)].expr), (yyvsp[(4) - (4)].statement), NULL);

	if ( (yyval.statement) == NULL ) {
		yyerror("[-] Failed to generate IF statement.");
		return 1;
	}
}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 1107 "parser.y"
    {
  (yyval.statement) = statement_if((yyvsp[(2) - (6)].expr), (yyvsp[(4) - (6)].statement), (yyvsp[(6) - (6)].statement));

	if ( (yyval.statement) == NULL ) {
		yyerror("[-] Failed to generate IF statement.");
		return 1;
	}
}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 1118 "parser.y"
    {
	(yyval.statement) = statement_assignment((yyvsp[(1) - (3)].variable), (yyvsp[(3) - (3)].expr), scope);
	
	if ( (yyval.statement) == NULL ) {
		yyerror("[-] Failed to generate ASSIGNMENT statement.");
		return 1;
	}
}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 1127 "parser.y"
    {
  (yyval.statement) = statement_assignment_str((yyvsp[(1) - (3)].variable), (yyvsp[(3) - (3)].string), scope);

	if ( (yyval.statement) == NULL ) {
		yyerror("[-] Failed to generate ASSIGNMENT_STR statement");
		return 1;
	}
}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 1138 "parser.y"
    {
	(yyval.statement) = NULL;

	printf("[-] CASE statement is not supported at the moment.");

	return 0;
}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 1169 "parser.y"
    {
  (yyval.statement) = statement_while((yyvsp[(2) - (4)].expr), (yyvsp[(4) - (4)].statement));

	
	if ( (yyval.statement) == NULL ) {
		yyerror("[-] Failed to generate WHILE statement.");
		return 1;
	}

}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 1183 "parser.y"
    {
  (yyval.statement) = statement_for((yyvsp[(2) - (6)].id), &(yyvsp[(4) - (6)].iter_space), (yyvsp[(6) - (6)].statement), scope);

	if ( (yyval.statement) == NULL ) {
		yyerror("[-] Failed to generate FOR statement.");
		return 1;
	}
}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 1194 "parser.y"
    {
  (yyval.iter_space).from = (yyvsp[(1) - (3)].expr);
  (yyval.iter_space).to = (yyvsp[(3) - (3)].expr);
  (yyval.iter_space).type = FT_To;
}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 1200 "parser.y"
    {
  (yyval.iter_space).from = (yyvsp[(1) - (3)].expr);
  (yyval.iter_space).to = (yyvsp[(3) - (3)].expr);
  (yyval.iter_space).type = FT_DownTo;
}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 1209 "parser.y"
    {
  scope = st_init(scope);
  (yyval.statement) = statement_with((yyvsp[(2) - (3)].variable), NULL, scope);
}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 1214 "parser.y"
    {
  (yyval.statement) = (yyvsp[(4) - (5)].statement);
  scope = st_destroy(scope);
  (yyval.statement)->with.statement = (yyvsp[(5) - (5)].statement);
}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 1222 "parser.y"
    {
  scope_t *global;
  for ( global = scope; global->parent; global = global->parent);

  (yyval.statement) = statement_call((yyvsp[(1) - (1)].id),NULL, 0, global);

	if ( (yyval.statement) == NULL ) {
		yyerror("[-] Failed to generate CALL statement.");
		return 1;
	}
}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 1234 "parser.y"
    {
  scope_t *global;
  for ( global = scope; global->parent; global = global->parent);

  (yyval.statement) = statement_call((yyvsp[(1) - (4)].id), (yyvsp[(3) - (4)].expressions).exprs, (yyvsp[(3) - (4)].expressions).size, global);

	if ( (yyval.statement) == NULL ) {
		yyerror("[-] Failed to generate CALL statement.");
		return 1;
	}
}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 1248 "parser.y"
    {
	(yyval.statement) = NULL; 

	yyerror("[-] io_statement(READ) is not supported at the moment\n");
	return 1;
}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 1255 "parser.y"
    {
	(yyval.statement) = NULL;
	
	yyerror("[-] io_statement(WRITE) is not supported at the moment\n");
}
    break;



/* Line 1806 of yacc.c  */
#line 3272 "parser.tab.c"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1277 "parser.y"



void yyerror(const char *fmt, ...)
{
	const char *p;
	va_list argp;
	int i;
	char *s;

  printf("Error[%d]: ", yylineno);
	va_start(argp, fmt);

	for(p = fmt; *p != '\0'; p++)
		{
		if(*p != '%')
			{
			putchar(*p);
			continue;
			}

		switch(*++p)
			{
			case 'c':
				i = va_arg(argp, int);
				putchar(i);
				break;

			case 'd':
				i = va_arg(argp, int);
				printf("%d", i);
				break;

			case 's':
				s = va_arg(argp, char *);
				fputs(s, stdout);
				break;

			case 'x':
				i = va_arg(argp, int);
				printf("%x", i);
				break;

			case '%':
				putchar('%');
				break;
			}
	}

	va_end(argp);
	printf("\n");
}

void print_help(char *path)
{
	char *file;
	file = strrchr(path, '/');
	
	if ( file == NULL )
		file = path;
	else
		file ++;
	
	printf("Usage: %s [OPTIONS] SOURCE_FILE\n"
				 "Options\n"
				 "\t-c --constant_propagation  Enable constant propagation\n"
				 "\t-d --dead_code_elimination Enable dead code elimination\n"
				 "\n"
				 
				 , file);
}


int main(int argc, char* argv[])
{
  int ret;
	
	if ( argc == 1 ) 
	{
		print_help(argv[0]);
		return 0;		
	}

	while ( ( ret = getopt_long(argc, argv, s_opts, l_opts, NULL) ) != -1 ) {
		switch ( ret ) {
			case 'h':
				print_help(argv[0]);
				return 0;
			break;

			case 'c':
				enable_constant_propagation = 1;
			break;

			case 'd':
				enable_dead_code_elimination = 1;
			break;
		}
	}
	
	if ( optind >= argc ) {
		printf("[-] Missing File argument.\n");
		return 1;
	}

	yyin = fopen(argv[optind], "r");

	if ( yyin == NULL ) {
		printf("[-] Could not open \"%s\".\n", argv[optind]);
		return 1;
	}

  ret = yyparse();

  printf("[$] yyparse: %d\n", ret);
  return 0;
}


