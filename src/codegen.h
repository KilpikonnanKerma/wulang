#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

void generate_code(ASTNode* ast, const char* output_path);

#endif