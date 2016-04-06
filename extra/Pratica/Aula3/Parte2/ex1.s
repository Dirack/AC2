.data
.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120

.text
.globl main

main:
######## Definiçao das Entradas/Saídas ###########

	lui $t0,SFR_BASE_HI
	lw $t1,TRISE($t0)
	andi $t1,$t1,0xFFF0
	sw $t1,TRISE($t0)	#RE0,RE1,RE2,RE3 -> Saidas

	lw $t1,TRISE($t0)
	ori $t1,$t1,0x00C0
	sw $t1,TRISE($t0)	#RE6,RE7 -> Entradas

	li $t5,0x00
	li $t6,0x00		
######## Leitura das Entradas e Inicializaçao ####################
	lw $t1,PORTE($t0)
	andi $t2,$t1,0x0040	#$t2 -> Valor de RE6
	srl $t2,$t2,6
	andi $t2,$t2,0x01
	andi $t3,$t1,0x0080 	#$t3 -> Valor de RE7
	srl $t3,$t3,7
	andi $t3,$t3,0x01
	move $t8,$t2
	move $t9,$t3

while:
	lw $t1,PORTE($t0)
	andi $t2,$t1,0x0040	#$t2 -> Valor de RE6
	srl $t2,$t2,6
	andi $t2,$t2,0x01
	andi $t3,$t1,0x0080 	#$t3 -> Valor de RE7
	srl $t3,$t3,7
	andi $t3,$t3,0x01
	bne $t8,$t2,if
	bne $t9,$t3,if 
	j core
if:
	li $t5,0x00
	li $t6,0x00
	
core:
######## Core Timer ##############################
	li $v0,12
	syscall 		#Reset CoreTimer()

whileTimer:

	li $v0,11
	syscall
	move $t4,$v0		#$t4 = CoreTimer()
	bge $t4,5000000,endWhile
	j whileTimer

endWhile:
######## Contador Binario #########################
Binary:		
	bne $t2,0x00,Johnson

	lw $t1,LATE($t0)
	andi $t1,$t1,0xF0
	or $t1,$t1,$t5
	sw $t1,LATE($t0)
	bne $t3,0x00,BinD
BinC:
	addu $t5,$t5,1
	j end
BinD:
	subu $t5,$t5,1
	
	j end
######## Contador Johnson #########################
Johnson:
	lw $t1,LATE($t0)
	andi $t1,$t1,0xF0
	or $t1,$t1,$t6
	sw $t1,LATE($t0)
	bne $t3,0x00,JohnsonD
JohnsonC:
	andi $t7,$t6,0x08
	nor $t7,$t7,0
	srl $t7,$t7,3
	andi $t7,$t7,0x01
	sll $t6,$t6,1
	or $t6,$t6,$t7
	
	j end
JohnsonD:
	andi $t7,$t6,0x01
	nor $t7,$t7,0
	sll $t7,$t7,3
	andi $t7,$t7,0x08
	srl $t6,$t6,1
	or $t6,$t6,$t7

	j end

end:
	move $t8,$t2
	move $t9,$t3
	j while

	jr $ra



