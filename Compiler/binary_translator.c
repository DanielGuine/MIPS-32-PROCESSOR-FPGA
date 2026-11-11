#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "binary_translator.h"

// Estrutura para facilitar a busca de opcodes
typedef struct
{
    char name[30];
    char opcode[7];
    char type;
} OpcodeEntry;

static const OpcodeEntry opcode_table[] = {
    {"MANTEMX", "000000", 'R'},
    {"NOT", "000001", 'R'},
    {"ADD", "000010", 'R'},
    {"SUB", "000011", 'R'},
    {"AND", "000100", 'R'},
    {"OR", "000101", 'R'},
    {"NOR", "000110", 'R'},
    {"DIV", "000111", 'R'},
    {"MUL", "001000", 'R'},
    {"SLT", "001001", 'R'},
    {"SLTE", "001010", 'R'},
    {"SGT", "001011", 'R'},
    {"SGTE", "001100", 'R'},
    {"EQUAL", "001101", 'R'},
    {"NEQ", "001110", 'R'},
    {"SL", "001111", 'R'},
    {"SRL", "010000", 'R'},
    {"INPUT", "011011", 'R'},
    {"JR", "011101", 'R'},

    {"ADDI", "010001", 'I'},
    {"SUBI", "010010", 'I'},
    {"ANDI", "010011", 'I'},
    {"ORI", "010100", 'I'},
    {"LI", "010101", 'I'},
    {"LW", "010110", 'I'},
    {"SW", "010111", 'I'},
    {"BEQ", "011001", 'I'},
    {"BNE", "011010", 'I'},
    {"JAL", "011100", 'I'},

    {"JUMP", "011000", 'J'},

    {"MOVE", "000000", 'P'},
    {"OUTPUT", "000000", 'P'},

    {"LOAD_PROG", "011110", 'S'},
    {"SET_LCD", "011111", 'S'},
    {"JALR", "100000", 'S'},
    {"SET_OFFSET_MD", "100001", 'S'},
    {"HALT", "100010", 'S'},
    {"SET_QUANTUM", "100011", 'S'}};

static const int table_size = sizeof(opcode_table) / sizeof(OpcodeEntry);

// Função que transforma o reg em binário
void get_register_binary(const char *reg_str, char *binary_out)
{
    if (!reg_str || reg_str[0] != 'r')
    {
        strcpy(binary_out, "00000");
        return;
    }
    int reg_num = atoi(reg_str + 1);
    for (int i = 4; i >= 0; i--)
    {
        binary_out[i] = ((reg_num >> (4 - i)) & 1) ? '1' : '0';
    }
    binary_out[5] = '\0';
}

// Função que transforma o inteiro em binário
void int_to_binary_str(int value, int bits, char *binary_out)
{
    for (int i = bits - 1; i >= 0; i--)
    {
        binary_out[i] = ((value >> (bits - 1 - i)) & 1) ? '1' : '0';
    }
    binary_out[bits] = '\0';
}

// Faz a busca do opcode da instrução
const OpcodeEntry *find_instruction(const char *name)
{
    char upper_name[30];
    int i = 0;
    while (name[i])
    {
        upper_name[i] = toupper(name[i]);
        i++;
    }
    upper_name[i] = '\0';

    for (i = 0; i < table_size; i++)
    {
        if (strcmp(upper_name, opcode_table[i].name) == 0)
        {
            return &opcode_table[i];
        }
    }
    return NULL;
}

