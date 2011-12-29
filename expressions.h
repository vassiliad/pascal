#ifndef __EXPRESSIONS_H
#define __EXPRESSIONS_H
#include "bison_union.h"
#include "constants.h"
#include "symbol_table.h"

expression_t *expression_binary(expression_t* left, expression_t *right, int op);
expression_t *expression_constant(int type, void *data);
expression_t *expression_not(expression_t* notExpr);
expression_t *expression_variable(variable_t *var, scope_t *scope);
expression_t *expression_set(expressions_t *expr);
expression_t *expression_call(char*id, expression_t *params, int size, scope_t *scope);

int expression_evaluate(expression_t *expr, constant_t *result, scope_t *scope);
int types_compatible(int type1, int type2);

#endif // __EXPRESSIONS_H
