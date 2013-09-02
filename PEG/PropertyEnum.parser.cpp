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
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "../../../../Property/PEG/PropertyEnum.y"

#include "PropertyEnumGeneratorCommon.h"
#include "PropertyEnumGenerator.h"
#include <stdio.h>

void yyerror(char const *);
#define YYLEX_PARAM &yylval, &yylloc
#define YYERROR_VERBOSE
#define YYPRINT(File, Type, Value) fprintf ((File), "%s", (Value).toLatin1().data())

using namespace Qtinuum;

extern void yy_push_state_cpp_code();
extern void yy_push_state_params_list();
extern void yy_push_state_initialization_list();



/* Line 268 of yacc.c  */
#line 90 "../../../../Property/PEG/PropertyEnum.parser.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
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
     PROPERTY_SET = 258,
     PROPERTY = 259,
     INCLUDE_H = 260,
     INCLUDE_CPP = 261,
     INCLUDE_NAME = 262,
     EXTERN = 263,
     ID = 264,
     CPP_BRACKET_OPEN = 265,
     CPP_BRACKET_CLOSE = 266,
     ASSIGN = 267,
     SEMICOLON = 268,
     COLON = 269,
     DBL_COLON = 270,
     COMMA = 271,
     DOT = 272,
     TILDE = 273,
     VALUE_TO_ASSIGN = 274,
     ACCESS_ID = 275,
     SLOT = 276,
     DELEGATE = 277,
     CODE_H = 278,
     CODE_CPP = 279,
     CPP_CODE = 280,
     ENUM = 281,
     NUMBER = 282,
     STR = 283,
     PARAMS_LIST = 284,
     INITIALIZATION_LIST = 285
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 175 "../../../../Property/PEG/PropertyEnum.parser.cpp"

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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   118

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  89
/* YYNRULES -- Number of states.  */
#define YYNSTATES  143

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      31,    32,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    13,    16,    19,    22,
      25,    26,    31,    32,    37,    40,    41,    45,    47,    48,
      53,    55,    59,    61,    64,    66,    70,    72,    76,    77,
      79,    84,    85,    88,    91,    94,    97,   100,   103,   106,
     109,   111,   113,   115,   119,   122,   127,   129,   131,   134,
     137,   139,   141,   145,   146,   149,   152,   155,   159,   161,
     165,   166,   167,   174,   175,   179,   182,   184,   186,   190,
     191,   194,   198,   199,   200,   210,   211,   212,   216,   217,
     225,   226,   234,   235,   237,   239,   243,   244,   253,   254
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      34,     0,    -1,    -1,    34,    35,    -1,    34,    36,    -1,
      34,    37,    -1,    34,    40,    -1,    34,    80,    -1,     5,
       7,    -1,     6,     7,    -1,    -1,    23,    10,    38,    25,
      -1,    -1,    24,    10,    39,    25,    -1,    41,    50,    -1,
      -1,     3,    42,    43,    -1,     9,    -1,    -1,     9,    14,
      44,    45,    -1,    46,    -1,    45,    16,    46,    -1,    47,
      -1,    20,    47,    -1,     9,    -1,    48,    15,     9,    -1,
       9,    -1,    48,    15,     9,    -1,    -1,    13,    -1,    10,
      51,    11,    49,    -1,    -1,    51,    57,    -1,    51,    62,
      -1,    51,    64,    -1,    51,    52,    -1,    51,    37,    -1,
      51,    67,    -1,    51,    73,    -1,    51,    78,    -1,    40,
      -1,    53,    -1,    54,    -1,    41,     9,    50,    -1,    55,
      56,    -1,     8,     3,    47,     9,    -1,    60,    -1,    13,
      -1,    58,    59,    -1,    47,     9,    -1,    60,    -1,    13,
      -1,    10,    61,    11,    -1,    -1,    61,    62,    -1,    61,
      64,    -1,    61,    67,    -1,    63,    12,    19,    -1,     9,
      -1,    63,    17,     9,    -1,    -1,    -1,    21,    63,    65,
      10,    66,    25,    -1,    -1,    22,    68,    69,    -1,     9,
      70,    -1,    70,    -1,    13,    -1,    10,    71,    11,    -1,
      -1,    71,    72,    -1,     9,    12,    19,    -1,    -1,    -1,
       9,    31,    74,    29,    32,    76,    10,    75,    25,    -1,
      -1,    -1,    14,    77,    30,    -1,    -1,    18,     9,    31,
      32,    10,    79,    25,    -1,    -1,    26,     9,    81,    10,
      82,    11,    49,    -1,    -1,    83,    -1,    84,    -1,    83,
      16,    84,    -1,    -1,     9,    31,    27,    16,    28,    32,
      85,    86,    -1,    -1,    86,     9,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    52,    52,    53,    54,    55,    56,    57,    60,    65,
      70,    70,    78,    78,    88,    91,    91,    95,    96,    96,
     100,   101,   104,   107,   112,   114,   118,   119,   122,   123,
     126,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     141,   142,   143,   146,   154,   157,   165,   170,   177,   180,
     187,   190,   195,   199,   200,   201,   202,   205,   213,   217,
     223,   230,   223,   239,   239,   251,   254,   257,   258,   261,
     262,   265,   269,   271,   269,   276,   277,   277,   283,   283,
     289,   289,   299,   300,   303,   304,   307,   307,   324,   325
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROPERTY_SET", "PROPERTY", "INCLUDE_H",
  "INCLUDE_CPP", "INCLUDE_NAME", "EXTERN", "ID", "CPP_BRACKET_OPEN",
  "CPP_BRACKET_CLOSE", "ASSIGN", "SEMICOLON", "COLON", "DBL_COLON",
  "COMMA", "DOT", "TILDE", "VALUE_TO_ASSIGN", "ACCESS_ID", "SLOT",
  "DELEGATE", "CODE_H", "CODE_CPP", "CPP_CODE", "ENUM", "NUMBER", "STR",
  "PARAMS_LIST", "INITIALIZATION_LIST", "'('", "')'", "$accept",
  "pef_body", "include_h", "include_cpp", "source_code", "$@1", "$@2",
  "property_set", "property_set_head", "$@3", "property_set_decl", "$@4",
  "property_set_inheritance_list", "inheritance_decl", "type_decl",
  "namespace_name", "optional_semicolon", "property_set_body",
  "property_set_declarations", "sub_property_set", "property_set_with_id",
  "property_set_extern", "property_set_extern_head",
  "property_set_extern_tail", "property_declaration",
  "property_declaration_head", "property_declaration_tail",
  "sub_item_body", "sub_item_declarations", "value_declaration",
  "name_to_assign", "slot_declaration", "$@5", "$@6",
  "delegate_declaration", "$@7", "delegate_tail", "delegate_body",
  "delegate_attributes", "delegate_attribute", "constructor", "$@8", "$@9",
  "initialization_list", "$@10", "destructor", "$@11", "enum", "$@12",
  "enum_values_list", "enum_values_list2", "enum_value", "$@13",
  "enum_states_list", 0
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
     285,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    34,    34,    34,    34,    34,    35,    36,
      38,    37,    39,    37,    40,    42,    41,    43,    44,    43,
      45,    45,    46,    46,    47,    47,    48,    48,    49,    49,
      50,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      52,    52,    52,    53,    54,    55,    56,    56,    57,    58,
      59,    59,    60,    61,    61,    61,    61,    62,    63,    63,
      65,    66,    64,    68,    67,    69,    69,    70,    70,    71,
      71,    72,    74,    75,    73,    76,    77,    76,    79,    78,
      81,    80,    82,    82,    83,    83,    85,    84,    86,    86
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       0,     4,     0,     4,     2,     0,     3,     1,     0,     4,
       1,     3,     1,     2,     1,     3,     1,     3,     0,     1,
       4,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     1,     1,     3,     2,     4,     1,     1,     2,     2,
       1,     1,     3,     0,     2,     2,     2,     3,     1,     3,
       0,     0,     6,     0,     3,     2,     1,     1,     3,     0,
       2,     3,     0,     0,     9,     0,     0,     3,     0,     7,
       0,     7,     0,     1,     1,     3,     0,     8,     0,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,    15,     0,     0,     0,     0,     0,     3,
       4,     5,     6,     0,     7,     0,     8,     9,    10,    12,
      80,    31,    14,    17,    16,     0,     0,     0,     0,    18,
      11,    13,    82,     0,    58,    28,     0,     0,    63,    36,
      40,     0,     0,     0,    35,    41,    42,     0,    32,     0,
      33,     0,    34,    37,    38,    39,     0,     0,     0,    83,
      84,     0,    72,    29,    30,     0,    58,    60,     0,     0,
      49,     0,    53,    47,    44,    46,    51,    48,    50,     0,
       0,    24,     0,    19,    20,    22,     0,    28,     0,     0,
       0,     0,     0,     0,    69,    67,    64,    66,    43,    25,
       0,    57,    59,    23,     0,     0,    81,    85,    45,     0,
       0,    61,    65,     0,    52,    54,    55,    56,    21,     0,
      75,    78,     0,     0,    68,    70,     0,    76,     0,     0,
      62,     0,    86,     0,    73,    79,    71,    88,    77,     0,
      87,    74,    89
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     9,    10,    11,    25,    26,    12,    13,    15,
      24,    56,    83,    84,    85,    43,    64,    22,    28,    44,
      45,    46,    47,    74,    48,    49,    77,    75,   100,    50,
      51,    52,    92,   122,    53,    68,    96,    97,   113,   125,
      54,    90,   139,   128,   133,    55,   129,    14,    27,    58,
      59,    60,   137,   140
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -33
static const yytype_int8 yypact[] =
{
     -33,     1,   -33,   -33,     2,     3,    16,    30,    37,   -33,
     -33,   -33,   -33,    38,   -33,    40,   -33,   -33,   -33,   -33,
     -33,   -33,   -33,    -3,   -33,    25,    27,    41,    20,   -33,
     -33,   -33,    44,    52,    -1,    43,    48,    49,   -33,   -33,
     -33,    11,    50,    45,   -33,   -33,   -33,    22,   -33,    24,
     -33,     0,   -33,   -33,   -33,   -33,    -7,    31,    53,    47,
     -33,    56,   -33,   -33,   -33,    35,   -33,    51,     9,    38,
     -33,    58,   -33,   -33,   -33,   -33,   -33,   -33,   -33,    42,
      60,    55,    56,    57,   -33,   -33,    54,    43,    44,    62,
      46,    59,    64,    26,   -33,   -33,   -33,   -33,   -33,    61,
      -6,   -33,   -33,   -33,    -7,    63,   -33,   -33,   -33,    65,
      67,   -33,   -33,    36,   -33,   -33,   -33,   -33,   -33,    66,
      68,   -33,    70,    71,   -33,   -33,    69,   -33,    74,    73,
     -33,    77,   -33,    72,   -33,   -33,   -33,   -33,   -33,    75,
      76,   -33,   -33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -33,   -33,   -33,   -33,    78,   -33,   -33,    79,    80,   -33,
     -33,   -33,   -33,   -32,   -28,   -33,    -9,    17,   -33,   -33,
     -33,   -33,   -33,   -33,   -33,   -33,   -33,    39,   -33,   -20,
      81,   -13,   -33,   -33,   -11,   -33,   -33,     6,   -33,   -33,
     -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,
     -33,     4,   -33,   -33
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -28
static const yytype_int16 yytable[] =
{
      42,     2,    81,    66,     3,   114,     4,     5,   -24,    16,
      17,    29,    79,    82,   -26,    37,    38,    80,    93,    94,
      69,    21,    95,     3,     6,     7,    18,     8,    33,    34,
      62,    35,    72,    89,    72,    73,    94,    76,    36,    95,
      19,    37,    38,     6,     7,   123,    20,   124,    21,    23,
      30,    32,    31,    57,   103,    61,    63,    65,    66,    70,
      71,   101,    86,    88,    87,    81,    91,    99,    80,   102,
     -26,   108,   118,   104,   111,   109,   -27,   121,   106,   119,
     115,   105,   127,   131,   134,   142,    98,   116,    78,   117,
       0,   110,   107,     0,   126,   130,   136,   120,   135,   112,
     141,   132,   138,     0,     0,     0,    39,    40,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-33))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      28,     0,     9,     9,     3,    11,     5,     6,     9,     7,
       7,    14,    12,    20,    15,    21,    22,    17,     9,    10,
       9,    10,    13,     3,    23,    24,    10,    26,     8,     9,
      31,    11,    10,    61,    10,    13,    10,    13,    18,    13,
      10,    21,    22,    23,    24,     9,     9,    11,    10,     9,
      25,    10,    25,     9,    82,     3,    13,     9,     9,     9,
      15,    19,    31,    16,    11,     9,    31,     9,    17,     9,
      15,     9,   104,    16,    10,    29,    15,    10,    87,    16,
     100,    27,    14,    12,    10,     9,    69,   100,    49,   100,
      -1,    32,    88,    -1,    28,    25,    19,    32,    25,    93,
      25,    32,    30,    -1,    -1,    -1,    28,    28,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    34,     0,     3,     5,     6,    23,    24,    26,    35,
      36,    37,    40,    41,    80,    42,     7,     7,    10,    10,
       9,    10,    50,     9,    43,    38,    39,    81,    51,    14,
      25,    25,    10,     8,     9,    11,    18,    21,    22,    37,
      40,    41,    47,    48,    52,    53,    54,    55,    57,    58,
      62,    63,    64,    67,    73,    78,    44,     9,    82,    83,
      84,     3,    31,    13,    49,     9,     9,    63,    68,     9,
       9,    15,    10,    13,    56,    60,    13,    59,    60,    12,
      17,     9,    20,    45,    46,    47,    31,    11,    16,    47,
      74,    31,    65,     9,    10,    13,    69,    70,    50,     9,
      61,    19,     9,    47,    16,    27,    49,    84,     9,    29,
      32,    10,    70,    71,    11,    62,    64,    67,    46,    16,
      32,    10,    66,     9,    11,    72,    28,    14,    76,    79,
      25,    12,    32,    77,    10,    25,    19,    85,    30,    75,
      86,    25,     9
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
		  Type, Value, Location); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
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
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

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

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

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
       `yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
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
  yylsp = yyls;

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 8:

/* Line 1806 of yacc.c  */
#line 60 "../../../../Property/PEG/PropertyEnum.y"
    {
    peg.code.append(QSharedPointer<Code>(new IncludeCode((yyvsp[(2) - (2)]), true)));
}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 65 "../../../../Property/PEG/PropertyEnum.y"
    {
    peg.code.append(QSharedPointer<Code>(new IncludeCode((yyvsp[(2) - (2)]), false)));
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 70 "../../../../Property/PEG/PropertyEnum.y"
    { yy_push_state_cpp_code(); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 71 "../../../../Property/PEG/PropertyEnum.y"
    {
    QSharedPointer<SourceCode> code(new SourceCode((yyvsp[(4) - (4)]), true));
    if (pegContext.currPropertySet)
        pegContext.currPropertySet->sourceCodes.append(code);
    else
        peg.code.append(code);
}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 78 "../../../../Property/PEG/PropertyEnum.y"
    { yy_push_state_cpp_code(); }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 79 "../../../../Property/PEG/PropertyEnum.y"
    {
    QSharedPointer<SourceCode> code(new SourceCode((yyvsp[(4) - (4)]), false));
    if (pegContext.currPropertySet)
        pegContext.currPropertySet->sourceCodes.append(code);
    else
        peg.code.append(code);
}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 88 "../../../../Property/PEG/PropertyEnum.y"
    { pegContext.commitPropertySet(); }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 91 "../../../../Property/PEG/PropertyEnum.y"
    { pegContext.startPropertySet(); }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 95 "../../../../Property/PEG/PropertyEnum.y"
    { pegContext.currPropertySet->setName((yyvsp[(1) - (1)])); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 96 "../../../../Property/PEG/PropertyEnum.y"
    { pegContext.currPropertySet->setName((yyvsp[(1) - (2)])); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 104 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.currPropertySet->parentTypes.append(QPair<QString, QString>("", pegContext.currentType()));
}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 107 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.currPropertySet->parentTypes.append(QPair<QString, QString>((yyvsp[(1) - (2)]), pegContext.currentType()));
}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 112 "../../../../Property/PEG/PropertyEnum.y"
    { pegContext.currType.namespaceName = "";
                                                  pegContext.currType.name = (yyvsp[(1) - (1)]); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 114 "../../../../Property/PEG/PropertyEnum.y"
    { pegContext.currType.namespaceName = (yyvsp[(1) - (3)]);
                                                  pegContext.currType.name = (yyvsp[(3) - (3)]); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 126 "../../../../Property/PEG/PropertyEnum.y"
    {
}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 146 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.currPropertySet->name = (yyvsp[(2) - (3)]);
    pegContext.commitPropertySet();
    // add sub property set as member
    pegContext.currPropertySet->members.push_back(pegContext.currPropertySet->subPropertySets.last());
}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 157 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.startPropertySet();
    pegContext.currPropertySet->_extern = true;
    pegContext.currPropertySet->selfType = pegContext.currentPropertySetType();
    pegContext.currPropertySet->name = (yyvsp[(4) - (4)]);
}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 165 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.commitPropertySet();
    // add sub property set as member
    pegContext.currPropertySet->members.push_back(pegContext.currPropertySet->subPropertySets.last());
}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 170 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.commitPropertySet();
    // add sub property set as member
    pegContext.currPropertySet->members.push_back(pegContext.currPropertySet->subPropertySets.last());
}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 180 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.startProperty();
    pegContext.currProperty->selfType = pegContext.currentPropertyType();
    pegContext.currProperty->name = (yyvsp[(2) - (2)]);
}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 187 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.commitProperty();
}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 190 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.commitProperty();
}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 195 "../../../../Property/PEG/PropertyEnum.y"
    {
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 205 "../../../../Property/PEG/PropertyEnum.y"
    {
    AssignInfo info;
    info.member = pegContext.currAssignMember;
    info.value = (yyvsp[(3) - (3)]);
    pegContext.current()->assignments.insert(pegContext.currAssignFunc, info);
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 213 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.currAssignFunc = (yyvsp[(1) - (1)]);
    pegContext.currAssignMember = "";
}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 217 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.currAssignFunc = (yyvsp[(3) - (3)]);
    pegContext.currAssignMember = (yyvsp[(1) - (3)]);
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 223 "../../../../Property/PEG/PropertyEnum.y"
    {
    if (!pegContext.checkSlotName(pegContext.currAssignFunc)) {
        QString error = peg.tr("Unrecognized slot name <%1>.").arg(pegContext.currAssignFunc);
        yyerror(error.toLatin1().data());
        exit(1);
    }
}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 230 "../../../../Property/PEG/PropertyEnum.y"
    { yy_push_state_cpp_code(); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 231 "../../../../Property/PEG/PropertyEnum.y"
    {
    AssignInfo info;
    info.member = pegContext.currAssignMember;
    info.value = (yyvsp[(6) - (6)]);
    pegContext.current()->slots_code.insert(pegContext.currAssignFunc, info);
}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 239 "../../../../Property/PEG/PropertyEnum.y"
    {
    if (pegContext.current()->delegateInfo.data()) {
        QString error = peg.tr("Delegate for object <%1> already defined.").arg(pegContext.current()->name);
        yyerror(error.toLatin1().data());
        exit(1);
    }

    pegContext.current()->delegateInfo.reset(new DelegateInfo());
}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 251 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.current()->delegateInfo->name = (yyvsp[(1) - (2)]);
}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 265 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.current()->delegateInfo->attributes[(yyvsp[(1) - (3)])] = (yyvsp[(3) - (3)]);
}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 269 "../../../../Property/PEG/PropertyEnum.y"
    { yy_push_state_params_list(); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 271 "../../../../Property/PEG/PropertyEnum.y"
    { yy_push_state_cpp_code(); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 271 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.currPropertySet->setConstructorCode((yyvsp[(1) - (9)]), (yyvsp[(4) - (9)]), (yyvsp[(6) - (9)]), (yyvsp[(9) - (9)]));
}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 277 "../../../../Property/PEG/PropertyEnum.y"
    { yy_push_state_initialization_list(); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 278 "../../../../Property/PEG/PropertyEnum.y"
    {
    (yyval) = (yyvsp[(3) - (3)]);
}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 283 "../../../../Property/PEG/PropertyEnum.y"
    { yy_push_state_cpp_code(); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 284 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.currPropertySet->setDestructorCode((yyvsp[(2) - (7)]), (yyvsp[(7) - (7)]));
}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 289 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.enumCode.reset(new EnumCode((yyvsp[(2) - (2)])));
}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 293 "../../../../Property/PEG/PropertyEnum.y"
    {
    peg.code.append(pegContext.enumCode);
    pegContext.enumCode.reset();
}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 307 "../../../../Property/PEG/PropertyEnum.y"
    {
    EnumCode::EnumItemCode item;
    item.name = (yyvsp[(1) - (6)]);
    bool success = false;
    item.id = (yyvsp[(3) - (6)]).toInt(&success);
    if (!success) {
        QString error = peg.tr("Unrecognized enum item id <%1>.").arg((yyvsp[(3) - (6)]));
        yyerror(error.toLatin1().data());
        exit(1);
    }
    item.text = (yyvsp[(5) - (6)]);

    pegContext.enumCode->items.append(item);
}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 325 "../../../../Property/PEG/PropertyEnum.y"
    {
    pegContext.enumCode->items.last().states.append((yyvsp[(2) - (2)]));
}
    break;



/* Line 1806 of yacc.c  */
#line 1997 "../../../../Property/PEG/PropertyEnum.parser.cpp"
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
  *++yylsp = yyloc;

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

  yyerror_range[1] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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
#line 330 "../../../../Property/PEG/PropertyEnum.y"


void yyerror(char const *s)
{
    fprintf(stderr, "Parser error: (%d,%d)-(%d,%d) '%s'\n"
                    ,yylloc.first_line
                    ,yylloc.first_column
                    ,yylloc.last_line
                    ,yylloc.last_column
                    ,s);
}


