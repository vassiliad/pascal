#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "printer.h"
#include "subexpression_elim.h"

#define EXPRS_IDENTICAL(past, cur) do { printf("%p \e[32mIDENTICAL \e[m%p\n", past, cur); \
                                        printf("\033[22;34m");\
                                        print_instruction(past, stdout);\
                                        printf("\033[22;33m");\
                                        print_instruction(cur, stdout);\
                                        printf("\e[m");\
                                        update_node(cur->parent, cur, past);\
                                        return 1;\
                                        } while(0)
#define EXPRS_DIFFERENT(past, cur) do { /*printf("%ld DIFFERENT %ld\n", past->post, cur->post);*/ return 0; } while(0)



enum TraverseMode { TraversePast, TraverseCur };

void update_node(node_t *update, node_t *prev, node_t *new);
int subexpr_eliminate(node_t *past, node_t *cur);

int subexpr_eliminate_children(node_t *past, node_t *cur, enum TraverseMode mode)
{
  // printf("visiting %ld - %ld [%d,%d] [dir: %d]\n", past->post, cur->post, past->type, cur->type, mode);
  assert(past);
  assert(cur);
  // We need to visit every node of the @past tree for every node of @cur tree,
  // we traverse both in post order
  if ( mode == TraverseCur ) {

    switch(cur->type) {
      case NT_Lui:
      case NT_Iconst:
      case NT_Bconst:
      case NT_Rconst:
      case NT_Cconst:
      case NT_Load:
      break;

      case NT_Store:
        subexpr_eliminate_children(past, cur->store.data, TraverseCur);
      break;

      case NT_Add:
      case NT_Mult:
      case NT_Sub:
      case NT_Div:
      case NT_LessThan:
      case NT_Mod: {
        subexpr_eliminate_children(past, cur->bin.left, TraverseCur);
        subexpr_eliminate_children(past, cur->bin.right, TraverseCur);
      }
      break;

      case NT_String:
        assert( 0 && "Not implemented");
        break;

      case NT_Not:
        assert( 0 && "Not implemented");
        break;

      case NT_If:{
        subexpr_eliminate_children(past, cur->_if.branch, TraverseCur);
        subexpressions_eliminate(cur->_if._true);
        subexpressions_eliminate(cur->_if._false);
        break;
      }
      case NT_BranchZ:{
        subexpr_eliminate_children(past, cur->branchz.condition, TraverseCur);
        break;
      }

      case NT_While:{
        subexpr_eliminate_children(past, cur->_while.branch, TraverseCur);
        subexpressions_eliminate(cur->_while.loop);
        break;
      }
      case NT_For:{
        subexpr_eliminate_children(past, cur->_for.init, TraverseCur);
        subexpressions_eliminate(cur->_for.loop);
        break;
      }
      
      case NT_Addi:
      case NT_Ori:
      case NT_Subi:
      case NT_LessThani:
        subexpr_eliminate_children(past, cur->bin_semi.left, TraverseCur);
      break;
      case NT_Jump:
      case NT_Nop:
        break;
      default:
        assert(0 && "Unhandled type in tree");
    }
    subexpr_eliminate_children(past, cur, TraversePast);
  } else {
    switch(past->type) {
      case NT_Lui:
      case NT_Iconst:
      case NT_Bconst:
      case NT_Rconst:
      case NT_Cconst:
      case NT_Load:
        if ( past->type == cur->type )
          return subexpr_eliminate(past, cur);
        return 0;
      break;

      case NT_Store:
        subexpr_eliminate_children(past->store.data, cur, TraversePast);
        if ( past->type == cur->type )
          return subexpr_eliminate(past, cur);
        return 0;
      break;


      case NT_Add:
      case NT_Mult:
      case NT_Sub:
      case NT_Div:
      case NT_LessThan:
      case NT_Mod: {
        subexpr_eliminate_children(past->bin.left, cur, TraversePast);
        subexpr_eliminate_children(past->bin.right, cur, TraversePast);

        if ( past->type == cur->type )
          return subexpr_eliminate(past, cur);
        return 0;
      }
      break;
      
      case NT_Addi:
      case NT_Subi:
      case NT_Ori:
      case NT_LessThani: {
        subexpr_eliminate_children(past->bin_semi.left, cur, TraversePast);

        if ( past->type == cur->type )
          return subexpr_eliminate(past, cur);
        return 0;
      }
      break;


      case NT_String:
        assert( 0 && "Not implemented");
        break;

      case NT_Not:
        assert( 0 && "Not implemented");
        break;

      case NT_If:{
        subexpr_eliminate_children(past->_if.branch, cur, TraversePast);
        break;
      }
      case NT_BranchZ:{
        subexpr_eliminate_children(past->branchz.condition, cur, TraversePast);
        break;
      }

      case NT_While:{
        subexpr_eliminate_children(past->_while.branch, cur, TraversePast);
        break;
      }
      case NT_For:{
        subexpr_eliminate_children(past->_for.init, cur, TraversePast);
        break;
      }
      

      case NT_Jump:
      case NT_Nop:
        break;
      default:
        assert(0 && "Unhandled type in tree");
    }
  }

  return 0;
}

