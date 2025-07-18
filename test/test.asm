global main
extern printf
section .data
fmt db "%d", 10, 0
section .text
main:
    push rbp
    mov rbp, rsp
    sub rsp, 1024
    mov rax, 5
    mov [rbp-8], rax
    mov rax, [rbp-8]
    push rax
    mov rax, 7
    pop rbx
    add rax, rbx
    mov [rbp-16], rax
    mov rax, [rbp-16]
    mov rsp, rbp
    pop rbp
    ret
