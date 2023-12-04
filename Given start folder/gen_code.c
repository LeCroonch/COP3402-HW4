#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "pl0.tab.h"
#include "utilities.h"
#include "ast.h"
#include "bof.h"
#include "instruction.h"
#include "code.h"
#include "regname.h"
#include "id_use.h"
#include "literal_table.h"
#include "gen_code.h"


// Initialize the code generator
extern void gen_code_initialize(){
    literal_table_initialize();

}

static void gen_code_output_seq(BOFFILE bf, code_seq cs) {
    while (!code_seq_is_empty(cs)) {
        bin_instr_t inst = code_seq_first(cs)->instr;
        instruction_write_bin_instr(bf, inst);
        cs = code_seq_rest(cs);
    }
}
static BOFHeader gen_code_program_header(code_seq main_cs) {
    BOFHeader ret;
    strncpy(ret.magic, "BOF", MAGIC_BUFFER_SIZE);
    ret.text_start_address = 0;
    ret.text_length = code_seq_size(main_cs) * BYTES_PER_WORD;
    ret.data_start_address = MAX(ret.text_length, 1024) + BYTES_PER_WORD;
    ret.data_length = literal_table_size() * BYTES_PER_WORD;
    ret.stack_bottom_addr = ret.data_start_address + ret.data_length + ret.text_length + 4096;
    return ret;
}

// Requires: bf if open for writing in binary
// Generate code for prog into bf
extern void gen_code_program(BOFFILE bf, block_t prog) { 
    
    code_seq main_cs = gen_code_block(prog);
    
    BOFHeader header = gen_code_program_header(main_cs);
    
    bof_write_header(bf, header);
    
    gen_code_output_seq(bf, main_cs);
    literal_table_start_iteration();
    while(literal_table_iteration_has_next())
    {
        word_type w = literal_table_iteration_next();
        bof_write_word(bf, w);
    }
    literal_table_end_iteration();

    bof_close(bf);
}
// Requires: bf if open for writing in binary
// Generate code for the given AST
code_seq gen_code_block(block_t blk) {

    code_seq ret = code_seq_empty();
    
    int varslen = (code_seq_size(ret) / 2) * BYTES_PER_WORD;

    ret = code_seq_concat(ret, gen_code_var_decls(blk.var_decls));
    ret = code_seq_concat(ret, gen_code_const_decls(blk.const_decls));
    ret = code_seq_concat(ret, code_save_registers_for_AR());
    ret = code_seq_concat(ret, gen_code_stmt(blk.stmt));
    ret = code_seq_concat(ret, code_restore_registers_from_AR());
    ret = code_seq_concat(ret, code_deallocate_stack_space(varslen));
    ret = code_seq_add_to_end(ret, code_exit());
    return ret;
}

// Generate code for the const-decls, cds
// There are 3 instructions generated for each identifier declared
// (one to allocate space and two to initialize that space)
extern code_seq gen_code_const_decls(const_decls_t cds) {

    code_seq ret = code_seq_empty();
    const_decl_t *cdp = cds.const_decls;
    while ( cdp != NULL) {

        ret = code_seq_concat(gen_code_const_decl(*cdp), ret);
        cdp = cdp->next;

    }
    return ret;
}

// Generate code for the const-decl, cd
extern code_seq gen_code_const_decl(const_decl_t cd) {

    return gen_code_const_defs(cd.const_defs);

}

// Generate code for the const-defs, cdfs
extern code_seq gen_code_const_defs(const_defs_t cdfs) {

    code_seq ret = code_seq_empty();
    const_def_t *cdfp = cdfs.const_defs;
    while (cdfp != NULL) {

        ret = code_seq_concat(gen_code_const_def(*cdfp), ret);
        cdfp = cdfp->next;

    }
    return ret;
}

