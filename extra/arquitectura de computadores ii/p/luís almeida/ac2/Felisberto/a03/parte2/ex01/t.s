	.data

	.equ	SFR_BASE_HI, 0xBF88
	.equ	TRISE, 0x6100
	.equ	PORTE, 0x6110
	.equ	LATE, 0x6120
msg:	.asciiz	"----"	
	.text
	
	.globl main
main:
	lui	$t0,SFR_BASE_HI
	lw	$t1,TRISE($t0)
	andi	$t1,$t1,0xFFF0	#1111 1111 1111 0000 - RE0,RE1,RE2,RE3 - Saidas
	ori	$t1,$t1,0x00A0	#0000 0000 1100	0000 - RE6,RE7 - Entradas
	sw	$t1,TRISE($t0)	#Entradas e Saídas defenidas

timer_i:
	#li	$v0,12
	#syscall			#resetCoreTimer()
	
	#lw	$t8,LATE($t0)
	#andi	$t8,$t8,0xF0
	#sw	$t8,LATE($t0)

	#li	$t3,0
	li	$t4,0

timer:
	li	$v0,11
	syscall			#readCore()

	blt	$v0,40000000,n_end_time # salta se ainda não passou o tempo necessário

	#addi	$t3,$t3,1		
	#bgt	$t3,0x000F,timer_i	# salta se o número é maior que 0X000F
	lw	$t8,LATE($t0)	
	andi	$t8,$t8,0xF0
	#or	$t8,$t8,$t3
	or	$t8,$t8,$t4
	sw	$t8,LATE($t0)

	beq	$t4,0x78,timer_i
	sll	$t4,$t4,1
	
	beq	$t4,0x1E,esc
	beq	$t4,0x3A,esc
	beq	$t4,0x78,esc
	addi	$t4,$t4,1
esc:
	li	$v0,12
	syscall			#resetCoreTimer()

n_end_time:
	j	timer
	j	$ra	
