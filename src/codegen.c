#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "codegen.h"

typedef struct {
    char* name;
    int stack_offset;
} Symbol;

static Symbol symbols[256];
static int symbol_count = 0;
static int current_offset = 0;

static int get_stack_offset(const char* name) {
    if(!name) {
        fprintf(stderr, "get_stack_offset() called with NULL\n");
        exit(1);
    }

    for(int i = 0; i < symbol_count; i++) {
        if(strcmp(symbols[i].name, name) == 0) {
            return symbols[i].stack_offset;
        }
    }
    fprintf(stderr, "Error: Undefined variable %s\n", name);
    exit(1);
}

static void add_symbol(const char* name) {
    if (!name) {
        fprintf(stderr, "add_symbol() called with NULL\n");
        exit(1);
    }

    printf("[add_symbol] Adding variable: %s\n", name);
    
    symbols[symbol_count].name = strdup(name);
    current_offset += 8;
    symbols[symbol_count].stack_offset = current_offset;
    symbol_count++;
}

static void gen_expr(FILE* out, ASTNode* node) {
    if (!node) {
        fprintf(stderr, "gen_expr called with NULL node!\n");
        exit(1);
    }

    switch (node->type) {
        case NODE_LITERAL:
            fprintf(out, "    mov rax, %d\n", node->data.literal);
            break;

        case NODE_IDENTIFIER: {
            int offset = get_stack_offset(node->data.identifier);
            fprintf(out, "    mov rax, [rbp-%d]\n", offset);
            break;
        }

        case NODE_BIN_OP:
            gen_expr(out, node->data.bin_op.left);
            fprintf(out, "    push rax\n");
            gen_expr(out, node->data.bin_op.right);
            fprintf(out, "    pop rbx\n");
            fprintf(out, "    add rax, rbx\n");
            break;

        default:
            fprintf(stderr, "Unknown expression type!\n");
            exit(1);
    }
}

static void gen_stmt(FILE* out, ASTNode* node) {
    if (!node) {
        fprintf(stderr, "gen_stmt called with NULL node!\n");
        exit(1);
    }
    printf("[gen_stmt] Got stmt type: %d\n", node->type);
    fflush(stdout);
    if (node->type == NODE_VAR_DECL) {
        printf("  name: %s\n", node->data.var_decl.name ? node->data.var_decl.name : "NULL");
        printf("  expr: %p\n", (void*)node->data.var_decl.expr);
        fflush(stdout);
    }

    if (node->type == NODE_VAR_DECL) {
        printf("[gen_stmt] Got stmt type: %d\n", node->type);
        if (node->type == NODE_VAR_DECL) {
            if (!node->data.var_decl.name) {
                fprintf(stderr, "VAR_DECL has NULL name!!\n");
                exit(1);
            }
        }

        add_symbol(node->data.var_decl.name);
        gen_expr(out, node->data.var_decl.expr);
        int offset = get_stack_offset(node->data.var_decl.name);
        fprintf(out, "    mov [rbp-%d], rax\n", offset);
    }
}

void generate_code(ASTNode* ast, const char* output_path) {
    symbol_count = 0;
    current_offset = 0;

    printf("AST body_len: %d\n", ast->data.function.body_len);
    for (int i = 0; i < ast->data.function.body_len; i++) {
        ASTNode* node = ast->data.function.body[i];
        if (node) {
            printf("Node %d type: %d\n", i, node->type);
        } else {
            printf("Node %d is NULL\n", i);
        }
    }

    FILE* out = fopen(output_path, "w");

    if(!out) {
        perror("fopen");
        exit(1);
    }

    fprintf(out,
        "global main\n"
        "extern printf\n"
        "section .data\n"
        "fmt db \"%%d\", 10, 0\n"
        "section .text\n"
        "main:\n"
        "    push rbp\n"
        "    mov rbp, rsp\n"
        "    sub rsp, 1024\n" // reserve stack space\n"
    );

    if(ast->type != NODE_FUNCTION) {
        fprintf(stderr, "Expected function node as root!\n");
        fclose(out);
        exit(1);
    }

    for (int i = 0; i < ast->data.function.body_len; i++) {
        ASTNode* node = ast->data.function.body[i];
        if (node) gen_stmt(out, node);
    }

    // print last variable
    if (symbol_count > 0) {
        int last_offset = symbols[symbol_count - 1].stack_offset;
        fprintf(out,
            "    mov rax, [rbp-%d]\n" , last_offset);
    }

    fprintf(out,
        "    mov rsp, rbp\n"
        "    pop rbp\n"
        "    ret\n");

    fclose(out);
}