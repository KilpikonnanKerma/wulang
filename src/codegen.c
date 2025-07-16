#include <stdio.h>
#include "codegen.h"

void generate_code(ASTNode* ast, const char* output_path) {
    FILE* out = fopen(output_path, "w");
    fprintf(out, "global main\nsection .text\n\nmain:\n");

    for (int i = 0; i < ast->data.function.body_len; i++) {
        ASTNode* node = ast->data.function.body[i];
        if(!node) continue;
        if(node->type == NODE_VAR_DECL) {
            fprintf(out, "    mov rax, %d\n", node->data.var_decl.value);
        }
    }

    fprintf(out, "    ret\n");
    fclose(out);
}