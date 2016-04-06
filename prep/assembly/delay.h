	.equ	CALIBRATION_VALUE,9966	
	.text
delay:
	beqz	$a0,delay_end
	ori	$t0,$0,CALIBRATION_VALUE
	addi	$a0,$a0,-1
	delay_ms:	
	beqz	$t0,delay
		addiu	$t0,$t0,-1
		b	delay_ms
delay_end:
	jr 	$ra
	