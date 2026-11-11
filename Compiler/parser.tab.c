/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "symtable.h"
#include "ast.h"
#include "icode.h"
#include "assembly.h"
#include "binary_translator.h"

int get_arg_count(struct ASTNode *node);

extern int linha_atual;
extern int errors_num;

ASTNode *root = NULL; // Raiz da árvore sintática


int yylex();
void yyerror(const char *s) {
    fprintf(stderr, "\nERRO SINTATICO: %s LINHA: %d\n\n", s, linha_atual);
    errors_num++;
}

// Função para verificar se main() foi declarada corretamente
void verifica_main() {
    Simbolo *main_func = busca_simbolo("main", "global");
    if (!main_func || 
        !main_func->is_function || 
        strcmp(main_func->tipo_retorno, "void") != 0 || 
        !main_func->parametro_void) {
        fprintf(stderr, "ERRO SEMÂNTICO: Função 'void main(void)' não declarada ou inválida.\n");
        errors_num++;
        return;
    }
}

int linha_atual = 1;
int errors_num = 0;
char *escopo_atual = "global";
char *funcao_atual = NULL;
int contexto_espera_valor = 0;
int contexto_argumento_funcao = 0;


#line 117 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_NUM = 4,                        /* NUM  */
  YYSYMBOL_INT = 5,                        /* INT  */
  YYSYMBOL_VOID = 6,                       /* VOID  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_RETURN = 10,                    /* RETURN  */
  YYSYMBOL_ASSIGN = 11,                    /* ASSIGN  */
  YYSYMBOL_LE = 12,                        /* LE  */
  YYSYMBOL_LT = 13,                        /* LT  */
  YYSYMBOL_GT = 14,                        /* GT  */
  YYSYMBOL_GE = 15,                        /* GE  */
  YYSYMBOL_EQ = 16,                        /* EQ  */
  YYSYMBOL_NE = 17,                        /* NE  */
  YYSYMBOL_PLUS = 18,                      /* PLUS  */
  YYSYMBOL_MINUS = 19,                     /* MINUS  */
  YYSYMBOL_MULT = 20,                      /* MULT  */
  YYSYMBOL_DIV = 21,                       /* DIV  */
  YYSYMBOL_LPAREN = 22,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 23,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 24,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 25,                  /* RBRACKET  */
  YYSYMBOL_LBRACE = 26,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 27,                    /* RBRACE  */
  YYSYMBOL_SEMICOLON = 28,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 29,                     /* COMMA  */
  YYSYMBOL_IFX = 30,                       /* IFX  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_programa = 32,                  /* programa  */
  YYSYMBOL_declaracao_lista = 33,          /* declaracao_lista  */
  YYSYMBOL_declaracao = 34,                /* declaracao  */
  YYSYMBOL_var_declaracao = 35,            /* var_declaracao  */
  YYSYMBOL_tipo_especificador = 36,        /* tipo_especificador  */
  YYSYMBOL_fun_declaracao = 37,            /* fun_declaracao  */
  YYSYMBOL_cabecalho_funcao = 38,          /* cabecalho_funcao  */
  YYSYMBOL_params = 39,                    /* params  */
  YYSYMBOL_param_lista = 40,               /* param_lista  */
  YYSYMBOL_param = 41,                     /* param  */
  YYSYMBOL_composicao_decl = 42,           /* composicao_decl  */
  YYSYMBOL_local_declaracoes = 43,         /* local_declaracoes  */
  YYSYMBOL_statement_lista = 44,           /* statement_lista  */
  YYSYMBOL_statement = 45,                 /* statement  */
  YYSYMBOL_expressao_decl = 46,            /* expressao_decl  */
  YYSYMBOL_selecao_decl = 47,              /* selecao_decl  */
  YYSYMBOL_iteracao_decl = 48,             /* iteracao_decl  */
  YYSYMBOL_retorno_decl = 49,              /* retorno_decl  */
  YYSYMBOL_expressao = 50,                 /* expressao  */
  YYSYMBOL_var = 51,                       /* var  */
  YYSYMBOL_simples_expressao = 52,         /* simples_expressao  */
  YYSYMBOL_relacional = 53,                /* relacional  */
  YYSYMBOL_soma_expressao = 54,            /* soma_expressao  */
  YYSYMBOL_soma = 55,                      /* soma  */
  YYSYMBOL_termo = 56,                     /* termo  */
  YYSYMBOL_mult = 57,                      /* mult  */
  YYSYMBOL_fator = 58,                     /* fator  */
  YYSYMBOL_ativacao = 59,                  /* ativacao  */
  YYSYMBOL_args = 60,                      /* args  */
  YYSYMBOL_arg_lista = 61                  /* arg_lista  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_int8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   101

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  107

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    73,    73,    79,    85,    90,    96,    97,    99,   110,
     123,   126,   130,   140,   150,   156,   157,   159,   165,   171,
     181,   192,   199,   205,   209,   215,   219,   220,   221,   222,
     223,   224,   231,   236,   240,   246,   254,   261,   265,   275,
     287,   289,   294,   301,   308,   310,   311,   312,   313,   314,
     315,   317,   324,   326,   327,   329,   336,   338,   339,   341,
     342,   343,   344,   346,   354,   355,   357,   363
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "NUM", "INT",
  "VOID", "IF", "ELSE", "WHILE", "RETURN", "ASSIGN", "LE", "LT", "GT",
  "GE", "EQ", "NE", "PLUS", "MINUS", "MULT", "DIV", "LPAREN", "RPAREN",
  "LBRACKET", "RBRACKET", "LBRACE", "RBRACE", "SEMICOLON", "COMMA", "IFX",
  "$accept", "programa", "declaracao_lista", "declaracao",
  "var_declaracao", "tipo_especificador", "fun_declaracao",
  "cabecalho_funcao", "params", "param_lista", "param", "composicao_decl",
  "local_declaracoes", "statement_lista", "statement", "expressao_decl",
  "selecao_decl", "iteracao_decl", "retorno_decl", "expressao", "var",
  "simples_expressao", "relacional", "soma_expressao", "soma", "termo",
  "mult", "fator", "ativacao", "args", "arg_lista", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-89)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-15)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      47,   -19,   -89,   -89,    24,    44,   -89,   -89,    66,   -89,
      39,   -89,   -89,   -89,    32,    58,    67,   -89,    50,    69,
      51,    41,   -89,    52,    54,    53,    44,    48,    55,   -89,
     -89,   -89,   -89,   -89,    44,   -89,    72,     1,    32,    56,
      13,   -89,    59,    60,     3,    12,   -89,   -89,   -89,   -89,
     -89,   -89,   -89,   -89,    57,    75,   -89,    28,    45,   -89,
     -89,   -89,    12,    12,    12,    12,   -89,    61,    64,   -89,
      12,   -89,   -89,   -89,   -89,   -89,   -89,   -89,   -89,    12,
      12,   -89,   -89,    12,   -89,    65,    62,    68,    71,    73,
     -89,   -89,   -89,   -89,    49,    45,   -89,   -89,    12,   -89,
      29,    29,   -89,    82,   -89,    29,   -89
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    10,    11,     0,     2,     4,     6,     0,     7,
       0,     5,     1,     3,    13,    16,     0,     8,    11,     0,
       0,    15,    18,     0,    19,     0,     0,     0,     0,    23,
      12,    17,     9,    20,    25,    22,     0,     0,     0,     0,
      41,    62,     0,     0,     0,     0,    21,    33,    27,    24,
      26,    28,    29,    30,     0,    60,    40,    44,    52,    56,
      61,    31,    65,     0,     0,     0,    37,     0,     0,    32,
       0,    45,    46,    47,    48,    49,    50,    53,    54,     0,
       0,    57,    58,     0,    67,     0,    64,     0,     0,     0,
      38,    59,    39,    60,    43,    51,    55,    63,     0,    42,
       0,     0,    66,    34,    36,     0,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -89,   -89,   -89,    78,    63,   -12,   -89,   -89,   -89,   -89,
      74,    70,   -89,   -89,   -88,   -89,   -89,   -89,   -89,   -44,
     -21,   -89,   -89,    19,   -89,    21,   -89,     9,   -89,   -89,
     -89
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,     7,     8,     9,    10,    20,    21,
      22,    48,    34,    37,    49,    50,    51,    52,    53,    54,
      55,    56,    79,    57,    80,    58,    83,    59,    60,    85,
      86
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      67,    68,    39,    19,    40,    41,    40,    41,    42,    11,
      43,    44,   103,   104,    19,    40,    41,   106,    84,    87,
      88,    89,    36,    45,    12,    45,    92,    29,    46,    47,
      39,    66,    40,    41,    45,    62,    42,    63,    43,    44,
      71,    72,    73,    74,    75,    76,    77,    78,     1,     2,
       3,    45,     2,     3,   102,    29,    16,    47,    93,    93,
      17,    15,    93,     2,    18,    81,    82,    77,    78,    14,
      26,    23,    24,   -14,    25,    38,    32,    27,    28,    29,
      33,    64,    65,    13,    61,    69,    70,    91,    97,    90,
     105,    98,    96,    99,   100,    30,   101,    35,    94,     0,
      31,    95
};

static const yytype_int8 yycheck[] =
{
      44,    45,     1,    15,     3,     4,     3,     4,     7,    28,
       9,    10,   100,   101,    26,     3,     4,   105,    62,    63,
      64,    65,    34,    22,     0,    22,    70,    26,    27,    28,
       1,    28,     3,     4,    22,    22,     7,    24,     9,    10,
      12,    13,    14,    15,    16,    17,    18,    19,     1,     5,
       6,    22,     5,     6,    98,    26,    24,    28,    79,    80,
      28,    22,    83,     5,     6,    20,    21,    18,    19,     3,
      29,     4,     3,    23,    23,     3,    28,    25,    24,    26,
      25,    22,    22,     5,    28,    28,    11,    23,    23,    28,
       8,    29,    83,    25,    23,    25,    23,    34,    79,    -1,
      26,    80
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     5,     6,    32,    33,    34,    35,    36,    37,
      38,    28,     0,    34,     3,    22,    24,    28,     6,    36,
      39,    40,    41,     4,     3,    23,    29,    25,    24,    26,
      42,    41,    28,    25,    43,    35,    36,    44,     3,     1,
       3,     4,     7,     9,    10,    22,    27,    28,    42,    45,
      46,    47,    48,    49,    50,    51,    52,    54,    56,    58,
      59,    28,    22,    24,    22,    22,    28,    50,    50,    28,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    53,
      55,    20,    21,    57,    50,    60,    61,    50,    50,    50,
      28,    23,    50,    51,    54,    56,    58,    23,    29,    25,
      23,    23,    50,    45,    45,     8,    45
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    33,    33,    34,    34,    35,    35,
      36,    36,    37,    38,    39,    39,    39,    40,    40,    41,
      41,    42,    43,    43,    44,    44,    45,    45,    45,    45,
      45,    45,    46,    46,    47,    47,    48,    49,    49,    50,
      50,    51,    51,    52,    52,    53,    53,    53,    53,    53,
      53,    54,    54,    55,    55,    56,    56,    57,    57,    58,
      58,    58,    58,    59,    60,    60,    61,    61
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     1,     3,     6,
       1,     1,     5,     2,     1,     1,     0,     3,     1,     2,
       4,     4,     2,     0,     2,     0,     1,     1,     1,     1,
       1,     2,     2,     1,     5,     7,     5,     2,     3,     3,
       1,     1,     4,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     4,     1,     0,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* programa: declaracao_lista  */
