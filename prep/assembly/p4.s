	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.data
s_msg0: .asciiz "Prima i para inicar contador c/ valor no dip-switch: "
	.text
	.globl main
main:
	lui	$t1,SFR_BASE_HI
	lw	$t2,TRISE($t1)
	andi	$t2,$t2,0xFFF0
	sw	$t2,TRISE($t1)

	la 	$a0,s_msg0
	ori	$v0,$0,8
	syscall

	ori	$v0,$0,2
	syscall
	bne	$v0,'i',reset
	lw 	$t2,PORTE($t1)
	andi	$t2,$t2,0xF0
	srl	$t2,$t2,4
	move	$t0,$t2
	b	check_timer

	reset:
	ori $t0,$0,0xF	#contador inicia a 15

	check_timer:
	ori	$v0,$0,11	#read Coretimer
	syscall
	blt	$v0,20000000,check_timer
	ori	$v0,$0,12	#reset Coretimer
	syscall
	lw	$t2,LATE($t1)
	andi	$t2,$t2,0xFFF0
	or	$t2,$t2,$t0
	sw	$t2,LATE($t1)
	sub_counter:
	addiu	$t0,$t0,-1
	andi 	$t0,$t0,0xF
	b 	check_timer


ori $v0,$0,0
jr $ra
