// $Id: type_check.c,v 1.4 2023/11/12 02:20:22 leavens Exp $
#include <assert.h>
#include "type_check.h"
#include "ast.h"
#include "id_use.h"
#include "float.tab.h"
#include "utilities.h"

// Requires scope checking has already been performed on exp
// and the attributes and types of identifiers are known.
// Type check the statement and record types of identifiers
// in the returned AST
stmt_t type_check_stmt(stmt_t stmt)
{
    switch (stmt.stmt_kind) {
    case assign_stmt:
	stmt.data.assign_stmt
	    = type_check_assign_stmt(stmt.data.assign_stmt);
	break;
    case begin_stmt:
	stmt.data.begin_stmt
	    = type_check_begin_stmt(stmt.data.begin_stmt);
	break;
    case if_stmt:
	stmt.data.if_stmt
	    = type_check_if_stmt(stmt.data.if_stmt);
	break;
    case read_stmt:
	stmt.data.read_stmt
	    = type_check_read_stmt(stmt.data.read_stmt);
	break;
    case write_stmt:
	stmt.data.write_stmt
	    = type_check_write_stmt(stmt.data.write_stmt);
	break;
    default:
	bail_with_error("Call to type_check_stmt with an AST that is not a statement!");
	break;
    }
    return stmt;
}

// Type check the statement and record types of identifiers
// in the returned AST
assign_stmt_t type_check_assign_stmt(assign_stmt_t stmt)
{
    assert(stmt.idu != NULL);
    type_exp_e tvar = id_use_get_attrs(stmt.idu)->type;
    *(stmt.expr) = type_check_expr(*(stmt.expr));
    type_exp_e exp_typ = type_check_expr_type(*(stmt.expr));
    if (tvar != exp_typ) {
	bail_with_prog_error(*(stmt.file_loc),
			     "Incompatible types for assignment (%s vs. %s)",
			     type_exp_string(tvar), type_exp_string(exp_typ));
    }
    return stmt;
}

// Type check the statement and record types of identifiers
// in the returned AST
begin_stmt_t type_check_begin_stmt(begin_stmt_t stmt)
{
    stmt.stmts = type_check_stmts(stmt.stmts);
    return stmt;
}

// Type check the statement and record types of identifiers
// in the returned AST
stmts_t type_check_stmts(stmts_t stmts)
{
    stmt_t *sp = stmts.stmts;
    while (sp != NULL) {
	*sp = type_check_stmt(*sp);
	sp = sp->next;
    }
    return stmts;
}

// Type check the statement and record types of identifiers
// in the returned AST
if_stmt_t type_check_if_stmt(if_stmt_t stmt)
{
    stmt.expr = type_check_expr(stmt.expr);
    type_exp_e exp_typ = type_check_expr_type(stmt.expr);
    if (exp_typ != bool_te) {
	bail_with_prog_error(*(stmt.file_loc),
			     "type of expression in if statment must be bool");
    }
    *(stmt.body) = type_check_stmt(*(stmt.body));
    return stmt;
}

// Type check the statement and record types of identifiers
// in the returned AST
read_stmt_t type_check_read_stmt(read_stmt_t stmt)
{
    if (id_use_get_attrs(stmt.idu)->type != float_te) {
	bail_with_prog_error(*(stmt.file_loc),
			     "type of variable in a read statment must be float");
    }
    return stmt;
}

// Type check the statement and record types of identifiers
// in the returned AST
write_stmt_t type_check_write_stmt(write_stmt_t stmt)
{
    stmt.expr = type_check_expr(stmt.expr);
    type_exp_e exp_typ = type_check_expr_type(stmt.expr);
    if (exp_typ != float_te) {
	bail_with_prog_error(*(stmt.file_loc),
			     "type of expression in a write statment must be float");
    }
    return stmt;
}

// Return the modified AST with type information recorded
// (or issue an error message if it is ill-typed)
expr_t type_check_expr(expr_t exp)
{
    switch (exp.expr_kind) {
    case expr_bin_op:
	exp.data.binary
	    = type_check_binary_op_expr(exp.data.binary);
	break;
    case expr_ident:
	exp.data.ident
	    = type_check_ident_expr(exp.data.ident);
	break;
    case expr_number:
	// nothing to do, no type errors possible here
	break;
    case expr_logical_not:
	*(exp.data.logical_not)
	    = type_check_logical_not(*(exp.data.logical_not));
	break;
    default:
	bail_with_error("Unexpected expr_kind_e (%d) in type_check_expr",
			exp.expr_kind);
	break;
    }
    // the following should never occur
    return exp;
}

// Requires scope checking has already been performed on exp
// and the attributes and types of identifiers are known.
// Return the modified AST with type information recorded
// (or issue an error message if it is ill-typed)
binary_op_expr_t type_check_binary_op_expr(binary_op_expr_t exp)
{
    *(exp.expr1) = type_check_expr(*(exp.expr1));
    *(exp.expr2) = type_check_expr(*(exp.expr2));
    type_exp_e t1 = type_check_expr_type(*(exp.expr1));
    type_exp_e t2 = type_check_expr_type(*(exp.expr2));
    if (t1 != t2) {
	bail_with_prog_error(*(exp.file_loc),
			     "Incompatible types for operator %s (%s and %s)",
			     exp.op.text, type_exp_string(t1), type_exp_string(t2));
    }
    assert(t1 != unknown_te);
    switch (exp.op.code) {
    case eqsym: case neqsym: case ltsym: case leqsym: case gtsym: case geqsym:
	exp.type = bool_te;
	break;
    case plussym: case minussym: case multsym: case divsym:
	if (t1 == bool_te) {
	    bail_with_prog_error(*(exp.file_loc),
				 "Cannot do operator (%s) on Booleans!",
				 exp.op.text);
	}
	assert(t1 == float_te);
	exp.type = t1;
	break;
    default:
	bail_with_error("Unknown operator (code %d) in type_check_binary_op_expr!",
			exp.op.code);
	break;
    }
    // should never happen
    return exp;
}

// Requires scope checking has already been performed on exp
// and the attributes and types of identifiers are known.
// Return the modified AST with type information recorded
// (or issue an error message if it is ill-typed)
ident_t type_check_ident_expr(ident_t exp)
{
    // the type should already be recorded in the id_use
    return exp;
}

// Requires scope checking has already been performed on exp
// and the attributes and types of identifiers are known.
// Return the modified AST with type information recorded
// (or issue an error message if it is ill-typed)
expr_t type_check_logical_not(expr_t exp)
{
    expr_t ret = type_check_expr(exp);
    type_exp_e t = type_check_expr_type(ret);
    if (t != bool_te) {
	bail_with_prog_error(*(ret.file_loc),
			     "type of expression must be bool to negate it");
    }
    return ret;
}

// Requires: exp is the result of
// a previous run of type_check_expr 
// Return the type of the given expression, exp
type_exp_e type_check_expr_type(expr_t exp)
{
    switch (exp.expr_kind) {
    case expr_bin_op:
	return exp.data.binary.type;
	break;
    case expr_ident:
	id_attrs *attrs = id_use_get_attrs(exp.data.ident.idu);
	return attrs->type;
	break;
    case expr_number:
	// only float literals in FLOAT
	return float_te;
	break;
    case expr_logical_not:
	return bool_te;
	break;
    default:
	bail_with_error("Unexpected expr_kind_e (%d) in type_check_expr_type",
			exp.expr_kind);
	break;
    }
    // the following should never be run
    return unknown_te;

}
