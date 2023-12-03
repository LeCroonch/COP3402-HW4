/* $Id: parser.h,v 1.5 2023/11/01 00:56:49 leavens Exp $ */
// This header file defines the externally-visible entry points to the parser
#ifndef _PARSER_H
#define _PARSER_H
#include "ast.h"

extern program_t progast;

// Parse a PL/0 program using the tokens from the lexer,
// returning the program's AST
extern program_t parseProgram(char *file_name);

#endif