// Generate code for the const-def, cdf
code_seq gen_code_const_def(const_def_t cdf) {
    unsigned int global_offset = literal_table_lookup(cdf.number.text, cdf.number.value);
    return code_seq_concat(code_seq_singleton(code_lw(GP, V0, global_offset)), code_push_reg_on_stack(V0));
}

// Generate code for the var_decls_t vds to out
// There are 2 instructions generated for each identifier declared
// (one to allocate space and another to initialize that space)
extern code_seq gen_code_var_decls(var_decls_t vds){ //Should be good

    code_seq ret = code_seq_empty();
    var_decl_t *vdp = vds.var_decls;
    while (vdp != NULL) {
        // generate these in reverse order,
        // so the addressing offsets work properly
        ret = code_seq_concat(gen_code_var_decl(*vdp), ret);
        vdp = vdp->next;
    }
    return ret;
}

// Generate code for a single <var-decl>, vd,
// There are 2 instructions generated for each identifier declared
// (one to allocate space and another to initialize that space)
extern code_seq gen_code_var_decl(var_decl_t vd){ //Should be good

    return gen_code_idents(vd.idents);

}

// Generate code for the identififers in idents
// in reverse order (so the first declared are allocated last).
// There are 2 instructions generated for each identifier declared
// (one to allocate space and another to initialize that space)
code_seq gen_code_idents(idents_t idents)
{
    code_seq ret = code_seq_empty();

    ident_t *idpointer = idents.idents;
    while (idpointer != NULL)
    {
        code_seq alloc_and_init = code_seq_singleton(code_addi(SP, SP, -BYTES_PER_WORD));

        alloc_and_init = code_seq_add_to_end(alloc_and_init, code_sw(SP, 0, 0));

        ret = code_seq_concat(alloc_and_init, ret);
        idpointer = idpointer->next;
    }

    return ret;
}

// EXTRA CREDIT

// (Stub for:) Generate code for the procedure declarations
extern void gen_code_proc_decls(proc_decls_t pds) {
     //code_seq not void before
    // code_seq ret = code_seq_empty();
    // proc_decl_t *pdp = pdp->block;
    // while (pdp != NULL) {
	// // generate these in reverse order,
	// // so the addressing offsets work properly
	// ret = code_seq_concat(gen_code_proc_decl(*pdp), ret); 
	// pdp = pdp->next;
    // }
    // return ret;

}

// (Stub for:) Generate code for a procedure declaration
extern void gen_code_proc_decl(proc_decl_t pd) {
     //code_seq not void before
    // return gen_code_block(*(pd.block));

}

// END EXTRA CREDIT

// Generate code for stmt
extern code_seq gen_code_stmt(stmt_t stmt){

    switch (stmt.stmt_kind) {
    case assign_stmt:
	return gen_code_assign_stmt(stmt.data.assign_stmt);
	break;
    // case call_stmt:
    // return gen_code_call_stmt(stmt.data.call_stmt);
    // break;
    case begin_stmt:
	return gen_code_begin_stmt(stmt.data.begin_stmt);
	break;
    case if_stmt:
	return gen_code_if_stmt(stmt.data.if_stmt);
	break;
    case while_stmt:
    return gen_code_while_stmt(stmt.data.while_stmt);
    break;
    case read_stmt:
	return gen_code_read_stmt(stmt.data.read_stmt);
	break;
    case write_stmt:
	return gen_code_write_stmt(stmt.data.write_stmt);
	break;
    case skip_stmt:
    return gen_code_skip_stmt(stmt.data.skip_stmt);
    break;
    default:
	bail_with_error("Call to gen_code_stmt with an AST that is not a statement!");
	break;
    }
    // The following can never execute, but this quiets gcc's warning
    return code_seq_empty();

}

