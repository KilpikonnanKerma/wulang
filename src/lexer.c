#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define MAX_TOKENS 1024

static Token tokens[MAX_TOKENS];
static int token_index = 0;

static char* strndup(const char* s, size_t n) {
    char* result = malloc(n + 1);
    if (!result) return NULL;
    strncpy(result, s, n);
    result[n] = '\0';
    return result;
}

static void add_token(TokenType type, const char* start, int length) {
    Token* t = &tokens[token_index++];
    t->type = type;
    t->lexeme = strndup(start, length);
}

Token* tokenize(const char* source) {
    const char* p = source;

    while(*p) {
        if(isspace(*p)) { p++; continue; }

        if(isalpha(*p)) {
            const char* start = p;
            while(isalnum(*p)) p++;

            int len = p - start;

            if(strncmp(start, "fn", len) == 0 ) add_token(TOKEN_FN, start, len);
            else if(strncmp(start, "int", len) == 0 ) add_token(TOKEN_INT, start, len);
            else if(strncmp(start, "return", len) == 0 ) add_token(TOKEN_RETURN, start, len);
            else add_token(TOKEN_IDENTIFIER, start, len);

            continue;
        }

        if(isdigit(*p)) {
            const char* start = p;
            while(isdigit(*p)) p++;

            add_token(TOKEN_LITERAL, start, p - start);

            continue;
        }

        switch (*p) {
            case '(': add_token(TOKEN_LPAREN, p++, 1); break;
            case ')': add_token(TOKEN_RPAREN, p++, 1); break;
            case '{': add_token(TOKEN_LBRACE, p++, 1); break;
            case '}': add_token(TOKEN_RBRACE, p++, 1); break;
            case ';': add_token(TOKEN_SEMI, p++, 1); break;
            case '=': add_token(TOKEN_ASSIGN, p++, 1); break;
            case '+': add_token(TOKEN_PLUS, p++, 1); break;
            default: add_token(TOKEN_UNKNOWN, p++, 1); break;
        }
    }

    add_token(TOKEN_EOF, p, 0);
    return tokens;
}