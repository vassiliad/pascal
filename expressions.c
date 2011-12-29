#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "expressions.h"

/*TODO
  [0] Error checking for the operators
 */

extern int yylineno;

int types_compatible(int type1, int type2)
{
  switch ( type1 ) 
  {
    case VT_Iconst:
    case VT_Integer:
      if ( type2 == VT_Iconst || type2 == VT_Integer )
        return 1;
      else
        return 0;
      break;

    case VT_Rconst:
    case VT_Real:
      if ( type2 == VT_Rconst || type2 == VT_Real )
        return 1;
      else
        return 0;
      break;

    case VT_Cconst:
    case VT_Char:
      if ( type2 == VT_Cconst || type2 == VT_Char ) 
        return 1;
      else
        return 0;
      break;
    case VT_Boolean:
    case VT_Bconst:
      if ( type2 == VT_Boolean || type2 == VT_Bconst )
        return 1;
      else
        return 0;
  }

  return 0;
}


expression_t *expression_set(expressions_t *exprs)
{
  int i;
  expression_t *ret;


  if ( exprs == NULL ) {
    printf("%d) expression_set:: sapio exprs\n", yylineno);
    return 0;
  }


  if ( exprs->size == 0 ) {
    printf("%d) expression_set:: einai adeio\n", yylineno);
    return 0;
  }


  if ( exprs->exprs[0].dataType == VT_User ) {
    printf("%d) expression_set:: Den mporw na exw userTypes mesa sto set\n", yylineno);
    return 0;
  }


  for ( i = 1; i < exprs->size; i ++ ) {
    if ( types_compatible(exprs->exprs[i-1].dataType, exprs->exprs[i].dataType) == 0 ) {
      printf("%d) expression_set:: Prepei na einai symvata ta melh tou set\n", yylineno);
      return 0;
    } 
  }

  ret= ( expression_t * ) calloc(1, sizeof(expression_t));
  ret->type = ET_Set;
  ret->dataType = exprs->exprs->dataType;
  ret->exprs = exprs;

  return ret;
}

expression_t *expression_binary(expression_t* left, expression_t *right, int op)
{
  expression_t *ret;


  if ( !left || !right ) {
    printf("%d expression_binary::sapio left/right\n", yylineno);
    return NULL;
  }

  if ( left->dataType != right->dataType )  {
    printf("%d) expression_binary::sapio datatype (%d-%d) gia praksh %d\n",
        yylineno, left->dataType, right->dataType, op);
    return NULL;
  }


  if ( op == Inop && right->type != ET_Set) {
    printf("%d) expression_binary::inop Sapio deksi melos (%d)\n", yylineno, right->type);
    return NULL;
  }

  ret = (expression_t*) calloc(1, sizeof(expression_t));
  ret->binary.left = left;
  ret->binary.right = right;
  ret->binary.op = op;

  switch ( op )
  {
    case Inop:
    case RelopG:
    case RelopL:
    case RelopLE:
    case RelopD:
      ret->dataType = VT_Boolean;
    break;
    default:
      ret->dataType = left->dataType; // if op is inop then the result is boolean
    break;
  }

  return ret;
}

expression_t *expression_constant(int type, void *data)
{
  expression_t *ret;

  switch ( type ) {
    case VT_Cconst:
      ret = (expression_t*) calloc(1, sizeof(expression_t));
      ret->constant.cconst = *((char*)(data));
      ret->dataType = VT_Char;
      break;

    case VT_Bconst:
      ret = (expression_t*) calloc(1, sizeof(expression_t));
      ret->constant.bconst = *((char*)data);
      ret->dataType = VT_Boolean;
      break;

    case VT_Rconst:
      ret = (expression_t*) calloc(1, sizeof(expression_t));
      ret->constant.rconst = *((float*)data);
      ret->dataType = VT_Real;
      break;

    case VT_Iconst:
      ret = (expression_t*) calloc(1, sizeof(expression_t));
      ret->constant.iconst = *((int*)data);
      ret->dataType = VT_Integer;
      break;

    default:
      printf("%d) Is not a constant\n", yylineno);
      return NULL;
  }
  ret->type = ET_Constant;
  ret->constant.type = type;

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
  //expr->next = NULL;

  return expr;
}


