%{
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

%}

%union {
    char *id;
    struct ASTNode *node; // Nó da AST
    struct {
        int num_args;
    } args;
}

%token <id> ID
%token <id> NUM
%token INT VOID IF ELSE WHILE RETURN
%token ASSIGN LE LT GT GE EQ NE PLUS MINUS MULT DIV
%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE SEMICOLON COMMA

%type <node> programa declaracao_lista declaracao var_declaracao fun_declaracao
%type <node> tipo_especificador params param_lista param composicao_decl
%type <node> local_declaracoes statement_lista statement expressao_decl
%type <node> selecao_decl iteracao_decl retorno_decl expressao simples_expressao
%type <node> soma_expressao termo fator ativacao args arg_lista relacional
%type <node> var cabecalho_funcao soma mult 

%nonassoc IFX
%nonassoc ELSE

%%

programa : declaracao_lista {
    root = $1;
    verifica_main(); // Verifica se main() existe após o parsing
    $$ = root;
};

declaracao_lista : declaracao_lista declaracao {
    ASTNode *list = create_node("DECL_LIST", NULL);
    add_child(list, $1);
    add_child(list, $2);
    $$ = list;
}
| declaracao {
    ASTNode *list = create_node("DECL_LIST", NULL);
    add_child(list, $1);
    $$ = list;
} 
| error SEMICOLON {  // Recupera após ponto e vírgula
    yyerrok;
    yyclearin;
    $$ = create_node("DECL_LIST", NULL);
};

declaracao : var_declaracao { $$ = $1; }
           | fun_declaracao { $$ = $1; };

var_declaracao : tipo_especificador ID SEMICOLON {
    ASTNode *decl = create_node("VAR_DECL", $2);
    add_child(decl, $1);
    
    if (strcmp($1->value, "void") == 0) {
        fprintf(stderr, "\nERRO SEMÂNTICO: Variáveis não podem ser void. LINHA: %d\n\n", linha_atual);
        errors_num++;
    }
    insere_simbolo($2, "variavel", 0, 0, escopo_atual, NULL);
    $$ = decl;
}
| tipo_especificador ID LBRACKET NUM RBRACKET SEMICOLON {
    ASTNode *decl = create_node("VETOR_DECL", $2);
    add_child(decl, $1);
    add_child(decl, create_node("NUM", $4));
    
    if (strcmp($1->value, "void") == 0) {
        fprintf(stderr, "ERRO SEMÂNTICO: Vetores não podem ser void. LINHA: %d\n", linha_atual);
        exit(1);
    }
    insere_simbolo($2, "vetor", 0, 1, escopo_atual, NULL);
    $$ = decl;
};

tipo_especificador : INT {
    $$ = create_node("TYPE", "int");
}
| VOID {
    $$ = create_node("TYPE", "void");
};

fun_declaracao : cabecalho_funcao LPAREN params RPAREN composicao_decl {
    ASTNode *func = create_node("FUNC_DECL", $1->value);
    add_child(func, $1); // Tipo de retorno e nome
    add_child(func, $3); // Parâmetros
    add_child(func, $5); // Corpo
    
    escopo_atual = "global";
    $$ = func;
};

cabecalho_funcao : tipo_especificador ID {
    ASTNode *header = create_node("FUNC_HEADER", $2);
    add_child(header, $1); // Tipo de retorno
    
    insere_simbolo($2, "funcao", 1, 0, "global", $1->value);
    funcao_atual = $2;
    escopo_atual = $2;
    $$ = header;
};

params : VOID {
    // Marca a função como tendo parâmetros void
    Simbolo *simbolo = busca_simbolo(funcao_atual, "global");
    if (simbolo) simbolo->parametro_void = 1;
    $$ = create_node("PARAMS", "void");
}
| param_lista { $$ = $1; }
| /* vazio */ { $$ = create_node("PARAMS", NULL); };

param_lista : param_lista COMMA param {
    ASTNode *list = create_node("PARAM_LIST", NULL);
    add_child(list, $1);
    add_child(list, $3);
    $$ = list;
}
| param {
    ASTNode *list = create_node("PARAM_LIST", NULL);
    add_child(list, $1);
    $$ = list;
};

param : tipo_especificador ID {
    ASTNode *p = create_node("PARAM", $2);
    add_child(p, $1);
    if (strcmp($1->value, "void") == 0) {
        fprintf(stderr, "\nERRO SEMÂNTICO: Variáveis não podem ser void. LINHA: %d\n\n", linha_atual);
        exit(1);
    }
    insere_simbolo($2, "variavel", 0, 0, escopo_atual, NULL);
    $$ = p;
}
| tipo_especificador ID LBRACKET RBRACKET {
    ASTNode *p = create_node("PARAM_VETOR", $2);
    add_child(p, $1);
    if (strcmp($1->value, "void") == 0) {
        fprintf(stderr, "\nERRO SEMÂNTICO: Variáveis não podem ser void. LINHA: %d\n\n", linha_atual);
        exit(1);
    }
    insere_simbolo($2, "vetor", 0, 1, escopo_atual, NULL);
    $$ = p;
};

composicao_decl : LBRACE local_declaracoes statement_lista RBRACE {
    ASTNode *block = create_node("BLOCK", NULL);
    add_child(block, $2); // Declarações locais
    add_child(block, $3); // Statements
    $$ = block;
};

local_declaracoes : local_declaracoes var_declaracao {
    ASTNode *decls = create_node("LOCAL_DECLS", NULL);
    add_child(decls, $1);
    add_child(decls, $2);
    $$ = decls;
}
| /* vazio */ {
    $$ = create_node("LOCAL_DECLS", NULL);
};

