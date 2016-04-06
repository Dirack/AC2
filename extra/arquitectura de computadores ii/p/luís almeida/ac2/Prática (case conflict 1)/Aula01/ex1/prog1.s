        .equ PRINT_STR, 8

        .data
        .global msg

msg:    .asciiz "AC2 - DETPIC32 primer\n"

        .text
        .global main

main:   la $a0, msg
        li $v0, PRINT_STR
        syscall

        li $v0, 0
        jr $ra
