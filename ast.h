#ifndef AST_H
#define AST_H
#include <stdlib.h>

typedef enum { N_PROGRAM, N_BLOCK, N_DECLS, N_DECL, N_VARLIST, N_TYPE, N_COMPOUND, N_COMMAND, N_ASSIGN, N_IF, N_WHILE, N_EXPR, N_SIMPLE_EXPR, N_TERM, N_FACTOR, N_ID, N_NUM, N_TOKEN } NodeType;

typedef struct AST {
    NodeType type;
    char *label; // optional lexeme or token name
    struct AST **children;
    int nchild;
} AST;

AST* ast_new(NodeType t, const char* label);
void ast_add(AST* parent, AST* child);
void ast_print(AST* node, int depth);
void ast_free(AST* node);

#endif // AST_H
