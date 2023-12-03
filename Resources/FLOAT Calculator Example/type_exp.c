// $Id: type_exp.c,v 1.2 2023/11/11 19:26:37 leavens Exp $
#include "type_exp.h"
#include "utilities.h"

// Return a string representing the type_exp given
const char *type_exp_string(type_exp_e t)
{
    switch (t) {
    case float_te:
	return "float";
	break;
    case bool_te:
	return "bool";
	break;
    default:
	bail_with_error("Unknown type_exp_e (%d) passed to type_exp_string!",
			t);
	break;
    }
    // the following should never happen
    return "UnknownType!";
}
