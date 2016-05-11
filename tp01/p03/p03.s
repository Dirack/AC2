	.equ	GET_CHAR,2
	.equ	PUT_CHAR,3
	.text
	.globl main
main:
while:
	ori	$v0,$0,GET_CHAR
	syscall
	beq	$v0,'\n',exit
	or	$a0,$0,$v0	
	ori	$v0,$0,PUT_CHAR	
	syscall
	b	while

exit:
	ori	$v0,$0,1
	jr	$ra
