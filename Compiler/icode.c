#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "icode.h"
#include "symtable.h"

static Quad *quad_list = NULL;
static int temp_count = 0;
static int label_count = 0;
static char *funcao_atual = NULL;
static int contexto_passando_argumento = 0;

void init_icode()
{
    quad_list = NULL;
    temp_count = 0;
    label_count = 0;
    funcao_atual = NULL;
    contexto_passando_argumento = 0;
}

char *new_temp()
{
    char *temp = malloc(32);
    sprintf(temp, "$t%d", temp_count++);
    return temp;
}

char *new_label()
{
    char *label = malloc(32);
    sprintf(label, "L%d", label_count++);
    return label;
}

static Quad *get_last_quad()
{
    if (!quad_list)
        return NULL;
    Quad *current = quad_list;
    while (current->next)
    {
        current = current->next;
    }
    return current;
}

void emit_quad(char *op, char *arg1, char *arg2, char *result)
{
    Quad *new_quad = malloc(sizeof(Quad));
    new_quad->op = op ? strdup(op) : NULL;
    new_quad->arg1 = arg1 ? strdup(arg1) : NULL;
    new_quad->arg2 = arg2 ? strdup(arg2) : NULL;
    new_quad->result = result ? strdup(result) : NULL;
    new_quad->next = NULL;

    if (!quad_list)
    {
        quad_list = new_quad;
    }
    else
    {
        Quad *current = quad_list;
        while (current->next)
        {
            current = current->next;
        }
        current->next = new_quad;
    }
}

void collect_actual_arg_expr_nodes(ASTNode *current_arg_list_node, ASTNode ***flat_list, int *count)
{
    if (!current_arg_list_node)
        return;

    if (strcmp(current_arg_list_node->type, "ARG_LIST") == 0)
    {
        if (current_arg_list_node->num_children == 1)
        {
            *flat_list = realloc(*flat_list, (*count + 1) * sizeof(ASTNode *));
            (*flat_list)[*count] = current_arg_list_node->children[0];
            (*count)++;
        }
        else if (current_arg_list_node->num_children == 2)
        {
            collect_actual_arg_expr_nodes(current_arg_list_node->children[0], flat_list, count);
            *flat_list = realloc(*flat_list, (*count + 1) * sizeof(ASTNode *));
            (*flat_list)[*count] = current_arg_list_node->children[1];
            (*count)++;
        }
    }
}

