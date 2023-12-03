/* $Id: ast.c,v 1.17 2023/11/11 19:26:37 leavens Exp $ */
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "utilities.h"
#include "ast.h"
#include "id_use.h"
#include "lexer.h"

// Return the file location from an AST
file_location *ast_file_loc(AST t) {
    return t.generic.file_loc;
}

// Return the filename from the AST t
const char *ast_filename(AST t) {
    return ast_file_loc(t)->filename;
}

// Return the line number from the AST t
unsigned int ast_line(AST t) {
    return ast_file_loc(t)->line;
}


// Return an AST for the program
program_t ast_program(var_decls_t vds, stmt_t stmt)
{
    program_t ret;
    ret.file_loc = vds.file_loc;
    ret.var_decls = vds;
    ret.stmt = stmt;
    return ret;
}

// Return an AST for varDecls that are empty
var_decls_t ast_var_decls_empty(empty_t empty)
{
    var_decls_t ret;
    ret.file_loc = empty.file_loc;
    ret.var_decls = NULL;
    return ret;
}

// Return an AST varDecls that have a var_decl
var_decls_t ast_var_decls(var_decls_t var_decls, var_decl_t var_decl)
{
    var_decls_t ret = var_decls;
    // make a copy of var_decl on the heap
    var_decl_t *p = (var_decl_t *) malloc(sizeof(var_decl_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for %s!", "var_decl_t");
    }
    *p = var_decl;
    p->next = NULL;
    var_decl_t *last = ast_last_list_elem(ret.var_decls);
    if (last == NULL) {
	ret.var_decls = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a float var decl
var_decl_t ast_var_decl_float(idents_t idents)
{
    var_decl_t ret;
    ret.file_loc = idents.file_loc;
    ret.next = NULL;
    ret.type = float_te;
    ret.idents = idents;
    return ret;
}

// Return an AST for a bool var decl
var_decl_t ast_var_decl_bool(idents_t idents)
{
    var_decl_t ret;
    ret.file_loc = idents.file_loc;
    ret.next = NULL;
    ret.type = bool_te;
    ret.idents = idents;
    return ret;
}

// Return an AST made for one ident
extern idents_t ast_idents_singleton(ident_t ident)
{
    idents_t ret;
    ret.file_loc = ident.file_loc;
    // make a copy of ident on the heap
    ident_t *p = (ident_t *) malloc(sizeof(ident_t));	
    if (p == NULL) {							
	bail_with_error("Unable to allocate space for a %s!", "ident_t"); 
    }		    
    *p = ident;		
    p->next = NULL;    
    ret.idents = p;						
    return ret;
}

// Return an AST made for idents
extern idents_t ast_idents(idents_t idents, ident_t ident)
{
    idents_t ret = idents;
    // make a copy of ident on the heap
    ident_t *p = (ident_t *) malloc(sizeof(ident_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for %s!", "ident_t");
    }
    *p = ident;
    p->next = NULL;
    ident_t *last = ast_last_list_elem(ret.idents);
    if (last == NULL) {
	ret.idents = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a write statement
write_stmt_t ast_write_stmt(expr_t expr) {
    write_stmt_t ret;
    ret.file_loc = expr.file_loc;
    ret.expr = expr;
    return ret;
}

// Return an AST for a read statement
read_stmt_t ast_read_stmt(ident_t ident) {
    read_stmt_t ret;
    ret.file_loc = file_location_copy(ident.file_loc);
    ret.name = ident.name;
    return ret;
}

// Return an AST for an instruction
// with the given information
if_stmt_t ast_if_stmt(expr_t expr, stmt_t body)
{
    if_stmt_t ret;
    ret.file_loc = expr.file_loc;
    ret.expr = expr;
    // copy then_stmt to the heap
    stmt_t *p = (stmt_t *) malloc(sizeof(stmt_t));			
    if (p == NULL) {							
	bail_with_error("Unable to allocate space for a %s!", "stmt_t"); 
    }									
    *p = body;	
    ret.body = p;						
    return ret;
}

// Return an AST for a begin statement
// containing the given list of statements
begin_stmt_t ast_begin_stmt(var_decls_t vds, stmts_t stmts)
{
    begin_stmt_t ret;
    ret.file_loc = stmts.file_loc;
    ret.var_decls = vds;
    ret.stmts = stmts;
    return ret;
}

// Return an AST for an assignment statement
assign_stmt_t ast_assign_stmt(ident_t ident, expr_t expr)
{
    assign_stmt_t ret;
    ret.file_loc = file_location_copy(ident.file_loc);
    ret.name = ident.name;
    assert(ret.name != NULL);
    expr_t *p = (expr_t *) malloc(sizeof(expr_t));
    if (p == NULL) {
	bail_with_error("Unable to allocate space for a %s!", "expr_t");
    }
    *p = expr;
    ret.expr = p;
    assert(ret.expr != NULL);
    return ret;
}

// Return an AST for the list of statements 
stmts_t ast_stmts_singleton(stmt_t stmt) {
    // debug_print("Entering ast_stmts_singleton\n");
    stmts_t ret;
    ret.file_loc = stmt.file_loc;
    stmt.next = NULL;
    // copy stmt to the heap
    stmt_t *p = (stmt_t *) malloc(sizeof(stmt_t));	
    if (p == NULL) {							
	bail_with_error("Unable to allocate space for a %s!", "stmt_t"); 
    }		    
    *p = stmt;
    p->next = NULL;
    // there will be no statments in front of stmt in the list
    ret.stmts = p;					
    return ret;
}

// Return an AST for the list of statements 
stmts_t ast_stmts(stmts_t stmts, stmt_t stmt) {
    // debug_print("Entering ast_stmts...\n");
    stmts_t ret = stmts;
    // copy stmt to the heap
    stmt_t *s = (stmt_t *) malloc(sizeof(stmt_t));
    if (s == NULL) {
	bail_with_error("Cannot allocate space for %s!", "stmt_t");
    }
    *s = stmt;
    s->next = NULL;
    stmt_t *last = ast_last_list_elem(ret.stmts);
    assert(last != NULL); // because there are no empty lists of stmts
    last->next = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_assign(assign_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = assign_stmt;
    ret.data.assign_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_begin(begin_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = begin_stmt;
    ret.data.begin_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_if(if_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = if_stmt;
    ret.data.if_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_read(read_stmt_t s)
{
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = read_stmt;
    ret.data.read_stmt = s;
    return ret;
}

// Return an AST for the given statment
stmt_t ast_stmt_write(write_stmt_t s)
{
    // debug_print("Entering ast_stmt_write...\n");
    stmt_t ret;
    ret.file_loc = s.file_loc;
    ret.next = NULL;
    ret.stmt_kind = write_stmt;
    ret.data.write_stmt = s;
    return ret;
}

// Return the type of the given expression.
type_exp_e ast_expr_type(expr_t exp)
{
    switch (exp.expr_kind) {
    case expr_bin_op:
	return exp.data.binary.type;
	break;
    case expr_ident:
	return id_use_get_attrs(exp.data.ident.idu)->type;
	break;
    case expr_number:
	return exp.data.number.type;
	break;
    case expr_logical_not:
	return bool_te;
	break;
    default:
	bail_with_error("Unknown expr_kind_e (%d) passed to ast_expr_type!",
			exp.expr_kind);
	break;
    }
    // should never execute, but shuts up a warning from gcc
    return unknown_te;
}


// Return an AST for a binary operator expression
binary_op_expr_t ast_binary_op_expr(expr_t expr1, token_t op, expr_t expr2)
{
    binary_op_expr_t ret;
    ret.file_loc = expr1.file_loc;
    expr_t *p = (expr_t *) malloc(sizeof(expr_t));
    if (p == NULL) {
	bail_with_error("Unable to allocate space for a %s!", "expr_t");
    }
    *p = expr1;
    ret.expr1 = p;

    ret.op = op;
    
    p = (expr_t *) malloc(sizeof(expr_t));
    if (p == NULL) {
	bail_with_error("Unable to allocate space for a %s!", "expr_t");
    }
    *p = expr2;
    ret.expr2 = p;

    ret.type = unknown_te;

    return ret;
}

// Return an expression AST for a binary operation expresion
expr_t ast_expr_binary_op(binary_op_expr_t e)
{
    expr_t ret;
    ret.file_loc = e.file_loc;
    ret.expr_kind = expr_bin_op;
    ret.data.binary = e;
    return ret;
}

// Return an AST for the given token
token_t ast_token(file_location *file_loc, const char *text, int code)
{
    token_t ret;
    ret.file_loc = file_loc;
    ret.text = text;
    ret.code = code;
    return ret;
}

// Return an AST for a number with the given value
number_t ast_number(token_t sgn, float_type value)
{
    number_t ret;
    ret.file_loc = file_location_copy(sgn.file_loc);
    ret.value = value;
    ret.type = float_te;
    return ret;
}

// Return an AST for an identifier
ident_t ast_ident(file_location *file_loc, const char *name)
{
    ident_t ret;
    ret.file_loc = file_loc;
    ret.name = name;
    return ret;
}

// Return an AST for an expression that's a binary expression
expr_t ast_expr_binary_op_expr(binary_op_expr_t e)
{
    expr_t ret;
    ret.file_loc = e.file_loc;
    ret.expr_kind = expr_bin_op;
    ret.data.binary = e;
    return ret;
}

// Return an AST for an expression that's an identifier
expr_t ast_expr_ident(ident_t e)
{
    expr_t ret;
    ret.file_loc = e.file_loc;
    ret.expr_kind = expr_ident;
    ret.data.ident = e;
    return ret;
}

// Return an AST for an expression that's a number
expr_t ast_expr_number(number_t e)
{
    expr_t ret;
    ret.file_loc = e.file_loc;
    ret.expr_kind = expr_number;
    ret.data.number = e;
    return ret;
}

// Return an expression AST for the logical negation of exp
expr_t ast_expr_logical_not(expr_t exp)
{
    expr_t ret;
    ret.file_loc = exp.file_loc;
    ret.expr_kind = expr_logical_not;
    expr_t *ep = (expr_t *) malloc(sizeof(expr_t));
    if (ep == NULL) {
	bail_with_error("Cannot allocate space for an expr_t!");
    }
    *ep = exp;
    ret.data.logical_not = ep;
    return ret;
}

// Return an AST for empty found in the given file location
empty_t ast_empty(file_location *file_loc)
{
    empty_t ret;
    ret.file_loc = file_loc;
    return ret;
}

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Return a pointer to the last element in lst.
// This only returns NULL if lst == NULL.
void *ast_last_list_elem(void *lst)
{
    // debug_print("Entering ast_last_list_elem\n");
    if (lst == NULL) {
	return lst;
    }
    assert(lst != NULL);
    void *prev = NULL;
    while (lst != NULL) {
	prev = lst;
	lst = ((generic_t *)lst)->next;
    }
    // assert(lst == NULL);
    return prev;
}

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Return the number of elements in the linked list lst
int ast_list_length(void *lst)
{
    int ret = 0;
    generic_t *p = (generic_t *) lst;
    while (p != NULL) {
	p = p->next;
	ret++;
    }
    return ret;
}

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Is lst empty?
bool ast_list_is_empty(void *lst)
{
    return lst == NULL;
}
