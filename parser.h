#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "ast.h"

int parser_parse(const char* filename, AST** out_root);

#endif // PARSER_H
