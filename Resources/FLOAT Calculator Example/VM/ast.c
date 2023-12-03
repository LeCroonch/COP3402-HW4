/* $Id: ast.c,v 1.28 2023/11/06 09:36:24 leavens Exp $ */
#include <string.h>
#include <stdlib.h>
#include "utilities.h"
#include "ast.h"
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

// Return a pointer to a fresh copy of t
// that has been allocated on the heap
AST *ast_heap_copy(AST t) {
    AST *ret = (AST *)malloc(sizeof(AST));
    if (ret == NULL) {
	bail_with_error("Cannot allocate an AST heap copy!");
    }
    *ret = t;
    return ret;
}

// Return an AST for a program,
// which contains the given ASTs.
program_t ast_program(text_section_t textSec, data_section_t dataSec, stack_section_t stackSec)
{
    program_t ret;
    ret.file_loc = textSec.file_loc;
    ret.textSection = textSec;
    ret.dataSection = dataSec;
    ret.stackSection = stackSec;
    return ret;
}

// Return an AST for the text section
// with the given entry point and instructions.
text_section_t ast_text_section(token_t tok, lora_t entryPoint,
				asm_instrs_t instrs)
{
    text_section_t ret;
    ret.file_loc = file_location_copy(tok.file_loc);
    ret.entryPoint = entryPoint;
    ret.instrs = instrs;
    return ret;
}

// Return an AST for an entry point
// that represents the label given
lora_t ast_lora_label(ident_t label)
{
    lora_t ret;
    assert(label.file_loc != NULL);
    assert(label.file_loc->filename != NULL);
    ret.file_loc = file_location_copy(label.file_loc);
    ret.address_defined = false;
    ret.label = label.name;
    ret.addr = 0;
    return ret;
}

// Return an AST for an entry point
// that represents the address given
lora_t ast_lora_addr(unsigned_int_t addr)
{
    lora_t ret;
    assert(addr.file_loc != NULL);
    assert(addr.file_loc->filename != NULL);
    ret.file_loc = file_location_copy(addr.file_loc);
    ret.address_defined = true;
    ret.label = NULL;
    ret.addr = addr.value;
    return ret;
}


// Return an AST for an asm instr AST
// with the given label-opt and instruction
asm_instr_t ast_asm_instr(label_opt_t labelOpt, instr_t instr)
{
    asm_instr_t ret;
    ret.file_loc = labelOpt.file_loc;
    ret.next = NULL;
    ret.label_opt = labelOpt;
    ret.instr = instr;
    return ret;
}

