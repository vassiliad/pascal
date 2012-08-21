#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "subexpression_elim.h"

void node_expr_eliminate(node_t *n)
{
  switch(n->type) {
    case NT_Iconst: 
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:{
      break;
    }
    case NT_Load:{
      //givepostnumbers(start->load.address);
      break;
    }
    case NT_Store:{
      // givepostnumbers(start->load.data); 
      // givepostnumbers(start->load.address);
      break;
    }
    case NT_Add:
    case NT_Sub:
    case NT_Div:
    case NT_Mult:
    case NT_Mod:{
      // givepostnumbers(start->bin.left);
      // givepostnumbers(start->bin.right);
      break;
    }
    case NT_String:
      assert( 0 && "Not implemented");
    break;

    case NT_Not:
      assert( 0 && "Not implemented");
    break;

    case NT_If:{
      // givepostnumbers(start->_if.branch);
      break;
    }
    case NT_BranchZ:{
      // givepostnumbers(start->branchz.condition);
      break;
    }
    case NT_LessThan:{
      // givepostnumbers(start->bin.left);
      // givepostnumbers(start->bin.right);
      break;
    }
    case NT_While:{
      // givepostnumbers(start->_while.branch);
      break;
    }
    case NT_For:{
      // givepostnumbers(start->_for.init);
      // givepostnumbers(start->_for.branch);
      break;
    }

    case NT_Jump:
    case NT_Nop:
    break;
    default:
      assert(0 && "Unhandled type in tree");

  }
}

void subexpressions_eliminate(node_list_t *t)
{
  node_list_t *p;

  for (p = t; p; p = t->next)
    node_expr_eliminate(p->node);
}

