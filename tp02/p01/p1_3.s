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
	andi	$t2,$t2,0xFFF0
	sw	$t2,TRISE($t1)

while:
	lw	$t2,PORTE($t1)
	andi	$t2,$t2,0xC0
	srl	$t2,6
	
	andi	$t6,$t2,0x1	#RE6
	andi	$t7,$t2,0x2	#RE7
	srl	$t7,1
	
	and	$t2,$t6,$t7
	or	$t3,$t6,$t7
	xor	$t4,$t6,$t7
	nor	$t5,$t6,$t7

	or	$t8,$t8,$t5
	sll	$t8,1
	or	$t8,$t8,$t4
	sll	$t8,1
	or	$t8,$t8,$t3
	sll	$t8,1
	or	$t8,$t8,$t2

	lw	$t3,LATE($t1)
	andi	$t3,$t3,0xFFF0	# Flush existing values
	or	$t3,$t3,$t8
	sw	$t3,LATE($t1)
	
	b	while
