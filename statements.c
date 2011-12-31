#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressions.h"
#include "statements.h"
#include "constants.h"

extern int yylineno;

statement_t *statement_if(expression_t *condition, statement_t *_true, statement_t *_false)
{
  statement_t *_if;

  if ( condition == NULL ) {
    printf("statement_if: condition is NULL\n");
    return NULL;
  }

  if ( types_compatible(condition->dataType,  VT_Boolean ) == 0 ) {
    printf("statement_if: condition should be either VT_Boolean or VT_Bconst not %d\n", condition->dataType);
    return 0;
  }

  _if = ( statement_t * ) calloc(1,sizeof(statement_t));
  _if->type = ST_If;
  _if->_if._true = _true;
  _if->_if._false = _false;
  _if->_if.condition = condition;
  return _if;
}

statement_t *statement_while(expression_t *condition, statement_t *loop)
{
  statement_t *_while;

  if ( condition == NULL ) {
    printf("statement_while: condition is NULL\n");
    return NULL;
  }

  if ( types_compatible(condition->dataType,  VT_Boolean ) == 0 ) {
    printf("statement_while: condition should be either VT_Boolean or VT_Bconst not %d\n", condition->dataType);
    return 0;
  }


  _while = ( statement_t * ) calloc(1,sizeof(statement_t));
  _while->type = ST_While;
  _while->_while.loop = loop;
  _while->_while.condition = condition;

  return _while;
}

statement_t *statement_assignment(variable_t *var, expression_t *expr, scope_t *scope)
{
  statement_t *assignment = NULL;
  expression_t *_var = expression_variable(var,scope);
	var_t *st_var;

  if ( _var == NULL )  {
    printf("statement_assignment: sapio var\n");
    return NULL;
  }
	
	// If this is a read only variable it will be inserted in the symbol table

	st_var = st_var_find(var->id, scope);

	if ( st_var && st_var->readOnly ) {
		printf("[-] Cannot assign value to %s because it is read-only in this scope\n", var->id);
		return NULL;
	}
  
  assignment = ( statement_t *) calloc(1, sizeof(statement_t));
  assignment->type = ST_Assignment;
  assignment->assignment.var = var;
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
    return NULL;
  }
  
  assignment = ( statement_t *) calloc(1, sizeof(statement_t));
  assignment->type = ST_Assignment;
  assignment->assignment.type = AT_String;
  assignment->assignment.string = string;

  return assignment;
}

statement_t *statement_for(char *id, iter_space_t *iter_space, statement_t *loop, scope_t *scope)
{
  statement_t *_for;
  var_t *var;

  if ( iter_space == NULL) {
    printf("%d) statement_for: iter_space was not provided\n", yylineno);
    return NULL;
  }

  if ( iter_space->from == NULL ) {
    printf("%d) statement_for: invalid iter_space.from\n", yylineno);
    return NULL;
  }
 
  if ( iter_space->to == NULL) {
    printf("%d) statement_for: invalid iter_space.to\n", yylineno);
    return NULL;
  }

  if ( types_compatible(iter_space->from->dataType, VT_Integer ) == 0) {
    printf("%d) statement_for: iter_space.from must be integer/iconst\n", yylineno);
    return NULL;
  }

  if ( types_compatible(iter_space->to->dataType, VT_Integer ) == 0 ) {
    printf("%d) statement_for: iter_space.to must be integer/iconst\n", yylineno);
    return NULL;
  }
  
  var = st_var_find(id, scope);

  if ( var == NULL ) {
    printf("%d) statement_for: loop-var is not defined\n", yylineno);
    return NULL;
  }

  if ( var->type.dataType != VT_Integer ) {
    printf("%d) statement_for: loop-var is not an integer\n", yylineno);
    return NULL;
  }
  
  if ( iter_space->type != FT_To && iter_space->type != FT_DownTo ) {
    printf("%d) statement_for: kati paei poly strava , to iter_space->type einai sapio\n", yylineno);
    return NULL;
  }
  
  _for = ( statement_t * ) calloc(1, sizeof(statement_t));
  _for->type = ST_For;
  _for->_for.var = var;
  _for->_for.loop = loop;
  _for->_for.from = iter_space->from;
  _for->_for.to = iter_space->to;
	_for->_for.type = iter_space->type;

	printf("Setting readOnly for %s\n", var->id);
	var->readOnly = 1;

  return _for;
}

statement_t *statement_call(char*id, expression_t *params, int size, scope_t *scope)
{
  statement_t *call;
  int i = 0;
  func_t *func = st_func_find(id, scope);

  if ( func == NULL ) {
    printf("%d) statement_call: Function %s is not defined\n",yylineno, id);
    return 0;
  }
#warning kati prepei na kanw me tis parametrous kai ta pass
  
  if ( func->size != size ) {
    printf("%d) statement_call: Call arguments differ in number than the function definition for %s\n", yylineno, id);
    return 0;
  }

  for ( i = 0; i < size; i++ ) {
    if ( params[i].dataType != func->params[i].type.dataType ) {
      printf("%d) statement_call: Call argument %d differs in type for function %s\n", yylineno, i, id);
      return 0;
    } else if ( params[i].dataType == VT_User ) {
      if ( strcasecmp(params[i].variable->type.userType , func->params[i].type.userType) ) {
        printf("%d) statement_call: Call argument %d differs in userType for function %s\n", yylineno, i, id);
        return 0;
      }
    }

    if ( func->params[i].pass && params[i].type !=ET_Variable ) {
      printf("%d) statement_call: Call argument %d should be a variable for function %s, because"
      "it acts as a reference\n", yylineno, i, id);
      return 0;
    }
  }
  
  call = ( statement_t* ) calloc(1, sizeof(statement_t));
  call->type = ST_Call;
  call->call.type = func->type;
  call->call.size = size;
  call->call.params = params;

  return call;
}

statement_t *statement_with(variable_t *var, statement_t *statement, scope_t *scope)
{
  statement_t *with;
  expression_t *variable;
  typedefs_entry_t *type;
  int i = 0;

  variable = expression_variable(var, scope);
  
  if ( variable == NULL ) {
    printf("statement_with: With-variable is not defined\n");
    return 0;
  }
  
  if ( variable->variable->type.dataType != VT_User ) {
    printf("statement_with: With-variable should be a record (%d)\n", variable->dataType);
    return 0;
  }

  type = st_typedef_find( variable->variable->type.userType, scope);

  if ( type == NULL ) {
    printf("statement_with: With-variable's type ( %s ) is not defined\n",
      variable->variable->type.userType);
    return 0;
  }

  for ( i = 0 ; i < type->record.size; i ++ )

      if ( st_var_define(type->record.ids[i], type->record.types[i], scope) == 0 ) {
        printf("statement_with: Could not register %s\n", type->record.ids[i]);
        return 0;
      }

  with = ( statement_t* ) calloc(1, sizeof(statement_t));
  with->type = ST_With;
  with->with.statement = statement;
  with->with.var= var;

  return with;
}

const char* statement_type_to_string(statement_t *statement)
{
  static const char types[][15] = { "If", "While", "For", "Call", "Assignment", "Case", "With" };
  
  if ( statement == NULL )
    return "null";

  if ( ST_If > statement->type || statement->type > ST_With )
    return "invalid statement type";

  return types[ statement->type ];
}

