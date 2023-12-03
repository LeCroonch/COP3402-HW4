/* $Id: asm_unparser.h,v 1.6 2023/11/06 09:36:24 leavens Exp $ */
#ifndef _ASM_UNPARSER_H
#define _ASM_UNPARSER_H
#include <stdio.h>
#include "ast.h"

// Unparse prog, with output going to the file out
extern void unparseProgram(FILE *out, program_t prog);

// Unparse the given AST, with output going to out
extern void unparseTextSection(FILE *out, text_section_t ts);

// Unparse the given AST, with output going to out
extern void unparseEntryPoint(FILE *out, lora_t ep);

// Unparse the given AST, with output going to out
extern void unparseLora(FILE *out, lora_t l);

// Unparse the given AST, with output going to out
extern void unparseAsmInstrs(FILE *out, asm_instrs_t instrs);

// Unparse the given AST, with output going to out
extern void unparseAsmInstr(FILE *out, asm_instr_t instr);

// Unparse the given AST, with output going to out
extern void unparseLabelOpt(FILE *out, label_opt_t lopt);

// Unparse the given AST, with output going to out
extern void unparseInstr(FILE *out, instr_t instr);

// Unparse a register, with output going to out
extern void unparseReg(FILE *out, unsigned short n);

// Unparse the given AST, with output going to out
extern void unparseDataSection(FILE *out, data_section_t ds);

// Unparse the given AST, with output going to out
extern void unparseStaticIntDecls(FILE *out, static_int_decls_t sids);

// Unparse the given AST, with output going to out
extern void unparseStaticFloatDecls(FILE *out, static_float_decls_t sfds);

// Unparse the given AST, with output going to out
extern void unparseStaticIntDecl(FILE *out, static_int_decl_t dcl);

// Unparse the given AST, with output going to out
extern void unparseStaticFloatDecl(FILE *out, static_float_decl_t dcl);

// Unparse the given static declaration, with output going to out
extern void unparseIntStaticDecl(FILE *out, ident_t id,
				 int_initializer_t iinit);

// Unparse the given static declaration, with output going to out
extern void unparseFloatStaticDecl(FILE *out, ident_t id,
				   float_initializer_t finit);

// Unparse the given AST, with output going to out
extern void unparseIdent(FILE *out, ident_t id);


// Unparse the given AST, with output going to out
extern void unparseIntInitializer(FILE *out, int_initializer_t init);

// Unparse the given AST, with output going to out
extern void unparseFloatInitializer(FILE *out, float_initializer_t init);

// Unparse the given AST, with output going to out
extern void unparseStackSection(FILE *out, stack_section_t ss);

#endif
