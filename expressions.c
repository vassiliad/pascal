#include <stdlib.h>
#include <stdio.h>
#include "expressions.h"


/*TODO
	[0] Error checking for the operators
*/
extern int yylineno;

expression_t *expression_binary(expression_t* left, expression_t *right, int op)
{
	expression_t *ret;
	
	if ( !left || !right ) {
		printf("%d expression_binary::sapio left/right\n", yylineno);
		return NULL;
	}

	if ( left->dataType != right->dataType )  {
		printf("expression_binary::sapio datatype (%d-%d)\n", left->dataType, right->dataType);
		return NULL;
	}

	ret = (expression_t*) calloc(1, sizeof(expression_t));
	
	ret->dataType = left->dataType;
	ret->binary.left = left;
	ret->binary.right = right;
	ret->binary.op = op;
	ret->next = NULL;

	return ret;
}

expression_t *expression_constant(int type, void *data)
{
	expression_t *ret;

	switch ( type ) {
		case CT_Cconst:
			ret = (expression_t*) calloc(1, sizeof(expression_t));
			ret->constant.cconst = *((char*)(data));
			ret->dataType = VT_Char;
		break;

		case CT_Bconst:
			ret = (expression_t*) calloc(1, sizeof(expression_t));
			ret->constant.bconst = *((char*)data);
			ret->dataType = VT_Boolean;
		break;

		case CT_Rconst:
			ret = (expression_t*) calloc(1, sizeof(expression_t));
			ret->constant.rconst = *((float*)data);
			ret->dataType = VT_Real;
		break;

		case CT_Iconst:
			ret = (expression_t*) calloc(1, sizeof(expression_t));
			ret->constant.iconst = *((int*)data);
			ret->dataType = VT_Integer;
		break;

		default:
		return NULL;
	}
	ret->type = ET_Constant;
	ret->constant.type = type;
	ret->next = NULL;

	return ret;
}

expression_t *expression_not(expression_t *notExpr)
{
	expression_t *expr;

	if ( !notExpr ) {
		printf("expression_not::sapio notExpr\n");
		return NULL;
	}

	expr = (expression_t*) calloc(1, sizeof(expression_t));
	expr->type = ET_Not;
	expr->notExpr = notExpr;
	expr->next = NULL;

	return expr;
}


expression_t *expression_variable(variable_t *var, const scope_t *scope)
{
	/*TODO prepei na mpei edw o pinakas sumvolwn gia na mou pei an ontws
	einai dhlwmenh auth h metavlhth, kai ti typou einai wste na perasw
	auth thn plhroforia ena epipedo parapanw sto ( ret->dataType ) me th logikh
	na mporoun na ginoun elegxoi kata th dhmiourgia twn praksewn
	*/
	expression_t *ret;
  variable_t *p;
  var_t *st_var;
  char *id;

	if ( var == NULL ) {
		printf("%d expression_variable sapio var\n", yylineno);
		return NULL;
	}
//TODO DEN doulevei gia records
#warning fix for records
  
  for ( p = var; p->parent; p=p->parent );

  
  st_var = st_var_find(p->id, scope);

  if ( st_var == NULL ) {
    printf("%d: %s is not defined\n", yylineno, var->id);
    return NULL;
  }

	ret = calloc(1, sizeof(expression_t));
	ret->type = ET_Variable;
	ret->variable = var;

	return ret;
}
