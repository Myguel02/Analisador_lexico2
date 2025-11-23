#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"

int main(int argc, char** argv){
    if(argc<2){
        printf("Usage: %s source.pas\n", argv[0]);
        return 1;
    }
    AST* root = NULL;
    if(!parser_parse(argv[1], &root)){
        printf("Failed to parse file.\n");
        return 1;
    }
    ast_print(root, 0);
    ast_free(root);
    return 0;
}
