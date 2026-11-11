#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Definição da estrutura Simbolo
typedef struct Simbolo {
    char *nome;
    char *tipo;  // "variavel", "vetor", "funcao"
    int is_function;
    int is_vector;
    char *escopo;
    char *tipo_retorno;
    int parametro_void; // 1 se a função tem parâmetros void
    struct Simbolo *prox;
} Simbolo;

// Lista de parâmetros de funções
typedef struct ParamList {
    char *name;
    struct ParamList *next;
} ParamList;

// Tabela de símbolos
void inicializa_tabela();
void imprime_tabela();
void insere_simbolo(char *nome, char *tipo, int is_function, int is_vector, char *escopo, char *tipo_retorno);
Simbolo *busca_simbolo(char *nome, char *escopo);
void verifica_variavel(char *nome, int is_vector_access, int linha, char *escopo, int is_function_arg);
void verifica_tipo_funcao(char *nome, char *tipo_esperado, int linha, char *escopo);
void verifica_chamada_funcao(char *nome, int num_args, int linha, char *escopo);

#endif