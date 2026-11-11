#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembly.h"

// --- Variáveis Globais ---
static Scope *scopes = NULL;
static Label *labels = NULL;
static ParamStack *param_stack = NULL;
static int assembly_address = 0;
static Scope *current_scope = NULL;

const char *reg_map[] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10",
    "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19", "r20",
    "r21", "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"};

// --- Funções da Pilha de Parâmetros ---
void push_param(char *param_name)
{
    ParamStack *new_node = (ParamStack *)malloc(sizeof(ParamStack));
    new_node->param_name = strdup(param_name);
    new_node->next = param_stack;
    param_stack = new_node;
}

char *pop_param()
{
    if (!param_stack)
        return NULL;
    ParamStack *top = param_stack;
    char *param_name = strdup(top->param_name);
    param_stack = top->next;
    free(top->param_name);
    free(top);
    return param_name;
}

// --- Gerenciamento de Escopo, Variáveis e Labels ---
Scope *create_scope(char *name)
{
    Scope *new_scope = (Scope *)malloc(sizeof(Scope));
    new_scope->name = strdup(name);

    if (strcmp(name, "main") == 0)
    {
        new_scope->next_address = 0;
    }
    else
    {
        new_scope->next_address = 2;
    }

    new_scope->local_var_count = 0;
    new_scope->variables = NULL;
    new_scope->next = scopes;
    scopes = new_scope;
    return new_scope;
}

Scope *find_scope(char *name)
{
    for (Scope *s = scopes; s; s = s->next)
        if (strcmp(s->name, name) == 0)
            return s;
    return NULL;
}

void add_variable(Scope *scope, char *name, int is_param)
{
    Variable *new_var = (Variable *)malloc(sizeof(Variable));
    new_var->name = strdup(name);
    new_var->type = 0;
    new_var->size = 1;
    new_var->address = scope->next_address++;
    new_var->next = scope->variables;
    scope->variables = new_var;
    if (!is_param)
    {
        scope->local_var_count++;
    }
}

Variable *find_variable(const char *scope_name, const char *var_name)
{
    Scope *scope = find_scope((char *)scope_name);
    if (scope)
    {
        for (Variable *v = scope->variables; v; v = v->next)
            if (strcmp(v->name, var_name) == 0)
                return v;
    }
    return NULL;
}

void add_label(char *name, int address)
{
    Label *new_label = (Label *)malloc(sizeof(Label));
    new_label->name = strdup(name);
    new_label->address = address;
    new_label->next = labels;
    labels = new_label;
}

int find_label_address(char *name)
{
    for (Label *l = labels; l; l = l->next)
        if (strcmp(l->name, name) == 0)
            return l->address;
    fprintf(stderr, "Erro Crítico: Label '%s' não encontrada!\n", name);
    return -1;
}

int is_numeric(const char *s)
{
    if (!s || !*s)
        return 0;
    char *p;
    strtod(s, &p);
    return *p == '\0';
}

// Função para "pegar" o número do registrador do parâmetro
Register get_register_enum(const char *reg_name)
{
    if (!reg_name || !reg_name[0])
        return R_ZERO;
    if (reg_name[0] == '$' && reg_name[1] == 't')
    {
        int temp_num = atoi(reg_name + 2);

        if (temp_num >= 23)
        {
            return (Register)(R_T0 + temp_num + 2); // Pula R_PROG e R_SO
        }

        return (Register)(R_T0 + temp_num);
    }
    if (strcmp(reg_name, "$rf") == 0)
        return R_RF;
    return R_ZERO;
}

