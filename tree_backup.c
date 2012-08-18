#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "tree.h"
#include "expressions.h"
enum LabelType
{
	Label_Else,
	Label_Join,
	Label_Loop,
	Label_Enter,
	Label_Exit,
	/*  Add more as needed then update instr_label_unique::counter */
};


statement_t *tree_generate_node(node_t *prev, statement_t *statement, scope_t *scope, char *label, node_t **node);
node_t *tree_generate_address(variable_t *var);
node_t *tree_generate_store_str(variable_t *var, char *string, scope_t *scope);
node_t *tree_generate_store(variable_t *var, node_t *data, scope_t *scope);
node_t *tree_generate_load(variable_t *var, scope_t *scope);
node_t *tree_generate_sconst(char *string);
node_t *tree_generate_value( expression_t *expr, scope_t *scope);
node_t *tree_generate_branchz( node_t *condition, char *label);
node_t *tree_generate_jump(char* label);
void node_list_append(node_list_t **root, node_t *node);

char *instr_label_unique(enum LabelType type);


node_t *tree_generate_jump(char *label)
{
	node_t *ret;

	ret = ( node_t* ) calloc(1, sizeof(node_t));

	ret->type = NT_Jump;
	ret->jump_label = label;

	return ret;
}

node_t *tree_generate_branchz(node_t *condition, char *label)
{
	node_t *ret;

	ret = ( node_t * ) calloc(1, sizeof(node_t));

	ret->type = NT_BranchZ;
	ret->branchz.label = label;
	ret->branchz.condition = condition;

	return ret;
}

char *instr_label_unique(enum LabelType type)
{
	static unsigned int counter[] = { 0, 0, 0, 0, 0 };
	static char lookup[5][10] = { "Else", "Join", "Loop", "Enter", "Exit" };

	char temp[100];

	sprintf(temp, "%s_%d", lookup[type], ++counter[type]);
	return strdup(temp);
}


node_list_t *tree_generate_tree(statement_t *root, scope_t *scope)
{ 
  node_list_t *nodes = NULL;
	node_t *cur;
	statement_t *p;

  p = root;

  while (p) {
		p = tree_generate_node(cur, p, scope, NULL , &cur);
    node_list_append(&nodes, cur);
    assert(cur && "tree_generate_node returned NULL\n");
	}
	return nodes;
}

node_t *tree_generate_address(variable_t *var)
{
	/*
		 Traverse var, and accumulate offsets based on the types of each
		 @child in var ( see variable_t in bison_union.h ) then add the base
		 address and return it as a icosnt type node_t.
	 */
	return NULL;
}

node_t *tree_generate_store_str(variable_t *var, char *string, scope_t *scope)
{
	node_t *address = tree_generate_address(var);
	node_t *ret = calloc(1, sizeof(node_t));

	ret->type = NT_Store;
	ret->store.address = address;
	return ret;
}

node_t *tree_generate_store(variable_t *var, node_t *data, scope_t *scope)
{
	node_t *address = tree_generate_address(var);
	node_t *ret = calloc(1, sizeof(node_t));

	ret->type = NT_Store;
	ret->store.address = address;
	return ret;
}

node_t *tree_generate_load(variable_t *var, scope_t *scope)
{
	node_t *address = tree_generate_address(var);
	node_t *ret = calloc(1, sizeof(node_t));

	ret->type = NT_Load;
	ret->load.address = address;
	return ret;
}

node_t *tree_generate_sconst(char *string)
{
	return NULL;
}

node_t *tree_generate_value( expression_t *expr, scope_t *scope)
{
	node_t *node = NULL;

	switch ( expr->type )
	{
		case ET_Constant:
			node = ( node_t* ) calloc(1, sizeof(node_t));

			switch ( expr->constant.type )
			{
				case VT_Bconst:
					node->type = NT_Bconst;
					node->bconst = expr->constant.bconst;
					break;

				case VT_Iconst:
					node->type = NT_Iconst;
					node->iconst = expr->constant.iconst;
					break;

				case VT_Rconst:
					node->type = NT_Rconst;
					node->rconst = expr->constant.rconst;
					break;

				case VT_Cconst:
					node->type = NT_Cconst;
					node->cconst = expr->constant.cconst;
					break;
				default:
					return 0;
				break;
			}
			break;

		case ET_Set:
			break;

		case ET_Not:
			node = ( node_t* ) calloc(1, sizeof(node_t));
			node->type = NT_Not;
			node->not = tree_generate_value( expr->notExpr, scope );
			break;

		case ET_Variable:
			node = tree_generate_load( expr->variable, scope);
			break;

		case ET_Call:

			break;

		case ET_Binary:
			{
				switch ( expr->binary.op )
				{
					case AddopM:
					case MuldivandopDiv:
					case MuldivandopM:
					case MuldivandopMod:
					case AddopP:
						{
							expression_t *e_left, *e_right;
							node_t *left, *right;

							e_left = expr->binary.left;
							e_right = expr->binary.right;

							// slight optimization when both operands are constants
							// compute the value in compile time
							
							left = tree_generate_value( e_left, scope );
							right = tree_generate_value( e_right, scope );

							node = ( node_t * ) calloc(1, sizeof(node_t));

							switch ( expr->binary.op )
							{
								case AddopP:
									node->type = NT_Add;
									break;

								case AddopM:
									node->type = NT_Sub;
									break;

								case MuldivandopDiv:
									node->type = NT_Div;
									break;

								case MuldivandopMod:
									node->type = NT_Mod;
									break;

								case MuldivandopM:
									node->type = NT_Mult;
									break;
							}

							node->bin.left = left;
							node->bin.right = right;
						} 
						break;
				}
			}
	}

	return node;
}

