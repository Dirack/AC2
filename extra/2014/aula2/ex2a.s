		.data
		.text
		.globl main
main:	
		li $v0,12
		syscall
while:
		li $v0,11
		syscall
		
		move $t0,$v0
		move $a0,$t0
		li $v0,7
		syscall
		
		li $a0,'\n'
		li $v0,3
		syscall

		
		j while

		jr $ra