int subexpr_eliminate(node_t *past, node_t *cur) {
  
  if ( past->type != cur->type )
    EXPRS_DIFFERENT(past, cur);

  if ( past == cur )
    EXPRS_IDENTICAL(past, cur);

  switch(past->type) {
    case NT_Lui:
    case NT_Iconst:
      if ( past->iconst == cur->iconst )
        EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);

    case NT_Bconst:
      if (past->bconst == cur->bconst)
        EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);

    case NT_Rconst:
      if ( past->rconst == cur->rconst )
        EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);

    case NT_Cconst:
      if ( past->cconst == cur->cconst )
        EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);

    case NT_Load:{
#warning should work on this more
        if ( past->load.unique_id != -1  && cur->load.unique_id != -1 ) {
          if ( past->load.unique_id == cur->load.unique_id )
            EXPRS_IDENTICAL(past, cur);
        }

        EXPRS_DIFFERENT(past, cur);
    }

    case NT_Store:{
#warning should work on this more
      return 0;
    }
    case NT_Add:
    case NT_Mult:{
      if ( (subexpr_eliminate(past->bin.left, cur->bin.left) &&
             subexpr_eliminate(past->bin.right, cur->bin.right))
           || (subexpr_eliminate(past->bin.left, cur->bin.right) &&
             subexpr_eliminate(past->bin.right, cur->bin.left)))
         EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);
      break;
     }
    
    case NT_Subi:
    case NT_Ori:
    case NT_Addi:
    case NT_LessThani: {
      if ( subexpr_eliminate(past->bin_semi.left, cur->bin_semi.left) &&
           past->bin_semi.immediate == cur->bin_semi.immediate )
        EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);
      break;
    }

    case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod: {
      if (subexpr_eliminate(past->bin.left, cur->bin.left) &&
        subexpr_eliminate(past->bin.right, cur->bin.right))
        EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);
      break;
    }

    case NT_String:
                 assert( 0 && "Not implemented");
                 break;

    case NT_Not:
                 assert( 0 && "Not implemented");
                 break;

    case NT_While:{
      subexpr_eliminate_children(past->_while.branch, cur->_while.branch, TraverseCur);
      break;
    }
    case NT_For:{
      return 0;
      break;
    }

    case NT_If:{
      subexpr_eliminate_children(past->_if.branch, cur->_if.branch, TraverseCur);
      break;
    }

    case NT_BranchZ:{
      assert(0 && "Should never happen");
      break;
    }

    case NT_Jump:
    case NT_Nop:
      break;
    default:
      assert(0 && "Unhandled type in tree");

  }
  return 0;
}

void node_expr_eliminate(node_list_t *n)
{
  node_list_t *p;

  for ( p = n->prev; p; p=p->prev ) {
    subexpr_eliminate_children(p->node, n->node, TraverseCur);
    if ( subexpr_eliminate(p->node, n->node) )
      break;  
  }
}

void subexpressions_eliminate(node_list_t *t)
{
  node_list_t *p;

  for (p = t; p; p = p->next) {
    printf("**** %ld\n", p->node->post);
    node_expr_eliminate(p);
  }
}


void update_node(node_t *update, node_t *prev, node_t *new)
{ 
  
  if ( new == prev )
    return;

  prev->parent = NULL;
  
  if ( update )
    switch(update->type) {
      case NT_Iconst:
      case NT_Bconst:
      case NT_Rconst:
      case NT_Cconst:
      case NT_Load:
        assert(0 && "Should never happen");

      case NT_Store:
        if ( update->store.data == prev )
          update->store.data = new;
        else if ( update->store.address == prev )
          update->store.address = new;
        else
          assert(0);
        new->parent = update;
      break;

      break;

      case NT_Addi:
      case NT_Subi:
      case NT_Ori:
      case NT_LessThani:
        assert( update->bin_semi.left == prev );

        update->bin_semi.left = new;
        new->parent = update;
      break;

      case NT_Add:
      case NT_Mult:
      case NT_Sub:
      case NT_Div:
      case NT_LessThan:
      case NT_Mod: {
        if ( update->bin.left == prev )
          update->bin.left = new;
        else if ( update->bin.right == prev )
          update->bin.right = new;
        else {
          assert(0 && "Should never happen");
        }

        printf("REPLACING %p with %p\n\n\n", prev, new);
        new->parent = update;
      }
      break;

      case NT_String:
        assert( 0 && "Not implemented");
        break;

      case NT_Not:
        assert( 0 && "Not implemented");
        break;

      case NT_If:{
        // assert(0 && "Should never happen");
        break;
      }
      case NT_BranchZ:{
        update->branchz.condition = new;
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
    
  switch(prev->type) {
    case NT_Iconst:
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:
    case NT_Load:
    break;

    case NT_Store:
      if ( prev->store.data == prev )
        prev->store.data->parent = new;
      else if ( prev->store.address == prev )
        prev->store.address->parent = new;
      else
        assert(0);
    break;

    break;

    case NT_Add:
    case NT_Mult:
    case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod: {
      prev->bin.left->parent = new;
      prev->bin.right->parent = new;
    }
    break;

    case NT_String:
      assert( 0 && "Not implemented");
      break;

    case NT_Not:
      assert( 0 && "Not implemented");
      break;

    case NT_If:{
      // assert(0 && "Should never happen");
      break;
    }
    case NT_BranchZ:{
      prev->branchz.condition->parent = new;
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
 
}

