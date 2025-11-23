#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token curr;
static Token nexttk(){
    curr = lexer_next_token();
    return curr;
}
static void error_unexpected(Token t){
    if(t.token==T_EOF){
        printf("%d:fim de arquivo não esperado.\n", t.linha);
    } else {
        printf("%d:token nao esperado [%s].\n", t.linha, t.lexema);
    }
    lexer_close();
    exit(1);
}

static void CasaToken(TokenType esperado){
    if(curr.token==esperado){
        curr = lexer_next_token();
    } else {
        error_unexpected(curr);
    }
}

/* Forward declarations for non-terminals */
static AST* programa();
static AST* bloco();
static AST* parte_decl_var();
static AST* declaracao_var();
static AST* lista_id();
static AST* tipo();
static AST* comando_composto();
static AST* comando();
static AST* atribuicao();
static AST* comando_condicional();
static AST* comando_repetitivo();
static AST* expressao();
static AST* expressao_simples();
static AST* termo();
static AST* fator();

/* Implementations */

static AST* programa(){
    AST* node = ast_new(N_PROGRAM, NULL);
    CasaToken(T_PROGRAM);
    if(curr.token==T_IDENTIFIER){
        ast_add(node, ast_new(N_ID, curr.lexema));
        CasaToken(T_IDENTIFIER);
    } else error_unexpected(curr);
    CasaToken(T_SEMI);
    AST* b = bloco();
    ast_add(node, b);
    CasaToken(T_DOT);
    return node;
}

static AST* bloco(){
    AST* node = ast_new(N_BLOCK, NULL);
    AST* decls = parte_decl_var();
    ast_add(node, decls);
    AST* comp = comando_composto();
    ast_add(node, comp);
    return node;
}

static AST* parte_decl_var(){
    AST* node = ast_new(N_DECLS, NULL);
    if(curr.token==T_VAR){
        CasaToken(T_VAR);
        AST* d = declaracao_var();
        ast_add(node, d);
        while(curr.token==T_SEMI){
            CasaToken(T_SEMI);
            if(curr.token==T_IDENTIFIER){
                AST* d2 = declaracao_var();
                ast_add(node, d2);
            } else break;
        }
        CasaToken(T_SEMI);
    }
    return node;
}

static AST* declaracao_var(){
    AST* node = ast_new(N_DECL, NULL);
    AST* ids = lista_id();
    ast_add(node, ids);
    CasaToken(T_COLON);
    AST* t = tipo();
    ast_add(node, t);
    return node;
}

static AST* lista_id(){
    AST* node = ast_new(N_VARLIST, NULL);
    if(curr.token==T_IDENTIFIER){
        ast_add(node, ast_new(N_ID, curr.lexema));
        CasaToken(T_IDENTIFIER);
        while(curr.token==T_COMMA){
            CasaToken(T_COMMA);
            if(curr.token==T_IDENTIFIER){
                ast_add(node, ast_new(N_ID, curr.lexema));
                CasaToken(T_IDENTIFIER);
            } else error_unexpected(curr);
        }
    } else error_unexpected(curr);
    return node;
}

static AST* tipo(){
    AST* node = ast_new(N_TYPE, curr.lexema);
    if(curr.token==T_INTEGER) CasaToken(T_INTEGER);
    else if(curr.token==T_REAL) CasaToken(T_REAL);
    else error_unexpected(curr);
    return node;
}

static AST* comando_composto(){
    AST* node = ast_new(N_COMPOUND, NULL);
    CasaToken(T_BEGIN);
    AST* first = comando();
    ast_add(node, first);
    while(curr.token==T_SEMI){
        CasaToken(T_SEMI);
        if(curr.token==T_END) break;
        AST* c = comando();
        ast_add(node, c);
    }
    CasaToken(T_END);
    return node;
}

static AST* comando(){
    if(curr.token==T_IDENTIFIER){
        return atribuicao();
    } else if(curr.token==T_BEGIN){
        return comando_composto();
    } else if(curr.token==T_IF){
        return comando_condicional();
    } else if(curr.token==T_WHILE){
        return comando_repetitivo();
    } else {
        error_unexpected(curr);
        return NULL;
    }
}

static AST* atribuicao(){
    AST* node = ast_new(N_ASSIGN, NULL);
    if(curr.token==T_IDENTIFIER){
        ast_add(node, ast_new(N_ID, curr.lexema));
        CasaToken(T_IDENTIFIER);
        CasaToken(T_ASSIGN);
        AST* e = expressao();
        ast_add(node, e);
        return node;
    } else error_unexpected(curr);
    return NULL;
}

static AST* comando_condicional(){
    AST* node = ast_new(N_IF, NULL);
    CasaToken(T_IF);
    AST* e = expressao();
    ast_add(node, e);
    CasaToken(T_THEN);
    AST* c1 = comando();
    ast_add(node, c1);
    if(curr.token==T_ELSE){
        CasaToken(T_ELSE);
        AST* c2 = comando();
        ast_add(node, c2);
    }
    return node;
}

static AST* comando_repetitivo(){
    AST* node = ast_new(N_WHILE, NULL);
    CasaToken(T_WHILE);
    AST* e = expressao();
    ast_add(node, e);
    CasaToken(T_DO);
    AST* c = comando();
    ast_add(node, c);
    return node;
}

/* Expressions */

static AST* expressao(){
    AST* left = expressao_simples();
    if(curr.token==T_EQ || curr.token==T_NEQ || curr.token==T_LT || curr.token==T_LE || curr.token==T_GT || curr.token==T_GE){
        AST* node = ast_new(N_EXPR, curr.lexema);
        CasaToken(curr.token);
        ast_add(node, left);
        AST* right = expressao_simples();
        ast_add(node, right);
        return node;
    }
    return left;
}

static AST* expressao_simples(){
    if(curr.token==T_PLUS || curr.token==T_MINUS){
        AST* s = ast_new(N_SIMPLE_EXPR, curr.lexema);
        CasaToken(curr.token);
        AST* t = termo();
        ast_add(s, t);
        return s;
    }
    AST* t = termo();
    while(curr.token==T_PLUS || curr.token==T_MINUS){
        AST* bin = ast_new(N_SIMPLE_EXPR, curr.lexema);
        CasaToken(curr.token);
        AST* t2 = termo();
        ast_add(bin, t);
        ast_add(bin, t2);
        t = bin;
    }
    return t;
}

static AST* termo(){
    AST* t = fator();
    while(curr.token==T_MUL || curr.token==T_DIV){
        AST* bin = ast_new(N_TERM, curr.lexema);
        CasaToken(curr.token);
        AST* f2 = fator();
        ast_add(bin, t);
        ast_add(bin, f2);
        t = bin;
    }
    return t;
}

static AST* fator(){
    if(curr.token==T_IDENTIFIER){
        AST* n = ast_new(N_ID, curr.lexema);
        CasaToken(T_IDENTIFIER);
        return n;
    } else if(curr.token==T_NUMBER){
        AST* n = ast_new(N_NUM, curr.lexema);
        CasaToken(T_NUMBER);
        return n;
    } else if(curr.token==T_LPAREN){
        CasaToken(T_LPAREN);
        AST* e = expressao();
        CasaToken(T_RPAREN);
        return e;
    } else {
        error_unexpected(curr);
        return NULL;
    }
}

int parser_parse(const char* filename, AST** out_root){
    if(!lexer_init(filename)) return 0;
    curr = lexer_next_token();
    AST* root = NULL;
    root = programa();
    *out_root = root;
    lexer_close();
    return 1;
}
