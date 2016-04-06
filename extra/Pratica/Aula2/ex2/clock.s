	.data
str:	.asciiz "HH:MM:SS\n"
str1:	.asciiz ":"
str2:	.asciiz "\n"
str3:	.asciiz "Insert time: (HH:MM:SS)\n"
str4:	.asciiz "HH: "
str5:	.asciiz "MM: "
str6:	.asciiz "SS: "
	.align 2
	.text
	.globl main
	
main:	
	la $a0,str3
	li $v0,8
	syscall		#printStr("Insert time")
	
	li $t0,00	#$t0 = 00 /HH
	li $t1,00	#$t1 = 00 /MM
	li $t2,00	#$t2 = 00 /SS
	
	li $v0,5
	syscall		#readInt(HH)
	move $t0,$v0
	
	la $a0,str1
	li $v0,8
	syscall		#printStr(":")

	li $v0,5
	syscall		#readInt(MM)
	move $t1,$v0
	
	la $a0,str1
	li $v0,8
	syscall		#printStr(":")

	li $v0,5
	syscall		#printInt(SS)
	move $t2,$v0

	la $a0,str2
	li $v0,8
	syscall		#printStr(\n)
	
	li $v0,12
	syscall

while:	
	li $v0,11
	syscall
	move $t3,$v0
	bge $t3,20000000,endWhile
	
	j while

endWhile:
	add $t2,$t2,1
	li $t4,60
if:
	bne $t2,$t4,print
	li $t2,00
	addi $t1,$t1,1
if2:
	bne $t1,60,print
	li $t1,00
	addi $t0,$t0,1
if3:
	li $t5,24
	bne $t0,$t5,print
	li $t0,00
	addi $t2,$t2,1

print:
	move $a0,$t0
	li $v0,7
	syscall		#printInt(HH)
	
	la $a0,str1
	li $v0,8
	syscall		#printStr(":")

	move $a0,$t1
	li $v0,7
	syscall		#printInt(MM)
	
	la $a0,str1
	li $v0,8
	syscall		#printStr(":")

	move $a0,$t2
	li $v0,7
	syscall		#printInt(SS)

	la $a0,str2
	li $v0,8
	syscall		#printStr(\n)


	li $v0,12
	syscall
	j while

	jr $ra


