/* $Id: ast.h,v 1.26 2023/11/06 09:36:24 leavens Exp $ */
#ifndef _AST_H
#define _AST_H
#include <stdbool.h>
#include "machine_types.h"
#include "file_location.h"
#include "instruction.h"

// forward declaration, so can use the type AST* below
typedef union AST_u AST;

// The following types for structs named N_t
// are used in the declaration of the AST_u union below.
// The struct N_t is the type of information kept in the AST
// that is related to the nonterminal N in the abstract syntax.

// The generic struct type (generic_t) has the fields that
// should be in all alternatives of the union AST_u
typedef struct {
    file_location *file_loc;
    void *next; // for lists
} generic_t;

// empty ::=
typedef struct {
    file_location *file_loc;
} empty_t;

// label ::= ident
typedef struct {
    file_location *file_loc;
    const char *name;
} ident_t;

// (signed) ints (and offsets)
typedef struct {
    file_location *file_loc;
    const char *text;
    int_type value;
} signed_int_t;

// unsigned int literals
typedef struct {
    file_location *file_loc;
    const char *text;
    unsigned int value;
} unsigned_int_t;

// float literals
typedef struct {
    file_location *file_loc;
    const char *text;
    float value;
} floatlit_t;

// registers
typedef struct {
    file_location *file_loc;
    const char *text;
    unsigned short number;
} reg_t;

// tokens as ASTs
typedef struct {
    file_location *file_loc;
    const char *text;
    int code;
} token_t;


// label-opt ::= label | empty
typedef struct {
    file_location *file_loc;
    const char *name;  // NULL if there was no label
} label_opt_t;

// lora ::= label | unsigned-int
typedef struct {
    file_location *file_loc;
    bool address_defined;
    const char *label; // NULL if address_defined
    address_type addr; // 0 if !address_defined
} lora_t;

// kinds of immediate data
typedef enum { id_number, id_unsigned, id_lora, id_syscall_code, id_empty
} immed_data_kind_t;

// immed-data ::= number | unsigned-int | lora | empty
typedef struct {
    immed_data_kind_t id_data_kind;
    union {
	int immed;
	unsigned int uimmed;
	lora_t lora;
	syscall_type syscall_code;
	int none;
    } data;
} immedData_t;

// kinds of immediate operands
typedef enum {ik_immed, ik_uimmed, ik_offset, ik_syscall_code, ik_none
} immed_kind_t;

// instr ::= three-reg-instr | two-reg-instr | shift-instr | one-reg-instr
//         | immed-arith-instr | immed-bool-instr | branch-test-instr
//         | load-store-instr | jump-instr
typedef struct {
    file_location *file_loc;
    instr_type itype;
    const char *opname;
    int opcode;
    func_type func; // function code for register instructions
    unsigned short regs_used;
    unsigned short regs[3];
    immed_kind_t immed_kind;
    immedData_t immed_data;
} instr_t;

// asmInstr ::= label-opt instr
typedef struct asm_instr_s {
    file_location *file_loc;
    struct asm_instr_s *next;  // for lists
    label_opt_t label_opt;
    instr_t instr;
} asm_instr_t;

// asmInstrs ::= asmInstr asmInstr*
typedef struct {
    file_location *file_loc;
    asm_instr_t *instrs;
} asm_instrs_t;

// intInitializer ::= number
typedef struct {
    file_location *file_loc;
    int_type value;
} int_initializer_t;

// floatInitializer ::= floatlit
typedef struct {
    file_location *file_loc;
    float_type value;
} float_initializer_t;

// staticIntDecl ::= INT ident signed-int
typedef struct static_int_decl_s {
    file_location *file_loc;
    struct static_int_decl_s *next;  // for lists
    ident_t ident;
    int_type value;
} static_int_decl_t;

// staticIntDecl ::= FLOAT ident float-lit
typedef struct static_float_decl_s {
    file_location *file_loc;
    struct static_float_decl_s *next;  // for lists
    ident_t ident;
    float_type value;
} static_float_decl_t;

// staticIntDecls ::= staticIntDecl*
typedef struct {
    file_location *file_loc;
    static_int_decl_t *int_decls;
} static_int_decls_t;

// staticFloatDecls ::= staticFloatDecl*
typedef struct {
    file_location *file_loc;
    static_float_decl_t *float_decls;
} static_float_decls_t;

// text-section ::= entry-point asmInstr*
typedef struct {
    file_location *file_loc;
    lora_t entryPoint;
    asm_instrs_t instrs;
} text_section_t;

// data-section ::= static-int-decls static-float-decls
typedef struct {
    file_location *file_loc;
    address_type static_start_addr;
    static_int_decls_t staticIntDecls;
    static_float_decls_t staticFloatDecls;
} data_section_t;

// stack-section ::= .stack stack-bottom-addr
typedef struct {
    file_location *file_loc;
    address_type stack_bottom_addr;
} stack_section_t;

// program ::= text-section data-section
typedef struct {
    file_location *file_loc;
    text_section_t textSection;
    data_section_t dataSection;
    stack_section_t stackSection;
} program_t;

// The actual AST definition:
typedef union AST_u {
    generic_t generic;
    program_t program;
    text_section_t text_section;
    lora_t lora;
    asm_instrs_t asm_instrs;
    asm_instr_t asm_instr;
    label_opt_t label_opt;
    empty_t empty;
    instr_t instr;
    data_section_t data_section;
    static_int_decls_t static_int_decls;
    static_float_decls_t static_float_decls;
    static_int_decl_t static_int_decl;
    static_float_decl_t static_float_decl;
    int_initializer_t int_initializer;
    float_initializer_t float_initializer;
    stack_section_t stack_section;
    ident_t ident;
    signed_int_t signed_int;
    unsigned_int_t unsigned_int;
    floatlit_t floatlit;
    reg_t reg;
    token_t token;
} AST;

