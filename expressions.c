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
  }

  return 0;
}


expression_t *expression_set(expressions_t *exprs)
{
  expression_t *expr;
  expressions_t *p;
  int i;
  int type;

  if ( exprs == NULL ) {
    printf("%d) expression_set:: sapio exprs\n", yylineno);
    return 0;
  }

  p = exprs->exprs;
  
  if ( p->size == 0 ) {
    printf("%d) expression_set:: einai adeio\n", yylineno);
    return 0;
  }
  
  type = p->exprs[0].dataType;
  if ( type == VT_User ) {
    printf("%d) expression_set:: Den mporw na exw userTypes mesa sto set\n", yylineno);
    return 0;
  }
  

  for ( i = 1; i < p->size; i ++ ) {
    if ( types_compatible(type, p->exprs[i].dataType) == 0 ) {
      printf("%d) expression_set:: Prepei na einai idia ta melh tou set\n");
      return 0;
    }
  }
  
  expr = ( expression_t * ) calloc(1, sizeof(expression_t));
  expr->dataType = type;
  expr->exprs = exprs;

  return expr;
}

expression_t *expression_binary(expression_t* left, expression_t *right, int op)
{
  expression_t *ret;
  

  if ( !left || !right ) {
    printf("%d expression_binary::sapio left/right\n", yylineno);
    return NULL;
  }

  if ( op != Inop ) {
    
    if ( left->dataType != right->dataType )  {
      printf("%d) expression_binary::sapio datatype (%d-%d) gia praksh %d\n",yylineno, left->dataType, right->dataType, op);
      return NULL;
    }

    ret = (expression_t*) calloc(1, sizeof(expression_t));
    ret->dataType = left->dataType;
    ret->binary.left = left;
    ret->binary.right = right;
    ret->binary.op = op;

  } else {
    int i = 0;

    if ( right->type != ET_Set) {
      printf("%d) expression_binary::inop Sapio deksi melos\n", yylineno);
      return NULL;
    }


    for ( i = 0 ; i < right->exprs->size; i++ ) {
    }
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
  //ret->next = NULL;

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
  int i, j, found;


  if ( var == NULL ) {
    printf("%d expression_variable sapio var\n", yylineno);
    return NULL;
  }

  // first see if the top-level variable is defined
  st_var = st_var_find(var->id, scope);

#warning na to kanw constant? h kanoniko???
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

int expression_evaluate(expression_t *expr, constant_t *result)
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
      if ( expression_evaluate(expr->notExpr, &temp1) == Success ) {
        ret = temp1;
        if ( ret.type != VT_Bconst ) {
          printf("expression_evaluate: NOT operator is only applied to BCONST\n");
          return 0;
        }
        ret.bconst = !temp1.bconst;
      } else {
        printf("expression_evaluate: Invalid notExpr\n");
        return 0;
      }
      break;

    case ET_Variable:
      printf("Constant expressions cannot use non-constant variables\n");
      return 0;
      break;

    case ET_Binary:
      if ( expression_evaluate(expr->binary.left, &temp1) != Success ) {
        printf("expression_evaluate: Binary expression has invalid left operand\n");
        return 0;
      }
      if ( expression_evaluate(expr->binary.right, &temp2) != Success ) {
        printf("expression_evaluate: Binary expression has invalid right operand\n");
        return 0;
      }

#warning Should we allow bconst+iconst ? etc


      ret = temp1;

      switch ( expr->binary.op ) {
        case AddopP:
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          } 

          switch( ret.type ) {
            case VT_Iconst:
              ret.iconst+= temp2.iconst;
              break;

            case VT_Bconst:
              printf("expression_evaluate: BCONST + BCONST is invalid\n");
              return 0;
              break;

            case VT_Cconst:
              ret.cconst += temp2.cconst;
              break;

            case VT_Rconst:
              ret.rconst += temp2.rconst;
              break;
          }
          break;

        case AddopM:
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          switch( ret.type ) {
            case VT_Iconst:
              ret.iconst-= temp2.iconst;
              break;

            case VT_Bconst:
              printf("expression_evaluate: BCONST - BCONST is invalid\n");
              return 0;
              break;

            case VT_Cconst:
              ret.cconst -= temp2.cconst;
              break;

            case VT_Rconst:
              ret.rconst -= temp2.rconst;
              break;
          }
          break;

        case RelopG:
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              // ret.bconst = temp1.bconst > temp2.bconst;
#warning prepei na koitaw booleans gia REL
              printf("expression_evaluate: Cannot compare booleans for RelopG\n");
              return 0;
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
          }

          break;

        case RelopGE:
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              // ret.bconst = temp1.bconst > temp2.bconst;
#warning prepei na koitaw booleans gia REL
              printf("expression_evaluate: Cannot compare booleans for RelopGE\n");
              return 0;
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
          }
          break;

        case RelopL:
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              // ret.bconst = temp1.bconst > temp2.bconst;
#warning prepei na koitaw booleans gia REL
              printf("expression_evaluate: Cannot compare booleans for RelopL\n");
              return 0;
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
          }

          break;

        case RelopLE:
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              // ret.bconst = temp1.bconst > temp2.bconst;
#warning prepei na koitaw booleans gia REL
              printf("expression_evaluate: Cannot compare booleans for RelopLE\n");
              return 0;
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
          }

          break;

        case RelopD:
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              ret.bconst = temp1.bconst != temp2.bconst;
              return 0;
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
          }

          break;

        case Orop:
          if ( temp1.type != VT_Bconst ) {
            printf("expression_evaluate: OROP's left operand must be VT_Bconst\n");
            return 0;
          }

          if ( temp2.type != VT_Bconst ) {
            printf("expression_evaluate: OROP's right operand must be VT_Bconst\n");
            return 0;
          }

          ret.type = VT_Bconst;
          ret.bconst = temp1.bconst | temp2.bconst;
          break;

        case MuldivandopM:

