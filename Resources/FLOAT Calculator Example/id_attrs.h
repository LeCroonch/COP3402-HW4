/* $Id: id_attrs.h,v 1.9 2023/11/11 03:13:54 leavens Exp $ */
#ifndef _ID_ATTRS_H
#define _ID_ATTRS_H
#include "ast.h"
#include "type_exp.h"
#include "file_location.h"

// attributes of identifiers
// attributes of identifiers
typedef struct {
    file_location file_loc;
    type_exp_e type;
    // num. of variable decls before
    // this one in this scope
    unsigned int offset_count;
} id_attrs;

// Allocate and return an id_attrs struct
// with field file_loc set to floc,
// var_type set to vt,
// and offset_count set to ofst_cnt.
// This should never return NULL.
extern id_attrs *id_attrs_loc_create(
        file_location floc, type_exp_e t,
        unsigned int ofst_cnt);
#endif
