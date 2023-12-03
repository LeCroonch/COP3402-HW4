	# $Id: unparser_test1.asm,v 1.4 2023/11/05 22:37:35 leavens Exp $
	.text go
go:     ADD $8, $9, $10
	ADDI $0, $9, 52
        ADDI $0, $10, -0934
	EXIT
	.data 0x1000
	FLOAT almost_e  =  2.7182
	INT sixteen = 0x10
	INT b25 = 25
	.stack 0x4000
	.end
	
