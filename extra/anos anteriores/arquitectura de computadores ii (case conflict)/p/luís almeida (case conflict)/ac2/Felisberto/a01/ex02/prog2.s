	.data
msg:	.asciiz "Key pressed\n"
	.text

	.globl main
main:

while:
	li	$v0,1
	syscall
	move	$t0,$v0
	bne	$t0,$0,endwhile
	j 	while

endwhile:
	
	beq	$t0,'\n',end
	la 	$a0,msg
	li	$v0,8
	syscall
	j while
	
end:	
	li 	$v0,0
	jr	$ra


