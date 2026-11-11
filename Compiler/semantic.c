#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

extern int linha_atual;    // Declaração da variável global linha_atual
extern int errors_num;     // Armazena o número de erros
extern char *escopo_atual; // Declaração da variável global escopo_atual
extern char *funcao_atual; // Declaração da variável global funcao_atual

Simbolo *tabela[TABLE_SIZE];

// Função de hash simples
unsigned int hash(char *str)
{
    unsigned int hash = 0;
    while (*str)
    {
        hash = (hash * 31) + *str;
        str++;
    }
    return hash % TABLE_SIZE;
}

// Inicializa a tabela de símbolos
void inicializa_tabela()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        tabela[i] = NULL;
    }

    // Insere as funções input e output como pré-declaradas
    insere_simbolo("input", "funcao", 1, 0, "global", "int");
    insere_simbolo("output", "funcao", 1, 0, "global", "void");
    insere_simbolo("Load_Prog", "funcao", 1, 0, "global", "void");
    insere_simbolo("SetLCD", "funcao", 1, 0, "global", "void");
    insere_simbolo("SetOffsetMD", "funcao", 1, 0, "global", "void");
    insere_simbolo("Jalr", "funcao", 1, 0, "global", "void");
    insere_simbolo("salva_regs", "funcao", 1, 0, "global", "void");
    insere_simbolo("carrega_regs", "funcao", 1, 0, "global", "void");
    insere_simbolo("limpa_regs", "funcao", 1, 0, "global", "void");
    insere_simbolo("Set_Quantum", "funcao", 1, 0, "global", "void");
}

// Insere um símbolo na tabela de símbolos
void insere_simbolo(char *nome, char *tipo, int is_function, int is_vector, char *escopo, char *tipo_retorno)
{
    // Verifica se o símbolo já existe no mesmo escopo
    Simbolo *existente = busca_simbolo(nome, escopo);
    if (existente != NULL)
    {
        fprintf(stderr, "\nERRO SEMÂNTICO: Redeclaração de '%s' no mesmo escopo. LINHA: %d\n\n", nome, linha_atual);
        errors_num++;
    }

    // Verifica se o símbolo já existe no escopo global (se o escopo atual não for global)
    if (strcmp(escopo, "global") != 0)
    { // Se o escopo atual não for global
        Simbolo *global_existente = busca_simbolo(nome, "global");
        if (global_existente != NULL)
        {
            fprintf(stderr, "\nERRO SEMÂNTICO: '%s' já declarado no escopo global. LINHA: %d\n\n", nome, linha_atual);
            errors_num++;
        }
    }

    unsigned int index = hash(nome);
    Simbolo *novo = (Simbolo *)malloc(sizeof(Simbolo));
    novo->nome = strdup(nome);
    novo->tipo = strdup(tipo);
    novo->is_function = is_function;
    novo->is_vector = is_vector;
    novo->escopo = strdup(escopo);
    novo->tipo_retorno = is_function ? strdup(tipo_retorno) : NULL;
    novo->parametro_void = 0; // Inicialmente, assume que a função não tem parâmetros void
    novo->prox = tabela[index];
    tabela[index] = novo;

    if (is_function)
    {
        printf("Inserindo função: %s no escopo %s\n", nome, escopo);
    }
    else if (is_vector)
    {
        printf("Inserindo vetor: %s no escopo %s\n", nome, escopo);
    }
    else
    {
        printf("Inserindo variável: %s no escopo %s\n", nome, escopo);
    }
}

