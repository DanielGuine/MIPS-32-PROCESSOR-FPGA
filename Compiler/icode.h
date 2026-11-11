#ifndef ICODE_H
#define ICODE_H

#include "ast.h"

typedef struct Quad
{
    char *op;          // Operação
    char *arg1;        // Primeiro argumento
    char *arg2;        // Segundo argumento
    char *result;      // Resultado
    struct Quad *next; // Próxima quádrupla
} Quad;

// Interface de funções
void init_icode();
Quad *get_quads();
void gen_icode(ASTNode *node);
void print_quads();
void free_quads();

// Funções auxiliares
char *new_temp();
char *new_label();
void emit_quad(char *op, char *arg1, char *arg2, char *result);

#endif