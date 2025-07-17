#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "lexer.h"
#include "ast.h"

#define ENSURE(cond, msg) \
    if (!(cond)) { \
        fprintf(stderr, "Parser error: %s\n", msg); \
        exit(1); \
    }

typedef struct ASTNode ASTNode;

static ASTNode* parse_expression();
static ASTNode* parse_var_decl();
static ASTNode* parse_expression();
ASTNode* parse(Token* tokens);

#endif