// Return an AST for a singleton asm instrs AST
// with the given instruction
asm_instrs_t ast_asm_instrs_singleton(asm_instr_t asminstr)
{
    asm_instrs_t ret;
    ret.file_loc = asminstr.file_loc;
    asm_instr_t *p = (asm_instr_t *)malloc(sizeof(asm_instr_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for an asm_instr!");
    }
    *p = asminstr;
    p->next = NULL;
    ret.instrs = p;
    return ret;
}

// Return an AST made from adding instr to the end of lst
asm_instrs_t ast_asm_instrs_add(asm_instrs_t lst, asm_instr_t asminstr)
{
    asm_instrs_t ret = lst;
    asm_instr_t *p = (asm_instr_t *)malloc(sizeof(asm_instr_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for an asm_instr!");
    }
    *p = asminstr;
    p->next = NULL;
    // splice p onto the end of lst.instrs
    asm_instr_t *last = ast_last_list_elem(lst.instrs);
    if (last == NULL) {
	ret.instrs = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a label_opt AST
// with the given identifier
label_opt_t ast_label_opt_label(ident_t id)
{
    label_opt_t ret;
    ret.file_loc = file_location_copy(id.file_loc);
    ret.name = id.name;
    return ret;
}

// Return an AST for a label_opt AST
// with the empty AST (t)
label_opt_t ast_label_opt_empty(empty_t t)
{
    label_opt_t ret;
    ret.file_loc = file_location_copy(t.file_loc);
    ret.name = NULL;
    return ret;
}

// Return an AST for a signed number, which has the same value as t
signed_int_t ast_offset_unsigned(unsigned_int_t t)
{
    signed_int_t ret;
    assert(t.file_loc != NULL);
    assert(t.file_loc->filename != NULL);
    ret.file_loc = file_location_copy(t.file_loc);
    ret.value = t.value;
    if (ret.value < 0) {
	bail_with_prog_error(*(t.file_loc),
			     "offset (%u) is too big for an integer!",
			     t.value);
    }
    return ret;
}


// Return an immediate data holding an int
immedData_t ast_immed_number(int n) {
    immedData_t ret;
    ret.id_data_kind = id_number;
    ret.data.immed = n;
    return ret;
}

// Return an immediate data holding an unsigned int
immedData_t ast_immed_unsigned(unsigned int u) {
    immedData_t ret;
    ret.id_data_kind = id_unsigned;
    ret.data.uimmed = u;
    return ret;
}

// Return an immediate data that is nothing
immedData_t ast_immed_none() {
    immedData_t ret;
    ret.id_data_kind = id_empty;
    ret.data.none = 0;
    return ret;
}

// Return an immediate data holding an address
immedData_t ast_immed_lora(lora_t a) {
    immedData_t ret;
    ret.id_data_kind = id_lora;
    ret.data.lora = a;
    return ret;
}

// Return an AST for an instruction
// with the given information
instr_t ast_instr(token_t op, instr_type itype,
		  unsigned short num_regs_used,
		  unsigned short reg1, unsigned short reg2,
		  unsigned short reg3, func_type func,
		  immed_kind_t ik, immedData_t im)
{
    instr_t ret;
    ret.file_loc = file_location_copy(op.file_loc);
    ret.itype = itype;
    ret.opcode = lexer_token2opcode(op.code);
    ret.opname = op.text;
    ret.func = func;
    ret.regs_used = num_regs_used;
    ret.regs[0] = reg1;
    ret.regs[1] = reg2;
    ret.regs[2] = reg3;
    ret.immed_kind = ik;
    ret.immed_data = im;
    return ret;
}

// Return an AST for the data section AST
// with the given lists of static declarations.
data_section_t ast_data_section(token_t kw, unsigned int static_start,
				static_int_decls_t staticIntDecls,
				static_float_decls_t staticFloatDecls)
{
    data_section_t ret;
    ret.file_loc = file_location_copy(kw.file_loc);
    ret.static_start_addr = static_start;
    ret.staticIntDecls = staticIntDecls;
    ret.staticFloatDecls = staticFloatDecls;
    return ret;
}

// Return an AST for an empty list of static int declarations
static_int_decls_t ast_static_int_decls_empty(empty_t e)
{
    static_int_decls_t ret;
    ret.file_loc = file_location_copy(e.file_loc);
    ret.int_decls = NULL;
    return ret;
}

// Return an AST for an empty list of static float declarations
static_float_decls_t ast_static_float_decls_empty(empty_t e)
{
    static_float_decls_t ret;
    ret.file_loc = file_location_copy(e.file_loc);
    ret.float_decls = NULL;
    return ret;
}

// Return an AST for a list of static int declarations
// with sid added to the end of sids
static_int_decls_t ast_static_int_decls_add(static_int_decls_t sids,
					    static_int_decl_t sid)
{
    static_int_decls_t ret = sids;
    static_int_decl_t *p = (static_int_decl_t *)
	                     malloc(sizeof(static_int_decl_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for a static_int_decl!");
    }
    *p = sid;
    p->next = NULL;
    // splice p onto the end of sids.int_decls
    static_int_decl_t *last = ast_last_list_elem(sids.int_decls);
    if (last == NULL) {
	ret.int_decls = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a list of static float declarations
// with sfd added to the end of sfds
static_float_decls_t ast_static_float_decls_add(static_float_decls_t sfds,
						static_float_decl_t sfd)
{
    static_float_decls_t ret = sfds;
    static_float_decl_t *p = (static_float_decl_t *)
	                        malloc(sizeof(static_float_decl_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for a static_float_decl!");
    }
    *p = sfd;
    p->next = NULL;
    // splice p onto the end of sfds.float_decls
    static_float_decl_t *last = ast_last_list_elem(sfds.float_decls);
    if (last == NULL) {
	ret.float_decls = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a static declaration
// found in the file named fn, on line ln, with the given data size,
// identifier, and initializer.
static_int_decl_t ast_static_int_decl(ident_t ident,
				      int_initializer_t iinit)
{
    static_int_decl_t ret;
    ret.file_loc = file_location_copy(ident.file_loc);
    ret.ident = ident;
    ret.value = iinit.value;
    return ret;
}

// Return an AST for a static declaration
// found in the file named fn, on line ln, with the given data size,
// identifier, and initializer.
static_float_decl_t ast_static_float_decl(ident_t ident,
					  float_initializer_t finit)
{
    static_float_decl_t ret;
    ret.file_loc = file_location_copy(ident.file_loc);
    ret.ident = ident;
    ret.value = finit.value;
    return ret;
}


// Return an AST for an initializer with the given int value
int_initializer_t ast_int_initializer(token_t eqs, int_type value)
{
    int_initializer_t ret;
    ret.file_loc = file_location_copy(eqs.file_loc);
    ret.value = value;
    return ret;
}

// Return an AST for an initializer with the given int value
float_initializer_t ast_float_initializer(token_t eqs, float_type value)
{
    float_initializer_t ret;
    ret.file_loc = file_location_copy(eqs.file_loc);
    ret.value = value;
    return ret;
}

// Return an AST for a stack section AST
// with the given keyword and stack bottom address.
stack_section_t ast_stack_section(token_t kw, unsigned int stack_bottom)
{
    stack_section_t ret;
    ret.file_loc = file_location_copy(kw.file_loc);
    ret.stack_bottom_addr = stack_bottom;
    return ret;
}

// Return an AST for an empty int initializer 
int_initializer_t ast_int_initializer_empty(empty_t e)
{
    int_initializer_t ret;
    ret.file_loc = file_location_copy(e.file_loc);
    ret.value = 0;
    return ret;
}

// Return an AST for an empty int initializer 
float_initializer_t ast_float_initializer_empty(empty_t e)
{
    float_initializer_t ret;
    ret.file_loc = file_location_copy(e.file_loc);
    ret.value = 0.0;
    return ret;
}

// Return an AST for empty found in the file named fn, on line ln
empty_t ast_empty(const char *fn, unsigned int ln)
{
    empty_t ret;
    ret.file_loc = file_location_make(fn, ln);
    return ret;
}

// Return an AST for an identifier
// found in the file named fn, on line ln, with the given name.
ident_t ast_ident(const char *fn, unsigned int ln, const char *name)
{
    ident_t ret;
    ret.file_loc = file_location_make(fn, ln);
    ret.name = name;
    return ret;
}

// Return an AST for a (signed) number with the given value
signed_int_t ast_signed_int_lit(token_t sgn, int_type value)
{
    signed_int_t ret;
    ret.file_loc = file_location_copy(sgn.file_loc);
    ret.value = value;
    return ret;
}

// Return an AST for an (unsigned) int literal
// with the given value
unsigned_int_t ast_unsigned_int(const char *fn, unsigned int ln,
			      unsigned int value)
{
    unsigned_int_t ret;
    ret.file_loc = file_location_make(fn, ln);
    ret.value = value;
    return ret;
}

// Return an AST for a float literal with the given value
floatlit_t ast_floatlit(const char *fn, unsigned int ln,
			    float value)
{
    floatlit_t ret;
    ret.file_loc = file_location_make(fn, ln);
    ret.value = value;
    return ret;
}

// Return an AST for a token
token_t ast_token(const char *fn, unsigned int ln, int token_code)
{
    token_t ret;
    ret.file_loc = file_location_make(fn, ln);
    ret.code = token_code;
    return ret;
}    

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Return a pointer to the last element in lst.
// This only returns NULL if lst == NULL.
void *ast_last_list_elem(void *lst)
{
    if (lst == NULL) {
	return lst;
    }
    // assert lst is not NULL
    void *prev = NULL;
    while (lst != NULL) {
	prev = lst;
	lst = ((generic_t *)lst)->next;
    }
    // here lst == NULL;
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
