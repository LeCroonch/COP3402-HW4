	# $Id: test_float_io.asm,v 1.1 2023/11/12 12:39:48 leavens Exp $
	.text start
start:	# NOTR
	RFLT
	FADD $0, $v0, $a0
	PFLT
	ADDI $0, $a0, 10
	PCH
	EXIT
	
	.data 1024
	FLOAT rd = 1.5
	.stack 5028
	.end
	
