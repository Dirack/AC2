	.equ	READ_INT10,5
	.equ	PRINT_INT,6
	.equ	PRINT_INT10,7
	.equ	PRINT_STR,8
	.data
s_msg:	.asciiz	"\nIntroduza um numero (sinal e módulo): "
s_msg1:	.asciiz	"\nValor lido em base 2: "
s_msg2:	.asciiz	"\nValor lido em base 16: "
s_msg3:	.asciiz	"\nValor lido em base 10 (unsigned): "
s_msg4:	.asciiz "\nValor lido em base 10 (signed): "
	.text
	.globl main

main:
while:	la 	$a0,s_msg
	ori 	$v0,$0,PRINT_STR
	syscall
	ori 	$v0,$0,READ_INT10
	syscall
	move	$t0,$v0
	
	la 	$a0,s_msg1
	ori 	$v0,$0,PRINT_STR
	syscall
	move	$a0,$t0
	ori	$v0,$0,PRINT_INT
	ori	$a1,$0,2
	syscall
	
	la 	$a0,s_msg2
	ori 	$v0,$0,PRINT_STR
	syscall
	move	$a0,$t0
	ori	$v0,$0,PRINT_INT
	ori	$a1,$0,16
	syscall
	
	la 	$a0,s_msg3
	ori 	$v0,$0,PRINT_STR
	syscall
	move	$a0,$t0
	ori	$v0,$0,PRINT_INT
	ori	$a1,$0,10
	syscall
	
	la 	$a0,s_msg4
	ori 	$v0,$0,PRINT_STR
	syscall
	move	$a0,$t0
	ori	$v0,$0,PRINT_INT10
	syscall
	
	b while
