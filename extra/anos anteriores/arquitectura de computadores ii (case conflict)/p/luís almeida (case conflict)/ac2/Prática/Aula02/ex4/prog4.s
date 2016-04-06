            .data

msg:       .asciiz "\nCore Timmer: "

            .text
            .global main

main:       
while:      la $a0, msg
            li $v0, 8
            syscall

if:         li $v0, 1
            syscall

            beq $v0, $0, end
            li $v0, 12
            syscall

end:        li $v0, 11
            syscall
         
            move $a0, $v0
            li $v0, 7
            syscall

            j while

            jr $ra