// Função principal de tradução
void translate_quad_to_assembly(Quad *quad, FILE *output)
{
    char *op = quad->op;
    char *arg1 = quad->arg1;
    char *arg2 = quad->arg2;
    char *result = quad->result;

    if (strcmp(op, "ADD") == 0 || strcmp(op, "SUB") == 0)
    {
        char inst[10];
        strcpy(inst, op);
        if (is_numeric(result))
        {
            sprintf(inst, "%si", op); // ADDi ou SUBi
            // Formato: op rt, rs, imm  (rt=dest, rs=src1)
            // quad: (op, dest, src1, src2/imm) -> (op, arg1, arg2, result)
            fprintf(output, "%d-%s %s %s %s\n",
                    assembly_address++, inst, reg_map[get_register_enum(arg2)], reg_map[get_register_enum(arg1)], result);
        }
        else
        {
            // Formato: op rd, rs, rt (rd=dest, rs=src1, rt=src2)
            // quad: (op, dest, src1, src2) -> (op, arg1, arg2, result)
            fprintf(output, "%d-%s %s %s %s\n",
                    assembly_address++, inst, reg_map[get_register_enum(arg2)], reg_map[get_register_enum(result)], reg_map[get_register_enum(arg1)]);
        }
    }
    else if (strcmp(op, "MUL") == 0 || strcmp(op, "DIV") == 0)
    {
        if (is_numeric(result))
        {
            if (atoi(result) == 0)
            {
                fprintf(output, "%d-%s %s %s %s\n",
                        assembly_address++, op, reg_map[get_register_enum(arg2)], reg_map[R_ZERO], reg_map[get_register_enum(arg1)]);
            }
            else
            {
                fprintf(output, "%d-li %s %s\n",
                        assembly_address++, reg_map[R_AUX], result);
                fprintf(output, "%d-%s %s %s %s\n",
                        assembly_address++, op, reg_map[get_register_enum(arg2)], reg_map[R_AUX], reg_map[get_register_enum(arg1)]);
            }
        }
        else
        {
            fprintf(output, "%d-%s %s %s %s\n",
                    assembly_address++, op, reg_map[get_register_enum(arg2)], reg_map[get_register_enum(result)], reg_map[get_register_enum(arg1)]);
        }
    }
    // --- NOVO BLOCO CONSOLIDADO PARA OPERADORES RELACIONAIS ---
    else if (strcmp(op, "EQUAL") == 0 || strcmp(op, "NE") == 0 ||
             strcmp(op, "LT") == 0 || strcmp(op, "LE") == 0 ||
             strcmp(op, "GT") == 0 || strcmp(op, "GE") == 0)
    {
        char inst[10];
        if (strcmp(op, "EQUAL") == 0)
            strcpy(inst, "equal");
        else if (strcmp(op, "NE") == 0)
            strcpy(inst, "neq");
        else if (strcmp(op, "LT") == 0)
            strcpy(inst, "slt");
        else if (strcmp(op, "LE") == 0)
            strcpy(inst, "slte");
        else if (strcmp(op, "GT") == 0)
            strcpy(inst, "sgt");
        else if (strcmp(op, "GE") == 0)
            strcpy(inst, "sgte");

        char *source2_reg;
        if (is_numeric(result))
        {
            if (atoi(result) == 0)
            {
                source2_reg = reg_map[R_ZERO];
            }
            else
            {
                fprintf(output, "%d-li %s %s\n",
                        assembly_address++, reg_map[R_AUX], result);
                source2_reg = reg_map[R_AUX];
            }
        }
        else
        {
            source2_reg = reg_map[get_register_enum(result)];
        }
        // Formato: op rs, rt, rd (rd=dest)
        // Quad: (op, dest, src1, src2) -> (op, arg1, arg2, result)
        fprintf(output, "%d-%s %s %s %s\n",
                assembly_address++, inst, reg_map[get_register_enum(arg2)], source2_reg, reg_map[get_register_enum(arg1)]);
    }
    else if (strcmp(op, "LOAD") == 0)
    {
        Variable *var = find_variable(current_scope->name, arg2);
        if (var)
        {
            fprintf(output, "%d-lw %s %s %d\n", assembly_address++, reg_map[R_FP], reg_map[get_register_enum(arg1)], var->address);
        }
    }
    else if (strcmp(op, "STORE") == 0)
    {
        Variable *var = find_variable(current_scope->name, arg1);
        if (var)
        {
            if (is_numeric(arg2))
            {
                fprintf(output, "%d-li %s %s\n",
                        assembly_address++, reg_map[R_AUX], arg2);
                fprintf(output, "%d-sw %s %s %d\n",
                        assembly_address++, reg_map[R_FP], reg_map[R_AUX], var->address);
            }
            else
            {
                fprintf(output, "%d-sw %s %s %d\n",
                        assembly_address++, reg_map[R_FP], reg_map[get_register_enum(arg2)], var->address);
            }
        }
    }
    else if (strcmp(op, "LAB") == 0)
    {
        fprintf(output, "%s:\n", arg1);
    }
    else if (strcmp(op, "GOTO") == 0)
    {
        fprintf(output, "%d-jump %d\n", assembly_address++, find_label_address(arg1));
    }
    else if (strcmp(op, "IFF") == 0)
    {
        int target_addr = find_label_address(arg2);
        fprintf(output, "%d-beq %s %s %d\n", assembly_address++, reg_map[get_register_enum(arg1)], reg_map[R_ZERO], target_addr);
    }
    else if (strcmp(op, "FUN") == 0)
    {
        current_scope = find_scope(arg2);
        fprintf(output, "Função %s:\n", arg2);
        if (strcmp(arg2, "main") != 0)
        {
            fprintf(output, "%d-sw %s %s 1\n", assembly_address++, reg_map[R_FP], reg_map[R_RA]);
            fprintf(output, "%d-addi %s %s 1\n", assembly_address++, reg_map[R_SP], reg_map[R_SP]);
        }
    }
    else if (strcmp(op, "END") == 0)
    {
        if (strcmp(arg1, "main") != 0)
        {
            fprintf(output, "%d-lw %s %s 1\n", assembly_address++, reg_map[R_FP], reg_map[R_RA]);
            fprintf(output, "%d-jr %s %s %s\n", assembly_address++, reg_map[R_RA], reg_map[R_ZERO], reg_map[R_ZERO]);
        }
        current_scope = find_scope("global");
    }
    else if (strcmp(op, "PARAM") == 0)
    {
        push_param(arg1);
    }
    else if (strcmp(op, "CALL") == 0)
    {
        int num_args = atoi(result);
        fprintf(output, "%d-sw %s %s 0\n", assembly_address++, reg_map[R_SP], reg_map[R_FP]);
        fprintf(output, "%d-move %s %s 0\n", assembly_address++, reg_map[R_SP], reg_map[R_FP]);
        fprintf(output, "%d-addi %s %s 1\n", assembly_address++, reg_map[R_SP], reg_map[R_SP]);

        char **params = malloc(num_args * sizeof(char *));
        for (int i = num_args - 1; i >= 0; i--)
        {
            params[i] = pop_param();
        }

        for (int i = 0; i < num_args; i++)
        {
            fprintf(output, "%d-sw %s %s %d\n", assembly_address++, reg_map[R_FP], reg_map[get_register_enum(params[i])], i + 2);
            free(params[i]);
        }
        free(params);

        fprintf(output, "%d-jal %d\n", assembly_address++, find_label_address(arg2));
        fprintf(output, "%d-move %s %s 0\n", assembly_address++, reg_map[R_FP], reg_map[R_SP]);
        fprintf(output, "%d-lw %s %s 0\n", assembly_address++, reg_map[R_FP], reg_map[R_FP]);

        for (int i = 0; i < num_args; i++)
        {
            fprintf(output, "%d-subi %s %s 1\n", assembly_address++, reg_map[R_SP], reg_map[R_SP]);
        }

        fprintf(output, "%d-move %s %s 0\n", assembly_address++, reg_map[R_RF], reg_map[get_register_enum(arg1)]);
    }
    else if (strcmp(op, "RET") == 0)
    {
        if (arg1)
        {
            if (is_numeric(arg1))
            {
                fprintf(output, "%d-li %s %s\n",
                        assembly_address++,
                        reg_map[R_RF],
                        arg1);
            }
            else
            {
                fprintf(output, "%d-move %s %s 0\n",
                        assembly_address++,
                        reg_map[get_register_enum(arg1)],
                        reg_map[R_RF]);
            }
        }
        char end_label[40];
        sprintf(end_label, "L_end_%s", current_scope->name);
        fprintf(output, "%d-jump %d\n", assembly_address++, find_label_address(end_label));
    }
    else if (strcmp(op, "INPUT") == 0)
    {
        fprintf(output, "%d-input %s\n", assembly_address++, reg_map[R_RF]);
        fprintf(output, "%d-move %s %s 0\n", assembly_address++, reg_map[R_RF], reg_map[get_register_enum(result)]);
    }
    else if (strcmp(op, "OUTPUT") == 0)
    {
        fprintf(output, "%d-output %s\n", assembly_address++, reg_map[get_register_enum(arg1)]);
    }
    else if (strcmp(op, "LOAD_PROG") == 0)
    {
        if (is_numeric(arg1))
        {
            fprintf(output, "%d-li %s %s\n", assembly_address++, reg_map[R_AUX], arg1);
            fprintf(output, "%d-load_prog %s\n", assembly_address++, reg_map[R_AUX]);
        }
        else
        {
            fprintf(output, "%d-load_prog %s\n", assembly_address++, reg_map[get_register_enum(arg1)]);
        }
    }
    else if (strcmp(op, "SET_LCD") == 0)
    {
        if (is_numeric(arg1))
        {
            fprintf(output, "%d-li %s %s\n", assembly_address++, reg_map[R_AUX], arg1);
            fprintf(output, "%d-set_lcd %s\n", assembly_address++, reg_map[R_AUX]);
        }
        else
        {
            fprintf(output, "%d-set_lcd %s\n", assembly_address++, reg_map[get_register_enum(arg1)]);
        }
    }
    else if (strcmp(op, "SET_OFFSET_MD") == 0)
    {
        if (is_numeric(arg1))
        {
            fprintf(output, "%d-li %s %s\n", assembly_address++, reg_map[R_AUX], arg1);
            fprintf(output, "%d-set_offset_md %s\n", assembly_address++, reg_map[R_AUX]);
        }
        else
        {
            fprintf(output, "%d-set_offset_md %s\n", assembly_address++, reg_map[get_register_enum(arg1)]);
        }
    }
    else if (strcmp(op, "HALT") == 0)
    {
        fprintf(output, "%d-halt\n", assembly_address++);
    }
    else if (strcmp(op, "JALR") == 0)
    {
        fprintf(output, "%d-jalr\n", assembly_address++);
    }
    else if (strcmp(op, "SAVE_REGS") == 0)
    {
        for (int i = 0; i < 32; i++)
        {
            fprintf(output, "%d-sw %s %s %d\n",
                    assembly_address++,
                    "r0",
                    reg_map[i],
                    50 + i);
        }
    }
    else if (strcmp(op, "LOAD_REGS") == 0)
    {
        for (int i = 0; i < 32; i++)
        {
            fprintf(output, "%d-lw %s %s %d\n",
                    assembly_address++,
                    "r0",
                    reg_map[i],
                    50 + i);
        }
    }
    else if (strcmp(op, "CLEAR_REGS") == 0)
    {
        for (int i = 0; i < 32; i++)
        {
            fprintf(output, "%d-li %s 0\n",
                    assembly_address++,
                    reg_map[i]);
        }
    }
    else if (strcmp(op, "SET_QUANTUM") == 0)
    {
        fprintf(output, "%d-set_quantum\n", assembly_address++);
    }
    else if (strcmp(op, "ARG") == 0 || strcmp(op, "ALLOC") == 0)
    {
        fprintf(output, "%d-addi %s %s 1\n", assembly_address++, reg_map[R_SP], reg_map[R_SP]);
    }
}

