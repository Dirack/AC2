	.data
	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120	

	.text
	.globl main

main:
	lui $t0,SFR_BASE_HI
	lw $t1,TRISE($t0)
	ori $t1,$t1,0x0040 #RE6 = 1, entrada
	andi $t1,$t1,0xFFF0 #RE0/1/2/3 = 0, saídas
	sw $t1,TRISE($t0)

	lw $t1,PORTE($t0)
	andi $t1,$t1,0x0040
	srl $t1,$t1,6 	#t2 = valor de RE6
	
	li $t3,0x0001
while:
	beq $t1,0x00,endWhile
	li $v0,12
	syscall
whileTimer:
	li $v0,11
	syscall

	bge $v0,5000000,endwhile
	j whileTimer
endwhile:
	lw $t1,LATE($t0)
	andi $t1,$t1,0xFFF0
	or $t1,$t1,$t3
	sw $t1,LATE($t0)

if:
	beq $t3,0x0080,else
	sll $t3,$t3,1
	j end
else:
	li $t3,0x0001
endWhile:
	lw $t1,LATE($t0)
	andi $t1,$t1,0xFFF0
	sw $t1,LATE($t0)

	
end:
	lw $t1,PORTE($t0)
	andi $t1,$t1,0x0040
	srl $t1,$t1,6

	j while

	jr $ra
