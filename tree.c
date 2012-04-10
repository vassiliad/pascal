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

static unsigned int label_counter[] = { 0, 0, 0, 0, 0 };

node_t *tree_generate_node(node_t *prev, statement_t *node, scope_t *scope, char *label);
struct NODE_LOAD_STORE_T tree_generate_address(variable_t *var, scope_t *scope);
node_t *tree_generate_store_str(variable_t *var, char *string, scope_t *scope);
node_t *tree_generate_store(variable_t *var, node_t *data, scope_t *scope);
node_t *tree_generate_load(variable_t *var, scope_t *scope);
node_t *tree_generate_sconst(char *string);
node_t *tree_generate_value( expression_t *expr, scope_t *scope);
node_t *tree_generate_branchz( node_t *condition, char *label);
node_t *tree_generate_jump(char* label);
node_t *tree_generate_nop(char *label);
node_t *tree_generate_if(node_t *prev, statement_if_t *_if, 
		scope_t *scope, char *label);
node_t *tree_generate_assignment(node_t *prev,
		statement_assignment_t *assign, scope_t *scope, char *label);

node_t *tree_generate_nop(char *label)
{
	node_t *node;
	node = (node_t*) calloc(1, sizeof(node_t));
	node->type = NT_Nop;
	node->label = label;

	return node;
}

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

char *instr_label_last(enum LabelType type) {
	static char lookup[5][10] = { "Else", "Join", "Loop", "Enter", "Exit" };

	char temp[100];

	sprintf(temp, "%s_%d", lookup[type], label_counter[type]);
	return strdup(temp);
}

char *instr_label_unique(enum LabelType type)
{
	static char lookup[5][10] = { "Else", "Join", "Loop", "Enter", "Exit" };

	char temp[100];

	sprintf(temp, "%s_%d", lookup[type], ++label_counter[type]);
	return strdup(temp);
}


node_list_t *tree_generate_tree(statement_t *root, scope_t *scope)
{ 
	node_list_t *tree, *cur;
	statement_t *p;
	char *label = NULL;

	if ( root == NULL )
		return NULL;

	tree = (node_list_t*) calloc(1, sizeof(node_list_t));
	tree->node = tree_generate_node(NULL, root, scope, NULL );

	if ( tree->node == NULL ) {
		free(tree);
		return NULL;
	}

	cur = tree;
	p = root;

	while ( (p=p->next) ) {
		cur->next = (node_list_t*) calloc(1, sizeof(node_list_t));
		cur->next->prev = cur;
		cur->next->node = tree_generate_node(cur->node, p, scope, label);

		cur = cur->next;

		if ( cur->node->type == NT_If ) {
			label = instr_label_last(Label_Join);
			if ( p->next == NULL ) {
				cur->next = (node_list_t*) calloc(1, sizeof(node_list_t));
				cur->next->prev = cur;

				cur->next->node = tree_generate_nop(label);
				break;
			}
		} else
			label = NULL;
	}

	return tree;
}

struct NODE_LOAD_STORE_T tree_generate_address(variable_t *var, scope_t *scope)
{
	int i, idim, j;
	int length;
	int isConstant = 1; // assume that the memory address can be computed
	// in compile-time
	typedefs_entry_t *ty = NULL;
	node_list_t *dims;
	node_t *dim;
	struct NODE_LOAD_STORE_T ret;
	int *factors;

	/*
		 Traverse var, and accumulate offsets based on the types of each
		 @child in var ( see variable_t in bison_union.h ) then add the base
		 address and return it as a icosnt type node_t.
	 */


	ret.reg = rg_get_zero();
	ret.address = NULL; // TODO: This will be generated in case of user_type
	ret.offset = 0; // TODO: This should indicate where the variable is stored in memory

	if ( var->type.dataType == VT_User ) {
		ty = st_typedef_find(var->type.userType, scope);

		assert( (ty!=NULL) && "Type is not defined" );

		printf("builtin: (%s)\n", var->id);

		if ( var->expr.size )
			printf("Is an array!\n");
		else 
			printf("\tnot an array\n");

		dims = NULL;
		if ( var->expr.size ) {
			/*
				 First generate all factors

				 &x[i0,i1,i2,...,in-1] = sizeof(x)*( in-1 + (dim_n-1)*in-2 
				 + (dim_n-1*dim_n-2)*in-3 + 
				 + ... +
				 + (dim_n-1*dim_n-2*...*dim_i+1)*ii + 
				 + ... +
				 + (dim_n-1*dim_n-2*...*dim1)*i0 )

				 factors[i] = 1 * (dim_n-1*dim_n-2*dim_i+1)
			 */

			factors = (int*) malloc(sizeof(int)*var->expr.size);

			assert( (ty->type==TT_Array) && "Type is not an array!");

			for ( i=0; i<ty->array.dims.size; i++ ) {
				factors[i] = 1;

				for (j=ty->array.dims.size-1; j>i; j-- ) {
					printf("%d IsRange: %d\n", j, ty->array.dims.limits[j].isRange);
					if ( ty->array.dims.limits[j].isRange ) {
						length = ty->array.dims.limits[j].range.to.iconst -
							ty->array.dims.limits[j].range.from.iconst;

						printf("  %d==>%d\n",
							ty->array.dims.limits[j].range.to.iconst,
							ty->array.dims.limits[j].range.from.iconst);

					} else {
						length = ty->array.dims.limits[j].limit.iconst;
						printf("===> %d (%d)\n", 
						ty->array.dims.limits[j].limit.iconst,
						ty->array.dims.limits[j].limit.type);
					}

					printf("j: %d -- %d\n", j, length);

					factors[i] *= length;
				}

				printf("[%s] Factors[%d] = %d\n",var->id, i, factors[i]);
			}

			for ( i=0; i<var->expr.size; i++ )
			{
				dim = NULL;
				if ( var->expr.exprs[i].type != ET_Constant)
					dim = tree_generate_value(&(var->expr.exprs[i]), scope);
				else {
					switch( var->expr.exprs[i].constant.type ) {
						case VT_Iconst:
							idim = var->expr.exprs[i].constant.iconst;
							break;
						case VT_Cconst:
							idim = var->expr.exprs[i].constant.cconst;
							break;
						default:
							assert(0 && "Not supported index type");
					}
				}
			}
		}

	} else { // Scalar variable
		ret.address = NULL;
		ret.offset = 0;
		ret.reg = rg_get_zero();
	}