// Generate code for stmt
code_seq gen_code_assign_stmt(assign_stmt_t stmt) {

    code_seq ret = gen_code_expr(*(stmt.expr));
    unsigned int offset = id_use_get_attrs(stmt.idu)->offset_count;

    assert(stmt.idu != NULL);
    assert(id_use_get_attrs(stmt.idu) != NULL);
    assert(offset <= USHRT_MAX);

    ret = code_seq_concat(ret, code_pop_stack_into_reg(V0));
    ret = code_seq_concat(ret, code_compute_fp(T9, stmt.idu->levelsOutward));
    ret = code_seq_add_to_end(ret, code_sw(T9, V0, offset));
    return ret;
}


// EXTRA CREDIT

// Generate code for stmt
/*extern code_seq gen_code_call_stmt(call_stmt_t stmt) {

    // code_seq ret = gen_code_call_stmt(stmt);
    
    // ret = code_seq_concat(ret, code_pop_stack_into_reg(A0));
    // ret = code_seq_add_to_end(ret, code_pflt());
    // return ret;

}*/

// END EXTRA CREDIT

// Generate code for stmt
extern code_seq gen_code_begin_stmt(begin_stmt_t stmt) {

    return gen_code_stmts(stmt.stmts);

}

// Generate code for the list of statments given by stmts
extern code_seq gen_code_stmts(stmts_t stmts) {

    code_seq ret = code_seq_empty();
    stmt_t *sp = stmts.stmts;
    while (sp != NULL) {
	ret = code_seq_concat(ret, gen_code_stmt(*sp));
	sp = sp->next;
    }
    return ret;

}

// Generate code for the if-statement given by stmt
code_seq gen_code_if_stmt(if_stmt_t stmt) {

    code_seq ret = gen_code_condition(stmt.condition);

    code_seq thenstmt = gen_code_stmt(*(stmt.then_stmt));
    int thenlen = code_seq_size(thenstmt);

    code_seq elsestmt = gen_code_stmt(*(stmt.else_stmt));
    int elselen = code_seq_size(elsestmt);
    
    ret = code_seq_concat(ret, code_pop_stack_into_reg(V0));
    ret = code_seq_add_to_end(ret, code_beq(V0, 0, thenlen + 1));
    ret = code_seq_concat(ret, thenstmt);
    ret = code_seq_add_to_end(ret, code_beq(0, 0, elselen));
    ret = code_seq_concat(ret, elsestmt);

    return ret;
}

// Generate code for the if-statment given by stmt
code_seq gen_code_while_stmt(while_stmt_t stmt) { 
    
    code_seq ret = gen_code_condition(stmt.condition);
    code_seq bodystmt = gen_code_stmt(*(stmt.body));

    int bodylen = code_seq_size(bodystmt);

    ret = code_seq_concat(ret, code_pop_stack_into_reg(V0));
    ret = code_seq_add_to_end(ret, code_beq(V0, 0, bodylen + 1));
    ret = code_seq_concat(ret, bodystmt);
    ret = code_seq_add_to_end(ret, code_beq(0, 0, -code_seq_size(ret)));
    return ret;
}

// Generate code for the read statment given by stmt
extern code_seq gen_code_read_stmt(read_stmt_t stmt) {

    // put number read into $v0
    code_seq ret = code_seq_singleton(code_rch());
    // put frame pointer from the lexical address of the name
    // (using stmt.idu) into $t9
    assert(stmt.idu != NULL);
    ret = code_seq_concat(ret,
			  code_compute_fp(T9, stmt.idu->levelsOutward));
    assert(id_use_get_attrs(stmt.idu) != NULL);
    unsigned int offset_count = id_use_get_attrs(stmt.idu)->offset_count;
    assert(offset_count <= USHRT_MAX); // it has to fit!
    ret = code_seq_add_to_end(ret,
			      code_seq_singleton(code_sw(T9, V0, offset_count)));
    return ret;

}

// Generate code for the write statment given by stmt.
extern code_seq gen_code_write_stmt(write_stmt_t stmt) {
    code_seq ret = gen_code_expr(stmt.expr);
    
    ret = code_seq_concat(ret, code_pop_stack_into_reg(A0));

    ret = code_seq_add_to_end(ret, code_pint());
    
    return ret;
}

