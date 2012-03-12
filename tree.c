#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "tree.h"
#include "expressions.h"

#define NODE_UNIMPLEMENTED(X) assert(0 && "Node logic is not implemented" && X )

enum LabelType
{
	Label_Else,
	Label_Join,
	Label_Loop,
	Label_Enter,
	Label_Exit,
	/*  Add more as needed then update instr_label_unique::counter */
};


statement_t *tree_generate_node(node_t **result, node_t *prev, statement_t *node, scope_t *scope, char *label);
node_t *tree_generate_address(variable_t *var);
node_t *tree_generate_store_str(variable_t *var, char *string, scope_t *scope);
node_t *tree_generate_store(variable_t *var, expression_t *expr, scope_t *scope);
node_t *tree_generate_load(variable_t *var, scope_t *scope);
node_t *tree_generate_sconst(char *string);
node_t *tree_generate_value( expression_t *expr, scope_t *scope);
node_t *tree_generate_branchz( node_t *condition, char *label);
node_t *tree_generate_jump(char* label);

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


node_t *tree_generate_tree(statement_t *root, scope_t *scope)
{ 
	node_t *node, *cur;
	statement_t *p;

	if ( root == NULL )
		return NULL;

	node = NULL;
	p = tree_generate_node( &node, NULL, root, scope, NULL );

	if ( node == NULL )
		return NULL;

	cur = node;

	while ( p ) 
		p = tree_generate_node(&cur, cur, p, scope, NULL );

	return node;
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

node_t *tree_generate_store(variable_t *var, expression_t *expr, scope_t *scope)
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

							printf("left: %d -- right: %d\n", left->type, right->type);

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

void tree_generate_assignment(node_t **result, node_t *prev,
		statement_assignment_t *assign, scope_t *scope, char *label)
{
	node_t *var, *data, *ret;

	*result = NULL;
	
	if ( assign->type == AT_Expression ) {
		var  = tree_generate_store(assign->var, assign->expr, scope);
		data = tree_generate_value(assign->expr, scope);
	} else if ( assign->type == AT_String ) {
		assert( 0 && "Unimplemented string assignment");
	} else
		return;	
	
	ret = (node_t*) calloc(1, sizeof(node_t));
	ret->type = NT_Store;
	ret->load.address = var;
	ret->load.data = data;
	ret->prev = prev;
	ret->label = label;

	if ( prev )
		prev->next = ret;

	*result = ret;
}

statement_t *tree_generate_if(node_t **result, node_t *prev, 
	statement_if_t *_if, statement_t *join, scope_t *scope, char *label)
{
	// blocks: btrue, bfalse, bjoin
	// condition check: ccond
	statement_t *next = NULL;
	node_t *btrue = NULL,
			 *bfalse= NULL,
			 *ccon  = NULL,
			 *bjoin = NULL,
			 *jbranch=NULL;

	char *label_false = NULL,
			 *label_join  = NULL;
	
	if ( join )
		label_join = instr_label_unique(Label_Join);
	else
		assert( 0 && "No join for if statement, place a nop maybe?");
	
	if ( _if->_false )
		label_false = instr_label_unique(Label_Else);
	else
		label_false = label_join;

	ccon   = tree_generate_value(_if->condition, scope);
	jbranch = tree_generate_branchz(ccon, label_false);
	
	
}

statement_t *tree_generate_node(node_t **result, node_t *prev, statement_t *stmt, scope_t *scope, char* label)
{  
	statement_t *next;

	if ( stmt == NULL )
		return NULL;

	printf("node_type: %s\n", statement_type_to_string(stmt));

	switch ( stmt->type )
	{
		case ST_Assignment:
		{
			tree_generate_assignment(result, prev, &(stmt->assignment),
										scope, label);
			assert(*result!=NULL && "Failed to generate Assignment");

			next = stmt->next;
		}
		break;

		case ST_If:
		{
			next = tree_generate_if(result, prev, &stmt->_if, stmt->join,
							scope, label);

			assert( *result!=NULL && "Failed to generate if statement");
		}
		break;

		case ST_While:
		{
		}
		break;

		case ST_For:
			NODE_UNIMPLEMENTED("For statement");
		break;

		case ST_With:
			NODE_UNIMPLEMENTED("With statement");
		break;

		case ST_Call:
			NODE_UNIMPLEMENTED("Call statement");
		break;

		case ST_Case:
			NODE_UNIMPLEMENTED("Case statement");
		break;
	}

	return next;
}

