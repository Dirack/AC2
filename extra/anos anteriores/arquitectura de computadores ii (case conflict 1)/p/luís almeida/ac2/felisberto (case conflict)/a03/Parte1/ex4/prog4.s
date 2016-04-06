	.data

	.equ SFR_BASE_HI, 0xBF88	
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
msg:	.asciiz "-------------"
	.text

	.globl main
main:
	lui	$t6,SFR_BASE_HI
	lw	$t0,TRISE($t6)
	andi	$t0,$t2,0xFFF0	#1111 1111 1111 0000 - RE0;RE1;RE2;RE3 - Escrita
	ori	$t0,$t2,0x00A0	#0000 0000 1100 0000 - RE6;RE7 - Leitura
	sw	$t0,TRISE($t6)	#Ativa o porto como saida
while:
	lw	$t7,LATE($t6)
	andi	$t0,$t7,0xF0	#1111 0000


	lw	$t7,PORTE($t6)
	srl	$t4,$t7,6
	srl	$t5,$t7,7
	andi	$t4,$t4,0x01
	andi	$t5,$t5,0x01
	

	and	$t0,$t4,$t5
	andi	$t0,$t0,0x01

	or	$t1,$t4,$t5
	sll	$t1,$t1,1
	andi	$t1,$t1,0x02

	xor	$t2,$t4,$t5
	sll	$t2,$t2,2
	andi	$t2,$t2,0x04
#ciclo:
#	li 	$v0,7
	#li	$a1,2
#	move	$a0,$t1
#	syscall
	
#	li	$v0,8
#	la 	$a0,msg
#	syscall
	
	xori	$t3,$t1,0x02

#	li 	$v0,7
#	li	$a1,2
#	move	$a0,$t3
#	syscall

#	li	$v0,8
#	la 	$a0,msg
#	syscall

	sll	$t3,$t3,2

#	li 	$v0,7
#	li	$a1,2
#	move	$a0,$t3
#	syscall

#	li	$v0,8
#	la 	$a0,msg
#	syscall


	andi	$t3,$t3,0x08

#	li 	$v0,7
#	li	$a1,2
#	move	$a0,$t3
#	syscall

#j end_while	
	or	$t0,$t0,$t1
	or	$t0,$t0,$t2
	or	$t0,$t0,$t3
	lw	$t8,LATE($t6)
	andi	$t8,$t8,0xf0
	or	$t8,$t0,$t8
	sw	$t8,LATE($t6)
	j while
end_while:	
	jr	$ra	
	