// Generate code for the skip statment, stmt
extern code_seq gen_code_skip_stmt(skip_stmt_t stmt){
    // Assuming skip_stmt is meant to do nothing, we return an empty sequence
    return code_seq_empty();
}

// Requires: reg != T9
// Generate code for cond, putting its truth value
// on top of the runtime stack
// and using V0 and AT as temporary registers
// May modify HI,LO when executed
extern code_seq gen_code_condition(condition_t cond){
    code_seq ret;

    // Depending on the condition kind, call the appropriate function
    switch (cond.cond_kind) {
        case ck_odd:
            // Generate code for odd condition
            ret = gen_code_odd_condition(cond.data.odd_cond);
            break;
        case ck_rel:
            // Generate code for relational operator condition
            ret = gen_code_rel_op_condition(cond.data.rel_op_cond);
            break;
        default:
            bail_with_error("Unknown condition kind (%d) in gen_code_condition!", cond.cond_kind);
            break;
    }

    // The result of evaluating the condition should now be on top of the stack
    return ret;
}

// Generate code for cond, putting its truth value
// on top of the runtime stack
// and using V0 and AT as temporary registers
// Modifies SP, HI,LO when executed
extern code_seq gen_code_odd_condition(odd_condition_t cond){
    code_seq ret = gen_code_expr(cond.expr);
    ret = code_seq_concat(ret, code_pop_stack_into_reg(V0));
    ret = code_seq_add_to_end(ret, code_andi(V0, V0, 1)); // V0 = V0 & 1 (to check if odd)
    ret = code_seq_concat(ret, code_push_reg_on_stack(V0));
    return ret;
}

// Generate code for cond, putting its truth value
// on top of the runtime stack
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
code_seq gen_code_rel_op_condition(rel_op_condition_t cond) {
    code_seq ret = code_seq_empty();
    ret = code_seq_concat(ret, gen_code_expr(cond.expr1));
    ret = code_seq_concat(ret, gen_code_expr(cond.expr2));
    ret = code_seq_concat(ret, gen_code_rel_op(cond.rel_op));
    return ret;
}

// Generate code for the rel_op
// applied to 2nd from top and top of the stack,
// putting the result on top of the stack in their place,
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
extern code_seq gen_code_rel_op(token_t rel_op) {
    // load top of the stack (the second operand) into AT
    code_seq ret = code_pop_stack_into_reg(AT);
    // load next element of the stack into V0
    ret = code_seq_concat(ret, code_pop_stack_into_reg(V0));

    // start out by doing the comparison
    // and skipping the next 2 instructions if it's true
    code_seq do_op = code_seq_empty();
    switch (rel_op.code) {
        case eqsym:
            do_op = code_seq_singleton(code_beq(V0, AT, 2));
            break;
        case neqsym:
            do_op = code_seq_singleton(code_bne(V0, AT, 2));
            break;
        case ltsym:
            do_op = code_seq_singleton(code_sub(V0, AT, V0));
            do_op = code_seq_add_to_end(do_op, code_bltz(V0, 2));
            break;
        case leqsym:
            do_op = code_seq_singleton(code_sub(V0, AT, V0));
            do_op = code_seq_add_to_end(do_op, code_blez(V0, 2));
            break;
        case gtsym:
            do_op = code_seq_singleton(code_sub(V0, AT, V0));
            do_op = code_seq_add_to_end(do_op, code_bgtz(V0, 2));
            break;
        case geqsym:
            do_op = code_seq_singleton(code_sub(V0, AT, V0));
            do_op = code_seq_add_to_end(do_op, code_bgez(V0, 2));
            break;
        default:
            bail_with_error("Unknown token code (%d) in gen_code_rel_op", rel_op.code);
            break;
    }
    ret = code_seq_concat(ret, do_op);
    // rest of the code for the comparisons
    ret = code_seq_add_to_end(ret, code_addi(AT, 0, 0)); // put false in AT
    ret = code_seq_add_to_end(ret, code_beq(0, 0, 1)); // skip next instr
    ret = code_seq_add_to_end(ret, code_addi(AT, 0, 1)); // put true in AT
    ret = code_seq_concat(ret, code_push_reg_on_stack(AT));
    return ret;
}