// Busca um símbolo na tabela de símbolos
Simbolo *busca_simbolo(char *nome, char *escopo)
{
    printf("DEBUG: Buscando '%s' no escopo '%s'\n", nome, escopo);
    unsigned int index = hash(nome);
    Simbolo *atual = tabela[index];

    while (atual)
    {
        printf("DEBUG: Na posição %u - Nome: %s, Escopo: %s\n", index, atual->nome, atual->escopo);
        if (strcmp(atual->nome, nome) == 0 && strcmp(atual->escopo, escopo) == 0)
        {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

// Verifica se uma variável foi declarada corretamente
void verifica_variavel(char *nome, int is_vector_access, int linha, char *escopo, int is_function_arg)
{
    printf("DEBUG: Escopo atual antes de verificar '%s': %s\n", nome, escopo);

    Simbolo *simbolo = busca_simbolo(nome, escopo);
    if (!simbolo)
    {
        simbolo = busca_simbolo(nome, "global");
        if (!simbolo)
        {
            fprintf(stderr, "\nERRO SEMANTICO: '%s' LINHA: %d\n\n", nome, linha);
            errors_num++;
            return;
        }
    }

    printf("DEBUG: Verificando '%s' como %s (is_vector_access = %d) no escopo %s\n", nome, simbolo->tipo, is_vector_access, escopo);

    // Verifica se é um vetor usado sem colchetes fora de argumentos de função

    if (!simbolo->is_vector && is_vector_access)
    {
        fprintf(stderr, "ERRO SEMÂNTICO: '%s' não é um vetor, mas está sendo usado como um. LINHA: %d\n", nome, linha);
        errors_num++;
        return;
    }
}

// Verifica o tipo de retorno de uma função
void verifica_tipo_funcao(char *nome, char *tipo_esperado, int linha, char *escopo)
{
    Simbolo *simbolo = busca_simbolo(nome, escopo);
    if (!simbolo)
    {
        // Se a função não for encontrada no escopo atual, verifica no escopo global
        simbolo = busca_simbolo(nome, "global");
        if (!simbolo)
        {
            fprintf(stderr, "ERRO SEMANTICO: '%s' LINHA: %d\n", nome, linha);
            exit(1);
        }
    }
    if (!simbolo->is_function)
    {
        fprintf(stderr, "ERRO SEMÂNTICO: '%s' não é uma função. LINHA: %d\n", nome, linha);
        exit(1);
    }
    if (strcmp(simbolo->tipo_retorno, tipo_esperado) != 0)
    {
        fprintf(stderr, "ERRO SEMÂNTICO: A função '%s' retorna '%s', mas era esperado '%s'. LINHA: %d\n", nome, simbolo->tipo_retorno, tipo_esperado, linha);
        exit(1);
    }
}

// Verifica chamadas de função
void verifica_chamada_funcao(char *nome, int num_args, int linha, char *escopo)
{
    Simbolo *simbolo = busca_simbolo(nome, escopo);
    fprintf(stderr, "Verificando chamada de função: %s com %d argumentos e espera\n", nome, num_args);
    if (!simbolo)
    {
        simbolo = busca_simbolo(nome, "global");
        if (!simbolo)
        {
            fprintf(stderr, "\nERRO SEMÂNTICO: Função '%s' não declarada. LINHA: %d\n\n", nome, linha);
            errors_num++;
            return;
        }
    }

    if (simbolo->parametro_void)
    {
        fprintf(stderr, "teste\n");
    }

    if (simbolo->parametro_void && num_args > 0)
    {
        fprintf(stderr, "ERRO SEMÂNTICO: Função '%s' não aceita argumentos. LINHA: %d\n", nome, linha);
        errors_num++;
        return;
    }
}

void imprime_tabela()
{
    printf("\nTabela de Símbolos:\n");
    printf("--------------------------------------------------\n");
    printf("Nome\t\tTipo\t\tEscopo\t\tFunção/Vetor\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Simbolo *atual = tabela[i];
        while (atual)
        {
            printf("%s\t\t%s\t\t%s\t\t%s\n",
                   atual->nome,
                   atual->tipo,
                   atual->escopo,
                   atual->is_function ? "Função" : (atual->is_vector ? "Vetor" : "Variável"));
            atual = atual->prox;
        }
    }
    printf("--------------------------------------------------\n");
}