expression_t *expression_variable(variable_t *var, scope_t *scope)
{
  expression_t *ret;
  variable_t *p, *parent;
  var_t *st_var, temp;
  typedefs_entry_t *type;
  int variableType = var->type.dataType;
  int i, found;


  if ( var == NULL ) {
    printf("%d expression_variable sapio var\n", yylineno);
    return NULL;
  }

  // first see if the top-level variable is defined
  st_var = st_var_find(var->id, scope);

  //#warning na to kanw constant? h kanoniko???
  if ( st_var == NULL ) {
    const_t *constant = st_const_find(var->id, scope);

    if ( constant ) {
      st_var = &temp;
      temp.id = constant->id;
      temp.type.dataType = constant->constant.type;
    } else {
      printf("%d: %s is not defined\n", yylineno, var->id);
      return NULL;
    }
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
        if ( !strcasecmp(type->record.ids[i], p->id) ) {
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

int expression_evaluate(expression_t *expr, constant_t *result, scope_t *scope)
{
  constant_t ret, temp1, temp2;

  if ( expr == NULL ) {
    printf("expression_evaluate() expr=NULL\n");
    return Failure;
  }

  switch ( expr->type ) {
    case ET_Constant:
      ret = expr->constant;
      break;

    case ET_Not:
      if ( expression_evaluate(expr->notExpr, &temp1, scope) == Success ) {
        ret = temp1;
        if ( ret.type != VT_Bconst ) {
          return Failure; 
        }
        ret.bconst = !temp1.bconst;
      } else {
        return Failure;
      }
      break;

    case ET_Variable:
		{
			const_t *constant;

			constant = st_const_find(expr->variable->id, scope);

			if ( constant == NULL )
				return Failure;
			
			*result = constant->constant;
			return Success;
		}
		break;

    case ET_Binary:
      if ( expression_evaluate(expr->binary.left, &temp1, scope) != Success ) {
        return Failure;
      }
      if ( expression_evaluate(expr->binary.right, &temp2, scope) != Success ) {
        return Failure;
      }

      ret = temp1;

      switch ( expr->binary.op ) {
        case AddopP:
          if ( temp1.type != temp2.type ) {
            return Failure;
          } 

          switch( ret.type ) {
            case VT_Iconst:
              ret.iconst+= temp2.iconst;
              break;

            case VT_Bconst:
              return Failure;
              break;

            case VT_Cconst:
              ret.cconst += temp2.cconst;
              break;

            case VT_Rconst:
              ret.rconst += temp2.rconst;
              break;
 						
						default:
							return Failure;
						break;
         }
          break;

        case AddopM:
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          switch( ret.type ) {
            case VT_Iconst:
              ret.iconst-= temp2.iconst;
              break;

            case VT_Bconst:
              return Failure;
              break;

            case VT_Cconst:
              ret.cconst -= temp2.cconst;
              break;

            case VT_Rconst:
              ret.rconst -= temp2.rconst;
              break;
 						
						default:
							return Failure;
						break;
         }
          break;

        case RelopG:
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              // ret.bconst = temp1.bconst > temp2.bconst;
              return Failure;
              break;

            case VT_Cconst:
              ret.bconst = temp1.cconst > temp2.cconst;
              break;

            case VT_Iconst:
              ret.bconst = temp1.iconst > temp2.iconst;
              break;

            case VT_Rconst:
              ret.bconst = temp1.rconst > temp2.rconst;
              break;
						
						default:
							return Failure;
						break;
          }

          break;

        case RelopGE:
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              // ret.bconst = temp1.bconst > temp2.bconst;
              return Failure;
              break;

            case VT_Cconst:
              ret.bconst = temp1.cconst >= temp2.cconst;
              break;

            case VT_Iconst:
              ret.bconst = temp1.iconst >= temp2.iconst;
              break;

            case VT_Rconst:
              ret.bconst = temp1.rconst >= temp2.rconst;
              break;
						
						default:
							return Failure;
						break;
          }
          break;

        case RelopL:
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              // ret.bconst = temp1.bconst > temp2.bconst;
              return Failure;
              break;

            case VT_Cconst:
              ret.bconst = temp1.cconst < temp2.cconst;
              break;

            case VT_Iconst:
              ret.bconst = temp1.iconst < temp2.iconst;
              break;

            case VT_Rconst:
              ret.bconst = temp1.rconst < temp2.rconst;
              break;
          						
						default:
							return Failure;
						break;
					}

          break;

        case RelopLE:
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              // ret.bconst = temp1.bconst > temp2.bconst;
              return Failure;
              break;

            case VT_Cconst:
              ret.bconst = temp1.cconst <= temp2.cconst;
              break;

            case VT_Iconst:
              ret.bconst = temp1.iconst <= temp2.iconst;
              break;

            case VT_Rconst:
              ret.bconst = temp1.rconst <= temp2.rconst;
              break;

          	default:
							return Failure;
						break;
					}

          break;

        case RelopD:
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              ret.bconst = temp1.bconst != temp2.bconst;
              return Failure;
              break;

            case VT_Cconst:
              ret.bconst = temp1.cconst != temp2.cconst;
              break;

            case VT_Iconst:
              ret.bconst = temp1.iconst != temp2.iconst;
              break;

            case VT_Rconst:
              ret.bconst = temp1.rconst != temp2.rconst;
              break;
						
						default:
							return Failure;
						break;
          }

          break;

        case Orop:
          if ( temp1.type != VT_Bconst ) {
            return Failure;
          }

          if ( temp2.type != VT_Bconst ) {
            return Failure;
          }

          ret.type = VT_Bconst;
          ret.bconst = temp1.bconst | temp2.bconst;
          break;

        case MuldivandopM:

          //#warning edw mhpws na kanw cast sto megalutero type? ( cconst->iconst->rconst ? )
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret = temp1;

          switch ( temp1.type ) {
            case VT_Bconst:
              return Failure;
              break;

            case VT_Cconst:
              ret.cconst *= temp2.cconst;
              break;

            case VT_Iconst:
              ret.iconst *= temp2.iconst;
              break;

            case VT_Rconst:
              ret.rconst *= temp2.rconst;
              break;
						default:
							return Failure;
						break;
          }


          break;

        case MuldivandopDiv:
          //#warning edw mhpws na kanw cast sto megalutero type? ( cconst->iconst->rconst ? )
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret = temp1;

          switch ( temp1.type ) {
            case VT_Bconst:
              return Failure;
              break;

            case VT_Cconst:
              ret.cconst /= temp2.cconst;
              break;

            case VT_Iconst:
              ret.iconst /= temp2.iconst;
              break;

            case VT_Rconst:
              ret.rconst /= temp2.rconst;
              break;
						default:
							return Failure;
						break;
          }


          break;

        case MuldivandopD:
          //#warning edw mhpws na kanw cast sto megalutero type? ( cconst->iconst->rconst ? )
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret = temp1;

          switch ( temp1.type ) {
            case VT_Bconst:
              return Failure;
              break;

            case VT_Cconst:
              ret.cconst /= temp2.cconst;
              break;

            case VT_Iconst:
              ret.iconst /= temp2.iconst;
              break;

            case VT_Rconst:
              ret.rconst = floor(ret.rconst/temp2.rconst);

              break;
						default: 
							return Failure;
						break;
          }

          break;

        case MuldivandopAnd:
          if ( temp1.type != VT_Bconst ) {
            return Failure;
          }

          if ( temp2.type != VT_Bconst ) {
            return Failure;
          }

          ret.type = VT_Bconst;
          ret.bconst = temp1.bconst & temp2.bconst;

          break;

        case MuldivandopMod:
          //#warning edw mhpws na kanw cast sto megalutero type? ( cconst->iconst->rconst ? )
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret = temp1;

          switch ( temp1.type ) {
            case VT_Bconst:
              return Failure;
              break;

            case VT_Cconst:
              ret.cconst %= temp2.cconst;
              break;

            case VT_Iconst:
              ret.iconst %= temp2.iconst;
              break;

            case VT_Rconst:
              return Failure;
              break;
						default:
							return Failure;
						break;
          }


          break;
        case Inop:
          //#warning NOT IMPLEMENTED
          return Failure;
          break;

        case  Equop:
          if ( temp1.type != temp2.type ) {
            return Failure;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              ret.bconst = temp1.bconst == temp2.bconst;
              return Failure;
              break;

            case VT_Cconst:
              ret.bconst = temp1.cconst == temp2.cconst;
              break;

            case VT_Iconst:
              ret.bconst = temp1.iconst == temp2.iconst;
              break;

            case VT_Rconst:
              ret.bconst = temp1.rconst == temp2.rconst;
              break;
						default:
							return Failure;
						break;
          }


          break;

        default:
          return Failure;
          break;
      }
      break;

    default:
      return Failure;
      break;
  }

  *result = ret;
  return Success;
}

