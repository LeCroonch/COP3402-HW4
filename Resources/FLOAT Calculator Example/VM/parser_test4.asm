	# $Id: parser_test4.asm,v 1.4 2023/11/05 22:37:35 leavens Exp $
	.text start
start:	BNE $0, $0, +4
	EXIT
	.data 2048
	INT five = +5
	.stack 6144
	.end
