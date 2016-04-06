		.data
		.text
		.globl main
main:	li $t0,1
while1:	bne $t0,1,end1
		li $v0,2
		syscall
		
if:		beq $v0,'\n',break
		move $a0,$v0
		li $v0,3
		syscall
break:	j while1
end1:	li $v0,0
		jr $ra
