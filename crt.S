.intel_syntax noprefix
.text
.global _start, read, write, getrandom

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

read:
    xor eax, eax
    syscall
    ret
write:
    xor eax, eax
    inc eax
    syscall
    ret
getrandom:
    mov eax, 318
    syscall
    ret
