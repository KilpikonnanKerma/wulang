#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_EOF,
    TOKEN_INT,
    TOKEN_IDENTIFIER,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMI,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_FN,
    TOKEN_RETURN,
    TOKEN_LITERAL,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;
} Token;

Token* tokenize(const char* source);

#endif