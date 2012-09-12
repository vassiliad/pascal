#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "printer.h"
#include "subexpression_elim.h"

#define EXPRS_IDENTICAL(past, cur) \
  do { \
    printf("%p \e[32mIDENTICAL \e[m%p\n", past, cur); \
    printf("\033[22;34m");\
    print_instruction(past, stdout);\
    printf("\033[22;33m");\
    print_instruction(cur, stdout);\
    printf("\e[m");\
    if ( cur != past )\
      update_node(cur->parent, cur, past);\
    return 1;\
  } while(0)

#define EXPRS_DIFFERENT(past, cur) do { /*printf("%ld DIFFERENT %ld\n", past->post, cur->post);*/ return 0; } while(0)


int subexpr_traverse_past(node_t *past, node_t *cur);
int subexpr_traverse_cur(node_t *past, node_t *cur);
void update_node(node_t *update, node_t *prev, node_t *new);
int subexpr_eliminate(node_t *past, node_t *cur);

int subexpr_traverse_past(node_t *past, node_t *cur)
{
  switch(past->type) {
    case NT_Lui:
    case NT_Iconst:
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:
    break;

    case NT_Load:
      if ( past->load.address )
        subexpr_traverse_past(past->load.address, cur);
      break;

    case NT_Store:
      if ( past->store.address )
        subexpr_traverse_past(past->store.address, cur);
      subexpr_traverse_past(past->store.data, cur);
      break;

    case NT_Addi:
    case NT_Subi:
    case NT_Ori:
    case NT_LessThani:
      subexpr_traverse_past(past->bin_semi.left, cur);
      break;

    case NT_Add:
    case NT_Mult:
    case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod:
      subexpr_traverse_past(past->bin.left, cur);
      subexpr_traverse_past(past->bin.right, cur);
      break;

    case NT_String:
      assert( 0 && "Not implemented");
      break;

    case NT_Not:
      subexpr_traverse_past(past->not, cur);
      break;

    case NT_If:
      subexpr_traverse_past(past->_if.branch->branchz.condition, cur);
      break;
    case NT_BranchZ:
      subexpr_traverse_past(past->branchz.condition, cur);
      break;

    case NT_While:
      subexpr_traverse_past(past->_while.branch->branchz.condition, cur);
      break;

    case NT_For:
      return 0;
      break;

    case NT_Jump:
    case NT_Nop:
      return 0;
    default:
      assert(0);
  }

  return subexpr_eliminate(past, cur);
}

int subexpr_traverse_cur(node_t *past, node_t *cur)
{
  /*
     Reach the leaves of @cur first, then traverse @past and try to find out curs that can
     share leaves.
   */

  switch(cur->type) {
    case NT_Lui:
    case NT_Iconst:
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:
      return 0;

    case NT_Load:
      if ( cur->load.address )
        subexpr_traverse_cur(past, cur->load.address);
      break;

    case NT_Store:
      if ( cur->store.address )
        subexpr_traverse_cur(past, cur->store.address);
      subexpr_traverse_cur(past, cur->store.data);
      break;

    case NT_Addi:
    case NT_Subi:
    case NT_Ori:
    case NT_LessThani:
      subexpr_traverse_cur(past, cur->bin_semi.left);
      break;

    case NT_Add:
    case NT_Mult:
    case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod:
      subexpr_traverse_cur(cur->bin.left, cur->bin.right); // attempt to find out common
          // subexpressions in the same statement
      if ( subexpr_eliminate(cur->bin.left, cur->bin.right) )
        update_node(cur, cur->bin.left, cur->bin.right);

      subexpr_traverse_cur(past, cur->bin.left);
      subexpr_traverse_cur(past, cur->bin.right);
      break;

    case NT_String:
      assert( 0 && "Not implemented");
      break;

    case NT_Not:
      subexpr_traverse_cur(past, cur->not);
      break;

    case NT_If:
      subexpr_traverse_cur(past, cur->_if.branch->branchz.condition);
      break;
    case NT_BranchZ:
      subexpr_traverse_cur(past, cur->branchz.condition);
      break;

    case NT_While:
      subexpr_traverse_cur(past, cur->_while.branch->branchz.condition);
      break;

    case NT_For:
      return 0;
      break;

    case NT_Jump:
    case NT_Nop:
      return 0;
    default:
      assert(0);
  }

  return subexpr_traverse_past(past, cur);
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

    case NT_Load:
#warning should work on this more
      if ( past->load.unique_id != -1  && cur->load.unique_id != -1 ) {
        if ( past->load.unique_id == cur->load.unique_id )
          EXPRS_IDENTICAL(past, cur);
      }

      EXPRS_DIFFERENT(past, cur);
      break;

    case NT_Store:
#warning should work on this more
      return 0;

    case NT_Add:
    case NT_Mult:
      if ( (subexpr_eliminate(past->bin.left, cur->bin.left) &&
            subexpr_eliminate(past->bin.right, cur->bin.right))
          || (subexpr_eliminate(past->bin.left, cur->bin.right) &&
            subexpr_eliminate(past->bin.right, cur->bin.left)))
        EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);
      break;

    case NT_Subi:
    case NT_Ori:
    case NT_Addi:
    case NT_LessThani: 
      if ( subexpr_eliminate(past->bin_semi.left, cur->bin_semi.left) &&
          past->bin_semi.immediate == cur->bin_semi.immediate )
        EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);
      break;


    case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod: 
      if (subexpr_eliminate(past->bin.left, cur->bin.left) &&
          subexpr_eliminate(past->bin.right, cur->bin.right))
        EXPRS_IDENTICAL(past, cur);
      else
        EXPRS_DIFFERENT(past, cur);
      break;


    case NT_String:
      assert( 0 && "Not implemented");
      break;

    case NT_Not:
      assert( 0 && "Not implemented");
      break;

    case NT_While:
      subexpr_traverse_cur(past->_while.branch, cur->_while.branch);
      break;

    case NT_For:
      return 0;
      break;


    case NT_If:
      subexpr_traverse_cur(past->_if.branch, cur->_if.branch);
      break;


    case NT_BranchZ:
      assert(0 && "Should never happen");
      break;


    case NT_Jump:
    case NT_Nop:
      break;
    default:
      assert(0 && "Unhandled type in tree");

  }
  return 0;
}

