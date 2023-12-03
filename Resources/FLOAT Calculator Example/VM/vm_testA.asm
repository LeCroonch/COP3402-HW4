	# $Id: vm_testA.asm,v 1.2 2023/11/10 01:19:25 leavens Exp $
	.text start
start:	STRA
	ADDI $0, $t0, 1   # $t0 is 1
	CVT $t0, $t1      # $t1 is 1.00000
	ADD $t0, $t0, $t2 # $t2 is 2
	CVT $t2, $t3      # $t3 is 2.0000
	FADD $t3, $t1,$t4 # $t4 is 3.0000
	FMUL $t4, $t3, $t0 # $t0 is 6.0000
	FADD $t0, $t0, $t0 # $t0 is 12.000
	FLW $gp, $t6, 0   # $t6 is 12.000
	FSUB $t0, $t3, $a0 # $t3 is 10.000
	PFLT
	ADDI $0, $t1, 89  # $t1 is 89 ('Y')
	NOTR
	ADD $0, $t1, $a0  # move 89 to $a0
	PCH
	ADDI $0, $a0, 10  # put 10 ('\n') in $a0
	PCH
	EXIT
	.data 1024
	FLOAT twelve = 12.0
	.stack 4096
	.end
	