// Prepara o ambiente para a geração do código
void init_assembly_generator()
{
    while (scopes)
    {
        Scope *ts = scopes;
        scopes = scopes->next;
        free(ts->name);
        while (ts->variables)
        {
            Variable *tv = ts->variables;
            ts->variables = tv->next;
            free(tv->name);
            free(tv);
        }
        free(ts);
    }
    while (labels)
    {
        Label *tl = labels;
        labels = labels->next;
        free(tl->name);
        free(tl);
    }
    while (param_stack)
    {
        char *p = pop_param();
        free(p);
    }
    assembly_address = 0;
    current_scope = create_scope("global");
}

// Contador de instruções
int count_instructions_for_quad(Quad *q)
{
    if (!q || !q->op)
        return 0;
    char *op = q->op;

    if (strcmp(op, "MUL") == 0 || strcmp(op, "DIV") == 0)
    {
        if (is_numeric(q->result) && atoi(q->result) != 0)
        {
            return 2;
        }
    }
    // --- BLOCO ATUALIZADO PARA CONTAGEM DOS OPERADORES RELACIONAIS ---
    if (strcmp(op, "EQUAL") == 0 || strcmp(op, "NE") == 0 ||
        strcmp(op, "LT") == 0 || strcmp(op, "LE") == 0 ||
        strcmp(op, "GT") == 0 || strcmp(op, "GE") == 0)
    {
        if (is_numeric(q->result) && atoi(q->result) != 0)
        {
            return 2; // li + instrução
        }
    }
    if (strcmp(op, "STORE") == 0)
    {
        if (is_numeric(q->arg2))
        {
            return 2;
        }
    }

    if (strcmp(op, "LAB") == 0 || strcmp(op, "PARAM") == 0)
        return 0;
    if (strcmp(op, "FUN") == 0)
        return (strcmp(q->arg2, "main") != 0) ? 2 : 0;
    if (strcmp(op, "END") == 0)
        return (strcmp(q->arg1, "main") != 0) ? 2 : 0;
    if (strcmp(op, "RET") == 0)
        return (q->arg1 ? 1 : 0) + 1;
    if (strcmp(op, "INPUT") == 0)
        return 2;
    if (strcmp(op, "LOAD_PROG") == 0)
    {
        if (is_numeric(q->arg1))
            return 2;
        return 1;
    }
    if (strcmp(op, "SET_LCD") == 0)
    {
        if (is_numeric(q->arg1))
            return 2;
        return 1;
    }
    if (strcmp(op, "SET_OFFSET_MD") == 0)
    {
        if (is_numeric(q->arg1))
            return 2;
        return 1;
    }
    if (strcmp(op, "CALL") == 0)
    {
        int num_args = atoi(q->result);
        return 3 + num_args + 1 + 2 + num_args + 1;
    }
    if (strcmp(op, "SAVE_REGS") == 0 ||
        strcmp(op, "LOAD_REGS") == 0 ||
        strcmp(op, "CLEAR_REGS") == 0)
    {
        return 32;
    }

    return 1;
}

