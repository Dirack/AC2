# inkey()

            .data

msg:        .asciiz "Key pressed\n"
            
            .text
            .global main

main:
while:      li $v0, 1           # leitura de inkey()
            syscall

            beq $v0, $0, while  # condição de while

            la $a0, msg         # printStr("Key Pressed\n")
            li $v0, 8
            syscall

            j while             # continuação do loop

            li $v0, 0
            jr $ra
