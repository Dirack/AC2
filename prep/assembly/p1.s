.include "delay.h"
.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120

	.data
s_msg1:	.asciiz "\nIntroduza um numero (sinal e modulo): "
	.text
	.globl main

main:
	addiu	$sp,$sp,-4
	sw	$ra,0($sp)
	lui	$t3,SFR_BASE_HI
	lw	$t4,TRISE($t3)
	andi	$t4,$t4,0xFFF0
	sw	$t4,TRISE($t3)
while:
	la 		$a0,s_msg1
	ori		$v0,$0,8
	syscall
	ori		$v0,$0,5
	syscall
	move 	$t0,$v0

	ori	$a0,$0,'\n'
	ori	$v0,$0,3
	syscall
	move	$a0,$t0
	andi 	$a0,$a0,0xF
	ori	$a1,$0,4
	sll 	$a1,$a1,16
	ori	$a1,$a1,2
	ori	$v0,$0,6
	syscall
	lw	$t4,LATE($t3)
	andi	$t4,$t4,0xFFF0
	or	$t4,$t4,$a0
	sw	$t4,LATE($t3)

	addiu	$sp,$sp,-4
	sw	$t0,0($sp)
	ori	$a0,$0,5000
	jal delay
	lw	$t0,0($sp)
	addiu	$sp,$sp,4

	ori	$a0,$0,'\n'
	ori	$v0,$0,3
	syscall
	move	$a0,$t0
	bgtz	$a0,jump
	addiu	$a0,$a0,-1
	jump:
	andi 	$a0,$a0,0xF
	ori	$a1,$0,4
	sll 	$a1,$a1,16
	ori	$a1,$a1,2
	ori	$v0,$0,6
	syscall
	lw	$t4,LATE($t3)
	andi	$t4,$t4,0xFFF0
	or	$t4,$t4,$a0
	sw	$t4,LATE($t3)

	j	while

	lw	$ra,0($sp)
	addiu	$sp,$sp,4
	ori	$v0,$0,0
	jr $ra
