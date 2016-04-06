	.data

	.equ	SFR_BASE_HI, 0xBF88
	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120
msg:	.asciiz	"----"	
	.text
	
	.globl main
main:
	subu	$sp,$sp,4
	sw	$s0,0($sp)


	lui	$t0,SFR_BASE_HI
	lw	$t1,TRISE($t0)
	andi	$t1,$t1,0xFFF0	#1111 1111 1111 0000 - RE0,RE1,RE2,RE3 - Saidas
	ori	$t1,$t1,0x00A0	#0000 0000 1100	0000 - RE6,RE7 - Entradas
	sw	$t1,TRISE($t0)	#Entradas e Saídas defenidas

do:
	lw	$t1,PORTE($t0)
	srl	$t6,$t1,6
	andi	$t6,$t6,0x01	#Escolhe o contador
	move	$s0,$t6
	
	beq	$t6,0,bin
	beq	$t6,1,joh

##Binário##
bin:
	li	$v0,12
	syscall			#resetCoreTimer()

	lw	$t8,LATE($t0)
	andi	$t8,$t8,0xF0
	sw	$t8,LATE($t0)

	li	$t2,0
b_timer:
	lw	$t5,PORTE($t0)	
	srl	$t6,$t5,6
	andi	$t6,$t6,0x01	#Escolhe o contador
	bne	$t6,$s0,do
	srl	$t7,$t5,7
	andi	$t7,$t7,0x01	#Escolhe crescente ou descrescente
	
	li	$v0,11
	syscall			#readCore()

	blt	$v0,40000000,b_end_time
	beq	$t7,1,bin_d
	addi	$t2,$t2,2
bin_d:
	addi	$t2,$t2,-1
	bgt	$t2,0x000F,bin
	lw	$t8,LATE($t0)
	andi	$t8,$t8,0xF0
	or	$t8,$t8,$t2
	sw	$t8,LATE($t0)
	
	li	$v0,12
	syscall			#resetCoreTimer()
b_end_time:
	j	b_timer
######################
##Johnson##
joh:
	li	$t4,0
	li	$t3,0

	li	$v0,12
	syscall			#resetCoreTimer()
j_timer:	
	lw	$t5,PORTE($t0)	
	srl	$t6,$t5,6
	andi	$t6,$t6,0x01	#Escolhe o contador
	bne	$t6,$s0,do
	srl	$t7,$t5,7
	andi	$t7,$t7,0x01	#Escolhe crescente ou descrescente
	
	li	$v0,11
	syscall			#readCore()

	blt	$v0,40000000,j_end_time
	lw	$t8,LATE($t0)
	andi	$t8,$t8,0xF0
	or	$t8,$t8,$t4
	sw	$t8,LATE($t0)
	
	beq	$t7,1,joh_d
	beq	$t7,0,joh_c
joh_d:
	beq	$t4,0x01,joh
	srl	$t4,$t4,1
	

	andi	$t5,$t4,0x01
	beq	$t5,0x01,esc
esc2:
	addi	$t4,$t4,8
esc:
	add	$t3,$t3,1	
	beq	$t3,4,esc2
	j	esc4

joh_c:
	beq	$t4,0x78,joh
	sll	$t4,$t4,1
	beq	$t4,0x0E,esc1
	srl	$t5,$t4,3
	andi	$t5,$t5,0x01
	beq	$t5,0x01,esc4
esc1:
	addi	$t4,$t4,1
esc4:
	li	$v0,12
	syscall
	
j_end_time:
	j	j_timer
######################i
	lw	$s0,0($sp)
	addu	$sp,$sp,4
	jr	$ra
	

	