#line 73 "parser.y"
                            {
    root = (yyvsp[0].node);
    verifica_main(); // Verifica se main() existe após o parsing
    (yyval.node) = root;
}
#line 1241 "parser.tab.c"
    break;

  case 3: /* declaracao_lista: declaracao_lista declaracao  */
#line 79 "parser.y"
                                               {
    ASTNode *list = create_node("DECL_LIST", NULL);
    add_child(list, (yyvsp[-1].node));
    add_child(list, (yyvsp[0].node));
    (yyval.node) = list;
}
#line 1252 "parser.tab.c"
    break;

  case 4: /* declaracao_lista: declaracao  */
#line 85 "parser.y"
             {
    ASTNode *list = create_node("DECL_LIST", NULL);
    add_child(list, (yyvsp[0].node));
    (yyval.node) = list;
}
#line 1262 "parser.tab.c"
    break;

  case 5: /* declaracao_lista: error SEMICOLON  */
#line 90 "parser.y"
                  {  // Recupera após ponto e vírgula
    yyerrok;
    yyclearin;
    (yyval.node) = create_node("DECL_LIST", NULL);
}
#line 1272 "parser.tab.c"
    break;

  case 6: /* declaracao: var_declaracao  */
#line 96 "parser.y"
                            { (yyval.node) = (yyvsp[0].node); }