char *gen_expr(ASTNode *node)
{
    if (!node)
        return NULL;

    if (strcmp(node->type, "NUM") == 0)
    {
        return strdup(node->value);
    }
    else if (strcmp(node->type, "VAR") == 0)
    {
        char *temp = new_temp();
        emit_quad("LOAD", temp, node->value, NULL);
        return temp;
    }
    else if (strcmp(node->type, "FUNC_CALL") == 0)
    {
        ASTNode **actual_args_nodes = NULL;
        int actual_args_count = 0;

        if (node->num_children > 0 && node->children[0] != NULL)
        {
            if (strcmp(node->children[0]->type, "ARG_LIST") == 0)
            {
                collect_actual_arg_expr_nodes(node->children[0], &actual_args_nodes, &actual_args_count);
            }
        }

        if (strcmp(node->value, "input") == 0)
        {
            char *temp = new_temp();
            emit_quad("INPUT", NULL, NULL, temp);
            return temp;
        }
        else if (strcmp(node->value, "output") == 0)
        {
            if (actual_args_count > 0)
            {
                char *arg_temp = gen_expr(actual_args_nodes[0]);
                emit_quad("OUTPUT", arg_temp, NULL, NULL);
                if (arg_temp)
                    free(arg_temp);
            }
            if (actual_args_nodes)
                free(actual_args_nodes);
            return NULL;
        }
        else if (strcmp(node->value, "Load_Prog") == 0)
        {
            if (actual_args_count > 0)
            {
                char *arg_temp = gen_expr(actual_args_nodes[0]);
                emit_quad("LOAD_PROG", arg_temp, NULL, NULL);
                if (arg_temp)
                    free(arg_temp);
            }
            if (actual_args_nodes)
                free(actual_args_nodes);
            return NULL;
        }
        else if (strcmp(node->value, "SetLCD") == 0)
        {
            if (actual_args_count > 0)
            {
                char *arg_temp = gen_expr(actual_args_nodes[0]);
                emit_quad("SET_LCD", arg_temp, NULL, NULL);
                if (arg_temp)
                    free(arg_temp);
            }
            if (actual_args_nodes)
                free(actual_args_nodes);
            return NULL;
        }
        else if (strcmp(node->value, "SetOffsetMD") == 0)
        {
            if (actual_args_count > 0)
            {
                char *arg_temp = gen_expr(actual_args_nodes[0]);
                emit_quad("SET_OFFSET_MD", arg_temp, NULL, NULL);
                if (arg_temp)
                    free(arg_temp);
            }
            if (actual_args_nodes)
                free(actual_args_nodes);
            return NULL;
        }
        else if (strcmp(node->value, "Jalr") == 0)
        {
            emit_quad("JALR", NULL, NULL, NULL);

            if (actual_args_nodes)
                free(actual_args_nodes);
            return NULL;
        }
        else if (strcmp(node->value, "salva_regs") == 0)
        {
            emit_quad("SAVE_REGS", NULL, NULL, NULL);
            if (actual_args_nodes)
                free(actual_args_nodes);
            return NULL;
        }
        else if (strcmp(node->value, "carrega_regs") == 0)
        {
            emit_quad("LOAD_REGS", NULL, NULL, NULL);
            if (actual_args_nodes)
                free(actual_args_nodes);
            return NULL;
        }
        else if (strcmp(node->value, "limpa_regs") == 0)
        {
            emit_quad("CLEAR_REGS", NULL, NULL, NULL);
            if (actual_args_nodes)
                free(actual_args_nodes);
            return NULL;
        }
        else if (strcmp(node->value, "Set_Quantum") == 0)
        {
            emit_quad("SET_QUANTUM", NULL, NULL, NULL);
            if (actual_args_nodes)
                free(actual_args_nodes);
            return NULL;
        }

        for (int i = 0; i < actual_args_count; i++)
        {
            char *arg_temp = gen_expr(actual_args_nodes[i]);
            emit_quad("PARAM", arg_temp, NULL, NULL);
            free(arg_temp);
        }

        if (actual_args_nodes)
            free(actual_args_nodes);

        char *temp_ret_val = new_temp();
        char num_args_str[10];
        sprintf(num_args_str, "%d", actual_args_count);
        emit_quad("CALL", temp_ret_val, node->value, num_args_str);
        return temp_ret_val;
    }
    else if (strcmp(node->type, "BIN_OP") == 0 || strcmp(node->type, "REL_OP") == 0)
    {

        char *left_val_str;
        char *right_val_str;
        ASTNode *left_node = node->children[1];
        ASTNode *right_node = node->children[2];

        if (strcmp(right_node->type, "FUNC_CALL") == 0)
        {
            right_val_str = gen_expr(right_node); // Avalia a chamada de função PRIMEIRO
            left_val_str = gen_expr(left_node);   // Avalia o operando simples DEPOIS
        }
        else
        {
            // Ordem de avaliação padrão para outros casos
            left_val_str = gen_expr(left_node);
            right_val_str = gen_expr(right_node);
        }

        char *op_str_ast = node->children[0]->value;
        char *temp_result_str = new_temp();
        char *op_icode;

        if (strcmp(op_str_ast, "==") == 0)
            op_icode = "EQUAL";
        else if (strcmp(op_str_ast, "!=") == 0)
            op_icode = "NE";
        else if (strcmp(op_str_ast, "<") == 0)
            op_icode = "LT";
        else if (strcmp(op_str_ast, "<=") == 0)
            op_icode = "LE";
        else if (strcmp(op_str_ast, ">") == 0)
            op_icode = "GT";
        else if (strcmp(op_str_ast, ">=") == 0)
            op_icode = "GE";
        else if (strcmp(op_str_ast, "+") == 0)
            op_icode = "ADD";
        else if (strcmp(op_str_ast, "-") == 0)
            op_icode = "SUB";
        else if (strcmp(op_str_ast, "*") == 0)
            op_icode = "MUL";
        else if (strcmp(op_str_ast, "/") == 0)
            op_icode = "DIV";
        else
            op_icode = "UNKNOWN";

        emit_quad(op_icode, temp_result_str, left_val_str, right_val_str);

        free(left_val_str);
        free(right_val_str);
        return temp_result_str;
    }
    return NULL;
}

void collect_actual_param_nodes(ASTNode *current_param_list_node, ASTNode ***flat_list, int *count)
{
    if (!current_param_list_node)
        return;

    if (strcmp(current_param_list_node->type, "PARAM_LIST") == 0)
    {
        if (current_param_list_node->num_children == 1)
        {
            *flat_list = realloc(*flat_list, (*count + 1) * sizeof(ASTNode *));
            (*flat_list)[*count] = current_param_list_node->children[0];
            (*count)++;
        }
        else if (current_param_list_node->num_children == 2)
        {
            collect_actual_param_nodes(current_param_list_node->children[0], flat_list, count);
            *flat_list = realloc(*flat_list, (*count + 1) * sizeof(ASTNode *));
            (*flat_list)[*count] = current_param_list_node->children[1];
            (*count)++;
        }
    }
}

