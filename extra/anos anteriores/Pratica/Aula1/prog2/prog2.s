	.data
str:	.asciiz "Key pressed\n"

	.text
	.globl main
	
main:
	li $t0,1	#$t0=1

while1:	bne $t0,1,end1
	
while2: 
	li $v0,1
	syscall
	bne $v0,0,if
	j while2

if:	beq $v0,'\n',end1
	la $a0,str
	li $v0,8
	syscall
	j while2

end1:
	li $v0,0
	jr $ra

