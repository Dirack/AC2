	.data
msg1:	.asciiz "\nIntroduza um numero (sinal e módulo): "
msg2:	.asciiz "\nValor lido em base 2: "
msg3:	.asciiz "\nValor lido em base 16: "
msg4:	.asciiz "\nValor lido em base 10 (unsigned): "
msg5:	.asciiz "\nValor lido em base 10:(signed) "
	.text
	
	.globl main
main:
begin:
	la	$a0,msg1
	li	$v0,8
	syscall

	li	$v0,5
	syscall
	move 	$t0,$v0
##	
	la	$a0,msg2
	li	$v0,8
	syscall

	move	$a0,$t0
	li 	$a1,2
	li 	$v0,6
	syscall
##
	la	$a0,msg3
	li	$v0,8
	syscall

	move	$a0,$t0
	li 	$a1,16
	li 	$v0,6
	syscall
##
	la	$a0,msg4
	li	$v0,8
	syscall

	move	$a0,$t0
	li 	$a1,10
	li 	$v0,6
	syscall
##
	la	$a0,msg5
	li	$v0,8
	syscall

	move	$a0,$t0
	li 	$a1,10
	li 	$v0,7
	syscall

	j	begin
	li 	$v0,0
	jr	$ra