#line 1278 "parser.tab.c"
    break;

  case 7: /* declaracao: fun_declaracao  */
#line 97 "parser.y"
                            { (yyval.node) = (yyvsp[0].node); }
#line 1284 "parser.tab.c"
    break;

  case 8: /* var_declaracao: tipo_especificador ID SEMICOLON  */
#line 99 "parser.y"
                                                 {
    ASTNode *decl = create_node("VAR_DECL", (yyvsp[-1].id));
    add_child(decl, (yyvsp[-2].node));
    
    if (strcmp((yyvsp[-2].node)->value, "void") == 0) {
        fprintf(stderr, "\nERRO SEMÂNTICO: Variáveis não podem ser void. LINHA: %d\n\n", linha_atual);
        errors_num++;
    }
    insere_simbolo((yyvsp[-1].id), "variavel", 0, 0, escopo_atual, NULL);
    (yyval.node) = decl;
}
#line 1300 "parser.tab.c"
    break;

  case 9: /* var_declaracao: tipo_especificador ID LBRACKET NUM RBRACKET SEMICOLON  */
#line 110 "parser.y"
                                                        {
    ASTNode *decl = create_node("VETOR_DECL", (yyvsp[-4].id));
    add_child(decl, (yyvsp[-5].node));
    add_child(decl, create_node("NUM", (yyvsp[-2].id)));
    
    if (strcmp((yyvsp[-5].node)->value, "void") == 0) {
        fprintf(stderr, "ERRO SEMÂNTICO: Vetores não podem ser void. LINHA: %d\n", linha_atual);
        exit(1);
    }
    insere_simbolo((yyvsp[-4].id), "vetor", 0, 1, escopo_atual, NULL);
    (yyval.node) = decl;
}
#line 1317 "parser.tab.c"
    break;

  case 10: /* tipo_especificador: INT  */
