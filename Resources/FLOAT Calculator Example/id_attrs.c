/* $Id: id_attrs.c,v 1.8 2023/11/11 03:13:54 leavens Exp $ */
// Attributes of identifiers in the symbol table
#include <stdlib.h>
#include <stddef.h>
#include "id_attrs.h"
#include "utilities.h"

// Return a freshly allocated id_attrs struct
// with its field file_loc set to floc, type set to t,
// and its offset_count set to ofst_cnt.
// If there is no space, bail with an error message,
// so this should never return NULL.
id_attrs *id_attrs_loc_create(file_location floc, type_exp_e t,
			      unsigned int ofst_cnt)
{
    id_attrs *ret = (id_attrs *)malloc(sizeof(id_attrs));
    if (ret == NULL) {
	bail_with_error("No space to allocate id_attrs!");
    }
    ret->file_loc = floc;
    ret->type = t;
    ret->offset_count = ofst_cnt;
    return ret;
}
