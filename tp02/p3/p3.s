#CALIBRATION_VALUE=6000 	=> Coretimer = 12068
#CALIBRATION_VALUE=12000 	=> Coretimer = 24068
#LINEST x=>Coretimer	y=>CALIBRATION_VALUE	y=0.5*x-34
#LINEST => y=0.5*20000-34	y= 9966
	.equ	CALIBRATION_VALUE,9966	
	.data
s_msg0:	.asciiz	"\r\n"
	.text
	.globl main

main:
	ori	$v0,$0,12
	syscall
	ori	$a0,$0,1
	jal delay
	ori	$v0,$0,11
	syscall
	move 	$a0,$v0
	ori	$a1,$0,10
	sll	$a1,$a1,16
	ori	$a1,$a1,10
	ori	$v0,$0,6
	syscall
	la	$a0,s_msg0
	ori	$v0,$0,8
	syscall
	b main
	
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
