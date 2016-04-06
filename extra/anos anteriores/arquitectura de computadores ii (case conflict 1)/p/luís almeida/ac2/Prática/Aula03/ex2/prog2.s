            .equ SFR_BASE_HI, 0xBF88
            .equ TRISE, 0x6100
            .equ PORTE, 0x6110
            .equ LATE, 0x6120

            .data
msg:        .asciiz "teste"

            .text
            .global main

main:       # Definicao de portas
            lui $t0, SFR_BASE_HI
            lw $t1, TRISE($t0)
            andi $t1, $t1, 0xFFF0
            ori $t1, $t1, 0x00C0
            sw $t1, TRISE($t0)

            # reset do core timer
            li $v0, 12
            syscall
            
            li $t2, 0               # reserva do valor do botao RE6
            li $t3, 0
            li $t4, 0
loop:       lw $t1, PORTE($t0)
            srl $t1, $t1, 6
            andi $t1, $t1, 0x01

            beq $t1, $t2, cont
            move $t2, $t1
            li $t1, 0
            sw $t1, LATE($t0)
            j loop

cont:       beq $t1, 1, johnson
binary:     lw $t1, PORTE($t0)
            srl $t1, $t1, 7
            andi $t1, $t1, 0x01
            
            li $v0, 11
            syscall

            blt $v0, 5000000, loop
            li $v0, 12
            syscall
            
            beq $t1, 1, bindesc

            lw $t1, LATE($t0)
            add $t1, $t1, 1
            bne $t1, 16, savebin
            li $t1, 0
savebin:    sw $t1, LATE($t0)
            j loop

bindesc:    lw $t1, LATE($t0)
            add $t1, $t1, -1
            bgt $t1, 0, savebind
            li $t1, 15
savebind:   sw $t1, LATE($t0)
            j loop

johnson:    lw $t1, PORTE($t0)
            srl $t1, $t1, 7
            andi $t1, $t1, 0x01
            
            li $v0, 11
            syscall

            blt $v0, 5000000, loop
            li $v0, 12
            syscall
            
            beq $t1, 1, johndesc

            beq $t3, 1, john12
            lw $t1, LATE($t0)
            sll $t1, $t1, 1
            add $t1, $t1, 1
            blt $t1, 15, savejohn
            li $t1, 15
            li $t3, 1
            j savejohn

john12:     lw $t1, LATE($t0)
            srl $t5, $t1, 1
            sub $t1, $t1, $t5
            add $t1, $t1, -1
            bgt $t1, 0, savejohn
            li $t1, 0
            li $t3, 0
savejohn:   sw $t1, LATE($t0)
            j loop

johndesc:   beq $t4, 1, john22
            lw $t1, LATE($t0)
            srl $t5, $t1, 1
            sub $t1, $t1, $t5
            add $t1, $t1, -1
            bgt $t1, 0, savejohnd
            li $t1, 0
            li $t4, 1
            j savejohnd
            
john22:     lw $t1, LATE($t0)
            sll $t1, $t1, 1
            add $t1, $t1, 1
            blt $t1, 15, savejohnd
            li $t1, 15
            li $t4, 0
savejohnd:  sw $t1, LATE($t0)
            j loop
