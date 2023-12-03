// $Id: type_check.h,v 1.2 2023/11/11 22:40:58 leavens Exp $
#ifndef _TYPE_CHECK_H
#define _TYPE_CHECK_H
#include "ast.h"
#include "type_exp.h"

// Requires scope checking has already been performed on exp
// and the attributes and types of identifiers are known.
// Type check the statement and record types of identifiers
// in the returned AST
extern stmt_t type_check_stmt(stmt_t stmt);

// Type check stmt, recording type information
// in the returned AST
extern assign_stmt_t type_check_assign_stmt(assign_stmt_t stmt);

// Type check stmt, recording type information
// in the returned AST
extern begin_stmt_t type_check_begin_stmt(begin_stmt_t stmt);

// Type check the list of statments given by stmts to out
// in the returned AST
extern stmts_t type_check_stmts(stmts_t stmts);

// Type check the if-statment given by stmt
// in the returned AST
extern if_stmt_t type_check_if_stmt(if_stmt_t stmt);

// Type check the read statment given by stmt
// in the returned AST
extern read_stmt_t type_check_read_stmt(read_stmt_t stmt);

// Type check the write statment given by stmt to out.
// in the returned AST
extern write_stmt_t type_check_write_stmt(write_stmt_t stmt);

// Requires scope checking has already been performed on exp
// and the attributes and types of identifiers are known.
// Return the modified AST with type information recorded
// (or issue an error message if it is ill-typed)
extern expr_t type_check_expr(expr_t exp);

// Requires scope checking has already been performed on exp
// and the attributes and types of identifiers are known.
// Return the modified AST with type information recorded
// (or issue an error message if it is ill-typed)
extern binary_op_expr_t type_check_binary_op_expr(binary_op_expr_t exp);

// Requires scope checking has already been performed on exp
// and the attributes and types of identifiers are known.
// Return the modified AST with type information recorded
extern ident_t type_check_ident_expr(ident_t exp);

// Requires scope checking has already been performed on exp
// and the attributes and types of identifiers are known.
// Return the modified AST with type information recorded
// (or issue an error message if it is ill-typed)
extern expr_t type_check_logical_not(expr_t exp);

// Requires: exp is the result of
// a previous run of type_check_expr 
// Return the type of the given expression, exp
extern type_exp_e type_check_expr_type(expr_t exp);

#endif
