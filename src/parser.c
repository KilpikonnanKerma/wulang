#include <stdlib.h>
#include <string.h>
#include "parser.h"

static Token* current;

static Token advance() { return *current++; }
static Token peek() { return *current; }
static int match(TokenType type) { return peek().type == type; }

void* safe_calloc(size_t n, size_t size) {
    void* ptr = calloc(n, size);
    if(!ptr) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    return ptr;
}

static ASTNode* parse_expression();

static ASTNode* parse_var_decl() {
    if (peek().type == TOKEN_EOF) {
        fprintf(stderr, "Unexpected end of input!\n");
        exit(1);
    }

    advance(); // int

    ENSURE(peek().type == TOKEN_IDENTIFIER, "Expected variable name after 'int'");
    Token name = advance(); // identifier
    if(!name.lexeme) {
        fprintf(stderr, "Variable name cannot be NULL\n");
        exit(1);
    }

    ENSURE(peek().type == TOKEN_ASSIGN, "Expected '=' after variable name");
    advance(); // =

    ASTNode* value = parse_expression();

    ENSURE(peek().type == TOKEN_SEMI, "Expected ';' after variable declaration");
    advance(); // ;

    ASTNode* node = safe_calloc(1, sizeof(ASTNode));
    node->type = NODE_VAR_DECL;
    node->data.var_decl.name = strdup(name.lexeme);
    node->data.var_decl.expr = value;
    return node;
}


static ASTNode* parse_expression() {
    if (peek().type == TOKEN_EOF) {
        fprintf(stderr, "Unexpected end of input!\n");
        exit(1);
    }

    Token left = advance();
    ASTNode* left_node = safe_calloc(1, sizeof(ASTNode));
    if (left.type == TOKEN_LITERAL) {
        left_node->type = NODE_LITERAL;
        left_node->data.literal = atoi(left.lexeme);
    } else if (left.type == TOKEN_IDENTIFIER) {
        left_node->type = NODE_IDENTIFIER;
        left_node->data.identifier = strdup(left.lexeme);
    } else {
        fprintf(stderr, "Expected literal or identifier, got token %d\n", left.type);
        exit(1);
    }

    if (peek().type == TOKEN_PLUS) {
        advance(); // skip '+'
        Token right = advance();

        ASTNode* right_node = safe_calloc(1, sizeof(ASTNode));
        if (right.type == TOKEN_LITERAL) {
            right_node->type = NODE_LITERAL;
            right_node->data.literal = atoi(right.lexeme);
        } else if (right.type == TOKEN_IDENTIFIER) {
            right_node->type = NODE_IDENTIFIER;
            right_node->data.identifier = strdup(right.lexeme);
        } else {
            fprintf(stderr, "Expected literal or identifier after '+'\n");
            exit(1);
        }

        ASTNode* binop = safe_calloc(1, sizeof(ASTNode));
        binop->type = NODE_BIN_OP;
        binop->data.bin_op.left = left_node;
        binop->data.bin_op.right = right_node;
        return binop;
    }

    return left_node;
}


ASTNode* parse(Token* tokens) {
    if (peek().type == TOKEN_EOF) {
        fprintf(stderr, "Unexpected end of input!\n");
        exit(1);
    }

    current = tokens;

    ASTNode* root = safe_calloc(1, sizeof(ASTNode));
    root->type = NODE_FUNCTION;
    root->data.function.name = strdup("main");
    root->data.function.body = safe_calloc(16, sizeof(ASTNode*));
    root->data.function.body_len = 0;

    advance(); // fn
    advance(); // main
    advance(); // (
    advance(); // )
    advance(); // {

    while (!match(TOKEN_RBRACE)) {
        if (match(TOKEN_INT)) {
            root->data.function.body[root->data.function.body_len++] = parse_var_decl();
        } else {
            advance(); // skip anything unknown
        }
    }

    return root;
}