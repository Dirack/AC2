	.data
	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	
	.text
	.globl main

main:
	lui $t1,SFR_BASE_HI
	lw $t2,TRISE($t1)
	andi $t2,$t2,0xFFFE
	sw $t2,TRISE($t1)	#RE0 -> 0

	lw $t2,TRISE($t1)
	ori $t2,$t2,0x0040
	sw $t2,TRISE($t1)	#RE1 -> 1

while:

	lw $t2,PORTE($t1)
	lw $t3,LATE($t1)
	andi $t3,$t3,0xFE
	srl $t2,$t2,6
	andi $t2,$t2,0x01
	or $t3,$t3,$t2
	sw $t3,LATE($t1)

	j while

	jr $ra
	
	
	

	
