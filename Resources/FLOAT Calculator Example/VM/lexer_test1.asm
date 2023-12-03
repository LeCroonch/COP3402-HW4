  # $Id: lexer_test1.asm,v 1.9 2023/11/12 04:57:27 leavens Exp $

.text start

start:	STRA
	ADD $0, $2, $3
        SUB $3, $2, $5
	FADD $0, $2, $3
        FSUB $3, $2, $5
	AND $4, $5, $4
	BOR $6, $7, $8 
	NOR $9, $10, $11  # why?
	XOR $12, $13, $14 # & why not?
        MUL $15, $16
	MFHI $17
	MFLO $18
	BORI $0, $17, 1
	DIV $18, $17
	FMUL $16, $18, $17
	FDIV $16, $18, $17
	ADDI $0, $13, 13
	ADD $13, $13, $13
	ADD $13, $13, $13
	ADDI $13, $13, 20
	JR $13
	ADD $0, $0, $31
here:	ANDI $7, $7, 8
        BORI $7, $8, 0x223ff
	XORI $8, $7, 0x34567A
	BEQ $8, $7, -0x10
        BNE $8, $7, +4
	BGEZ $8, 4
	BGTZ $9, +8
	BLTZ $10, 4
	BLEZ $9, 4
	BFEQ $8, $7, -0x10
        BFNE $8, $7, +4
	BFGEZ $8, 4
	BFGTZ $9, +8
	BFLTZ $10, 4
	BFLEZ $9, 4
	ADDI $0, $30, 100  # put 100 in fp
	ADDI $0, $9, 10    # put 10 in $9
	SB   $30, $9, 5
	LBU  $30, $8, 5
        SW   $30, $8, 0x8
	LW   $30, $9, 0x8
	CVT  $10, $11     # put 10.0000 in $11
	FSW  $30, $11, 0x12 # store 10.000 in GP+12
	RND  $11, $12     # put 10 in $12
	FLW  $30, $13, 0x12 #load 10.000 into $13

	ADD  $0,  $30, $4
	PSTR
	PINT
	PFLT
	PCH
	RFLT
	NOTR
        EXIT
       
.data   2048
	INT b = 10
	INT w
	INT c
	INT sixteen = 0x10
	FLOAT pi = 3.14159

.stack  8096	
.end