statement_lista : statement_lista statement {
    ASTNode *stmts = create_node("STMT_LIST", NULL);
    add_child(stmts, $1);
    add_child(stmts, $2);
    $$ = stmts;
}
| /* vazio */ {
    $$ = create_node("STMT_LIST", NULL);
};

statement : expressao_decl { $$ = $1; }
          | composicao_decl { $$ = $1; }
          | selecao_decl { $$ = $1; }
          | iteracao_decl { $$ = $1; }
          | retorno_decl { $$ = $1; }
          | error SEMICOLON { 
                yyerrok;
                yyclearin;
                $$ = create_node("ERROR_STMT", NULL);
          };
          

expressao_decl : expressao SEMICOLON {
    ASTNode *expr_stmt = create_node("EXPR_STMT", NULL);
    add_child(expr_stmt, $1);
    $$ = expr_stmt;
}
| SEMICOLON {
    $$ = create_node("EMPTY_STMT", NULL);
};

selecao_decl : IF LPAREN expressao RPAREN statement %prec IFX {
    ASTNode *if_node = create_node("IF", NULL);
    add_child(if_node, $3); // Condição
    add_child(if_node, $5); // Corpo
    $$ = if_node;
}
| IF LPAREN expressao RPAREN statement ELSE statement {
    ASTNode *if_else_node = create_node("IF_ELSE", NULL);
    add_child(if_else_node, $3); // Condição
    add_child(if_else_node, $5); // If body
    add_child(if_else_node, $7); // Else body
    $$ = if_else_node;
};

iteracao_decl : WHILE LPAREN expressao RPAREN statement {
    ASTNode *while_node = create_node("WHILE", NULL);
    add_child(while_node, $3); // Condição
    add_child(while_node, $5); // Corpo
    $$ = while_node;
};

retorno_decl : RETURN SEMICOLON {
    ASTNode *ret = create_node("RETURN", NULL);
    $$ = ret;
}
| RETURN expressao SEMICOLON {
    Simbolo *func_sym = busca_simbolo(funcao_atual, "global");
    if (func_sym) {
        verifica_tipo_funcao(funcao_atual, func_sym->tipo_retorno, linha_atual, escopo_atual);
    }
    ASTNode *ret = create_node("RETURN", NULL);
    add_child(ret, $2);
    $$ = ret;
};

expressao : var ASSIGN expressao {
    ASTNode *assign = create_node("ASSIGN", NULL);
    add_child(assign, $1);
    add_child(assign, $3);
    if(strcmp($3->type, "FUNC_CALL") == 0){
        Simbolo *func_sym = busca_simbolo($3->value, "global");
        if(strcmp(func_sym->tipo_retorno, "void") == 0) {
            printf("\nERRO SEMANTICO: a funcao %s não pode ser usada como atribuição\n", func_sym->nome);
        }
    }
    $$ = assign;
}
| simples_expressao { $$ = $1; };

var : ID {
    ASTNode *var_node = create_node("VAR", $1);
    verifica_variavel($1, 0, linha_atual, escopo_atual, 0);
    $$ = var_node;
}
| ID LBRACKET expressao RBRACKET {
    ASTNode *array_access = create_node("ARRAY_ACCESS", $1);
    add_child(array_access, $3);
    verifica_variavel($1, 1, linha_atual, escopo_atual, 0);
    $$ = array_access;
};

simples_expressao : soma_expressao relacional soma_expressao {
    ASTNode *op = create_node("REL_OP", NULL);
    add_child(op, $2);
    add_child(op, $1);
    add_child(op, $3);
    $$ = op;
}
| soma_expressao { $$ = $1; };

relacional : LE { $$ = create_node("OP", "<="); }
          | LT { $$ = create_node("OP", "<"); }
          | GT { $$ = create_node("OP", ">"); }
          | GE { $$ = create_node("OP", ">="); }
          | EQ { $$ = create_node("OP", "=="); }
          | NE { $$ = create_node("OP", "!="); };

soma_expressao : soma_expressao soma termo {
    ASTNode *op = create_node("BIN_OP", NULL);
    add_child(op, $2);
    add_child(op, $1);
    add_child(op, $3);
    $$ = op;
}
| termo { $$ = $1; };

soma : PLUS { $$ = create_node("OP", "+"); }
     | MINUS { $$ = create_node("OP", "-"); };

termo : termo mult fator {
    ASTNode *op = create_node("BIN_OP", NULL);
    add_child(op, $2);
    add_child(op, $1);
    add_child(op, $3);
    $$ = op;
}
| fator { $$ = $1; };

mult : MULT { $$ = create_node("OP", "*"); }
     | DIV { $$ = create_node("OP", "/"); };

fator : LPAREN expressao RPAREN { $$ = $2; }
      | var { $$ = $1; }
      | ativacao { $$ = $1; }
      | NUM { $$ = create_node("NUM", $1); };

ativacao : ID LPAREN args RPAREN {
    int num_args = get_arg_count($3);
    verifica_chamada_funcao($1, num_args, linha_atual, escopo_atual);
    ASTNode *call = create_node("FUNC_CALL", $1);
    add_child(call, $3); // Argumentos
    $$ = call;
};

args : arg_lista { $$ = $1; }
| /* vazio */ { $$ = create_node("ARGS", NULL); };

arg_lista : arg_lista COMMA expressao {
    ASTNode *args = create_node("ARG_LIST", NULL);
    add_child(args, $1);
    add_child(args, $3);
    $$ = args;
}
| expressao {
    ASTNode *args = create_node("ARG_LIST", NULL);
    add_child(args, $1);
    $$ = args;
};

%%

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