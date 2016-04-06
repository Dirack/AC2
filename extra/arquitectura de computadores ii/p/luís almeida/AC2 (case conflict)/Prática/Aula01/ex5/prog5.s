            .data

str1:       .space 20
str2:       .space 20
str3:       .space 40

msg1:       .asciiz "Introduza 2 strings: "
msg2:       .asciiz "\n\nResultados:"
msg3:       .asciiz "\nString 1 lenght: "
msg4:       .asciiz "\nString 2 lenght: "
msg5:       .asciiz "\nString Concatenate: "
msg6:       .asciiz "\nString Compare: "
msg7:       .asciiz "\nString 1: "
msg8:       .asciiz "\nString 2: "

msg9:       .asciiz "string S1 is less than string S2"
msg10:      .asciiz "string S1 is equal to string S2"
msg11:      .asciiz "string S1 is greater than string S2"

            .text
            .global main

main:       subu $sp, $sp, 12
            sw $ra, 0($sp)
            sw $s0, 4($sp)
            sw $s1, 8($sp)

            la $a0, msg1          # printStr(msg1)
            li $v0, 8
            syscall

            la $a0, msg7          # printStr(msg7)
            li $v0, 8
            syscall
            
            la $a0, str1          # readStr(str1, 21)
            li $a1, 21
            li $v0, 9
            syscall

            move $s0, $a0

            la $a0, msg8          # printStr(msg8)
            li $v0, 8
            syscall

            la $a0, str2          # readStr(str2, 21)
            li $a1, 21
            li $v0, 9
            syscall

            move $s1, $a0

            la $a0, msg2          # printStr(msg2)
            li $v0, 8
            syscall

            la $a0, msg3          # printStr(msg3)
            li $v0, 8
            syscall

            la $a0, str1          # strlen(str1)
            jal strlen
          
            move $a0, $v0         # printInt(strlen(str1), 10)
            li $a1, 10
            li $v0, 6
            syscall
            
            la $a0, msg4          # printStr(msg4)
            li $v0, 8
            syscall

            la $a0, str2          # strlen(str2)
            jal strlen
            
            move $a0, $v0         # printInt(strlen(str2), 10)
            li $a1, 10
            li $v0, 6
            syscall

            la $a0, msg5          # printStr(msg5)
            li $v0, 8
            syscall
            
            la $a0, str3          # strcpy(str3, str1)
            move $a1, $s0
            jal strcpy

            move $a0, $v0         # strcat(str3, str2)
            move $a1, $s1
            jal strcat

            move $a0, $v0         # printStr(strcat(str3, str2))
            li $v0, 8
            syscall

            la $a0, msg6          # printStr(msg6)
            li $v0, 8
            syscall

            move $a0, $s0
            move $a1, $s1
            jal strcmp

            beq $v0, 0, endf
            bgt $v0, 0, ends
            
            la $a0, msg9
            li $v0, 8
            syscall

            j end

endf:       la $a0, msg10
            li $v0, 8
            syscall

            j end

ends:       la $a0, msg11
            li $v0, 8
            syscall

end:        lw $ra, 0($sp)
            lw $s0, 4($sp)
            lw $s1, 8($sp)
            addu $sp, $sp, 12

            li $v0, 0
            jr $ra
# ------------------------------- #
# strlen(char *s)                 #
# ------------------------------- #
strlen:
            li $v0, 0             # len = 0
forlen:     lb $t0, 0($a0)
            beq $t0, 0, endlen
            addu $a0, $a0, 1
            addu $v0, $v0, 1
            j forlen

endlen:     jr $ra
# ------------------------------- #
# strcat(char *dst, char *src)    #
# ------------------------------- #
strcat:     subu $sp, $sp, 12
            sw $ra, 0($sp)
            sw $s0, 4($sp)
            sw $s1, 8($sp)

            move $s0, $a0
            move $s1, $a1

forcat:     lb $t0, 0($a0)
            beq $t0, 0, endcat
            
            move $a1, $s1
            jal strcpy
            addu $a0, $a0, 1
            j forcat

endcat:     move $v0, $s0

            lw $ra, 0($sp)
            lw $s0, 4($sp)
            lw $s1, 8($sp)
            addu $sp, $sp, 12
            jr $ra
# ------------------------------- #
# strcpy(char *dst, char *src)    #
# ------------------------------- #
strcpy:     subu $sp, $sp, 8
            sw $ra, 0($sp)
            sw $s0, 4($sp)
            
            move $s0, $a0         # $s0 = $a0

            lb $t0, 0($a1)
            sb $t0, 0($a0)

ifcpy:      beq $t0, 0, endcpy
            addu $a0, $a0, 1
            addu $a1, $a1, 1

            jal strcpy

endcpy:     move $v0, $s0

            lw $ra, 0($sp)
            lw $s0, 4($sp)
            addu $sp, $sp, 8
            jr $ra
# ------------------------------- #
# strcmp(char *s1, char *s2)      #
# nao percebi o que era pedido    #
# fiz de força a determinar o     #
# tamanho da string               #
# ------------------------------- #
strcmp:     subu $sp, $sp, 16
            sw $ra, 0($sp)
            sw $s0, 4($sp)
            sw $s1, 8($sp)
            sw $s2, 12($sp)

            move $s0, $a0
            move $s1, $a1

            li $s2, 0
            move $a0, $s0
            jal strlen
            
            add $s2, $s2, $v0

            move $a0, $s1
            jal strlen

            subu $s2, $s2, $v0

            move $v0, $s2

            lw $ra, 0($sp)
            lw $s0, 4($sp)
            lw $s1, 8($sp)
            lw $s2, 12($sp)
            addu $sp, $sp, 16

endcmp:     jr $ra