// Generate code for the expression exp
// putting the result on top of the stack,
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
extern code_seq gen_code_expr(expr_t exp) {

    switch (exp.expr_kind) { 
    case expr_bin:
	return gen_code_binary_op_expr(exp.data.binary);
	break;
    case expr_ident:
	return gen_code_ident(exp.data.ident);
	break;
    case expr_number:
	return gen_code_number(exp.data.number);
	break;
    default:
	bail_with_error("Unexpected expr_kind_e (%d) in gen_code_expr",
			exp.expr_kind);
	break;
    }
    // never happens, but suppresses a warning from gcc
    return code_seq_empty();


}

// Generate code for the expression exp
// putting the result on top of the stack,
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
extern code_seq gen_code_binary_op_expr(binary_op_expr_t exp) {
    // put the values of the two subexpressions on the stack
    code_seq ret = gen_code_expr(*(exp.expr1));
    ret = code_seq_concat(ret, gen_code_expr(*(exp.expr2)));
    
    // do the operation, putting the result on the stack
    ret = code_seq_concat(ret, gen_code_arith_op(exp.arith_op));
    return ret;
}

// Generate code to apply arith_op to the
// 2nd from top and top of the stack,
// putting the result on top of the stack in their place,
// and using V0 and AT as temporary registers
// May also modify SP, HI,LO when executed
extern code_seq gen_code_arith_op(token_t arith_op) {

    // load top of the stack (the second operand) into AT
    code_seq ret = code_pop_stack_into_reg(AT);
    // load next element of the stack into V0
    ret = code_seq_concat(ret, code_pop_stack_into_reg(V0));

    code_seq do_op = code_seq_empty();
    switch (arith_op.code) {  
    case plussym:
    do_op = code_seq_add_to_end(do_op, code_add(V0, AT, V0));
    break;
case minussym:
    do_op = code_seq_add_to_end(do_op, code_sub(V0, AT, V0));
    break;
case multsym:
    do_op = code_seq_add_to_end(do_op, code_mul(V0, AT));
    do_op = code_seq_add_to_end(do_op, code_mflo(V0));
    break;
case divsym:
    do_op = code_seq_add_to_end(do_op, code_div(V0, AT));
    do_op = code_seq_add_to_end(do_op, code_mflo(V0));
    break;
default:
    bail_with_error("Unexpected arithOp (%d) in gen_code_arith_op", arith_op.code);
    break;
    }
    do_op = code_seq_concat(do_op, code_push_reg_on_stack(V0));
    return code_seq_concat(ret, do_op);

}

// Generate code to put the value of the given identifier
// on top of the stack
// Modifies T9, V0, and SP when executed
// Generate code to put the value of the given identifier on top of the stack
// Modifies T9, V0, and SP when executed
extern code_seq gen_code_ident(ident_t id) {
    assert(id.idu != NULL);
    code_seq ret = code_compute_fp(T9, id.idu->levelsOutward);
    assert(id_use_get_attrs(id.idu) != NULL);
    unsigned int offset_count = id_use_get_attrs(id.idu)->offset_count;
    assert(offset_count <= USHRT_MAX); // it has to fit!

    ret = code_seq_add_to_end(ret, code_lw(T9, V0, offset_count * BYTES_PER_WORD));

    ret = code_seq_concat(ret, code_push_reg_on_stack(V0));
    return ret;
}


// Generate code to put the given number on top of the stack
extern code_seq gen_code_number(number_t num) {

    unsigned int global_offset = literal_table_lookup(num.text, num.value);
    return code_seq_concat(code_seq_singleton(code_lw(GP, V0, global_offset)), code_push_reg_on_stack(V0));

}