void node_intra_expr(node_t* n)
{
  if ( n == NULL )
    return;
  
  switch ( n->type ) {
    case NT_Add:
    case NT_Mult:
    case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod:
      subexpr_traverse_cur(n->bin.left, n->bin.right); // attempt to find out common
          // subexpressions in the same statement
      if ( subexpr_eliminate(n->bin.left, n->bin.right) )
        update_node(n, n->bin.left, n->bin.right);
      break;
    
    default:

    break;
  }

}

void node_expr_eliminate(node_list_t *n)
{
  node_list_t *p;
  node_t *cur = n->node;

  switch(cur->type) {

    case NT_If:
      node_intra_expr(cur->_if.branch);

      subexpressions_eliminate(cur->_if._true);
      subexpressions_eliminate(cur->_if._false);
      break;
    case NT_BranchZ:
      node_intra_expr(cur->branchz.condition);
      break;

    case NT_While:
      node_intra_expr(cur->_while.branch);

      subexpressions_eliminate(cur->_while.loop);
      break;

    case NT_For:
      return;
      break;

    default:
    break;
  }

  for ( p = n->prev; p; p=p->prev ) {
    subexpr_traverse_cur(p->node, cur);
    if ( subexpr_eliminate(p->node, cur) )
      return;  
  }
  
  if ( cur->type == NT_Store ) {
    node_intra_expr(cur->store.address);
    node_intra_expr(cur->store.data);
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
  if ( update == NULL )
    assert(0);

  if ( new == prev )
    return;


  if ( update )
    switch(update->type) {
      case NT_Iconst:
      case NT_Bconst:
      case NT_Rconst:
      case NT_Cconst:
        assert(0);

      case NT_Load:
        assert(update->load.address == prev);

        update->load.address = new;
        new->parent = update;
        break;

      case NT_Store:
        if ( update->store.data == prev )
          update->store.data = new;
        else if ( update->store.address == prev )
          update->store.address = new;
        else {
          printf("%p -- %p,%p (%p-%p)\n", update, update->store.address, update->store.data, prev, new);
          print_instruction(update, stdout);
          assert(0);
        }
        new->parent = update;
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
      case NT_Mod: 
        if ( update->bin.left == prev )
          update->bin.left = new;
        else if ( update->bin.right == prev )
          update->bin.right = new;
        else {
          assert(0 && "Should never happen");
        }

        printf("REPLACING %p with %p\n\n\n", prev, new);
        new->parent = update;

        break;

      case NT_String:
        assert( 0 && "Not implemented");
        break;

      case NT_Not:
        assert( 0 && "Not implemented");
        break;

      case NT_If:
        assert(update->_if.branch->branchz.condition == prev);
        update->_if.branch->branchz.condition = new;

        new->parent = update->_if.branch;
        break;

      case NT_BranchZ:
        assert(update->branchz.condition == prev);
        update->branchz.condition = new;

        new->parent = update;
        break;


      case NT_While:
        assert(update->_while.branch->branchz.condition == prev);
        update->_while.branch->branchz.condition = new;

        new->parent = update->_while.branch;
        break;

      case NT_For:
        assert(0 && "Should never happen");
        break;


      case NT_Jump:
      case NT_Nop:
        assert(0 && "Should never happen");
        break;
      default:
        assert(0 && "Unhandled type in tree");
    }

}

