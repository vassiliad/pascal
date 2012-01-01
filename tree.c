#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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


node_t *tree_generate_node(node_t *prev, statement_t *node, scope_t *scope, char *label);
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

	node = tree_generate_node( NULL, root, scope, NULL );

	if ( node == NULL )
		return NULL;

	cur = node;

	for ( p = root->next; p ; p = p->next ) {
		cur = tree_generate_node(cur, p, scope, NULL );

		if ( cur == NULL )
			return node;
	}

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

node_t *tree_generate_node(node_t *prev, statement_t *node, scope_t *scope, char* label)
{  
	node_t *_node = NULL;


	if ( node == NULL )
		return NULL;

	printf("node_type: %s\n", statement_type_to_string(node));

	switch ( node->type )
	{
		case ST_Assignment:
			{
				node_t *var, *data;

				if ( node->assignment.type == AT_Expression ) {
					var = tree_generate_store( node->assignment.var
							, node->assignment.expr, scope);
					data = tree_generate_value( node->assignment.expr, scope );
				} else {
					var = tree_generate_store_str(node->assignment.var
							, node->assignment.string, scope);
					data = tree_generate_sconst( node->assignment.string );
				}

				_node = calloc (1, sizeof(node_t));
				_node->type = NT_Store;
				_node->load.address = var;
				_node->load.data = data;

				_node->prev = prev;

				if ( prev )
					prev->next = _node;

				break;
			}

		case ST_If:
		{
			/* Final code for an IF statement is generated here, instructions
			 * are placed consequently :
			 *
			 *    	<condition  >
			 *    	<true_block >
			 *			<false_block>
			 *			<join       >
			 *
			 */

			node_t *_true=NULL, 
						 *_false=NULL, 
						 *condition=NULL,
						 *branch=NULL,
						 *join=NULL,
						 *t=NULL;
			statement_t *p = NULL;

			char *false_label=NULL,
					 *join_label=NULL;

			if ( node->join ) {
				join_label = instr_label_unique(Label_Join);
				printf("IF join\n");
				join = tree_generate_node(NULL,node->join, scope, join_label);
				t = join;

				for ( p = node->join->next; p && t; p = p->next )
					t = tree_generate_node(t, p, scope, NULL);
			} else
				join = NULL;

			condition = tree_generate_value(node->_if.condition, scope);
			branch = tree_generate_branchz(condition, false_label);
			
			branch->prev = condition;

			if ( prev )
				prev->next = condition;

			if ( condition ) {  //TODO remove
				condition->label = label;
				condition->prev = prev;
				condition->next = branch;
			}

			// Next we'll generate the true_block

			_true = tree_generate_node(branch, node->_if._true, scope, NULL );
			
			t = _true;
						
			if ( _true ) {
				for ( p = node->_if._true->next; p && t; p = p->next )
					t = tree_generate_node(t, p, scope, NULL);

				// place a jump instruction at the end of true_block to the join_block
				for ( t = _true; t->next; t=t->next);

				t->next = tree_generate_jump(join_label);
				
				t->next->prev = t;
				t = t->next;
			} else {
				// there are no statements inside the true_block
				// skip it and just place a jump to join
				_true = tree_generate_jump(join_label);
				t = _true;
			}	
			// t points to the last node in _true

			if ( node->_if._false ) {
				// now generate the false_block
				_false = tree_generate_node(t, node->_if._false, scope, false_label );
				
				// connect last node of true_block with the first node
				// of the false_block

				t->next = _false;

				if ( _false )
					_false->prev = t;

				// next we'll generate the rest of the false_block
				t = _false;

				for ( p = node->_if._false->next; p && t; p = p->next )
					t = tree_generate_node(t, p, scope, NULL);
				
				for ( t = _false; t && t->next; t=t->next);

				// link the last node of false_block with the first node of the
				// join_block

				if ( t ) {
					t->next = join;
					if ( join )
						join->prev = t;
				}
			} else {
				// if there's no false_block just connect the last node of true_block
				// with the first node of join_block

				t->next = join;
				if ( join )
					join->prev = t;
			}

			return NULL; // join has already been processed
		}
		break;

		case ST_While:
		{
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

	return _node;
}