#line 123 "parser.y"
                         {
    (yyval.node) = create_node("TYPE", "int");
}
#line 1325 "parser.tab.c"
    break;

  case 11: /* tipo_especificador: VOID  */
#line 126 "parser.y"
       {
    (yyval.node) = create_node("TYPE", "void");
}
#line 1333 "parser.tab.c"
    break;

  case 12: /* fun_declaracao: cabecalho_funcao LPAREN params RPAREN composicao_decl  */
#line 130 "parser.y"
                                                                       {
    ASTNode *func = create_node("FUNC_DECL", (yyvsp[-4].node)->value);
    add_child(func, (yyvsp[-4].node)); // Tipo de retorno e nome
    add_child(func, (yyvsp[-2].node)); // Parâmetros
    add_child(func, (yyvsp[0].node)); // Corpo
    
    escopo_atual = "global";
    (yyval.node) = func;
}
#line 1347 "parser.tab.c"
    break;

  case 13: /* cabecalho_funcao: tipo_especificador ID  */
#line 140 "parser.y"
                                         {
    ASTNode *header = create_node("FUNC_HEADER", (yyvsp[0].id));
    add_child(header, (yyvsp[-1].node)); // Tipo de retorno
    
    insere_simbolo((yyvsp[0].id), "funcao", 1, 0, "global", (yyvsp[-1].node)->value);
    funcao_atual = (yyvsp[0].id);
    escopo_atual = (yyvsp[0].id);
    (yyval.node) = header;
}
#line 1361 "parser.tab.c"
    break;

  case 14: /* params: VOID  */