	return ret;
}

node_t *tree_generate_store_str(variable_t *var, char *string, scope_t *scope)
{
	node_t *ret = calloc(1, sizeof(node_t));

	ret->type = NT_Store;
	return ret;
}

node_t *tree_generate_store(variable_t *var, node_t *data, scope_t *scope)
{
	node_t *ret = calloc(1, sizeof(node_t));

	ret->type = NT_Store;
	ret->store = tree_generate_address(var, scope);
	ret->store.offset = 0;
	ret->store.data = data;
	return ret;
}

node_t *tree_generate_load(variable_t *var, scope_t *scope)
{
	node_t *ret = calloc(1, sizeof(node_t));

	ret->type = NT_Load;

	printf("load : %s\n", var->id);
	ret->load = tree_generate_address(var, scope);
	ret->reg = rg_allocate();
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
			node->reg = rg_allocate();

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
					assert(0 && "Unknown constant type");
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
							node->reg = rg_allocate();
						} 
						break;
					case RelopL:
						{
							node_t *left, *right;
							left = tree_generate_value(expr->binary.left, scope);
							right = tree_generate_value(expr->binary.right, scope);
							node = (node_t*) calloc(1, sizeof(node_t));

							node->type = NT_LessThan;
							node->bin.left = left;
							node->bin.right = right;
							node->reg = rg_allocate();
							break;
						}
					default:
						printf("%d\n",expr->binary.op);
						assert(0 && "Unhandled binary expression");
				}

			}
	}

	return node;
}

node_t *tree_generate_assignment(node_t *prev,
		statement_assignment_t *assign, scope_t *scope, char *label)
{
	node_t *result = NULL;
	if ( assign->type == AT_Expression ) {
		result = tree_generate_store(assign->var, tree_generate_value(assign->expr, scope), scope);
	} else if ( assign->type == AT_String ) {
		assert( 0 && "Unimplemented string assignment");
	} else
		assert( 0 && "Invalid assignment type");

	if ( prev )
		prev->next = result;
	result->label = label;
	result->prev = prev;
	return result;
}

node_t *tree_generate_if(node_t *prev, 
		statement_if_t *_if,  scope_t *scope, char *label)
{
	node_list_t *btrue = NULL,
							*bfalse= NULL,
							*p = NULL;

	node_t *jbranch=NULL,
				 *ccon  = NULL,
				 *node = NULL;

	char *label_false = NULL,
			 *label_join = instr_label_unique(Label_Join);


	if ( _if->_false )
		label_false = instr_label_unique(Label_Else);
	else
		label_false = label_join;

	ccon   = tree_generate_value(_if->condition, scope);
	jbranch = tree_generate_branchz(ccon, label_false);
	btrue  = tree_generate_tree(_if->_true, scope);
	bfalse = tree_generate_tree(_if->_false, scope);

	if ( bfalse )
		bfalse->node->label = label_false;

	if ( btrue == NULL ) {
		assert( 0 && "If statements must have statements in their true block");
	}

	if ( bfalse ) {
		for ( p = btrue; p->next != NULL; p=p->next );

		p->next = (node_list_t*) calloc(1, sizeof(node_list_t));
		p->next->prev = p;
		p = p->next;

		p->node = tree_generate_jump(label_join);
	}

	node = (node_t*) calloc(1, sizeof(node_t));
	node->type  = NT_If;
	node->_if._true = btrue;
	node->_if._false = bfalse;
	node->_if.branch = jbranch;
	node->label = label;
	return node;
}

node_t *tree_generate_node(node_t *prev, statement_t *stmt, scope_t *scope, char* label)
{  
	node_t *result = NULL;
	if ( stmt == NULL )
		return NULL;

	//printf("node_type: %s\n", statement_type_to_string(stmt));

	switch ( stmt->type )
	{
		case ST_Assignment:
			{
				result = tree_generate_assignment(prev, &(stmt->assignment),
						scope, label);
				assert(result!=NULL && "Failed to generate Assignment");
				return result;
			}
			break;

		case ST_If:
			{
				result = tree_generate_if(prev, &stmt->_if,	scope, label);
				assert( result!=NULL && "Failed to generate if statement");
				return result;
			}
			break;

		case ST_While:
			NODE_UNIMPLEMENTED("While statement");
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

		default:
			NODE_UNIMPLEMENTED("Some kind of statement");
	}
}

