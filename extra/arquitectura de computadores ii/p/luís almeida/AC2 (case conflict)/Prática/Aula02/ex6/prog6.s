            .data

msg1:       .asciiz "\nInsira uma hora Inicial: "
msg2:       .asciiz "\nHoras: "
msg3:       .asciiz "\nMinutos: "
msg4:       .asciiz "\nSegundos: "

            .text
            .global main

main:       la $a0, msg1        # impressao de msg1
            li $v0, 8
            syscall

horas:      la $a0, msg2        # pedido de horas
            li $v0, 8
            syscall

            li $v0, 5
            syscall

            bgt $v0, 23, horas
            blt $v0, 0, horas

            move $t0, $v0
            
minutos:    la $a0, msg3        # pedido de minutos
            li $v0, 8
            syscall

            li $v0, 5
            syscall

            bgt $v0, 59, minutos
            blt $v0, 0, minutos

            move $t1, $v0

segundos:   la $a0, msg4        # pedido de segundos
            li $v0, 8
            syscall

            li $v0, 5
            syscall

            bgt $v0, 59, segundos
            blt $v0, 0, segundos

            move $t2, $v0

timer:      li $v0, 12
            syscall
whiletimer: li $v0, 11
            syscall

            bgt $v0, 20000000, endwhilet
            j whiletimer

endwhilet:  
            add $t2, $t2, 1

            bne $t2, 60, print
            li $t2, 0
            add $t1, $t1, 1
            bne $t1, 60, print
            li $t1, 0
            add $t0, $t0, 1
            bne $t0, 24, print
            li $t0, 0

print:      la $a0, '\n'
            li $v0, 3
            syscall

            bgt $t0, 9, hform
            la $a0, '0'
            li $v0, 3
            syscall

hform:      move $a0, $t0
            li $v0, 7
            syscall

            la $a0, ':'
            li $v0, 3
            syscall
            
            bgt $t1, 9, mform
            la $a0, '0'
            li $v0, 3
            syscall
            
mform:      move $a0, $t1
            li $v0, 7
            syscall

            la $a0, ':'
            li $v0, 3
            syscall
            
            bgt $t2, 9, sform
            la $a0, '0'
            li $v0, 3
            syscall

sform:      move $a0, $t2
            li $v0, 7
            syscall

            j timer

            jr $ra
