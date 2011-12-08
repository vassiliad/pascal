#include <stdio.h>
#include <stdlib.h>
#include "statements.h"
#include "constants.h"


statement_t *statement_if(expression_t *condition, statement_t *_true, statement_t *_false)
{
  statement_t *_if;

  if ( condition == NULL ) {
    printf("statement_if: condition is NULL\n");
    return NULL;
  }
#warning statement_if afhnw na exw null _true/_false, nomizw dn einai 8ema auto

  _if = ( statement_t * ) calloc(1,sizeof(statement_t));
  _if->type = ST_If;
  _if->_if._true = _true;
  _if->_if._false = _false;

  return _if;
}

statement_t *statement_assignment(variable_t *var, expression_t *expr, scope_t *scope)
{
  statement_t *assignment = NULL;

  return assignment;
}

statement_t *statement_assignment_str(variable_t *var, char *string, scope_t *scope)
{
  statement_t *assignment = NULL;

  return assignment;
}



