	.data

	.equ SFR_BASE_HI, 0xBF88
	.equ TRISE, 0x6100
	.equ PORTE, 0x6110
	.equ LATE, 0x6120
	.text

	.globl main
main:
	lui	$t0,SFR_BASE_HI
	lw	$t2,TRISE($t0)
	andi	$t2,$t2,0xFFFE	#1111 1111 1111 1110 - RE0 - Escrita
	ori	$t2,$t2,0x0040	#0000 0000 0100 0000 - RE6 - Leitura
	sw	$t2,TRISE($t0)	#Ativa o porto como saida
while:
	lw	$t2,LATE($t0)	#Ler o valor do porto RE0
	andi	$t2,$t2,0xFE

	lw	$t1,PORTE($t0)	#Ler o valor do porto RE6
	srl	$t1,$t1,6
	andi	$t1,$t1,0x01

	or	$t2,$t2,$t1	#Juntar valores lidos
	sw	$t2,LATE($t0)

	j while
	
	jr	$ra	
	
