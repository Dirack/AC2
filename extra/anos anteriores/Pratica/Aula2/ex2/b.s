	.text
	.globl main

main:
	
	li $t1,1
while:	
	li $v0,11
	syscall
	move $t0,$v0
	bge $t0,20000000,endWhile
	
	j while
endWhile:
		
	move $a0,$t1
	li $v0,7
	syscall
	
	addi $t1,$t1,1

	li $v0,12
	syscall
	j while

	jr $ra

