#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static FILE* f = NULL;
static int curr_char = ' ';
static int linha = 1;
static Token peeked;
static int has_peek = 0;

static void read_char(){
    curr_char = fgetc(f);
    if(curr_char == '\n') linha++;
}

static void skip_whitespace(){
    while(curr_char != EOF && (curr_char==' ' || curr_char=='\t' || curr_char=='\r' || curr_char=='\n')) read_char();
}

static int is_ident_start(int c){ return isalpha(c); }
static int is_ident_part(int c){ return isalnum(c) || c=='_'; }

int lexer_init(const char* filename){
    f = fopen(filename, "r");
    if(!f) return 0;
    linha = 1;
    curr_char = fgetc(f);
    return 1;
}

void lexer_close(){
    if(f) fclose(f);
    f = NULL;
    curr_char = EOF;
    has_peek = 0;
}

static Token make_token(TokenType t, const char* lex){
    Token tk;
    tk.token = t;
    strncpy(tk.lexema, lex, LEXEMA_LEN-1);
    tk.lexema[LEXEMA_LEN-1] = 0;
    tk.linha = linha;
    return tk;
}

Token lexer_next_token(){
    if(has_peek){ has_peek = 0; return peeked; }
    Token tk;
    skip_whitespace();
    if(curr_char==EOF || curr_char==EOF+1){
        return make_token(T_EOF, "EOF");
    }
    if(is_ident_start(curr_char)){
        char buf[LEXEMA_LEN]={0};
        int i=0;
        while(curr_char!=EOF && is_ident_part(curr_char) && i<LEXEMA_LEN-1){
            buf[i++]=curr_char; curr_char = fgetc(f);
        }
        buf[i]=0;
        // to lower
        for(int j=0;j<i;j++) if(buf[j]>='A' && buf[j]<='Z') buf[j]+=32;
        // keywords
        if(strcmp(buf,"program")==0) return make_token(T_PROGRAM, buf);
        if(strcmp(buf,"var")==0) return make_token(T_VAR, buf);
        if(strcmp(buf,"integer")==0) return make_token(T_INTEGER, buf);
        if(strcmp(buf,"real")==0) return make_token(T_REAL, buf);
        if(strcmp(buf,"begin")==0) return make_token(T_BEGIN, buf);
        if(strcmp(buf,"end")==0) return make_token(T_END, buf);
        if(strcmp(buf,"if")==0) return make_token(T_IF, buf);
        if(strcmp(buf,"then")==0) return make_token(T_THEN, buf);
        if(strcmp(buf,"else")==0) return make_token(T_ELSE, buf);
        if(strcmp(buf,"while")==0) return make_token(T_WHILE, buf);
        if(strcmp(buf,"do")==0) return make_token(T_DO, buf);
        return make_token(T_IDENTIFIER, buf);
    }
    if(isdigit(curr_char)){
        char buf[LEXEMA_LEN]={0};
        int i=0;
        while(curr_char!=EOF && (isdigit(curr_char) || curr_char=='.') && i<LEXEMA_LEN-1){
            buf[i++]=curr_char; curr_char = fgetc(f);
        }
        buf[i]=0;
        return make_token(T_NUMBER, buf);
    }
    // symbols and operators
    if(curr_char==':'){
        int c = fgetc(f);
        if(c=='='){ curr_char = fgetc(f); return make_token(T_ASSIGN, ":="); }
        else { curr_char = c; return make_token(T_COLON, ":"); }
    }
    if(curr_char==';'){ curr_char = fgetc(f); return make_token(T_SEMI, ";"); }
    if(curr_char==','){ curr_char = fgetc(f); return make_token(T_COMMA, ","); }
    if(curr_char=='.'){ curr_char = fgetc(f); return make_token(T_DOT, "."); }
    if(curr_char=='('){ curr_char = fgetc(f); return make_token(T_LPAREN, "("); }
    if(curr_char==')'){ curr_char = fgetc(f); return make_token(T_RPAREN, ")"); }
    if(curr_char=='+'){ curr_char = fgetc(f); return make_token(T_PLUS, "+"); }
    if(curr_char=='-'){ curr_char = fgetc(f); return make_token(T_MINUS, "-"); }
    if(curr_char=='*'){ curr_char = fgetc(f); return make_token(T_MUL, "*"); }
    if(curr_char=='/'){ curr_char = fgetc(f); return make_token(T_DIV, "/"); }
    if(curr_char=='='){ curr_char = fgetc(f); return make_token(T_EQ, "="); }
    if(curr_char=='<'){
        int c = fgetc(f);
        if(c=='>'){ curr_char = fgetc(f); return make_token(T_NEQ, "<>"); }
        if(c=='='){ curr_char = fgetc(f); return make_token(T_LE, "<="); }
        curr_char = c; return make_token(T_LT, "<");
    }
    if(curr_char=='>'){
        int c = fgetc(f);
        if(c=='='){ curr_char = fgetc(f); return make_token(T_GE, ">="); }
        curr_char = c; return make_token(T_GT, ">");
    }
    // unknown char
    char buf[2] = { (char)curr_char, 0 };
    curr_char = fgetc(f);
    return make_token(T_UNKNOWN, buf);
}

Token lexer_peek_token(){
    if(!has_peek){
        peeked = lexer_next_token();
        has_peek = 1;
    }
    return peeked;
}
