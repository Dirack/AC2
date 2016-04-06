			.equ SFR_BASE_HI, 0xBF88
            .equ TRISE, 0x6100
            .equ PORTE, 0x6110
            .equ LATE, 0x6120
            
            .data
msg:        .asciiz "\r\n"

            .text
            .global main

main:       subu $sp, $sp, 4
            sw $ra, 0($sp)

while:      li $v0, 12
            syscall
            
            li $a0, 1
            jal delay

            li $v0, 11
            syscall
            
            move $a0, $v0
            li $a1, 10
            li $v0, 6
            syscall

            la $a0, msg
            li $v0, 8
            syscall

            j while

            lw $ra, 0($sp)
            addu $sp, $sp, 4

            jr $ra

###################################
# delay                           #
###################################

# para 6000 deu 12068
# para 12000 deu 24068
# a recta obtida foi y = m * x + b
# onde m = 2, b = 28, logo como queremos y = 20000
# resolvendo vem x = 9966

delay:      li $t1, 9966        # Constante_Delay
for:        beq $a0, 0, end
for2:       beq $t1, 0, endfor2
            add $t1, $t1, -1
            j for2
endfor2:    add $a0, $a0, -1
            j for
end:        jr $ra
