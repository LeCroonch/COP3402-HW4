# $Id: vm_testB.asm,v 1.5 2023/11/10 16:48:59 leavens Exp $
# more Floating-point tests
	.text start
start:	FLW $gp, $t1, 0   # $t1 is 1.0000
	FLW $gp, $t9, 3   # $t9 is 10.000
	FDIV $t1, $t9, $t0 # $t0 is 0.1
	RND $t0, $t2      # $t2 is 0
	FDIV $t0, $t9, $t0 # $t0 is about 0.01 = 1.0e-2
	FDIV $t0, $t9, $t0 # $t0 is about 0.001 = 1.0e-3
	FSUB $t1, $t0, $t2 # $t2 is about 0.999 = 9.99e-1
	RND $t2, $t8      # $t8 is 1
	RND $t1, $t0      # $t0 is 1
	BEQ $t8, $t0, 3   # skip if equal ahead 3 instrs
	ADDI $0, $a0, 0xce # put ce in $a0, would be static link in PL/0
	JAL no
	BEQ $0, $0, 2     # skip next 2 instrs
	ADDI $0, $a0, 0xce # put ce in $a0, would be static link in PL/0
	JAL yes
	FLW $gp, $t3, 1   # $t3 is pi
	RND $t3, $t2      # $t2 is 3
	FMUL $t1, $t3, $t4 # $t4 is about pi
	FADD $0, $t4, $t5 # $t5 is about pi
	FSUB $t5, $t4, $t6 # $t6 is about 0.0
	FSW $gp, $t4, 5   # store pi past twelve
	FLW $gp, $t7, 2   # $t7 is e
	FLW $gp, $t8, 3   # $t8 is 10.000
	FMUL $t7, $t8, $a0 # $a0 is e * 10
	PFLT
	ADDI $0, $a0, 10  # put 10 ('\n') in $a0
	PCH
	FMUL $t7, $t3, $t9 # $t9 is e * pi
	RND  $t9, $a0     # $a0 is 8
	PINT
	JAL nl
	EXIT
	# saving registers and setting up new stack frame below
no:	SW $sp, $sp, -1    # save current SP register value
	ADDI $sp, $sp, -16 # allocate 4 words on runtime stack
	SW $sp, $fp, 2     # save current (now old) FP register
	ADDI $sp, $fp, 16  # save new frame base into FP register
	SW $fp, $a0, -3    # save the A0 register
	SW $fp, $ra, -4    # save RA register
        ADDI $0, $a0, 110  # $a0 is 'n'
	PCH
	ADDI $a0, $a0, 1   # $a0 is 'o'
	PCH
	JAL nl
	LW $fp, $ra, -4    # restore the RA register
	LW $fp, $fp, -2    # restore the old FP register
	ADDI $sp, $sp, 16  # restore the SP register
	JR $ra
	# saving registers and setting up new stack frame below
yes:	SW $sp, $sp, -1    # save current SP register value
	ADDI $sp, $sp, -16 # allocate 4 words on runtime stack
	SW $sp, $fp, 2     # save current (now old) FP register
	ADDI $sp, $fp, 16  # save new frame base into FP register
	SW $fp, $a0, -3    # save the A0 register
	SW $fp, $ra, -4    # save RA register
	ADDI $0, $a0, 121 # $a0 is 'y'
	PCH
	ADDI $0, $a0, 101  # $a0 is 'e'
	PCH
	ADDI $0, $a0, 115  # $a0 is 's'
	PCH
	JAL nl
	LW $fp, $ra, -4    # restore the RA register
	LW $fp, $fp, -2    # restore the old FP register
	ADDI $sp, $sp, 16  # restore the SP register
	JR $ra
nl:	ADDI $0, $a0, 10  # put 10 ('\n') in $a0
	PCH
	JR $ra

	.data 1024
	FLOAT one = 1.0000
	FLOAT pi = 3.14159265359e0
	FLOAT Eulers_number = +2.7182818284 # e
	FLOAT ten = 10.000
	FLOAT twelve = 12.0
	.stack 4096
	.end
	
