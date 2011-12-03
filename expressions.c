#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
  expression_t *ret;
  variable_t *p, *parent;
  var_t *st_var;
  typedefs_entry_t *type;
  int variableType = var->type.dataType;
  int i, j, found;


  if ( var == NULL ) {
    printf("%d expression_variable sapio var\n", yylineno);
    return NULL;
  }

  // first see if the top-level variable is defined
  st_var = st_var_find(var->id, scope);


  if ( st_var == NULL ) {
    printf("%d: %s is not defined\n", yylineno, var->id);
    return NULL;
  }

  var->type = st_var->type;

  // go all the way to the end of the variable expression and figure out if it's valid
  for ( parent=var, p = var->child; p; parent=p, p = p->child ) {
    if ( p->id ) {
      /*
         In case the previous level was a record, get the typedef description from the symbol table
         assert that p->id exists in the record, and update the variableType as well as st_var
         For every variable_t node that is visited we also update its data_type_t structure
       */

      if ( parent->type.dataType != VT_User ) {
        printf( "%d: %s Is not a record (%d)\n", yylineno, parent->id, parent->type.dataType);
        return NULL;
      }

      type = st_typedef_find(parent->type.userType, scope);
      if ( type == NULL ) {
        printf("%d: Typedef %s hasn't been defined\n", yylineno, st_var->type.userType);
        return NULL;
      }      

      if ( type->type != TT_Record ) {
        printf("%d: Typedef %s is not a record\n", yylineno, type->name);
        return 0;
      }

      found = 0;
      for ( i = 0; i < type->record.size && !found; i ++ )
        for ( j=0; j < type->record.ids[i].size && !found; j ++ ) 
          if ( !strcasecmp(type->record.ids[i].ids[j], p->id) ) {
            found = 1;
            p->type.dataType = type->record.types[i].dataType;
            p->type.userType = type->record.types[i].userType;
            variableType = type->record.types[i].dataType;
          }

      if ( found == 0 ) {
        printf("%d: Field %s is not part of type %s\n", yylineno, p->id, type->name);
        return 0;
      }

    } else if ( p->expr.size ) { 
       /*
         This is an array, first we make sure the type is defined as an array
         then we update the variable's datatype as well as current variableType
         then we assert that the limits given match the definition's dimensions
       */

      if ( parent->type.dataType != VT_User ) {
        printf("%d: Field %s is not an array .\n", yylineno, parent->id);
        return NULL;
      }
      type = st_typedef_find(parent->type.userType, scope);
      if ( type == NULL || type->type ) {
        printf("%d: Type %s is not defined\n", yylineno, parent->type.userType);
        return NULL;
      }

      if ( type->type != TT_Array ) {
        printf("%d: Type %s is not an array\n", yylineno, parent->type.userType);
        return NULL;
      }

      if ( type->array.dims.size != p->expr.size ) {
        printf("%d: Given limits (%d) do not match the defined dimensions of the array (%d).\n",
            yylineno, p->expr.size, type->array.dims.size);
        return NULL;
      }

      for ( i = 0; i < p->expr.size; i++ ) {
        /*
           TODO
           Can the check be done in compile time?
           if ( type->array.dims.limits[i].isRange ) {
           } else {

           }  
         */

         switch ( p->expr.exprs[i].dataType )
         {
          case VT_Char:
          case VT_Boolean:
          case VT_Integer:
          break;
          default:
            printf("%d: Invalid type for limit\n", yylineno);
            return NULL;
          break;
         }
      }
        p->type=parent->type = type->array.typename;
        variableType = parent->type.dataType;
        
    } else {
      printf("%d: Unknown variable\n", yylineno);
      return NULL;
    }
  }


  ret = calloc(1, sizeof(expression_t));
  ret->type = ET_Variable;
  ret->variable = var;
  ret->dataType = variableType;

  return ret;
}

