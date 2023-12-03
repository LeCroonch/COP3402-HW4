/* $Id: parser.c,v 1.16 2023/11/01 00:56:49 leavens Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "utilities.h"

// Parse a PL/0 program from the given file,
// putting the AST into progast
extern int yyparse (char const *file_name);

// Parse a PL/0 program using the tokens from the lexer,
// returning the program's AST
extern program_t parseProgram(char *file_name)
{
    lexer_init(file_name);
    int rc = yyparse(file_name);
    if (rc != 0) {
	exit(rc);
    }
    return progast;
}
