	.equ	SFR_BASE_HI, 0XBF88
	.equ	TRISE, 0X6100
	.equ	PORTE, 0X6110
	.equ	LATE, 0X6120
	.equ	PRINT_INT10, 7

	.data
	.text
	.globl main
main:
	lui	$t1,SFR_BASE_HI
	lw	$t2,TRISE($t1)
	andi	$t2,$t2,0xFFFE
	sw	$t2,TRISE($t1)

while:
	lw	$t2,PORTE($t1)
	andi	$t2,$t2,0x40
	srl	$t2,6
	not	$t2
	lw	$t3,LATE($t1)
	andi	$t3,$t3,0xFFFE
	or	$t3,$t3,$t2
	sw	$t3,LATE($t1)
	
	b	while
