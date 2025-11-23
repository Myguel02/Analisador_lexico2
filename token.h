#ifndef TOKEN_H
#define TOKEN_H
#define LEXEMA_LEN 128

typedef enum {
    T_EOF, T_PROGRAM, T_VAR, T_INTEGER, T_REAL, T_BEGIN, T_END,
    T_IF, T_THEN, T_ELSE, T_WHILE, T_DO,
    T_IDENTIFIER, T_NUMBER,
    T_ASSIGN, // :=
    T_SEMI, T_COMMA, T_COLON, T_DOT, T_LPAREN, T_RPAREN,
    T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_EQ, T_NEQ, T_LT, T_LE, T_GT, T_GE,
    T_UNKNOWN
} TokenType;

typedef struct {
    TokenType token;
    char lexema[LEXEMA_LEN];
    int linha;
} Token;

const char* token_name(TokenType t);

#endif // TOKEN_H
