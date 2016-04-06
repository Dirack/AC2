		.data
str:	.asciiz "Key pressed\n"
		.text
		.globl main
main:	li $t0,1
while1:	bne $t0,1,end1
		li $v0,1
		syscall
		move $t1,$v0
while2:	bnez $t1,end2
if:		bne $t1,'\n',break
end2:	la $a0,str
		li $v0,8
		syscall
break:	j while1
end1:	li $v0,0
		jr $ra