node_t *tree_generate_assignment(statement_assignment_t *assignment, scope_t *scope)
{
  node_t *var, *data;
  var = NULL;
  data = NULL;

  printf("Assignment\n");

  if ( assignment->type == AT_Expression ) {
    data = tree_generate_value(assignment->expr, scope);
    var  = tree_generate_store(assignment->var, data, scope);
  } else {
// #warning assignment store strings is not yet implemented
   assert(0 && "Attempted to store a string\n");
  }
  
  return var;
}

void node_list_append(node_list_t **root, node_t *node)
{
  node_list_t *p;

  if ( *root == NULL ) {
    *root = (node_list_t*) malloc(sizeof(node_list_t));
    assert(*root && "Failed to malloc root node for node_list");
    (*root)->next = (*root)->prev = *root;
  }

  p = (node_list_t*) malloc(sizeof(node_list_t));
  p->node = node;
  p->prev = (*root)->prev;
  p->next = *root;

  (*root)->prev->next = p;
  (*root)->prev = p;
}
/*
  tree_generate_if Actually uses both _if and its next statement since
  it needs to know where to continue when either of its blocks finish
  executing (branch/nobranch).
*/
statement_t *tree_generate_if(statement_if_t *_if, statement_t *next,scope_t *scope, char *label,
  node_t **node)
{
  node_t *cond_val=NULL,*condition=NULL, *temp = NULL;
  node_list_t *branch = NULL, *nobranch = NULL, *join = NULL;
  statement_t *stmt_next;

  printf("If\n");
    
  //1 --- Generate condition value
  cond_val = tree_generate_value(_if->condition, scope);
  cond_val->label = label;
  
  //2 --- Generate the two blocks ( branch/nobranch )
  //  2.1 Generate NoBranch ( lists of trees )
  nobranch = tree_generate_tree(_if->_true, scope);
  
  //  2.2 Generate Branch ( lists of trees )
  branch  = tree_generate_tree(_if->_false, scope);
  
  //3 --- Generate the join block
  
  
  return stmt_next;
}

statement_t *tree_generate_node(node_t *prev, statement_t *stmt, scope_t *scope, char* label, node_t **node)
{  
  assert(stmt == NULL && "Passed NULL statement to tree_generate_node\n");

	switch ( stmt->type )
	{
		case ST_Assignment:
    { 
      *node = tree_generate_assignment(&(stmt->assignment), scope);
      (*node)->prev = prev;
      (*node)->label = label;

      if ( prev )
        prev->next = *node;
      
      return stmt->next;
    }

		case ST_If:
		{
      *node = tree_generate_if(&stmt->_if, stmt->next, scope, label);

      stmt = stmt->next;

      if ( stmt )
        return stmt->next;
      else
        return NULL;
		}

		case ST_While:
		{
#if 0
      node_t *join = NULL;
			node_t *loop = NULL;
			node_t *condition = NULL;
			node_t *branch = NULL;
			node_t *temp = NULL;
			node_t *jump = NULL;
			node_t *t = NULL;
			statement_t *p = NULL;

			char *join_label = instr_label_unique(Label_Join);

			if ( node->join ) {
				join = tree_generate_node(NULL,node->join, scope, join_label );
				t = join;

				for ( p = node->join->next; p && t; p = p->next )
					t = tree_generate_node(t, p, scope, NULL);
			} else
				join = NULL;

			condition = tree_generate_value(node->_while.condition, scope);
			
			if ( condition) {
				condition->prev = prev;
				if ( label==NULL )
					label = instr_label_unique(Label_Loop);

				condition->label = label;
			}

			branch = tree_generate_branchz(condition, join_label);
			branch->prev = condition;
			
			if ( condition )
				condition->next = branch;

			if ( prev )
				prev->next = condition;

			loop = tree_generate_node(condition, node->_while.loop, scope, NULL);

			if ( loop ) {
				loop->prev = condition;

				temp = loop;

				while ( temp->next )
					temp = temp->next;
			} else {
				temp = tree_generate_jump(label);

				temp->prev = branch;
				branch->next = temp;
			}

			jump = tree_generate_jump(join_label);
			jump->next = join;

			if ( join )
				join->prev = jump;
#endif
			return NULL;
		}
		break;

		case ST_For:
			break;

		case ST_With:
			break;

		case ST_Call:
			break;

		case ST_Case:
			break;
	}

  return NULL;
}

