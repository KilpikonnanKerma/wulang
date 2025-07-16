#include <stdlib.h>
#include <string.h>
#include "parser.h"

static Token* current;

static Token advance() { return *current++; }
static Token peek() { return *current; }
static int match(TokenType type) { return peek().type == type; }

ASTNode* parse(Token* tokens) {
    current = tokens;
    ASTNode* root = calloc(1, sizeof(ASTNode));
    root->type = NODE_FUNCTION;
    root->data.function.name = strdup("main");
    root->data.function.body = calloc(3, sizeof(ASTNode*));
    root->data.function.body_len = 3;

    advance(); // fn
    advance(); // main
    advance(); // (
    advance(); // )
    advance(); // {

    // int a = 5;
    advance(); // int
    Token id = advance(); // a
    advance(); // =
    Token lit = advance(); // 5
    advance(); // ;

    ASTNode* decl = calloc(1, sizeof(ASTNode));
    decl->type = NODE_VAR_DECL;
    decl->data.var_decl.name = strdup(id.lexeme);
    decl->data.var_decl.value = atoi(lit.lexeme);

    root->data.function.body[0] = decl;

    // dummy ops a + b -> skip for now
    root->data.function.body[1] = NULL;
    root->data.function.body[2] = NULL;

    return root;
}