	# $Id: lexer_test3.asm,v 1.3 2023/11/05 22:37:35 leavens Exp $
	.text go
go:     ADDI $0, $9, 52
        ADDI $0, $10, -0934
	EXIT
	.data 512
	INT sixteen = 0x10
	.stack 4512
	.end
	
