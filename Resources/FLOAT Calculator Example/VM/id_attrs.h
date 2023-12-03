/* $Id: id_attrs.h,v 1.2 2023/11/06 09:36:24 leavens Exp $ */
#ifndef _ID_ATTRS_H
#define _ID_ATTRS_H

#include "machine_types.h"
#include "file_location.h"

// Kinds of names in the assembly language
typedef enum {id_label, id_int, id_float} id_attr_kind;

// attributes of a name in the assembly language
typedef struct {
    const char *name;
    id_attr_kind kind;
    file_location *file_loc;
    address_type addr;  // offset from start of text or data section
} id_attrs;

#endif