// Return the filename from the AST t
extern const char *ast_filename(AST t);

// Return the line number from the AST t
extern unsigned int ast_line(AST t);

// Return a pointer to a fresh copy of t
// that has been allocated on the heap
AST *ast_heap_copy(AST t);

// Return an AST for a program,
// which contains the given ASTs.
extern program_t ast_program(text_section_t textSec, data_section_t dataSec,
			     stack_section_t stackSec);

// Return an AST for the text section
// with the given entry point and instructions.
extern text_section_t ast_text_section(token_t tok, lora_t entryPoint,
				       asm_instrs_t instrs);

// Return an AST for an entry point
// that represents the label given
extern lora_t ast_lora_label(ident_t label);

// Return an AST for an entry point
// that represents the address given
extern lora_t ast_lora_addr(unsigned_int_t addr);

// Return an AST for an asm instr AST
// with the given label and instruction
extern asm_instr_t ast_asm_instr(label_opt_t label, instr_t instr);

// Return an AST for a singleton asm instrs AST
// with the given instruction
extern asm_instrs_t ast_asm_instrs_singleton(asm_instr_t asminstr);

// Return an AST made from adding the given asm instr to the end of lst
extern asm_instrs_t ast_asm_instrs_add(asm_instrs_t lst, asm_instr_t asminstr);

// Return an AST for a label_opt AST
// with the given identifier
extern label_opt_t ast_label_opt_label(ident_t id);

// Return an AST for a label_opt AST
// with the empty AST (t)
extern label_opt_t ast_label_opt_empty(empty_t t);

// Return an AST for a signed number, which has the same value as t
extern signed_int_t ast_offset_unsigned(unsigned_int_t t);

// Return an immediate data holding an int
extern immedData_t ast_immed_number(int n);

// Return an immediate data holding an unsigned int
extern immedData_t ast_immed_unsigned(unsigned int u);

// Return an immediate data holding an label or address
extern immedData_t ast_immed_lora(lora_t a);

// Return an immediate data that is nothing
extern immedData_t ast_immed_none();

// Return an AST for an instruction
// with the given information
extern instr_t ast_instr(token_t op, instr_type itype,
			 unsigned short num_regs_used,
			 unsigned short reg1, unsigned short reg2,
			 unsigned short reg3, func_type func,
			 immed_kind_t ik, immedData_t im);

// Return an AST for the data section AST
// with the given lists of static declarations.
extern data_section_t ast_data_section(token_t kw, unsigned int static_start,
				       static_int_decls_t staticIntDecls,
				       static_float_decls_t staticFloatDecls);

// Return an AST for an empty list of static int declarations
extern static_int_decls_t ast_static_int_decls_empty(empty_t e);

// Return an AST for an empty list of static float declarations
extern static_float_decls_t ast_static_float_decls_empty(empty_t e);

// Return an AST for a list of static int declarations
// with sid added to the end of sids
extern static_int_decls_t ast_static_int_decls_add(static_int_decls_t sids,
						   static_int_decl_t sid);

// Return an AST for a list of static float declarations
// with sfd added to the end of sfds
extern static_float_decls_t ast_static_float_decls_add(
					       static_float_decls_t sfds,
					       static_float_decl_t sfd);

// Return an AST for a static declaration of an int
// found in the file named fn, on line ln, with the given data size,
// identifier, and initializer.
extern static_int_decl_t ast_static_int_decl(ident_t ident,
					     int_initializer_t iinit);

// Return an AST for a static declaration of a float
// found in the file named fn, on line ln, with the given data size,
// identifier, and initializer.
extern static_float_decl_t ast_static_float_decl(ident_t ident,
						 float_initializer_t finit);

// Return an AST for an initializer with the given int value
extern int_initializer_t ast_int_initializer(token_t eqs, int_type value);

// Return an AST for an initializer with the given float value
extern float_initializer_t ast_float_initializer(token_t eqs, float_type value);

// Return an AST for an empty int initializer 
extern int_initializer_t ast_int_initializer_empty(empty_t e);

// Return an AST for an empty float initializer 
extern float_initializer_t ast_float_initializer_empty(empty_t e);

// Return an AST for a stack section AST
// with the given keyword and stack bottom address.
extern stack_section_t ast_stack_section(token_t kw, unsigned int stack_bottom);

// Return an AST for empty found in the file named fn, on line ln
extern empty_t ast_empty(const char *fn, unsigned int ln);

// found in the file named fn, on line ln, with the given name.
extern ident_t ast_ident(const char *fn, unsigned int ln, const char *name);

// Return an AST for a (signed) integer with the given value
extern signed_int_t ast_signed_int_lit(token_t sgn, int_type value);

// Return an AST for an unsigned int literal with the given value
extern unsigned_int_t ast_unsigned_int(const char *fn, unsigned int ln,
				      unsigned int value);

// Return an AST for a float literal with the given value
extern unsigned_int_t ast_float_lit(const char *fn, unsigned int ln,
				      float value);

// Return an AST for a token
extern token_t ast_token(const char *fn, unsigned int ln, int token_code);

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Return a pointer to the last element in lst.
// This only returns NULL if lst == NULL.
extern void *ast_last_list_elem(void *lst);

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Return the number of elements in the linked list lst
extern int ast_list_length(void *lst);

#endif
