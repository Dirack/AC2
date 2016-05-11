	.equ	PRINT_INT,6
	.equ	PRINT_INT10,7
	.equ	PRINT_STR,8
	.equ	READ_STR,9
	.equ	STR_MAX_SIZE,20
	.equ	STR_MAX_SIZE_1,21
	.equ	STR_MAX_SIZE_2,41

	.data
str1:	.byte	0:STR_MAX_SIZE_1
str2:	.byte	0:STR_MAX_SIZE_1
str3:	.byte	0:STR_MAX_SIZE_2
prompt1:.asciiz	"Introduza 2 Strings: "
msg1:	.asciiz	"Resultados:\n"
msg2:	.asciiz	"String1-> "
msg3:	.asciiz	"String2-> "
msg4:	.asciiz	"String1 length: "
msg5:	.asciiz	"String2 length: "
msg6:	.asciiz	"Strings concatenated: "
msg7:	.asciiz	"Strings compare: "

	.text
	.globl main
main:
	addiu	$sp,$sp,-4
	sw	$ra,0($sp)
	la	$a0,prompt1
	ori	$v0,$0,PRINT_STR
	syscall
	ori	$a0,$0,'\n'
	ori	$v0,$0,3
	syscall
	la	$a0,msg2
	ori	$v0,$0,PRINT_STR
	syscall
	la	$a0,str1
	ori	$a1,$0,STR_MAX_SIZE
	ori	$v0,$0,READ_STR
	syscall
	ori	$a0,$0,'\n'
	ori	$v0,$0,3
	syscall
	la	$a0,msg3
	ori	$v0,$0,PRINT_STR
	syscall
	la	$a0,str2
	ori	$a1,$0,STR_MAX_SIZE
	ori	$v0,$0,READ_STR
	syscall

	ori	$a0,$0,'\n'
	ori	$v0,$0,3
	syscall
	la	$a0,msg1
	ori	$v0,$0,PRINT_STR
	syscall

	la	$a0,msg4
	ori	$v0,$0,PRINT_STR
	syscall

	la	$t0,str1
	jal	strlen
	
	move	$a0,$v0
	ori	$a1,$0,10
	ori	$v0,$zero,PRINT_INT
	syscall
	ori	$a0,$0,'\n'
	ori	$v0,$0,3
	syscall

	la	$a0,msg5
	ori	$v0,$0,PRINT_STR
	syscall

	la	$t0,str2
	jal	strlen
	
	move	$a0,$v0
	ori	$a1,$0,10
	ori	$v0,$zero,PRINT_INT
	syscall
	ori	$a0,$0,'\n'
	ori	$v0,$0,3
	syscall

	la	$a0,str3
	la	$a1,str1
	jal	strcpy
	
	la	$a0,msg6
	ori	$v0,$0,PRINT_STR
	syscall

	la	$a0,str3
	la	$a1,str2
	jal	strcat
	move	$a0,$v0
	ori	$v0,$0,PRINT_STR
	syscall
	ori	$a0,$0,'\n'
	ori	$v0,$0,3
	syscall

	la	$a0,msg7
	ori	$v0,$0,PRINT_STR
	syscall

	la	$a0,str1
	la	$a1,str2
	jal	strcmp
	move	$a0,$v0
	ori	$v0,$0,PRINT_INT10
	syscall
	

	ori	$v0,$0,0
	lw	$ra,0($sp)
	addiu	$sp,$sp,4
	jr	$ra

# strlen inputs: s -> t0
strlen:
	ori	$v0,$0,0
	lb	$a0,0($t0)
	strlen_for:
		beqz	$a0,strlen_exit
		addiu	$v0,$v0,1
		addiu	$t0,$t0,1
		lb	$a0,0($t0)
		b	strlen_for
	strlen_exit:
		jr	$ra

#strcpy inputs: dst -> a0; src -> a1
strcpy:
	move	$v0,$a0
	strcpy_for:
		lb	$t1,0($a1)
		sb	$t1,0($a0)
		beqz	$t1,strcpy_exit
		addiu	$a0,$a0,1
		addiu	$a1,$a1,1
		b	strcpy_for

	strcpy_exit:
		jr	$ra

#strcat inputs: dst -> a0; src -> a1
strcat:
	move	$t1,$a0
	strcat_for:
		lb	$t0,0($a0)
		beqz	$t0,strcat_copy
		addiu	$a0,$a0,1
		b	strcat_for
	strcat_copy:
		addiu	$sp,$sp,-8
		sw	$ra,0($sp)
		sw	$t1,4($sp)
		jal	strcpy
		lw	$ra,0($sp)
		lw	$t1,4($sp)
		addiu	$sp,$sp,8
		move	$v0,$t1
		jr	$ra

#strcmp inputs: s1 -> a0; s2 -> a1
strcmp:
	strcmp_for:
		lb	$t0,0($a0)
		lb	$t1,0($a1)
		bne	$t0,$t1,strcmp_exit
		beqz	$t0,strcmp_exit
		addiu	$a0,$a0,1
		addiu	$a1,$a1,1
		b	strcmp_for
	strcmp_exit:
		sub	$v0,$t0,$t1
		jr	$ra
