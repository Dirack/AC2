		.data
str1:	.asciiz "Horas: \n"
str2:	.asciiz "\nMinutos: \n"
str3:	.asciiz "\nSegundos: \n"
		.text
		.globl main
main:	
		li $v0,12
		syscall
		
horas:	la $a0,str1
		li $v0,8
		syscall
		li $v0,5
		syscall
		bgt $v0,23,horas
		blt $v0,0,horas
		move $t3,$v0 
		
		
minutos:
		la $a0,str2
		li $v0,8
		syscall
		li $v0,5
		syscall
		bgt $v0,59,minutos
		blt $v0,0,minutos
		move $t4,$v0 
		
segundos:la $a0,str3
		li $v0,8
		syscall
		li $v0,5
		syscall
		bgt $v0,59,segundos
		blt $v0,0,segundos
		move $t5,$v0 
		
		
while:
		li $v0,11
		syscall
		
		move $t0,$v0	
		blt $t0,20000000,while
		
		li $v0,12
		syscall
	
		add $t5,$t5,1
		
		li $a0,'\n'
		li $v0,3
		syscall
		
		bge $t5,60,pmin
		move $a0,$t3
		li $v0,7
		syscall
		li $a0,':'
		li $v0,3
		syscall
		move $a0,$t4
		li $v0,7
		syscall
		li $a0,':'
		li $v0,3
		syscall
		move $a0,$t5
		li $v0,7
		syscall
		
		j while
	
pmin:	
		li $t5,0
		add $t4,$t4,1
		
		bge $t4,60,phor
		
		move $a0,$t3
		li $v0,7
		syscall
		li $a0,':'
		li $v0,3
		syscall
		move $a0,$t4
		li $v0,7
		syscall
		li $a0,':'
		li $v0,3
		syscall
		move $a0,$t5
		li $v0,7
		syscall
		
		j while
		
phor:	li $t4,0
		add $t3,$t3,1
		
			move $a0,$t3
		li $v0,7
		syscall
		li $a0,':'
		li $v0,3
		syscall
		move $a0,$t4
		li $v0,7
		syscall
		li $a0,':'
		li $v0,3
		syscall
		move $a0,$t5
		li $v0,7
		syscall
		
		j while
		
		
		jr $ra
