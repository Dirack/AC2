	.data
msg:	.asciiz	"\r\n"
	.text
	.globl main
main:
	subu	$sp,$sp,4
	sw	$ra,0($sp)
while:
	li	$v0,12
	syscall			#resetCoreTimer()
	
	li	$a0,1
	jal 	delay

	li	$v0,11
	syscall			#readCoreTimer()
		
	move 	$a0,$v0
	li	$v0,6
	li	$a1,10
	syscall			#printInt

	la	$a0,msg
	li	$v0,8
	syscall

	j	while
	
	lw	$ra,0($sp)
	addu	$sp,$sp,4
	jr	$ra
	
delay:
	li	$t1,6000	# Constante_Delay
for1:
	beq	$a0,0,endfor1
for2:
	beq	$t1,0,endfor2
	addi	$t1,$t1,-1
	j	for2
endfor2:
	addi	$a0,$a0,-1
	j	for1
endfor1:
	
	
