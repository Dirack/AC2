.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120

	.data
	.text
	.globl main

main:
	lui	$t1,SFR_BASE_HI
	lw	$t2,TRISE($t1)
	andi	$t2,$t2,0xFFF0
	sw	$t2,TRISE($t1)

check_dip:
	lw	$t2,PORTE($t1)	
	srl	$t2,$t2,4
	andi	$t2,$t2,0x7
		ori	$v0,$0,6
	andi	$t3,$t2,0x3 	#modulo
	andi	$t2,$t2,0x4	#sinal
		#move	$a0,$t2
		#ori	$a1,$0,2
		#syscall
		#ori	$v0,$0,3
		#ori	$a0,$0,'\n'
		#syscall	
	sll	$t2,$t2,1
	ori	$t4,$0,0
	beqz	$t2,jump
	xor	$t3,$t3,0x3
	jump:
	beqz	$t3, send_led
	sll	$t4,$t4,1
	ori	$t4,$t4,1
	addiu	$t3,$t3,-1
	b 	jump
	send_led:
		move	$a0,$t4
	ori	$a1,$0,2
	syscall
	ori	$v0,$0,3
	ori	$a0,$0,'\n'
	syscall
	move	$t3,$t2
	lw	$t2,LATE($t1)
	andi	$t2,$t2,0xFFF0
	or	$t2,$t2,$t3
	or	$t2,$t2,$t4
	sw	$t2,LATE($t1)
	j 	check_dip

	ori	$v0,$0,0
	jr	$ra
