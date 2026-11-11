#ifndef AST_H
#define AST_H

typedef struct ASTNode {
    char *type;               // Tipo do nó (ex: "PROGRAM", "ASSIGN", "IF", ...)
    char *value;              // Valor (para literais, IDs, etc.)
    struct ASTNode **children; // Array de filhos
    int num_children;         // Quantidade de filhos
} ASTNode;

ASTNode* create_node(const char *type, const char *value);
void add_child(ASTNode *parent, ASTNode *child);
void print_ast(ASTNode *node, int level);

#endif