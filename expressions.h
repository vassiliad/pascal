#ifndef __EXPRESSIONS_H
#define __EXPRESSIONS_H
#include "bison_union.h"
#include "constants.h"
#include "symbol_table.h"

expression_t *expression_binary(expression_t* left, expression_t *right, int op);
expression_t *expression_constant(int type, void *data);
expression_t *expression_not(expression_t* notExpr);
expression_t *expression_variable(variable_t *var, const scope_t *scope);

#endif // __EXPRESSIONS_H
