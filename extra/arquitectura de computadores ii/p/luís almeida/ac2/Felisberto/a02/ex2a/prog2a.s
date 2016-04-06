      	.data

msg:   	.asciiz "\nValor da variavel: "

      	.text
     	.global main

main:       
	li	$t1, 0	#i=0
while:      
	la 	$a0, msg
        li 	$v0, 8
        syscall		#printStr(msg)

        move 	$a0, $t1
        li 	$v0, 7
        syscall		#printInt10(i)
            
        li 	$v0, 12
        syscall		#resetCoreTimer()

while2:     
	li 	$v0, 11
        syscall		#readCore()

        bge 	$v0, 20000000, end2	#readCore()>=2000000
        j 	while2

end2:       
	add 	$t1, $t1, 1	#i++
        j 	while
        jr 	$ra
