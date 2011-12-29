#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tree.h"
#include "expressions.h"


node_t *tree_generate_node(node_t *prev, statement_t *node, scope_t *scope);
node_t *tree_generate_address(variable_t *var);
node_t *tree_generate_store_str(variable_t *var, char *string, scope_t *scope);
node_t *tree_generate_store(variable_t *var, expression_t *expr, scope_t *scope);
node_t *tree_generate_load(variable_t *var, scope_t *scope);
node_t *tree_generate_sconst(char *string);
node_t *tree_generate_value( expression_t *expr, scope_t *scope);
node_t *tree_generate_branchz( node_t *condition, node_t *destination);
node_t *tree_generate_jump(node_t* destination);

char *instr_label_unique(int type);

enum LabelTypes
{
	Label_Else,
	Label_Join,
	Label_Enter,
	Label_Exit,
	/*  Add more as needed then update instr_label_unique::counter */
};

node_t *tree_generate_jump(node_t* destination)
{
	node_t *ret;

	ret = ( node_t* ) calloc(1, sizeof(node_t));

	ret->type = NT_Jump;
	ret->jump = destination;

	return ret;
}

node_t *tree_generate_branchz(node_t *condition, node_t *destination)
{
	node_t *ret;

	ret = ( node_t * ) calloc(1, sizeof(node_t));

	ret->type = NT_BranchZ;
	ret->branchz.destination = destination;
	ret->branchz.condition = condition;

	return ret;
}

char *instr_label_unique(int type)
{
	static unsigned int counter[] = { 0, 0, 0, 0 };
	static char lookup[4][10] = { "Else", "Join", "Enter", "Exit" };

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

	node = tree_generate_node( NULL, root, scope );

	if ( node == NULL )
		return NULL;

	cur = node;

	for ( p = root->next; p ; p = p->next ) {
		cur = tree_generate_node(cur, p, scope );

		if ( cur == NULL )
			return NULL;
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

node_t *tree_generate_node(node_t *prev, statement_t *node, scope_t *scope)
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
				node_t *_true, *_false, *condition, *join;

				if ( node->join ) {
					join = tree_generate_node(NULL,node->join, scope);
					join->label = instr_label_unique(Label_Join);
				} else
					join = NULL;

				condition = tree_generate_value(node->_if.condition, scope);

				if ( node->_if._true )
					_true = tree_generate_node(prev, node->_if._true, scope);

				if ( node->_if._false ) {
					// when else exists make a jump to the beginning of
					// that branch, then just continue to the join node
					node_t *temp = _true;

					_false = tree_generate_node(NULL, node->_if._false, scope);
					_false->label = instr_label_unique(Label_Else);

					_true = tree_generate_branchz(condition, _false);

					_true->next = temp;
					temp->prev = _true;

					_true = temp;

					// Place a jump as the last instruction of _true
					// towards join

					temp = _true;

					while ( temp->next )
						temp = temp->next;

					temp->next = tree_generate_jump(join);

					// Then connect the last node of _false to join
					temp = _false;

					while ( temp->next )
						temp = temp->next;

					// join->prev will always point to _true
					temp->next = join;
					if ( join )
						join->prev = _true;

				} else {
					// otherwise make a jump at the join node
					// inject the branch before the actuall _true instructions

					node_t *temp = _true;

					_true = tree_generate_branchz(condition, join);

					_true->next = temp;
					temp->prev = _true;

					_true = temp;

					// Then connect the last node of _true to join
					temp = _true;

					while ( temp->next )
						temp = temp->next;

					// join->prev will always point to _true
					temp->next = join;
					if ( join )
						join->prev = _true;

				}

				_true->prev = prev;
				if ( prev )
					prev->next = _true;

				if ( join == NULL )
					return _true;
#warning above line is a hack
				return join;
			}
			break;

		case ST_While:
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

