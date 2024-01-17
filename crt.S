.intel_syntax noprefix
.text
.global read, write, getrandom

/* "--entry=main" is passed to the linker in build.sh, making main the entry
 * point instead of _start. The normal startup code is unnecessary:
 *
 * - main() runs in an infinite loop and never exits, no need to push return
 *   value onto the stack or call _exit
 *
 * - aligning the stack pointer (%rsp) is unnecessary on x86_64,
 *   the x86_64 SysV ABI guarentees it is already aligned by the OS at startup.
 *
 * - main() uses no arguments here, so no need to pass them
 */

read:
    xor eax, eax
    syscall
    ret
write:
    mov eax, 1
    syscall
    ret
getrandom:
    mov eax, 318
    syscall
    ret
