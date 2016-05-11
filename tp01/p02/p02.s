	.equ	IN_KEY,1
	.equ	PRINT_STR,8
	.data
s_msg:	.asciiz	"Key pressed\n"

	.globl main
	.text
main:
while:	
loop_while:
	ori	$v0,$0,IN_KEY
	syscall
	beqz	$v0,loop_while
	beq	$v0,'\n',exit
	ori	$v0,$0,PRINT_STR
	la	$a0,s_msg
	syscall
	b while
	
exit:
	ori	$v0,$0,0
	jr	$ra
