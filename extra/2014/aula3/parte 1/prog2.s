.text

	.globl main
main:
	addiu $sp,$sp,-4
	sw $ra,0($sp)
	
	lui	$t1,SFR_BASE_HI
	lw	$t2,TRISE($t1)
	and $t2,$t2,0xFFFFFFFE	
	ori	$t2,$t2,0x40	
	sw	$t2,TRISE($t1)	
while:
	lw	$t2,PORTE($t1)	
	move $t0,$t2
	srl $t0,6
	not $t0,$t0
	and $t0,$t0,1
	and $t2,$t2,0xFFFFFFFE
	xor $t2,$t2,$t0
	sw $t2,PORTE($t1)
	
	j while
	
	lw $ra,0($sp)
	addiu $sp,$sp,4
	
	jr	$ra	
	
