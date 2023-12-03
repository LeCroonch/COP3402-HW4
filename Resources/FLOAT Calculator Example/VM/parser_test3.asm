	# $Id: parser_test3.asm,v 1.4 2023/11/05 22:37:35 leavens Exp $
	.text start
start:	BNE $8, $7, +4
	ADDI $0, $30, 100
	EXIT 
	.data 1024
	INT hundred = 100
	INT forty = 40
	INT six = 6
	FLOAT e = 2.7182818284
	.stack 4096
	.end
