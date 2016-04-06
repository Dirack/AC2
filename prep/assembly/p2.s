.include "delay.h"
.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120

	.data
	.text
	.globl main

main:
	lui	$t0,SFR_BASE_HI
	lw	$t1,TRISE($t0)
	andi	$t1,$t1,0xFFF0
	ori	$t1,$t1,0xF0
	sw	$t1,TRISE($t0)
reset_led:
	lw	$t1,LATE($t0)
	andi	$t1,$t1,0xFFF0
	sw	$t1,LATE($t0)
	lw	$t1,PORTE($t0)
	andi	$t1,$t1,0x70
	srl	$t1,$t1,4
	or	$t4,$0,$t1
	move	$t2,$t4
	ori	$t3,$0,0	# 0-up // 1-down
check_dip:
	lw	$t1,PORTE($t0)
	srl	$t1,$t1,7
	beqz	$t1,reset_led
	check_timer:
		ori	$v0,$0,11
		syscall
		blt	$v0,20000000,check_dip
	ori	$v0,$0,12
	syscall
	lw	$t1,LATE($t0)
	andi	$t1,$t1,0xFFF0
	or	$t1,$t1,$t2
	sw	$t1,LATE($t0)
	
	beq	$t3,1,down
	up:
	srl	$t5,$t2,1
	bge	$t5,$t4,switch_ud
	sll 	$t2,$t2,1
	b	check_dip
	down:
	beq	$t2,$t4,switch_ud
	srl 	$t2,$t2,1
	b	check_dip

	switch_ud:
	xor	$t3,$t3,0x1
	srl	$t5,$t2,1
	beq	$t5,$t4,down
	beq	$t2,$t4,up
	b	check_dip

	ori	$v0,$0,0
	jr $ra


