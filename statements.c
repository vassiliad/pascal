#include <stdio.h>
#include <stdlib.h>
#include "expressions.h"
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
  expression_t *_var = expression_variable(var,scope);

  if ( _var == NULL )  {
    printf("statement_assignment: sapio var\n");
    return 0;
  }
  
  assignment = ( statement_t *) calloc(1, sizeof(statement_t));
  assignment->type = ST_Assignment;
  assignment->assignment.type = AT_Expression;
  assignment->assignment.expr = expr;

  return assignment;
}

statement_t *statement_assignment_str(variable_t *var, char *string, scope_t *scope)
{
  statement_t *assignment = NULL;
  expression_t *_var = expression_variable(var,scope);

  if ( _var == NULL )  {
    printf("statement_assignment: sapio var\n");
    return 0;
  }
  
  assignment = ( statement_t *) calloc(1, sizeof(statement_t));
  assignment->type = ST_Assignment;
  assignment->assignment.type = AT_String;
  assignment->assignment.string = string;

  return assignment;
}



