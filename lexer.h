#ifndef LEXER_H
#define LEXER_H
#include "token.h"

int lexer_init(const char* filename);
void lexer_close();
Token lexer_next_token();
Token lexer_peek_token();

#endif // LEXER_H
