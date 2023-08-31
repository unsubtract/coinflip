.intel_syntax noprefix
.text
.global _start, syscall3

_start:
    xor ebp, ebp
    /* program doesn't actually use arguments */
    //pop rdi
    //mov rsi, rsp
    and rsp, -16
    //call main
    jmp main

    /* program never actually exits */
    //mov edi, eax
    //mov eax, 60
    //syscall

syscall3:
    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    syscall
    ret