#line 150 "parser.y"
              {
    // Marca a função como tendo parâmetros void
    Simbolo *simbolo = busca_simbolo(funcao_atual, "global");
    if (simbolo) simbolo->parametro_void = 1;
    (yyval.node) = create_node("PARAMS", "void");
}
#line 1372 "parser.tab.c"
    break;

  case 15: /* params: param_lista  */
#line 156 "parser.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1378 "parser.tab.c"
    break;

  case 16: /* params: %empty  */
#line 157 "parser.y"
              { (yyval.node) = create_node("PARAMS", NULL); }
#line 1384 "parser.tab.c"
    break;

  case 17: /* param_lista: param_lista COMMA param  */
#line 159 "parser.y"
                                      {
    ASTNode *list = create_node("PARAM_LIST", NULL);
    add_child(list, (yyvsp[-2].node));
    add_child(list, (yyvsp[0].node));
    (yyval.node) = list;
}
#line 1395 "parser.tab.c"
    break;

  case 18: /* param_lista: param  */
#line 165 "parser.y"
        {
    ASTNode *list = create_node("PARAM_LIST", NULL);
    add_child(list, (yyvsp[0].node));
    (yyval.node) = list;
}
#line 1405 "parser.tab.c"
    break;

  case 19: /* param: tipo_especificador ID  */
#line 171 "parser.y"
                              {
    ASTNode *p = create_node("PARAM", (yyvsp[0].id));
    add_child(p, (yyvsp[-1].node));
    if (strcmp((yyvsp[-1].node)->value, "void") == 0) {
        fprintf(stderr, "\nERRO SEMÂNTICO: Variáveis não podem ser void. LINHA: %d\n\n", linha_atual);
        exit(1);
    }
    insere_simbolo((yyvsp[0].id), "variavel", 0, 0, escopo_atual, NULL);
    (yyval.node) = p;
}
#line 1420 "parser.tab.c"
    break;

  case 20: /* param: tipo_especificador ID LBRACKET RBRACKET  */
#line 181 "parser.y"
                                          {
    ASTNode *p = create_node("PARAM_VETOR", (yyvsp[-2].id));
    add_child(p, (yyvsp[-3].node));
    if (strcmp((yyvsp[-3].node)->value, "void") == 0) {
        fprintf(stderr, "\nERRO SEMÂNTICO: Variáveis não podem ser void. LINHA: %d\n\n", linha_atual);
        exit(1);
    }
    insere_simbolo((yyvsp[-2].id), "vetor", 0, 1, escopo_atual, NULL);
    (yyval.node) = p;
}
#line 1435 "parser.tab.c"
    break;

  case 21: /* composicao_decl: LBRACE local_declaracoes statement_lista RBRACE  */
#line 192 "parser.y"
                                                                  {
    ASTNode *block = create_node("BLOCK", NULL);
    add_child(block, (yyvsp[-2].node)); // Declarações locais
    add_child(block, (yyvsp[-1].node)); // Statements
    (yyval.node) = block;
}
#line 1446 "parser.tab.c"
    break;

  case 22: /* local_declaracoes: local_declaracoes var_declaracao  */
#line 199 "parser.y"
                                                     {
    ASTNode *decls = create_node("LOCAL_DECLS", NULL);
    add_child(decls, (yyvsp[-1].node));
    add_child(decls, (yyvsp[0].node));
    (yyval.node) = decls;
}
#line 1457 "parser.tab.c"
    break;

  case 23: /* local_declaracoes: %empty  */
#line 205 "parser.y"
              {
    (yyval.node) = create_node("LOCAL_DECLS", NULL);
}
#line 1465 "parser.tab.c"
    break;

  case 24: /* statement_lista: statement_lista statement  */
