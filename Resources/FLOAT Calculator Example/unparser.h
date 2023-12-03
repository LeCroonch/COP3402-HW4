/* $Id: unparser.h,v 1.9 2023/11/03 13:57:56 leavens Exp $ */
#ifndef _UNPARSER_H
#define _UNPARSER_H
#include <stdio.h>
#include "ast.h"

// Unparse the given program AST
// with output going to the file out
extern void unparseProgram(FILE *out,
			   program_t prog);

// Unparse the AST_list of var-decls vds to out
// with the given nesting level
// (note that if ast_list_is_empty(vds), then nothing is printed)
extern void unparseVarDecls(FILE *out, var_decls_t vds, int level);

// Unparse a single var-decl vd to out,
// indented for the given nesting level
extern void unparseVarDecl(FILE *out, var_decl_t vd, int level);

// unparse the identififers in idents, separated by commas, to out
extern void unparseIdents(FILE *out, idents_t idents);

// Unparse the statement given by the AST stmt to out,
// indented for the given level.
extern void unparseStmt(FILE *out, stmt_t stmt, int indentLevel);

// Unparse the assignment statment given by stmt to out
// with indentation level given by level,
extern void unparseAssignStmt(FILE *out, assign_stmt_t stmt, int level);

// Unparse the sequential statment given by stmt to out
// with indentation level given by level (indenting the body one more level)
extern void unparseBeginStmt(FILE *out, begin_stmt_t stmt, int level);

// Unparse the list of statments given by stmts to out
// with indentation level given by level.
extern void unparseStmts(FILE *out, stmts_t stmts, int level);

// Unparse the list of statments given by stmts to out
// with indentation level given by level.
extern void unparseStmtList(FILE *out, stmts_t stmts, int level);

// Unparse the if-statment given by stmt to out
// with indentation level given by level (and each body indented one more)
extern void unparseIfStmt(FILE *out, if_stmt_t stmt, int level);

// Unparse the read statment given by stmt to out
extern void unparseReadStmt(FILE *out, read_stmt_t stmt, int level);

// Unparse the write statment given by stmt to out.
extern void unparseWriteStmt(FILE *out, write_stmt_t stmt, int level);

// Unparse the expression given by the AST exp to out
// adding parentheses to indicate the nesting relationships
extern void unparseExpr(FILE *out, expr_t exp);

// Unparse the expression given by the AST exp to out
// adding parentheses (whether needed or not)
extern void unparseBinaryOpExpr(FILE *out, binary_op_expr_t exp);

// Unparse the given token to out
extern void unparseToken(FILE *out, token_t tok);

// Unparse the given identifier token to out
extern void unparseIdent(FILE *out, ident_t id);

// Unparse the given number to out in decimal format
extern void unparseNumber(FILE *out, number_t num);

// Unparse the given logical negation expression to out
extern void unparseLogicalNotExpr(FILE *out, expr_t exp);

// print a semicolon followed by a \n on the file out
extern void semiAndNewline(FILE *out);

#endif
