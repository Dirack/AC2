	.text
	.globl main

main:
	li $t0,1

while:
	bne $t0,1,end
	
	li $v0,2
	syscall

if:
	beq $v0,'\n',end
	move $a0,$v0
	li $v0,3
	syscall
	
	j while
		
end:
	li $v0,1
	jr $ra

