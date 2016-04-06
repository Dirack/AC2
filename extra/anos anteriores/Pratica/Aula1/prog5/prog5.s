	.equ STR_MAX_SIZE, 20
	.data
str:	.asciiz "Introduza 2 strings: "
str4:	.asciiz "Resultados:\n"
str1:	.space 21
	.align 2
str2:	.space 21
	.align 2
str3:	.space 41
	.align 2

	.text
	.globl main
main:
	la $a0,str
	li $v0,8
	syscall

	la $a0,str1
	la $a1,STR_MAX_SIZE
	li $v0,9
	syscall

	la $a0,str2
	li $a1,STR_MAX_SIZE
	li $v0,9
	syscall

	la $a0,str4
	li $v0,8
	syscall

	
	
