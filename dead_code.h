#ifndef __DEAD_CODE_H
#define __DEAD_CODE_H
#include "statements.h"
#include "symbol_table.h"

statement_t *dead_code_elimination(statement_t *root, scope_t *scope);

#endif // __DEAD_CODE_H