#line 209 "parser.y"
                                            {
    ASTNode *stmts = create_node("STMT_LIST", NULL);
    add_child(stmts, (yyvsp[-1].node));
    add_child(stmts, (yyvsp[0].node));
    (yyval.node) = stmts;
}
#line 1476 "parser.tab.c"
    break;

  case 25: /* statement_lista: %empty  */
#line 215 "parser.y"
              {
    (yyval.node) = create_node("STMT_LIST", NULL);
}
#line 1484 "parser.tab.c"
    break;

  case 26: /* statement: expressao_decl  */
#line 219 "parser.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 1490 "parser.tab.c"
    break;

  case 27: /* statement: composicao_decl  */
#line 220 "parser.y"
                            { (yyval.node) = (yyvsp[0].node); }
#line 1496 "parser.tab.c"
    break;

  case 28: /* statement: selecao_decl  */
#line 221 "parser.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1502 "parser.tab.c"
    break;

  case 29: /* statement: iteracao_decl  */
#line 222 "parser.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 1508 "parser.tab.c"
    break;

  case 30: /* statement: retorno_decl  */
#line 223 "parser.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 1514 "parser.tab.c"
    break;

  case 31: /* statement: error SEMICOLON  */
#line 224 "parser.y"
                            { 
                yyerrok;
                yyclearin;
                (yyval.node) = create_node("ERROR_STMT", NULL);
          }
#line 1524 "parser.tab.c"
    break;

  case 32: /* expressao_decl: expressao SEMICOLON  */
#line 231 "parser.y"
                                     {
    ASTNode *expr_stmt = create_node("EXPR_STMT", NULL);
    add_child(expr_stmt, (yyvsp[-1].node));
    (yyval.node) = expr_stmt;
}
#line 1534 "parser.tab.c"
    break;

  case 33: /* expressao_decl: SEMICOLON  */
#line 236 "parser.y"
            {
    (yyval.node) = create_node("EMPTY_STMT", NULL);
}
#line 1542 "parser.tab.c"
    break;

  case 34: /* selecao_decl: IF LPAREN expressao RPAREN statement  */
#line 240 "parser.y"
                                                              {
    ASTNode *if_node = create_node("IF", NULL);
    add_child(if_node, (yyvsp[-2].node)); // Condição
    add_child(if_node, (yyvsp[0].node)); // Corpo
    (yyval.node) = if_node;
}
#line 1553 "parser.tab.c"
    break;

  case 35: /* selecao_decl: IF LPAREN expressao RPAREN statement ELSE statement  */
#line 246 "parser.y"
                                                      {
    ASTNode *if_else_node = create_node("IF_ELSE", NULL);
    add_child(if_else_node, (yyvsp[-4].node)); // Condição
    add_child(if_else_node, (yyvsp[-2].node)); // If body
    add_child(if_else_node, (yyvsp[0].node)); // Else body
    (yyval.node) = if_else_node;
}
#line 1565 "parser.tab.c"
    break;

  case 36: /* iteracao_decl: WHILE LPAREN expressao RPAREN statement  */
#line 254 "parser.y"
                                                        {
    ASTNode *while_node = create_node("WHILE", NULL);
    add_child(while_node, (yyvsp[-2].node)); // Condição
    add_child(while_node, (yyvsp[0].node)); // Corpo
    (yyval.node) = while_node;
}
#line 1576 "parser.tab.c"
    break;

  case 37: /* retorno_decl: RETURN SEMICOLON  */
#line 261 "parser.y"
                                {
    ASTNode *ret = create_node("RETURN", NULL);
    (yyval.node) = ret;
}
#line 1585 "parser.tab.c"
    break;

  case 38: /* retorno_decl: RETURN expressao SEMICOLON  */
#line 265 "parser.y"
                             {
    Simbolo *func_sym = busca_simbolo(funcao_atual, "global");
    if (func_sym) {
        verifica_tipo_funcao(funcao_atual, func_sym->tipo_retorno, linha_atual, escopo_atual);
    }
    ASTNode *ret = create_node("RETURN", NULL);
    add_child(ret, (yyvsp[-1].node));
    (yyval.node) = ret;
}
#line 1599 "parser.tab.c"
    break;

  case 39: /* expressao: var ASSIGN expressao  */
