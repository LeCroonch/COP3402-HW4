/* $Id: id_use.h,v 1.4 2023/11/11 19:26:37 leavens Exp $ */
#ifndef _ID_USE_H
#define _ID_USE_H
#include "type_exp.h"
#include "id_attrs.h"
#include "lexical_address.h"

// An id_use struct gives information from
// looking up name in the symbol table:
// the attributes (id_attrs *),
// and the number of lexical levels out
// from the current scope,
// where the name was declared.
typedef struct id_use_s {
    id_attrs *attrs;
    unsigned int levelsOutward;    
} id_use;

// Requires: attrs != NULL
// Allocate and return an  id_use struct
// containing attrs and levelsOut.
extern id_use *id_use_create(
           id_attrs *attrs,
           unsigned int levelsOut);

// Requires: idu != NULL
// Return a pointer to the attributes of this id_use
// and the result will never be NULL
extern id_attrs *id_use_get_attrs(id_use *idu);

// Requires: idu != NULL
// Return (a pointer to) the lexical address for idu.
extern lexical_address *id_use_2_lexical_address(id_use *idu);
#endif
