            .equ SFR_BASE_HI, 0xBF88
            .equ TRISE, 0x6100
            .equ PORTE, 0x6110
            .equ LATE, 0x6120

            .text
            .global main

main:       lui $t1, SFR_BASE_HI
            lw $t2, TRISE($t1)
            andi $t2, $t2, 0xFFF0    #RE0, RE1, RE2, RE3 como saida 0x1111 1111 1111 0000
            ori $t2, $t2, 0x00C0      #RE6, RE7 como entrada 0x0000 0000 1100 0000
            sw $t2, TRISE($t1)

loop:       lw $t2, PORTE($t1)

            andi $t3, $t2, 0x40
            srl $t3, $t3, 6         # para obter o RE6
            andi $t4, $t2, 0x80
            srl $t4, $t4, 7         # para obter o RE7

            li $t7, 0
            
            # RE3 = ~(RE6 | RE7)
            nor $t5, $t3, $t4
            or $t7, $t7,$t5
            sll $t7, $t7, 1         # para passar para o bit seguinte
            
            # RE2 = RE6 ^ RE7 (xor)
            xor $t5, $t3, $t4
            or $t7, $t7, $t5
            sll $t7, $t7, 1         # para passar para o bit seguinte
            
            # RE1 = RE6 | RE7 (or)
            or $t5, $t3, $t4
            or $t7, $t7, $t5
            sll $t7, $t7, 1         # para passar para o bit seguinte

            # RE0 = RE6 & RE7
            and $t5, $t3, $t4   
            or $t7, $t7, $t5
            
            lw $t6, LATE($t1)       # para imprimir os resultados nos leds
            andi $t6, $t6, 0xFFF0
            or $t6, $t6, $t7
            sw $t6, LATE($t1)
            
            j loop

            jr $ra
