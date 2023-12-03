/* $Id: assemble.h,v 1.5 2023/11/06 09:36:24 leavens Exp $ */
#ifndef _ASSEMBLE_H
#define _ASSEMBLE_H
#include <stdio.h>
#include "ast.h"
#include "bof.h"

// Generate code for prog, with output going to the file out
extern void assembleProgram(BOFFILE bf, program_t prog);

// Assemble the given AST, with output going to bf
extern void assembleTextSection(BOFFILE bf, text_section_t ts);

// Assemble the given AST, with output going to bf
extern void assembleEntryPoint(BOFFILE bf, lora_t ep);

// Assemble the given AST, with output going to bf
extern void assembleLora(BOFFILE bf, lora_t l);

// Assemble the given AST, with output going to bf
extern void assembleAsmInstrs(BOFFILE bf, asm_instrs_t instrs);

// Assemble the given AST, with output going to bf
extern void assembleAsmInstr(BOFFILE bf, asm_instr_t instr);

// Assemble the given AST, with output going to bf
extern void assembleLabelOpt(BOFFILE bf, label_opt_t lopt);

// Assemble the given AST, with output going to bf
extern void assembleInstr(BOFFILE bf, instr_t instr);

// Assemble a register, with output going to bf
extern void assembleReg(BOFFILE bf, short unsigned n);

// Assemble the given AST, with output going to bf
extern void assembleDataSection(BOFFILE bf, data_section_t ds);

// Assemble the given AST, with output going to bf
extern void assembleStaticIntDecls(BOFFILE bf, static_int_decls_t sds);

// Assemble the given AST, with output going to bf
extern void assembleStaticFloatDecls(BOFFILE bf, static_float_decls_t sds);

// Assemble the given AST, with output going to bf
extern void assembleStaticIntDecl(BOFFILE bf, static_int_decl_t dcl);

// Assemble the given AST, with output going to bf
extern void assembleStaticFloatDecl(BOFFILE bf, static_float_decl_t dcl);

#endif