#line 275 "parser.y"
                                 {
    ASTNode *assign = create_node("ASSIGN", NULL);
    add_child(assign, (yyvsp[-2].node));
    add_child(assign, (yyvsp[0].node));
    if(strcmp((yyvsp[0].node)->type, "FUNC_CALL") == 0){
        Simbolo *func_sym = busca_simbolo((yyvsp[0].node)->value, "global");
        if(strcmp(func_sym->tipo_retorno, "void") == 0) {
            printf("\nERRO SEMANTICO: a funcao %s não pode ser usada como atribuição\n", func_sym->nome);
        }
    }
    (yyval.node) = assign;
}
#line 1616 "parser.tab.c"
    break;

  case 40: /* expressao: simples_expressao  */
#line 287 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1622 "parser.tab.c"
    break;

  case 41: /* var: ID  */
#line 289 "parser.y"
         {
    ASTNode *var_node = create_node("VAR", (yyvsp[0].id));
    verifica_variavel((yyvsp[0].id), 0, linha_atual, escopo_atual, 0);
    (yyval.node) = var_node;
}
#line 1632 "parser.tab.c"
    break;

  case 42: /* var: ID LBRACKET expressao RBRACKET  */
#line 294 "parser.y"
                                 {
    ASTNode *array_access = create_node("ARRAY_ACCESS", (yyvsp[-3].id));
    add_child(array_access, (yyvsp[-1].node));
    verifica_variavel((yyvsp[-3].id), 1, linha_atual, escopo_atual, 0);
    (yyval.node) = array_access;
}
#line 1643 "parser.tab.c"
    break;

  case 43: /* simples_expressao: soma_expressao relacional soma_expressao  */
#line 301 "parser.y"
                                                             {
    ASTNode *op = create_node("REL_OP", NULL);
    add_child(op, (yyvsp[-1].node));
    add_child(op, (yyvsp[-2].node));
    add_child(op, (yyvsp[0].node));
    (yyval.node) = op;
}
#line 1655 "parser.tab.c"
    break;

  case 44: /* simples_expressao: soma_expressao  */
#line 308 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1661 "parser.tab.c"
    break;

  case 45: /* relacional: LE  */
#line 310 "parser.y"
                { (yyval.node) = create_node("OP", "<="); }
#line 1667 "parser.tab.c"
    break;

  case 46: /* relacional: LT  */
#line 311 "parser.y"
               { (yyval.node) = create_node("OP", "<"); }
#line 1673 "parser.tab.c"
    break;

  case 47: /* relacional: GT  */
#line 312 "parser.y"
               { (yyval.node) = create_node("OP", ">"); }
#line 1679 "parser.tab.c"
    break;

  case 48: /* relacional: GE  */
#line 313 "parser.y"
               { (yyval.node) = create_node("OP", ">="); }
#line 1685 "parser.tab.c"
    break;

  case 49: /* relacional: EQ  */
#line 314 "parser.y"
               { (yyval.node) = create_node("OP", "=="); }
#line 1691 "parser.tab.c"
    break;

  case 50: /* relacional: NE  */
#line 315 "parser.y"
               { (yyval.node) = create_node("OP", "!="); }
#line 1697 "parser.tab.c"
    break;

  case 51: /* soma_expressao: soma_expressao soma termo  */
#line 317 "parser.y"
                                           {
    ASTNode *op = create_node("BIN_OP", NULL);
    add_child(op, (yyvsp[-1].node));
    add_child(op, (yyvsp[-2].node));
    add_child(op, (yyvsp[0].node));
    (yyval.node) = op;
}
#line 1709 "parser.tab.c"
    break;

  case 52: /* soma_expressao: termo  */
#line 324 "parser.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1715 "parser.tab.c"
    break;

  case 53: /* soma: PLUS  */
#line 326 "parser.y"
            { (yyval.node) = create_node("OP", "+"); }
#line 1721 "parser.tab.c"
    break;

  case 54: /* soma: MINUS  */
