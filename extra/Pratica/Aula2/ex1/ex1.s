	.data
str1:	.asciiz "\nLeitura e ordenacao de inteiros em base 10\n"
str2:	.asciiz "\Introduza 5 inteiros: "
str3: 	.asciiz "\nNumero de trocas realizado: "
str4:	.asciiz "\nResultado da ordenacao: "
	.align 2
lista:	.space 20


	.text
	.globl main

main:
	subu $sp,$sp,4
	sw $ra,0($sp)
	
	la $a0,str1
	li $v0,8
	syscall		#printStr("\nLeitura e ordenacao de inteiros em base 10\n");

	la $a0,str2
	li $v0,8
	syscall		#printStr("Introduza 5 Inteiros: ");

	li $t0,0
forR:	
	bge $t0,5,endforR
	li $v0,5
	syscall
	la $a0,lista
	sll $t1,$t0,2
	add $a0,$a0,$t1
	sw $v0,0($a0)
	add $t0,$t0,1
	
	li $a0,' '
	li $v0,3
	syscall

	j forR
endforR:	
	li $a0,5
	la $a1,lista

	jal sequentialSort	#n_trocas = sequentialSort( N_INT, lista );	
	move $t0,$v0
	la $a0,str3
	li $v0,8
	syscall 	#printStr("\nNumero de trocas realizado: ");
	
	move $a0,$t0
	li $a1,10
	li $v0,6
	syscall

	la $a0,str4	#printStr("\nResultado da ordenacao: ");
	li $v0,8
	syscall

	la $t1,lista 	#$t1 = lista
	add $t2, $t1,20
forW:	
	bge $t1,$t2,endforW
	lw $a0,0($t1)
	li $a1,10
	li $v0,6
	syscall
	
	li $a0,' '
	li $v0,3
	syscall
	
	addi $t1,$t1,4
	j forW	

endforW:
	lw $ra,0($sp)
	addu $sp,$sp,4

	jr $ra
	


sequentialSort:
	subu $sp,$sp,24
	sw $ra,0($sp)
	sw $s0,4($sp)
	sw $s1,8($sp)
	sw $s2,12($sp)
	sw $s3,16($sp)
	sw $s4,20($sp)
	
	li $s4,0 	#n_trocas = 0
	move $s0,$a0 	#s0 = n_val
	move $s1,$a1 	#s1 = array

	li $s2,0 	#i = 0
forS1:
	sub $t0, $s0,1 	#t0 = n_val -1
	bge $s2, $t0,endforS1
	
	addi $s3,$s2,1 	#j = i+1

forS2:
	bge $s3,$s0,endforS2

	sll $t0,$s2,2	#$t0 = i*4
	add $a0,$t0,$s1	#$a0 = &array[i]
	sll $t0,$s3,2	#$t0 = j*4
	add $a1,$t0,$s1
	
	lw $t0,0($a0)	#t0 = array[i]
	lw $t1,0($a1)	#t1 = array[j]

if:
	ble $t0,$t1,endif
	
	jal troca
	addi $s4,$s4,1 	#n_trocas++

endif:
	addi $s3,$s3,1	#j++
	j forS2

endforS2:
	addi $s2,$s2,1	#i++
	j forS1

endforS1:
	move $v0,$s4

	lw $ra,0($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	lw $s3,16($sp)
	lw $s4,20($sp)
	addu $sp,$sp,24
	
	jr $ra
troca:	
	lw $t0,0($a0)
	lw $t1,0($a1)
	sw $t1,0($a0)
	sw $t0,0($a1)	

	jr $ra