expression_t *expression_call(char*id, expression_t *params, int size, scope_t *scope)
{
  expression_t *call;
  int i = 0;
  func_t *proc = st_func_find(id, scope);

  if ( proc == 0 ) {
    printf("%d) expression_call: procedure %s is not defined\n",yylineno, id);
    return 0;
  }
#warning kati prepei na kanw me tis parametrous kai ta pass
  
  if ( proc->size != size ) {
    printf("%d) expression_call: Call arguments differ in number than the procedure definition for %s (%d,%d)\n",
            yylineno, id, proc->size, size);
    return 0;
  }

  for ( i = 0; i < size; i++ ) {
    if ( params[i].dataType != proc->params[i].type.dataType ) {
      printf("%d) expression_call: Call argument %d differs in type for procedure %s\n", yylineno, i, id);
      return 0;
    } else if ( params[i].dataType == VT_User ) {
      if ( strcasecmp(params[i].variable->type.userType , proc->params[i].type.userType) ) {
        printf("%d) expression_call: Call argument %d differs in userType for procedure %s\n", yylineno, i, id);
        return 0;
      }
    }

    if ( proc->params[i].pass && params[i].type !=ET_Variable ) {
      printf("%d) expression_call: Call argument %d should be a variable for procedure %s, because"
      "it acts as a reference\n", yylineno, i, id);
      return 0;
    }
  }
  
  call = ( expression_t* ) calloc(1, sizeof(expression_t));
  call->call = ( expression_call_t* ) calloc(1, sizeof(expression_call_t));
  call->type = ET_Call;
  call->call->isProcedure=1;
  call->call->type = proc->type;
  call->call->size = size;
  call->call->params = params;

  return call;
}

