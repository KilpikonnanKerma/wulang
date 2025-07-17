#ifndef AST_H
#define AST_H

typedef enum {
    NODE_FUNCTION,
    NODE_VAR_DECL,
    NODE_BIN_OP,
    NODE_LITERAL,
    NODE_IDENTIFIER,
    NODE_RETURN,
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        struct {
            char* name;
            struct ASTNode** body;
            int body_len;
        } function;

        struct {
            char* name;
            struct ASTNode* expr;
        } var_decl;

        struct {
            struct ASTNode* left;
            struct ASTNode* right;
        } bin_op;

        int literal;
        char* identifier;
    } data;

    struct ASTNode* next;
} ASTNode;

#endif