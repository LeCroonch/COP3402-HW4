// $Id: type_exp.h,v 1.1 2023/11/11 03:14:02 leavens Exp $
#ifndef _VAR_TYPE_H
#define _VAR_TYPE_H

// typeExp ::= unknown | float | bool
typedef enum {unknown_te, float_te, bool_te} type_exp_e;

// Return a string representing the var_type given
extern const char *type_exp_string(type_exp_e st);

#endif
