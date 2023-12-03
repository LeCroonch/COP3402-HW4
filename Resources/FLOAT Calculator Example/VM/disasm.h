// $Id: disasm.h,v 1.5 2023/11/06 10:21:14 leavens Exp $
#ifndef _DISASM_H
#define _DISASM_H
#include "instruction.h"

// Disassemble code from bf,
// with output going to the file out
extern void disasmProgram(FILE *out, BOFFILE bf);

// Disassemble the text section
// with output going to the file out
extern void disasmTextSection(FILE *out, BOFFILE bf, BOFHeader bh);

// Disassemble length instructions from bf
// with output going to the file out
extern void disasmInstrs(FILE *out, BOFFILE bf, int length);

// Disassemble the binary instruction bi, which would go at address i
// each instruction has a label of the form a%d, where %d is the value of i
extern void disasmInstr(FILE *out, bin_instr_t bi, unsigned int i);

// Disassemble the data section from bf, based on the information in bh,
// with output going to out
extern void disasmDataSection(FILE *out, BOFFILE bf, BOFHeader bh);

// Disassemble length static data words from bf, with output going to out
extern void disasmStaticIntDecls(FILE *out, BOFFILE bf, int length);

// Disassemble length static data words from bf, with output going to out
extern void disasmStaticFloatDecls(FILE *out, BOFFILE bf, int length);

// Disassemble the the given word as a static data declaration,
// with output going to out
extern void disasmStaticIntDecl(FILE *out, int_type w);

// Disassemble the the given word as a static data declaration,
// with output going to out
extern void disasmStaticFloatDecl(FILE *out, float_type w);

// Disassemble the stack section based on the given header information
extern void disasmStackSection(FILE *out, BOFHeader bh);

#endif