#line 327 "parser.y"
             { (yyval.node) = create_node("OP", "-"); }
#line 1727 "parser.tab.c"
    break;

  case 55: /* termo: termo mult fator  */
#line 329 "parser.y"
                         {
    ASTNode *op = create_node("BIN_OP", NULL);
    add_child(op, (yyvsp[-1].node));
    add_child(op, (yyvsp[-2].node));
    add_child(op, (yyvsp[0].node));
    (yyval.node) = op;
}
#line 1739 "parser.tab.c"
    break;

  case 56: /* termo: fator  */
#line 336 "parser.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1745 "parser.tab.c"
    break;

  case 57: /* mult: MULT  */
#line 338 "parser.y"
            { (yyval.node) = create_node("OP", "*"); }
#line 1751 "parser.tab.c"
    break;

  case 58: /* mult: DIV  */
#line 339 "parser.y"
           { (yyval.node) = create_node("OP", "/"); }
#line 1757 "parser.tab.c"
    break;

  case 59: /* fator: LPAREN expressao RPAREN  */
#line 341 "parser.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 1763 "parser.tab.c"
    break;

  case 60: /* fator: var  */
#line 342 "parser.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1769 "parser.tab.c"
    break;

  case 61: /* fator: ativacao  */
#line 343 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1775 "parser.tab.c"
    break;

  case 62: /* fator: NUM  */
#line 344 "parser.y"
            { (yyval.node) = create_node("NUM", (yyvsp[0].id)); }
#line 1781 "parser.tab.c"
    break;

  case 63: /* ativacao: ID LPAREN args RPAREN  */
#line 346 "parser.y"
                                 {
    int num_args = get_arg_count((yyvsp[-1].node));
    verifica_chamada_funcao((yyvsp[-3].id), num_args, linha_atual, escopo_atual);
    ASTNode *call = create_node("FUNC_CALL", (yyvsp[-3].id));
    add_child(call, (yyvsp[-1].node)); // Argumentos
    (yyval.node) = call;
}
#line 1793 "parser.tab.c"
    break;

  case 64: /* args: arg_lista  */
#line 354 "parser.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1799 "parser.tab.c"
    break;

  case 65: /* args: %empty  */
#line 355 "parser.y"
              { (yyval.node) = create_node("ARGS", NULL); }
#line 1805 "parser.tab.c"
    break;

  case 66: /* arg_lista: arg_lista COMMA expressao  */
#line 357 "parser.y"
                                      {
    ASTNode *args = create_node("ARG_LIST", NULL);
    add_child(args, (yyvsp[-2].node));
    add_child(args, (yyvsp[0].node));
    (yyval.node) = args;
}
#line 1816 "parser.tab.c"
    break;

  case 67: /* arg_lista: expressao  */
#line 363 "parser.y"
            {
    ASTNode *args = create_node("ARG_LIST", NULL);
    add_child(args, (yyvsp[0].node));
    (yyval.node) = args;
}
#line 1826 "parser.tab.c"
    break;


#line 1830 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 369 "parser.y"


int get_arg_count(struct ASTNode *node) {
    if (node == NULL || strcmp(node->type, "ARG_LIST") != 0) return 0;
    return node->num_children; // Directly return the number of children
}

int main() {
    printf("Iniciando análise...\n");
    inicializa_tabela();
    init_icode();  // Inicializa gerador de código
    
    if (yyparse() == 0 && errors_num == 0) {
        printf("\nÁrvore Sintática:\n");
        print_ast(root, 0);
        gen_icode(root);
        print_quads();     // Imprime quádruplas

        init_assembly_generator();
        translate_to_assembly(get_quads(), "output.asm");
        printf("\nCódigo assembly gerado em output.asm\n");

        translate_asm_to_binary("output.asm", "output.bin");
        printf("Código binário gerado em output.bin\n");
        
        printf("\nCompilação bem-sucedida!\n");
    } else {
        printf("\nErros encontrados: %d\n", errors_num);
    }
    
    imprime_tabela();
    free_quads();
    return 0;
}
