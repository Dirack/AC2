            .data

            .text
            .global main

main:       
while:      li $v0, 2           # getChar()
            syscall

if:         beq $v0, '\n', end  # condição do if
            
            move $a0, $v0       # putChar(c)
            li $v0, 3
            syscall

            j while

end:        li $v0, 1           # return 1
            jr $ra
