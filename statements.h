#ifndef __STATEMENTS_H
#define __STATEMENTS_H
#include "symbol_table.h"
#include "bison_union.h"

statement_t *statement_if(expression_t *condition, statement_t *_true, statement_t *_false);
statement_t *statement_while(expression_t *condition, statement_t *loop);
statement_t *statement_assignment(variable_t *var, expression_t *expr, scope_t *scope);
statement_t *statement_assignment_str(variable_t *var, char *string, scope_t *scope);
statement_t *statement_for(char *id, iter_space_t *iter_space, statement_t *loop, scope_t *scope);

#endif // __STATEMENTS_H
