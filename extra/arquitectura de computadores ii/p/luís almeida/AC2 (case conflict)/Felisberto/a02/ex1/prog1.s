	.data
lista:	.space 20
str1:	.asciiz "\nLeitura e ordenacao de inteiros em base 10\n"
str2:	.asciiz "Introduza 5 Inteiros: "	
str3:	.asciiz "\nResultado da ordenacao: "
str4:	.asciiz "\nNumero de trocas realizado: "	
	
	.text	
	.globl main
main:
	subu	$sp,$sp,4
	sw	$ra,0($sp)
	#---------#
	#--read--#
	la 	$a0,str1	#printStr(str1)
	li	$v0,8
	syscall

	la	$a0,str2	#pritnStr(str2)
	li	$v0,8
	syscall

	li 	$t0,0		#i=0
	la	$t1,lista	#$t1=lista
for:
	beq	$t0,5,endfor	#i=5
	sll	$t2,$t0,2	#j=i*4
	addu	$t2,$t2,$t1	#j=lista+j

	li	$a1,10		#readInt(10)
	li	$v0,4
	syscall

	sw	$v0,0($t2)	#lista[i]=readInt10()

	li	$a0,' '	#putchar('  ')
	li	$v0,3
	syscall

	addi	$t0,$t0,1	#i++
	j	for
endfor:
	
	la	$a0,str4	#printStr(str4)
	li	$v0,8
	syscall	

	li	$a0,20		
	la	$a1,lista
	jal	sequentialSort
	move	$a0,$v0		#n_trocas = sequentialSort(N_INT,lista)

	li	$v0,6
	li	$a1,10
	syscall
	

	#--------#
	#-print-#
	la	$a0,str3	#printStr(str3)
	li	$v0,8
	syscall
	
	la	$t1,lista	#$t1=lista
	addu	$t0,$t1,20	#$t0=lista+20
for1:
	beq	$t1,$t0,endfor1	#$t1=lista+20

	lw	$a0,0($t1)		#pritnInt(*ptr,10)
	li	$a1,10
	li	$v0,6
	syscall
	
	li	$a0,' '			#putchar('  ')
	li	$v0,3
	syscall

	addu	$t1,$t1,4		#lista++
	j	for1

endfor1:
	
	lw	$ra,0($sp)	
	addu	$sp,$sp,4
	jr	$ra

	#--------------#
	#sequentialSort#

sequentialSort:

	subu	$sp,$sp,24
	sw	$ra,0($sp)
	sw	$s0,4($sp)		#i
	sw	$s1,8($sp)		#j
	sw	$s2,12($sp)		#n_val
	sw	$s3,16($sp)
	sw	$s4,20($sp)	
	
	li	$s0,0			#i=0
	li	$s4,0			#n_trocas
	move	$s2,$a0			#n_val
	move	$s3,$a1			#lista[0]
for2:		
	sub	$t8,$s2,1		#n_val-1
	bge	$s0,$t8,endfor2		#i<n_val-1
	addi	$s1,$s0,1		#j=i+1
for3:
	bge	$s1,$s2,endfor3		#j<n_val
	
	sll	$t0,$s0,2		#i*4
	addu	$a0,$t0,$s3		#lista+i*4
	sll	$t1,$s1,2		#j*4
	addu	$a1,$t1,$s3		#lista+j*4
	
	lw	$t0,0($a0)
	lw	$t1,0($a1)
if:
	ble	$t0,$t1,endif
	jal	troca			#troca(&array[y] > &array[j])
	nop
	addu	$s4,$s4,1		#n_troca++
	li	$v0,3
	li	$a0,'x'
	syscall
endif:
	addiu	$s1,$s1,1		#j++
	j	for3
endfor3:
	addiu	$s0,$s0,1		#i++
	j	for2
endfor2:
	#li	$v0,7
	#move	$a0,$s4
	#syscall
	
	move	$v0,$s4			#return n_troca

	lw	$ra,0($sp)
	lw	$s0,4($sp)
	lw	$s1,8($sp)
	lw	$s2,12($sp)
	lw	$s3,16($sp)
	lw	$s4,20($sp)
	addu	$sp,$sp,24
	jr	$ra

	#-----#
	#troca#
troca:
	lw	$t0,0($a0)
	lw	$t1,0($a1)
	sw	$t1,0($a0)
	sw	$t0,0($a1)
	li	$v0,0
	jr	$ra
