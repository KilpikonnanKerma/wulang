/*

    Copyright (C) 2025 Nico Rajala. All rights reserved.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

char* preprocess_includes(const char* filename);

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <source.wu>\n", argv[0]);
        return 2;
    }

    const char* source_path = argv[1];
    const char* output_path = "out.asm";

    if(argc >= 4 && strcmp(argv[2], "-o") == 0) {
        output_path = argv[3];
    }

    char* source = preprocess_includes(source_path);
    if(!source) {
        fprintf(stderr, "Failed to read source file: %s\n", source_path);
        return 1;
    }

    printf("Tokenizing...\n");
    Token* tokens = tokenize(source);

    printf("Parsing...\n");
    ASTNode* ast = parse(tokens);

    printf("Generating code...\n");
    fflush(stdout);
    generate_code(ast, output_path);

    printf("Assembly written to %s\n", output_path);

    free(source);
    return 0;
}

char* preprocess_includes(const char* filename) {
    FILE* file = fopen(filename, "r");
    if(!file) return NULL;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);

    const char* include_directive = "#include \"";
    char* result = malloc(65536); // Arbitrary large size for simplicity
    result[0] = '\0';

    char* line = strtok(buffer, "\n");
    while(line) {
        if(strncmp(line, include_directive, strlen(include_directive)) == 0) {
            char* start = line + strlen(include_directive);
            char* end = strchr(start, '\"');
            if (end) {
                char include_file[256];
                strncpy(include_file, start, end - start);
                include_file[end - start] = '\0';

                char* included = preprocess_includes(include_file);
                if (included) {
                    strcat(result, included);
                    free(included);
                }
            }
        } else {
            strcat(result, line);
            strcat(result, "\n");
        }
        line = strtok(NULL, "\n");
    }

    free(buffer);
    return result;
}