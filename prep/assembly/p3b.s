.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120

	.data
s_msg0:	.asciiz	"Introduza um numero[-3:3]: "
	.text
	.globl main

main:
	lui	$t1,SFR_BASE_HI
	lw	$t2,TRISE($t1)
	andi	$t2,$t2,0xFFF0
	sw	$t2,TRISE($t1)

check_dip:
	#lw	$t2,PORTE($t1)	
	#srl	$t2,$t2,4
	
	la 	$a0,s_msg0
	ori	$v0,$0,8
	syscall
	ori	$v0,$0,5
	syscall
	move	$t2,$v0

	andi	$t2,$t2,0x7
	andi	$t3,$t2,0x3 	#modulo
	andi	$t2,$t2,0x4	#sinal	
	sll	$t2,$t2,1

	ori	$t4,$0,0
	beqz	$t2,jump
	addi 	$t3,$t3,-1
	xor	$t3,$t3,0x3
	jump:
	beqz	$t3, send_led
	sll	$t4,$t4,1
	ori	$t4,$t4,1
	addiu	$t3,$t3,-1
	b 	jump
	send_led:
	move	$t3,$t2
	lw	$t2,LATE($t1)
	andi	$t2,$t2,0xFFF0
	or	$t2,$t2,$t3
	or	$t2,$t2,$t4
	sw	$t2,LATE($t1)
	ori	$v0,$0,3
	ori	$a0,$0,'\n'
	syscall
	j 	check_dip

	ori	$v0,$0,0
	jr	$ra
