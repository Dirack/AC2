            .equ SFR_BASE_HI, 0xBF88
            .equ TRISE, 0x6100
            .equ PORTE, 0x6110
            .equ LATE, 0x6120

            .text
            .global main

main:       lui $t1, SFR_BASE_HI
            lw $t2, TRISE($t1)
            andi $t2, $t2, 0xFFFE
            or $t2, $t2, 0x0040
            sw $t2, TRISE($t1)

            li $t5, 0
loop:       lw $t4, PORTE($t1)

            andi $t4, $t4, 0x0040
            srl $t4, $t4, 6

            beq $t4, $t5, loop

            neg $t5, $t4

            or $t4, $t4, $t5

            sw $t4, LATE($t1)

            j loop

            jr $ra
