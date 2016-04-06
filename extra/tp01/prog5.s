		.data
frase1:	.asciiz "Introduza 2 strings: "
frase2:	.asciiz "Resultados:\n"
str1:	.space 21
str2: 	.space 21
str3:	.space 41
		.text
		.globl main
main:	
		subiu $sp,$sp,4
		sw $ra,0($sp)
		
		la $a0,frase1
		li $v0,8
		syscall
		
		la $a0,str1
		li $a1,20
		li $v0,9
		syscall
		
		la $a0,str2
		li $a1,20
		li $v0,9
		syscall
		
		la $a0,frase2
		li $v0,8
		syscall
		
		la $a0,str1
		li $a1,10
		li $v0,6
		syscall
		
		la $a0,str2
		li $a1,10
		li $v0,6
		syscall
		
		la $a0,str3
		la $a1,str1
		jal strcpy
		
		la $a0,str3
		la $a1,str2
		jal strcat
		
