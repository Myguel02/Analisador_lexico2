#include "ast.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

AST* ast_new(NodeType t, const char* label){
    AST* a = (AST*)malloc(sizeof(AST));
    a->type = t;
    if(label){
        a->label = strdup(label);
    } else a->label = NULL;
    a->children = NULL;
    a->nchild = 0;
    return a;
}

void ast_add(AST* parent, AST* child){
    if(!parent || !child) return;
    parent->children = (AST**)realloc(parent->children, sizeof(AST*)*(parent->nchild+1));
    parent->children[parent->nchild++] = child;
}

void ast_print(AST* node, int depth){
    if(!node) return;
    for(int i=0;i<depth;i++) printf("  ");
    const char *name = NULL;
    switch(node->type){
        case N_PROGRAM: name="program"; break;
        case N_BLOCK: name="block"; break;
        case N_DECLS: name="decls"; break;
        case N_DECL: name="decl"; break;
        case N_VARLIST: name="varlist"; break;
        case N_TYPE: name="type"; break;
        case N_COMPOUND: name="compound"; break;
        case N_COMMAND: name="command"; break;
        case N_ASSIGN: name="assign"; break;
        case N_IF: name="if"; break;
        case N_WHILE: name="while"; break;
        case N_EXPR: name="expr"; break;
        case N_SIMPLE_EXPR: name="simple_expr"; break;
        case N_TERM: name="term"; break;
        case N_FACTOR: name="factor"; break;
        case N_ID: name="id"; break;
        case N_NUM: name="num"; break;
        default: name="node"; break;
    }
    if(node->label) printf("%s: %s\n", name, node->label);
    else printf("%s\n", name);
    for(int i=0;i<node->nchild;i++) ast_print(node->children[i], depth+1);
}

void ast_free(AST* node){
    if(!node) return;
    for(int i=0;i<node->nchild;i++) ast_free(node->children[i]);
    if(node->label) free(node->label);
    if(node->children) free(node->children);
    free(node);
}
