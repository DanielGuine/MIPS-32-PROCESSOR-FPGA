#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "icode.h"

// Enumeração de registradores
typedef enum
{
    R_ZERO,
    R_T0,
    R_T1,
    R_T2,
    R_T3,
    R_T4,
    R_T5,
    R_T6,
    R_T7,
    R_T8,
    R_T9,
    R_T10,
    R_T11,
    R_T12,
    R_T13,
    R_T14,
    R_T15,
    R_T16,
    R_T17,
    R_T18,
    R_T19,
    R_T20,
    R_T21,
    R_T22,  // Reg Contador de Halt
    R_PROG, // Reg Prog
    R_SO,   // Reg SO
    R_AUX,
    R_T25,
    R_RF,
    R_FP,
    R_SP,
    R_RA
} Register;

// Estrutura para variável
typedef struct Variable
{
    char *name;
    int type;    // 0: variável, 1: vetor
    int address; // Endereço de memória (offset do base pointer)
    int size;    // Tamanho do vetor
    struct Variable *next;
} Variable;

// Estrutura para escopo
typedef struct Scope
{
    char *name;
    int next_address;    // Próximo endereço de offset disponível
    int local_var_count; // Contador de variáveis locais para alocação de pilha
    Variable *variables;
    struct Scope *next;
} Scope;

// Estrutura para label
typedef struct Label
{
    char *name;
    int address; // Endereço no código assembly
    struct Label *next;
} Label;

// Estrutura para a pilha de parâmetros
typedef struct ParamStack
{
    char *param_name;
    struct ParamStack *next;
} ParamStack;

// Funções principais
void init_assembly_generator();
void translate_to_assembly(Quad *quads, const char *output_file);
Register get_register_enum(const char *reg_name);

#endif