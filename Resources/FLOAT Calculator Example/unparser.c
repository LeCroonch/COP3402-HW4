/* $Id: unparser.c,v 1.20 2023/11/11 19:26:37 leavens Exp $ */
#include <stdio.h>
#include "ast.h"
#include "utilities.h"
#include "unparser.h"

// Amount of spaces to indent per nesting level
#define SPACES_PER_LEVEL 2

// Print SPACES_PER_LEVEL * level spaces to out
static void indent(FILE *out, int level)
{
    int num = SPACES_PER_LEVEL * level;
    for (int i = 0; i < num; i++) {
	fprintf(out, " ");
    }
}


// Unparse the given program AST
// with output going to the file out
void unparseProgram(FILE *out,
		    program_t prog)
{
    unparseVarDecls(out, prog.var_decls,
		    0);
    unparseStmt(out, prog.stmt, 0);
}

// Unparse the var_decls_t vds to out
// with the given nesting level
// (note that the list is empty,
//  then nothing is printed)
void unparseVarDecls(FILE *out,
		     var_decls_t vds,
		     int level)
{
    var_decl_t *vdp = vds.var_decls;
    while (vdp != NULL) {
	unparseVarDecl(out, *vdp, level);
	vdp = vdp->next;
    }
}

// Unparse a single var-decl, vd, to out
// indented for the given nesting level
void unparseVarDecl(FILE *out,
		    var_decl_t vd,
		    int level)
{
    indent(out, level);
    fprintf(out, "%s ", type_exp_string(vd.type));
    unparseIdents(out, vd.idents);
    semiAndNewline(out);
}

// unparse the identififers in idents, separated by commas, to out
void unparseIdents(FILE *out, idents_t idents)
{
    ident_t *idp = idents.idents;
    bool printed_any = false;
    while (idp != NULL) {
	if (printed_any) {
	    fprintf(out, ", ");
	}
	unparseIdent(out, *idp);
	printed_any = true;
	idp = idp->next;
    }
}

// Unparse stmt to out,
// indented for the given level.
void unparseStmt(FILE *out,
		 stmt_t stmt,
		 int indentLevel)
{
    switch (stmt.stmt_kind) {
    case assign_stmt:
	unparseAssignStmt(out, stmt.data.assign_stmt, indentLevel);
	break;
    case begin_stmt:
	unparseBeginStmt(out, stmt.data.begin_stmt, indentLevel);
	break;
    case if_stmt:
	unparseIfStmt(out, stmt.data.if_stmt, indentLevel);
	break;
    case read_stmt:
	unparseReadStmt(out, stmt.data.read_stmt, indentLevel);
	break;
    case write_stmt:
	unparseWriteStmt(out, stmt.data.write_stmt, indentLevel);
	break;
    default:
	bail_with_error("Call to unparseStmt with an AST that is not a statement!");
	break;
    }
}


// Unparse stmt to out
// with indentation level given by level,
void unparseAssignStmt(FILE *out,
		       assign_stmt_t stmt,
		       int level)
{
    indent(out, level);
    // can't call unparseIdent, since stmt.name is not an ident_t
    fprintf(out, "%s = ", stmt.name);
    unparseExpr(out, *(stmt.expr));
    semiAndNewline(out);
}

// Unparse stmt to out
// with indentation level given by level,
void unparseBeginStmt(FILE *out,
		      begin_stmt_t stmt,
		      int level)
{
    indent(out, level);
    fprintf(out, "{\n");
    unparseVarDecls(out, stmt.var_decls,
		    level+1);
    unparseStmts(out, stmt.stmts,level+1);
    indent(out, level);
    fprintf(out, "}\n");
}

// Unparse the list of statments given by stmts to out
// with indentation level given by level.
void unparseStmts(FILE *out, stmts_t stmts, int level)
{
    // debug_print("Entering unparseStmts...\n");
    stmt_t *sp = stmts.stmts;
    while (sp != NULL) {
	unparseStmt(out, *sp, level);
	sp = sp->next;
    }
}

// Unparse the if-statment given by stmt to out
// with indentation level given by level (and each body indented one more)
void unparseIfStmt(FILE *out, if_stmt_t stmt, int level)
{
    indent(out, level);
    fprintf(out, "if ( ");
    unparseExpr(out, stmt.expr);
    fprintf(out, " )\n");
    unparseStmt(out, *(stmt.body), level+1);
}

// Unparse the read statment given by stmt to out
void unparseReadStmt(FILE *out, read_stmt_t stmt, int level)
{
    indent(out, level);
    fprintf(out, "read %s", stmt.name);
    semiAndNewline(out);
}

// Unparse the write statment given by stmt to out.
void unparseWriteStmt(FILE *out, write_stmt_t stmt, int level)
{
    indent(out, level);
    fprintf(out, "write ");
    unparseExpr(out, stmt.expr);
    semiAndNewline(out);
}

// Unparse the expression exp to out
// adding parentheses to indicate nesting
void unparseExpr(FILE *out, expr_t exp)
{
    switch (exp.expr_kind) {
    case expr_bin_op:
	unparseBinaryOpExpr(out,
                         exp.data.binary);
	break;
    case expr_ident:
	unparseIdent(out, exp.data.ident);
	break;
    case expr_number:
	unparseNumber(out,
                      exp.data.number);
	break;
    case expr_logical_not:
	unparseLogicalNotExpr(out,
                 *(exp.data.logical_not));
	break;
    default:
	bail_with_error("Unexpected expr_kind_e (%d) in unparseExpr",
			exp.expr_kind);
	break;
    }
}

// Unparse the expression given by the AST exp to out
// adding parentheses (whether needed or not)
void unparseBinaryOpExpr(FILE *out, binary_op_expr_t exp)
{
    fprintf(out, "(");
    unparseExpr(out, *(exp.expr1));
    fprintf(out, " ");
    unparseToken(out, exp.op);
    fprintf(out, " ");
    unparseExpr(out, *(exp.expr2));
    fprintf(out, ")");
}

// Unparse the given token to out
void unparseToken(FILE *out, token_t tok)
{
    fprintf(out, "%s", tok.text);
}

// Unparse the given identifier to out
void unparseIdent(FILE *out, ident_t id)
{
    fprintf(out, "%s", id.name);
}

// Unparse the given number to out in decimal format
void unparseNumber(FILE *out, number_t num)
{
    fprintf(out, "%f", num.value);
}

// Unparse the given logical negation expression to out
void unparseLogicalNotExpr(FILE *out, expr_t exp)
{
    fprintf(out, "(!");
    unparseExpr(out, exp);
    fprintf(out, ")");
}

// Print a semicolon followed by a \n on the file out
void semiAndNewline(FILE *out)
{
    fprintf(out, ";\n");
}
