# $Id: vm_test8.asm,v 1.3 2023/11/10 01:54:46 leavens Exp $
# floating-point
	.text main
main:   FLW $gp, $t0, 0       # $t0 is 3.0
	FLW $gp, $t1, 1       # $t1 is 4.0
	FMUL $t0, $t1, $a0    # $a0 is 12.0
	PFLT
	FLW $gp, $t3, 2       # $t3 is 12.0
	FSUB $t2, $t3, $t4    # $t4 is 0.0
	BFEQ $t0, $t4, 3      # 
	ADDI $0, $a0, 78
	PCH 
	JAL nl
	ADDI $0, $a0, 89
	PCH
	JAL nl

nl:	ADDI $0, $a0, 10
	PCH
	EXIT
	
	.data 1024
	FLOAT three = 3.0
	FLOAT four = 4.0
	FLOAT twelve = 12.0
	.stack 4096
	.end