// Função de início
void translate_to_assembly(Quad *quads, const char *output_file)
{
    FILE *output = fopen(output_file, "w");
    if (!output)
    {
        perror("Erro ao abrir arquivo de saída");
        exit(1);
    }

    init_assembly_generator();

    assembly_address = 1;
    current_scope = find_scope("global");

    for (Quad *q = quads; q; q = q->next)
    {
        if (strcmp(q->op, "FUN") == 0)
        {
            current_scope = create_scope(q->arg2);
            add_label(q->arg2, assembly_address);
        }
        else if (strcmp(q->op, "END") == 0)
        {
            char end_label[40];
            sprintf(end_label, "L_end_%s", q->arg1);
            add_label(end_label, assembly_address);
            current_scope = find_scope("global");
        }
        else if (strcmp(q->op, "ARG") == 0)
        {
            add_variable(find_scope(q->result), q->arg2, 1);
        }
        else if (strcmp(q->op, "ALLOC") == 0)
        {
            add_variable(find_scope(q->arg2), q->arg1, 0);
        }
        else if (strcmp(q->op, "LAB") == 0)
        {
            add_label(q->arg1, assembly_address);
        }
        assembly_address += count_instructions_for_quad(q);
    }

    assembly_address = 0;
    fprintf(output, "%d-jump %d\n", assembly_address++, find_label_address("main"));

    for (Quad *q = quads; q; q = q->next)
    {
        translate_quad_to_assembly(q, output);
    }

    fclose(output);
}