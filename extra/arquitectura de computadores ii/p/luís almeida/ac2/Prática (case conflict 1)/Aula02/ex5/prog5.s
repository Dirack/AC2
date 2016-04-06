            .data

msg:        .asciiz "\nValor da variavel: "

            .text
            .global main

main:       li $t1, 0
while:      la $a0, msg
            li $v0, 8
            syscall

            move $a0, $t1
            li $v0, 7
            syscall
            
            li $v0, 12
            syscall
while2:     li $v0, 11
            syscall

            bge $v0, 20000000, end2
            j while2

end2:       add $t1, $t1, 1
            j while

            jr $ra
