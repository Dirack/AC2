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
	andi $t2,$t2,0xFFF0
	sw $t2,TRISE($t1)	#RE0,RE1,RE2,RE3 -> 0

	lw $t2,TRISE($t1)
	ori $t2,$t2,0x00C0
	sw $t2,TRISE($t1)	#RE6,RE7 -> 1

while:

	lw $t2,PORTE($t1)	#leitura das entradas	
	
	andi $t3,$t2,0x0040
	andi $t4,$t2,0x0080
	sll $t5,$t3,1
	
	and $t2,$t4,$t5
	or $t3,$t4,$t5
	xor $t6,$t4,$t5
	nor $t7,$t4,$t5
	
	srl $t2,$t2,7		#isolamento dos bits
	andi $t2,$t2,0x01
	srl $t3,$t3,6
	andi $t3,$t3,0x02
	srl $t6,$t6,5
	andi $t6,$t6,0x04
	srl $t7,$t7,4
	andi $t7,$t7,0x08

	lw $t8,LATE($t1)	#escrita nas saidas
	andi $t8,$t8,0xF0
	or $t8,$t8,$t2
	or $t8,$t8,$t3
	or $t8,$t8,$t6
	or $t8,$t8,$t7
	sw $t8,LATE($t1)

	j while

	jr $ra

