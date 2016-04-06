            .data

msg1:       .asciiz "\nIntroduza um numero (sinal e módulo): "
msg2:       .asciiz "\nValor lido em base 2: "
msg3:       .asciiz "\nValor lido em base 16: "
msg4:       .asciiz "\nValor lido em base 10 (unsigned): "
msg5:       .asciiz "\nValor lido em base 2 (signed): "

            .text
            .global main

main:
while:      la $a0, msg1      # printStr(msg1)
            li $v0, 8
            syscall

            li $v0, 5         # readInt10()
            syscall

            move $t0, $v0       # para guardar o valor lido em $t0

            la $a0, msg2      # printStr(msg2)
            li $v0, 8
            syscall

            move $a0, $t0       # printInt($t0, 2)
            li $a1, 2
            li $v0, 6
            syscall

            la $a0, msg3      # printStr(msg3)
            li $v0, 8
            syscall

            move $a0, $t0       # printInt($t0, 16)
            li $a1, 16
            li $v0, 6
            syscall

            la $a0, msg4      # printStr(msg4)
            li $v0, 8
            syscall

            move $a0, $t0       # printInt($t0, 10)
            li $a1, 10
            li $v0, 6
            syscall

            la $a0, msg5      # printStr(msg5)
            li $v0, 8
            syscall

            move $a0, $t0       # printInt10($y0)
            li $v0, 7
            syscall

            j while

            jr $ra
