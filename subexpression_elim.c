#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "printer.h"
#include "subexpression_elim.h"

#define EXPRS_IDENTICAL(...) return 1
#define EXPRS_DIFFERENT(...) return 0

/*#define EXPRS_IDENTICAL(n1, n2) \
  do { \
    printf("%p \e[32mIDENTICAL \e[m%p\n", n1, n2); \
    printf("\033[22;34m");\
    print_instruction(n1, stdout);\
    printf("\033[22;33m");\
    print_instruction(n2, stdout);\
    printf("\e[m");\
    if ( n2 != n1 )\
      update_node(n2->parent, n2, n1);\
    return 1;\
  } while(0)

#define EXPRS_DIFFERENT(n1, n2) do { printf("%ld DIFFERENT %ld\n", n1->post, n2->post); return 0; } while(0)
*/
void update_node(node_t *cur, node_t *old)
{
	static int me = 0;
	printf("me :%d \n",me++);	
	node_t *parent;
	
  assert(old);
	assert(cur);
	
	
  parent = cur->parent;
	
	assert(parent);
	
  
	switch(parent->type) {
		case NT_Iconst:
		case NT_Bconst:
		case NT_Rconst:
		case NT_Cconst:
			assert(0);

		case NT_Load:
			assert(parent->load.address == cur);

			parent->load.address = old;
			old->parent = parent;
			
			break;

		case NT_Store:
			if ( parent->store.data == cur ){
				parent->store.data = old;
				printf("visited witch success 0 %d\n",me-1);
			}
			else if ( parent->store.address == cur ){
				parent->store.address = old;
				printf("visited witch success 1 %d\n",me-1);
				
			}
			else {
				printf("%p -- %p,%p (%p-%p)\n", parent, parent->store.address, parent->store.data, cur, old);
				print_instruction(parent, stdout);
				assert(0);
			}
			old->parent = parent;
			break;

		case NT_Addi:
		case NT_Subi:
		case NT_Ori:
		case NT_LessThani:
			assert( parent->bin_semi.left == cur );

			parent->bin_semi.left = old;
			old->parent = parent;
			break;

		case NT_Add:
		case NT_Mult:
		case NT_Sub:
		case NT_Div:
		case NT_LessThan:
		case NT_Mod:
			if ( parent->bin.left == cur )
				parent->bin.left = old;
			else if ( parent->bin.right == cur )
				parent->bin.right = old;
			else {
				assert(0 && "Should never happen");
			}

			printf("REPLACING %p with %p\n\n\n", cur, old);
			old->parent = parent;

			break;

		case NT_String:
			assert( 0 && "Not implemented");
			break;

		case NT_Not:
			assert( 0 && "Not implemented");
			break;

		case NT_If:
			assert(parent->_if.branch->branchz.condition == cur);
			parent->_if.branch->branchz.condition = old;

			old->parent = parent->_if.branch;
			break;

		case NT_BranchZ:
			assert(parent->branchz.condition == cur);
			parent->branchz.condition = old;

			old->parent = parent;
			break;


		case NT_While:
			assert(parent->_while.branch->branchz.condition == cur);
			parent->_while.branch->branchz.condition = old;

			old->parent = parent->_while.branch;
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
	delete_tree(cur);
}

int identical(node_t *n1, node_t *n2) {
	if ( n1 == NULL || n2 == NULL )
			EXPRS_DIFFERENT(n1, n2);
	
  if ( n1->type != n2->type )
    EXPRS_DIFFERENT(n1, n2);

  if ( n1 == n2 )
    EXPRS_IDENTICAL(n1, n2);

  switch(n1->type) {
    case NT_Lui:
    case NT_Iconst:
      if ( n1->iconst == n2->iconst )
        EXPRS_IDENTICAL(n1, n2);
      else
        EXPRS_DIFFERENT(n1, n2);

    case NT_Bconst:
      if (n1->bconst == n2->bconst)
        EXPRS_IDENTICAL(n1, n2);
      else
        EXPRS_DIFFERENT(n1, n2);

    case NT_Rconst:
      if ( n1->rconst == n2->rconst )
        EXPRS_IDENTICAL(n1, n2);
      else
        EXPRS_DIFFERENT(n1, n2);

    case NT_Cconst:
      if ( n1->cconst == n2->cconst )
        EXPRS_IDENTICAL(n1, n2);
      else
        EXPRS_DIFFERENT(n1, n2);

    case NT_Load:
#warning should work on this more
      if ( n1->load.is_scalar && n2->load.is_scalar ) {
        if ( n1->load.unique_id == n2->load.unique_id )
          EXPRS_IDENTICAL(n1, n2);
      }

      EXPRS_DIFFERENT(n1, n2);
      break;

    case NT_Store:
#warning should work on this more
      return 0;

    case NT_Add:
    case NT_Mult:
      if ( (identical(n1->bin.left, n2->bin.left) &&
            identical(n1->bin.right, n2->bin.right))
          || (identical(n1->bin.left, n2->bin.right) &&
            identical(n1->bin.right, n2->bin.left)))
        EXPRS_IDENTICAL(n1, n2);
      else
        EXPRS_DIFFERENT(n1, n2);
      break;

    case NT_Subi:
    case NT_Ori:
    case NT_Addi:
    case NT_LessThani:
      if ( identical(n1->bin_semi.left, n2->bin_semi.left) &&
          n1->bin_semi.immediate == n2->bin_semi.immediate )
        EXPRS_IDENTICAL(n1, n2);
      else
        EXPRS_DIFFERENT(n1, n2);
      break;


    case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod:
      if (identical(n1->bin.left, n2->bin.left) &&
          identical(n1->bin.right, n2->bin.right))
        EXPRS_IDENTICAL(n1, n2);
      else
        EXPRS_DIFFERENT(n1, n2);
      break;


    case NT_String:
      assert( 0 && "Not implemented");
      break;

    case NT_Not:
      assert( 0 && "Not implemented");
      break;

    case NT_While:
      identical(n1->_while.branch, n2->_while.branch);
      break;

    case NT_For:
      return 0;
      break;


    case NT_If:
      identical(n1->_if.branch, n2->_if.branch);
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


int check_tree(node_t *old, node_t *cur) {
	
	if ( identical(old, cur) ){
		update_node(cur, old);
		return 1;
	}
	

	if ( old == NULL || cur == NULL )
		return 0;

	switch( old->type ) {
		case NT_Iconst:
		case NT_Lui:
				return 0;
				
    case NT_Load:
			if ( check_tree(old->load.address, cur) ) {
				// update_node(cur, old->load.address);
				return 1;
			}
      break;

    case NT_Store:
			if ( check_tree(old->store.address, cur) ) {
			//	update_node(cur, old->store.address);
				return 1;
			} else if ( check_tree(old->store.data, cur)) {
			//	update_node(cur, old->store.data);
				return 1;
			}
      break;

    case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod:
		case NT_Add:
    case NT_Mult:
      if ( check_tree(old->bin.left, cur)) {
			//	update_node(cur, old->bin.left);
				return 1;
			}
      
      if ( check_tree(old->bin.right, cur) ) {
			//	update_node(cur, old->bin.right);
				return 1;
			}
      break;

    case NT_Subi:
    case NT_Ori:
    case NT_Addi:
    case NT_LessThani:
      if ( check_tree(old->bin_semi.left, cur)) {
			//	update_node(cur, old->bin_semi.left);
				return 1;
			} 
			break;

    case NT_Not:
      if ( check_tree(old->not, cur) )  {
			//	update_node(cur, old->not);
				return 1;
			} 
      break;

    case NT_While:
      if ( check_tree(old->_while.branch, cur) )  {
			//	update_node(cur, old->bin.right);
				return 1;
			} 
      break;

    case NT_For:
      return 0;
      break;


    case NT_If:
      if ( check_tree(old->_if.branch, cur) )  {
			//	update_node(cur, old->_if.branch);
				return 1;
			} 
      break;


    case NT_BranchZ:
      if ( check_tree(old->branchz.condition, cur) )  {
			//	update_node(cur, old->branchz.condition);
				return 1;
			}
      break;


    case NT_Jump:
    case NT_Nop:
      break;
    default:
      printf("TYPOS OLD: %d\n\n\n\n", old->type);
			return 0;
	}
	return 0;
}

int eliminate_stmt(node_list_t *start, node_t *cur){
	node_list_t *c;
	if ( cur == NULL )
		return 0;
	
	for(c=start->prev ; c!=NULL ; c= c->prev)
		if (check_tree(c->node, cur) ) {
			return 1;
		}
	
	// edw exw checkarei ta panta gia emena
	switch( cur->type ) {
		case NT_Addi:
		case NT_Subi:
		case NT_Ori:
		case NT_LessThani:
			eliminate_stmt(start, cur->bin_semi.left);
			break;
		
		case NT_Sub:
    case NT_Div:
    case NT_LessThan:
    case NT_Mod:
		case NT_Add:
    case NT_Mult:
			if ( eliminate_stmt(start, cur->bin.right) == 0 )
				eliminate_stmt(start, cur->bin.left);
			break;
		case NT_Load:
				eliminate_stmt(start, cur->load.address);
			break;
		case NT_Store:
			if ( eliminate_stmt(start, cur->store.data) == 0 )
				eliminate_stmt(start, cur->store.address);
			break;
	}
	
	return 0;
}


void subexpressions_eliminate(node_list_t *start){
	node_list_t *c;
	for(c = start->next ; c!=NULL ; c= c->next ){
		eliminate_stmt(c,c->node);
	}
}