#warning edw mhpws na kanw cast sto megalutero type? ( cconst->iconst->rconst ? )
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret = temp1;

          switch ( temp1.type ) {
            case VT_Bconst:
              printf("expression_evaluate: MuldivandopM cannot have BCONST as operands\n");
              return 0;
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
          }


          break;

        case MuldivandopDiv:
#warning edw mhpws na kanw cast sto megalutero type? ( cconst->iconst->rconst ? )
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret = temp1;

          switch ( temp1.type ) {
            case VT_Bconst:
              printf("expression_evaluate: MuldivandopDiv cannot have BCONST as operands\n");
              return 0;
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
          }


          break;

        case MuldivandopD:
#warning edw mhpws na kanw cast sto megalutero type? ( cconst->iconst->rconst ? )
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret = temp1;

          switch ( temp1.type ) {
            case VT_Bconst:
              printf("expression_evaluate: MuldivandopDiv cannot have BCONST as operands\n");
              return 0;
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
          }

          break;

        case MuldivandopAnd:
          if ( temp1.type != VT_Bconst ) {
            printf("expression_evaluate: MuldivandopAnd's left operand must be VT_Bconst\n");
            return 0;
          }

          if ( temp2.type != VT_Bconst ) {
            printf("expression_evaluate: MuldivandopAnd's right operand must be VT_Bconst\n");
            return 0;
          }

          ret.type = VT_Bconst;
          ret.bconst = temp1.bconst & temp2.bconst;

          break;

        case MuldivandopMod:
#warning edw mhpws na kanw cast sto megalutero type? ( cconst->iconst->rconst ? )
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret = temp1;

          switch ( temp1.type ) {
            case VT_Bconst:
              printf("expression_evaluate: MuldivandopMod cannot have BCONST as operands\n");
              return 0;
              break;

            case VT_Cconst:
              ret.cconst %= temp2.cconst;
              break;

            case VT_Iconst:
              ret.iconst %= temp2.iconst;
              break;

            case VT_Rconst:
              printf("expression_evaluate: MuldivandopMod cannot have either operands as RCONST\n");
              return 0;
              break;
          }


          break;
        case Inop:
#warning NOT IMPLEMENTED
          printf("expression_evaluate: Inop is not implemented yet\n");
          return 0;
          break;

        case  Equop:
          if ( temp1.type != temp2.type ) {
            printf("expression_evaluate: Missmatching left/right operand types\n");
            return 0;
          }

          ret.type = VT_Bconst;

          switch ( temp1.type ) {
            case VT_Bconst:
              ret.bconst = temp1.bconst == temp2.bconst;
              return 0;
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
          }


          break;

        default:
          printf("expression_evaluate: unknown binary operator\n");
          return 0;
          break;
      }
      break;

    default:
      printf("Unknown expression time in expression_evaluate\n");
      return 0;
      break;
  }

  *result = ret;
  return Success;
}

