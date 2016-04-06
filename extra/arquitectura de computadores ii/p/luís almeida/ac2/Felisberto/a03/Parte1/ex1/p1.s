	.data

	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
msg:	.asciiz "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
	.text

	.globl main
main:
	lui	$t0,SFR_BASE_HI
	lw	$t2,TRISE($t0)
	andi	$t2,$t2,0xFFF0	#1111 1111 1111 0000 - RE0 - Escrita
	sw	$t2,TRISE($t0)
	lw	$t2,LATE($t0)
	andi	$t2,0xFFF0
	li	$t3,0x000F
	or	$t2,$t2,$t3
	sw	$t2,LATE($t0)

	jr	$ra

