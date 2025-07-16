#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct ASTNode ASTNode;

ASTNode* parse(Token* tokens);

#endif