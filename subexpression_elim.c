#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "printer.h"
#include "subexpression_elim.h"

int expr_is_expr(node_t *n1, node_t *n2) {
  if ( n1->type != n2->type )
    return 0;

  switch(n1->type) {
    case NT_Iconst:
      return n1->iconst == n2->iconst; 
    case NT_Bconst:
      return n1->bconst == n2->bconst;
    case NT_Rconst:
      return n1->rconst == n2->rconst;
    case NT_Cconst:
      return n1->cconst == n2->cconst;
    case NT_Load:{
#warning should work on this more
      return n1->load.unique_id == n2->load.unique_id
              && n1->load.unique_id!=-1;
    }
    case NT_Store:{
 #warning should work on this more
      return n1->store.unique_id == n2->store.unique_id
              && n1->store.unique_id!=-1;
    }
    case NT_Add:
    case NT_Mult:{
      return ( (expr_is_expr(n1->bin.left, n2->bin.left) &&
                expr_is_expr(n1->bin.right, n2->bin.right))
              || (expr_is_expr(n1->bin.left, n2->bin.right) &&
                  expr_is_expr(n1->bin.right, n2->bin.left)));
      break;
    }

    case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod: {
      return (expr_is_expr(n1->bin.left, n2->bin.left) &&
                expr_is_expr(n1->bin.right, n2->bin.right));
    }

    case NT_String:
      assert( 0 && "Not implemented");
    break;

    case NT_Not:
      assert( 0 && "Not implemented");
    break;

    case NT_If:{
      assert(0 && "Should never happen");
      break;
    }
    case NT_BranchZ:{
      assert(0 && "Should never happen");
      break;
    }

    case NT_While:{
      assert(0 && "Should never happen");
      break;
    }
    case NT_For:{
      assert(0 && "Should never happen");
      break;
    }

    case NT_Jump:
    case NT_Nop:
      assert(0 && "Should never happen");
    break;
    default:
      assert(0 && "Unhandled type in tree");

  }
  return 0;
}

void node_expr_eliminate(node_list_t *n)
{
   node_list_t *p;

  for ( p = n->prev; p; p=p->prev )
    if ( expr_is_expr(n->node, p->node) ) {
      
      printf("Ole!\n");
      printf("-----------\n");
      print_instruction(n->node, stdout);
      printf("-----------\n");
      print_instruction(p->node, stdout);
      
      break;  
    }
}

void subexpressions_eliminate(node_list_t *t)
{
  node_list_t *p;

  for (p = t; p; p = p->next)
    node_expr_eliminate(p);
}

