	# $Id: parser_test1.asm,v 1.4 2023/11/05 22:37:35 leavens Exp $

.text begin
begin:	SUB $3, $2, $5
        EXIT
.data   0x40
	INT b = 10
	INT w
	INT c
	INT sixteen = 16
	FLOAT pi = 3.14159
.stack  0x4040
.end
