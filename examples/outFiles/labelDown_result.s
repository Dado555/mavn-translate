.globl main

.data
m1: .word 6
m2: .word 5
m3: .word 0

.text
main:
	la $t0, m1
	lw $t0, 0($t0)
	la $t1, m2
	lw $t4, 0($t1)
	li $t3, 1
	li $t2, 0
lab:
	add $t2, $t2, $t0
	sub $t1, $t3, $t4
	addi $t3, $t3, 1
	bltz $t1, lab
	b nxtlbl
nxtlbl:
	la $t0, m3
	sw $t2, 0($t0)
	nop
	li $v0, 10
	syscall