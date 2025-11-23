#include "token.h"
const char* token_name(TokenType t){
    switch(t){
        case T_EOF: return "EOF";
        case T_PROGRAM: return "program";
        case T_VAR: return "var";
        case T_INTEGER: return "integer";
        case T_REAL: return "real";
        case T_BEGIN: return "begin";
        case T_END: return "end";
        case T_IF: return "if";
        case T_THEN: return "then";
        case T_ELSE: return "else";
        case T_WHILE: return "while";
        case T_DO: return "do";
        case T_IDENTIFIER: return "id";
        case T_NUMBER: return "number";
        case T_ASSIGN: return ":=";
        case T_SEMI: return ";";
        case T_COMMA: return ",";
        case T_COLON: return ":";
        case T_DOT: return ".";
        case T_LPAREN: return "(";
        case T_RPAREN: return ")";
        case T_PLUS: return "+";
        case T_MINUS: return "-";
        case T_MUL: return "*";
        case T_DIV: return "/";
        case T_EQ: return "=";
        case T_NEQ: return "<>";
        case T_LT: return "<";
        case T_LE: return "<=";
        case T_GT: return ">";
        case T_GE: return ">=";
        default: return "unknown";
    }
}
