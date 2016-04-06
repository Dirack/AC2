            .data
            
            .align 2
array:      .space 20

msg1:       .asciiz "\nLeitura e ordenacao de inteiros em base 10\n"
msg2:       .asciiz "Introduza 5 inteiros: "
msg3:       .asciiz "\nNumero de trocas realizado: "
msg4:       .asciiz "\nResultado da ordenacao: "

            .text
            .global main

main:       subu $sp, $sp, 8
            sw $ra, 0($sp)
            sw $s0, 4($sp)

            la $a0, msg1          # printStr(msg1)
            li $v0, 8
            syscall

            la $a0, msg2          # printStr(msg2)
            li $v0, 8
            syscall

            la $t0, array         # $t0 = array
            li $t1, 0             # i = 0

forread:    beq $t1, 5, endforread
            li $v0, 5             # readint10()
            syscall

            sll $t2, $t1, 2       # i * 4
            addu $t2, $t2, $t0    # $t2 = array[i]
            sw $v0, 0($t2)        # array[i] = readint10()
            addi $t1, $t1, 1      # i++

            la $a0, ' '           # putChar(' ')
            li $v0, 3
            syscall

            j forread
            
endforread: la $s0, array

            la $a0, msg3          # printStr(msg3)
            li $v0, 8
            syscall

            li $a0, 5             # sequentialSort(N_INT, lista)
            move $a1, $s0
            jal seqSort
            
            move $a0, $v0         # printInt(n_trocas, 10)
            li $a1, 10
            li $v0, 6
            syscall

            la $a0, msg4          # printStr(msg4)
            li $v0, 8
            syscall

            li $t0, 0             # i = 0
            move $t2, $s0

forordem:   beq $t0, 5, endforordem
            sll $t1, $t0, 2       # i * 4
            addu $t1, $t1, $t2
            
            lw $a0, 0($t1)        # printInt(*ptr, 10)
            li $a1, 10
            li $v0, 6
            syscall
            
            la $a0, ' '           # putChar(' ')
            li $v0, 3
            syscall

            addi $t0, $t0, 1      # i++

            j forordem

endforordem:
             
            li $v0, 0             # return 0

            lw $ra, 0($sp)
            lw $s0, 4($sp)
            addu $sp, $sp, 8
            jr $ra

#----------------------------------------------#
# sequentialSort(unsg int val, unsg int array) #
#----------------------------------------------#

seqSort:    subu $sp, $sp, 24
            sw $ra, 0($sp)
            sw $s0, 4($sp)
            sw $s1, 8($sp)
            sw $s2, 12($sp)
            sw $s3, 16($sp)
            sw $s4, 20($sp)
            
            move $s0, $a0         # $s0 = 5 (N_INT)
            move $s1, $a1         # $s1 = array
            
            li $s2, 0             # n_trocas = 0
            li $s3, 0             # i = 0

forSort1:   subu $t0, $s0, 1
            beq $s3, $t0, endforSort1
            
            addu $s4, $s3, 1       # j = i + 1
forSort2:   beq $s4, $s0, endforSort2
            
            sll $t1, $s3, 2       # i * 4
            sll $t2, $s4, 2       # j * 4
            addu $t1, $t1, $s1    # $t1 = array[]
            addu $t2, $t2, $s1    # $t2 = array[]

            lw $t3, 0($t1)        # $t1 = array[i]
            lw $t4, 0($t2)        # $t2 = array[j]

ifSort:     ble $t3, $t4, endifSort
            
            add $s2, $s2, 1       # n_trocas++
            
            move $a0, $t1
            move $a1, $t2
            jal troca
endifSort:

            addu $s4, $s4, 1
            j forSort2

endforSort2:
            
            addu $s3, $s3, 1
            j forSort1
  
endforSort1:

            move $v0, $s2           # return n_trocas

            lw $ra, 0($sp)
            lw $s0, 4($sp)
            lw $s1, 8($sp)
            lw $s2, 12($sp)
            lw $s3, 16($sp)
            lw $s4, 20($sp)
            addu $sp, $sp, 24

            jr $ra

#----------------------------------------------#
# troca(unsg int v1, unsg int v2)              #
#----------------------------------------------#

troca:      lw $t0, 0($a0)
            lw $t1, 0($a1)
            sw $t1, 0($a0)
            sw $t0, 0($a1)
            jr $ra