void gen_icode(ASTNode *node)
{
    if (!node)
        return;

    if (strcmp(node->type, "DECL_LIST") == 0 ||
        strcmp(node->type, "BLOCK") == 0 ||
        strcmp(node->type, "LOCAL_DECLS") == 0 ||
        strcmp(node->type, "STMT_LIST") == 0)
    {
        for (int i = 0; i < node->num_children; i++)
        {
            gen_icode(node->children[i]);
        }
    }
    else if (strcmp(node->type, "FUNC_DECL") == 0)
    {
        // Reinicia a contagem de temporários para o novo contexto da função.
        temp_count = 0;

        char *func_name = node->value;
        char *return_type = node->children[0]->children[0]->value;

        char *previous_funcao_context = funcao_atual;
        funcao_atual = func_name;

        emit_quad("FUN", return_type, func_name, NULL);

        ASTNode *params_container_node = node->children[1];
        if (params_container_node && strcmp(params_container_node->type, "PARAM_LIST") == 0)
        {
            ASTNode **param_nodes = NULL;
            int param_count = 0;
            collect_actual_param_nodes(params_container_node, &param_nodes, &param_count);
            for (int i = 0; i < param_count; i++)
            {
                emit_quad("ARG", param_nodes[i]->children[0]->value, param_nodes[i]->value, func_name);
            }
            if (param_nodes)
                free(param_nodes);
        }

        gen_icode(node->children[2]); // Corpo da função (BLOCK)

        emit_quad("END", func_name, NULL, NULL);
        if (strcmp(func_name, "main") == 0)
        {
            emit_quad("HALT", "-", "-", "-");
        }
        funcao_atual = previous_funcao_context;
    }
    else if (strcmp(node->type, "VAR_DECL") == 0)
    {
        emit_quad("ALLOC", node->value, funcao_atual ? funcao_atual : "global", NULL);
    }
    else if (strcmp(node->type, "ASSIGN") == 0)
    {
        char *rhs_val = gen_expr(node->children[1]);
        char *lhs_var = node->children[0]->value;
        emit_quad("STORE", lhs_var, rhs_val, NULL);
        if (rhs_val)
            free(rhs_val);
    }
    else if (strcmp(node->type, "IF") == 0)
    {
        char *cond_result = gen_expr(node->children[0]);
        char *end_label = new_label();
        emit_quad("IFF", cond_result, end_label, NULL);
        free(cond_result);
        gen_icode(node->children[1]); // Bloco then
        emit_quad("LAB", end_label, NULL, NULL);
        free(end_label);
    }
    else if (strcmp(node->type, "IF_ELSE") == 0)
    {
        char *cond_result = gen_expr(node->children[0]);
        char *else_label = new_label();
        char *end_label = new_label();
        emit_quad("IFF", cond_result, else_label, NULL);
        free(cond_result);

        gen_icode(node->children[1]);

        Quad *last_quad = get_last_quad();
        int ends_with_ret = (last_quad && strcmp(last_quad->op, "RET") == 0);

        if (!ends_with_ret)
        {
            emit_quad("GOTO", end_label, NULL, NULL);
        }

        emit_quad("LAB", else_label, NULL, NULL);
        free(else_label);

        gen_icode(node->children[2]);
        emit_quad("LAB", end_label, NULL, NULL);
        free(end_label);
    }
    else if (strcmp(node->type, "WHILE") == 0)
    {
        char *loop_start_label = new_label();
        char *loop_end_label = new_label();
        emit_quad("LAB", loop_start_label, NULL, NULL);
        char *cond_result = gen_expr(node->children[0]);
        emit_quad("IFF", cond_result, loop_end_label, NULL);
        free(cond_result);
        gen_icode(node->children[1]);
        emit_quad("GOTO", loop_start_label, NULL, NULL);
        emit_quad("LAB", loop_end_label, NULL, NULL);
        free(loop_start_label);
        free(loop_end_label);
    }
    else if (strcmp(node->type, "RETURN") == 0)
    {
        if (node->num_children > 0)
        {
            char *retval = gen_expr(node->children[0]);
            emit_quad("RET", retval, NULL, NULL);
            free(retval);
        }
        else
        {
            emit_quad("RET", NULL, NULL, NULL);
        }
    }
    else if (strcmp(node->type, "EXPR_STMT") == 0)
    {
        if (node->num_children > 0 && node->children[0] != NULL)
        {
            if (strcmp(node->children[0]->type, "ASSIGN") == 0)
            {
                gen_icode(node->children[0]);
            }
            else
            {
                char *temp_val = gen_expr(node->children[0]);
                if (temp_val)
                    free(temp_val);
            }
        }
    }
}

void free_quads()
{
    Quad *current = quad_list;
    Quad *next_quad;
    while (current)
    {
        next_quad = current->next;
        if (current->op)
            free(current->op);
        if (current->arg1)
            free(current->arg1);
        if (current->arg2)
            free(current->arg2);
        if (current->result)
            free(current->result);
        free(current);
        current = next_quad;
    }
    quad_list = NULL;
}

Quad *get_quads()
{
    return quad_list;
}

void print_quads()
{
    printf("\n=== CÓDIGO INTERMEDIÁRIO ===\n");
    Quad *current = quad_list;
    while (current)
    {
        printf("(%s, %s, %s, %s)\n",
               current->op ? current->op : "-",
               current->arg1 ? current->arg1 : "-",
               current->arg2 ? current->arg2 : "-",
               current->result ? current->result : "-");
        current = current->next;
    }
    printf("=============================\n");
}