// Função de tradução das instruções em loop
void translate_asm_to_binary(const char *input_asm_file, const char *output_binary_file)
{
    FILE *in_file = fopen(input_asm_file, "r");
    FILE *out_file = fopen(output_binary_file, "w");

    if (!in_file || !out_file)
    {
        perror("Erro ao abrir arquivos para tradução binária");
        return;
    }

    char line[256];
    int instruction_count = 0;

    while (fgets(line, sizeof(line), in_file))
    {
        if (strchr(line, ':') || line[0] == '\n' || line[0] == '\0')
        {
            continue;
        }

        char *mnemonic_ptr;
        char mnemonic[20], op1[20] = "r0", op2[20] = "r0", op3[20] = "r0";

        mnemonic_ptr = strchr(line, '-');
        if (!mnemonic_ptr)
            continue;
        sscanf(mnemonic_ptr + 1, "%s %s %s %s", mnemonic, op1, op2, op3);

        const OpcodeEntry *entry = find_instruction(mnemonic);
        if (!entry)
        {
            fprintf(stderr, "Aviso: Mnemônico desconhecido '%s' na linha %d\n", mnemonic, instruction_count);
            continue;
        }

        char bin_opcode[7], bin_r1[6], bin_r2[6], bin_r3[6], bin_imm[27];
        strcpy(bin_opcode, entry->opcode);
        char final_binary[50] = {0};

        if (strcasecmp(entry->name, "MOVE") == 0)
        {
            get_register_binary(op1, bin_r1);
            get_register_binary("r0", bin_r2);
            get_register_binary(op2, bin_r3);
            sprintf(final_binary, "%s%s%s%s00000000000", bin_opcode, bin_r1, bin_r2, bin_r3);
        }
        else if (strcasecmp(entry->name, "OUTPUT") == 0)
        {
            get_register_binary(op1, bin_r1);
            get_register_binary("r0", bin_r2);
            get_register_binary("r27", bin_r3);
            sprintf(final_binary, "%s%s%s%s00000000000", bin_opcode, bin_r1, bin_r2, bin_r3);
        }
        else if (strcasecmp(entry->name, "LOAD_PROG") == 0)
        {
            get_register_binary(op1, bin_r1);
            char bin_fixed[6];
            get_register_binary("r27", bin_fixed);
            sprintf(final_binary, "%s%s%s0000000000000000", bin_opcode, bin_r1, bin_fixed);
        }
        else if (strcasecmp(entry->name, "SET_LCD") == 0)
        {
            get_register_binary(op1, bin_r1);
            char bin_fixed[6];
            get_register_binary("r27", bin_fixed);
            sprintf(final_binary, "%s%s%s0000000000000000", bin_opcode, bin_r1, bin_fixed);
        }
        else if (strcasecmp(entry->name, "SET_OFFSET_MD") == 0)
        {
            get_register_binary(op1, bin_r1);
            sprintf(final_binary, "%s%s000000000000000000000", bin_opcode, bin_r1);
        }
        else if (strcasecmp(entry->name, "HALT") == 0)
        {
            sprintf(final_binary, "%s11001110000000000000000000", bin_opcode);
        }
        else if (strcasecmp(entry->name, "SET_QUANTUM") == 0)
        {
            sprintf(final_binary, "%s00000000000000000000000000", bin_opcode);
        }
        // Bloco para JALR
        else if (strcasecmp(entry->name, "JALR") == 0)
        {
            sprintf(final_binary, "%s11000110010000000000000000", bin_opcode);
        }
        else if (entry->type == 'R')
        {
            if (strcasecmp(entry->name, "INPUT") == 0)
            {
                get_register_binary("r0", bin_r1);
                get_register_binary(op1, bin_r2);
                get_register_binary("r0", bin_r3);
            }
            else
            {
                get_register_binary(op1, bin_r1);
                get_register_binary(op2, bin_r2);
                get_register_binary(op3, bin_r3);
            }
            sprintf(final_binary, "%s%s%s%s00000000000", bin_opcode, bin_r1, bin_r2, bin_r3);
        }
        else if (entry->type == 'I')
        {
            if (strcasecmp(entry->name, "JAL") == 0)
            {
                get_register_binary("r0", bin_r1);
                get_register_binary("r31", bin_r2);
                int_to_binary_str(atoi(op1), 16, bin_imm);
            }
            else if (strcasecmp(entry->name, "LI") == 0)
            {                                              // li <destino>, <imediato>
                get_register_binary("r0", bin_r1);         // rs não é usado
                get_register_binary(op1, bin_r2);          // rt é o destino (op1)
                int_to_binary_str(atoi(op2), 16, bin_imm); // imediato é op2
            }
            else if (strcasecmp(entry->name, "LW") == 0 || strcasecmp(entry->name, "SW") == 0)
            {
                get_register_binary(op1, bin_r1);
                get_register_binary(op2, bin_r2);
                int_to_binary_str(atoi(op3), 16, bin_imm);
            }
            else
            {
                get_register_binary(op1, bin_r1);
                get_register_binary(op2, bin_r2);
                int_to_binary_str(atoi(op3), 16, bin_imm);
            }
            sprintf(final_binary, "%s%s%s%s", bin_opcode, bin_r1, bin_r2, bin_imm);
        }
        else if (entry->type == 'J')
        {
            int_to_binary_str(atoi(op1), 26, bin_imm);
            sprintf(final_binary, "%s%s", bin_opcode, bin_imm);
        }

        fprintf(out_file, "rom[%d] <= 32'b%s;\n", instruction_count, final_binary);
        instruction_count++;
    }

    fclose(in_file);
    fclose(out_file);
}