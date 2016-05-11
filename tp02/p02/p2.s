	.equ	SFR_BASE_HI, 0XBF88
	.equ	TRISE, 0X6100
	.equ	PORTE, 0X6110
	.equ	LATE, 0X6120
	.data
	.text
	.globl main
main:	
	
	lui	$t1,SFR_BASE_HI
	lw	$t2,TRISE($t1)
	andi	$t2,$t2,0xFFF0
	sw	$t2,TRISE($t1)		# RE0/1/2/3 0ut
	
	
	lw	$t2,LATE($t1)
	andi	$t2,$t2,0xFFF0
	sw	$t2,LATE($t1)
	#counter in $t0
	ori	$t0,$0,0
cpu_cnt:
	ori	$v0,$0,11		# readCoreTimer()
	syscall
	lw	$t2,PORTE($t1)		# Values in PORTE
	blt	$v0,5000000,cpu_cnt	# 1Hz(1s) 	= 20000000;
					# 2Hz(0.5s)	= 10000000;
					# 4Hz(0.25s) 	=  5000000; 
	
	ori	$v0,$0,12		# resetCoreTime()
	syscall
	
	
	
	andi	$t2,$t2,0xC0		# Mask PORTE RE6/7
	srl	$t2,6			# $t2 >> 6
	
	andi	$t3,$t2,0x1		# $t3 = RE6 (Check counter)
	beq	$t3,$t4,counterN	# compare counterN to zero counter
	ori	$t0,$0,0		# reset counter
	move	$t4,$t3			# save current counter

counterN:
	lw	$t5,LATE($t1)
	andi	$t5,$t5,0xFFF0
	or	$t5,$t5,$t0
	sw	$t5,LATE($t1)
	####### Print Counter Value in Terminal #######
	ori	$v0,$0,6
	move	$a0,$t0
	lui	$a1,4
	ori	$a1,$a1,2
	syscall				# printInt($t0,2)
	ori	$v0,$0,3
	ori	$a0,$0,'\n'
	syscall				# putChar('\n')
	####### ------------------------------- #######
	move	$a0,$t0	
	andi	$a1,$t2,0x2		# $a1 = RE7 (Check Up/Down)
	bnez	$t3,counter2
counter1:
	jal	bin_cnt
	b	counter_refresh
counter2:
	addiu	$sp,$sp,-4
	sw	$t1,0($sp)		# save $t1(SFR_BASE_HI) on stack
	jal john_cnt
	lw	$t1,0($sp)		# restore $t1 from stack
	addiu	$sp,$sp,4
counter_refresh:
	move	$t0,$v0
	b	cpu_cnt			# return to CPU cycles counter

#Johnson 4 bits Counter inputs: $a0 -> counter_value; $a1=0/1 Up/Down
john_cnt:
	bnez	$a1,john_cntdwn		# if($a1 == 0)
		andi	$t1,$a0,0x8	# Mask Counter MSb
		srl	$t1,3		# $t1 >> 3
		xori	$t1,$t1,0x1	# negate bit
		sll	$a0,$a0,1	# $a0 << 1
		b	john_cnt_end
	john_cntdwn:			# else (if ($a0 == 1))
		andi	$t1,$a0,0x1	# Mask Counter LSb
		sll	$t1,3		# $t1 << 3
		xori	$t1,$t1,0x8	# negate bit
		srl	$a0,$a0,1	# $a0 >> 1

	john_cnt_end:
		andi	$a0,$a0,0xF	# Mask $a0 4 LSb
		or	$v0,$a0,$t1	# Concatenate negated bit
	
		jr	$ra

#Binary 4 bits Counter inputs: $a0 -> counter_value; $a1=0/1 Up/Down
bin_cnt:
	bnez	$a1,bin_cntdwn		# if($a1 == 0)
		addi	$a0,$a0,1	# counter + 1
		b	bin_cnt_end
	bin_cntdwn:			# else (if ($a0 == 1))
		addi	$a0,$a0,-1	# counter - 1
	
	bin_cnt_end:
		andi	$v0,$a0,0xF	# mask $a0 4 LSb

		jr $ra

