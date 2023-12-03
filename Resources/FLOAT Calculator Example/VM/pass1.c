// $Id: pass1.c,v 1.3 2023/11/06 09:36:24 leavens Exp $
#include <stddef.h>
#include "pass1.h"
#include "symtab.h"
#include "utilities.h"

// Build the symbol table and check for duplicate declarations in the given AST
void pass1(program_t progast)
{
    symtab_initialize();
    pass1TextSection(progast.textSection);
    pass1DataSection(progast.dataSection);
}

// Check the given AST and put its declarations in the symbol table
void pass1TextSection(text_section_t ts)
{
    pass1AsmInstrs(ts.instrs);
}

// Check the given AST and put its declarations in the symbol table
void pass1AsmInstrs(asm_instrs_t instrs)
{
    asm_instr_t *ip = instrs.instrs;
    address_type count = 0;
    while (ip != NULL) {
	pass1AsmInstr(*ip, count);
	ip = ip->next;
	count++;
    }
}

// Check the given AST and put its declarations in the symbol table
void pass1AsmInstr(asm_instr_t instr, address_type count)
{
    pass1LabelOpt(instr.label_opt, count);
}

// Check the given AST and put its declarations in the symbol table
void pass1LabelOpt(label_opt_t lopt, address_type count)
{
    if (lopt.name != NULL) {
	id_attrs attrs;
	if (symtab_declared(lopt.name)) {
	    bail_with_error("Duplicate declaration of label \"%s\"",
			    lopt.name);
	}
	attrs.kind = id_label;
	attrs.addr = count;
	symtab_insert(lopt.name, attrs);
    }
}

// Check the given AST and put its declarations in the symbol table
void pass1DataSection(data_section_t ds)
{
    pass1StaticIntDecls(ds.staticIntDecls);
    pass1StaticFloatDecls(ds.staticFloatDecls);
}

// Check the given AST and put its declarations in the symbol table
void pass1StaticIntDecls(static_int_decls_t sds)
{
    address_type offset = 0;
    static_int_decl_t *dcl = sds.int_decls;
    while (dcl != NULL) {
	pass1StaticIntDecl(*dcl, offset);
	offset = offset + BYTES_PER_WORD;
	dcl = dcl->next;
    }
}

// Check the given AST and put its declarations in the symbol table
void pass1StaticFloatDecls(static_float_decls_t sfds)
{
    address_type offset = 0;
    static_float_decl_t *dcl = sfds.float_decls;
    while (dcl != NULL) {
	pass1StaticFloatDecl(*dcl, offset);
	offset = offset + BYTES_PER_WORD;
	dcl = dcl->next;
    }
}

// Check the given AST and put its declarations in the symbol table
void pass1StaticIntDecl(static_int_decl_t dcl, address_type offset)
{
    pass1Ident(dcl.ident, id_int, offset);
}

// Check the given AST and put its declarations in the symbol table
void pass1StaticFloatDecl(static_float_decl_t dcl, address_type offset)
{
    pass1Ident(dcl.ident, id_float, offset);
}

// Check the given AST and put its declarations in the symbol table
void pass1Ident(ident_t id, id_attr_kind iak, address_type offset)
{
    if (symtab_declared(id.name)) {
	bail_with_error("Duplicate declaration of data name \"%s\"",
			id.name);
    }
    id_attrs attrs;
    attrs.kind = iak;
    attrs.file_loc = file_location_copy(id.file_loc);
    attrs.addr = offset;
    symtab_insert(id.name, attrs);
}

// Print, on out, the header line for the printed output
// followed by a newline
static void pass1_print_header(FILE *out)
{
    fprintf(out, "%-5s %s\t%s\n", "Kind", "Name", "Address");
}

static const char *id_attr_kind_2_string(id_attr_kind k) {
    switch (k) {
    case id_label:
	return "Label";
	break;
    case id_int:
	return "INT";
	break;
    case id_float:
	return "FLOAT";
	break;
    default:
	bail_with_error("Unknown id_attr_kind in id_attr_kind_2_string (%d)",
			k);
	break;
    }
    return NULL;
}

// Requires: symtab_declared(name);
// Print, on out, a line with the symbol table's information about name
// followed by a newline
static void pass1_print_name_info(FILE *out, const char *name)
{
    id_attrs na, *p;
    p = symtab_lookup(name);
    if (p == NULL) {
	bail_with_error("pass1_print_name_info given NULL pointer!");
    } else {
	na = *p;
    }
    fprintf(out, "%-5s %s\t%u\n", id_attr_kind_2_string(na.kind),
	    na.name, na.addr);
}

// Requires: out is a character file (like stdout)
//           that is open for writing
// Print debugging information about the symbol table on out
void pass1_print(FILE *out)
{
    pass1_print_header(out);
    const char *name = name = symtab_first_name();
    while (symtab_more_after(name)) {
	pass1_print_name_info(out, name);
	name = symtab_next_name(name);
    }
}
