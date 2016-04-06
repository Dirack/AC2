	.data

str1:	.space 20
str2:	.space 20
str3:	.space 40

mgs1:	.asciiz	"Introduza 2 strings: "
msg2:	.asciiz "Resultados:\n"
	.text
	.globl 	main
main:
	subu	$sp,$sp,12
	sw 	$ra, 0($sp)
	sw	$s0, 4($sp)
	sw	$s1, 8($sp)
	
	la 	$a0,msg1	#printStr(msg2)
	li 	$v0,8
	syscall
	
	li 	$a0,str1	#readStr(str1)
	li 	$a1,21
	li 	$v0,9
	syscall
	move	$s0,$a0		#experimentar substituir por la	$s0,str1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	li 	$a0,str2	#readStr(str2)
	li 	$a1,21
	li 	$v0,9
	syscall
	move	$s1,$a0		#experimentar substituir por la	$s1,str2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	la 	$a0,msg2	#pritnStr(msg2)
	li 	$v0,8
	syscall

	move	$a0,s0		#strlen(str1)
	jal	strlen
	
	move 	$a0,$v0		#printInt(strlen(str1),10)
	li	$a1,10
	li	$v0,6
	syscall
	
	move	$a0,$s1		#strlen(str2)
	jal	strlen
	
	move 	$a0,$v0		#printInt(strlen(str2),10)
	li	$a1,10
	li	$v0,6
	syscall

	la	$a0,str3	#strcpy(str3,str1)
	move	$a1,$s0
	jal	strcpy

	move	$a0,$v0		#strcat(str3,str2)
	move	$a1,$s1
	jal	strcat
	
	move	$a0,$v0		#printStr(strcat(str3,str2))
	li 	$vo,8
	syscall
	
	move	$a0,$s0		#strcmp(str1,str2)
	move	$a1,$s1
	jal	strcmp

	move	$a0,$v0		#printInt10(strcmp(str1,str2))
	li	$v0,7
	syscall
	
	lw	$ra,0($sp)
	lw	$s0,4($sp)
	lw	$s1,8($sp)
	addu	$sp,$sp,12

	li	$v0,0		#return 0
	jr	$ra

	#---------------
	#strlen
	#---------------
strlen:
	li	$v0,0		#len=0
for1:
	lb	$t0,0($a0)	#Coloca um char em $t0
	beq	$t0,0,endfor1	#Verifica se $t0=0
	addu	$a0,$a0,1	#Passa para o char seguinte	
	addu	$v0,$v0,1	#len++
	j 	for1
endfor1:
	jr	$ra


	#---------------
	#strcat
	#---------------
strcat:
	
for2:
	lb	$t0,0($a0)
	beq	$t0,0,endfor2
	addu	$a0,$a0,1 ######################################dif da do Luis##############
	j	for2
	move	$a0,$t0
endfor2:

	#---------------
	#strcpy
	#---------------
strcpy:

for3:
	j	for3
