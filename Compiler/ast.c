#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* create_node(const char *type, const char *value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = strdup(type);
    node->value = value ? strdup(value) : NULL;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

void add_child(ASTNode *parent, ASTNode *child) {
    parent->children = realloc(parent->children, sizeof(ASTNode*) * (parent->num_children + 1));
    parent->children[parent->num_children++] = child;
}

void print_ast(ASTNode *node, int level) {
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s", node->type);
    if (node->value) printf(" (%s)", node->value);
    printf("\n");
    for (int i = 0; i < node->num_children; i++) {
        print_ast(node->children[i], level + 1);
    }
}