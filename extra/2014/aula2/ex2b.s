		.data
		.text
		.globl main
main:	
		li $t1,0
		li $v0,12
		syscall
while:
		li $v0,11
		syscall
		
		move $t0,$v0	
		mul $t2,$t1,20000000
		bge $t0,$t2,varia
		
		j while

varia:	
		move $a0,$t1
		li $v0,7
		syscall
		add $t1,$t1,1
		
		li $a0,'\n'
		li $v0,3
		syscall
		
		j while
		
		jr $ra
