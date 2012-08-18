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
struct NODE_LOAD_STORE_T tree_generate_address(variable_t *parent,
	 variable_t *var, scope_t *scope);
node_t *tree_generate_store_str(variable_t *var, char *string, scope_t *scope);
node_t *tree_generate_store(variable_t *var, node_t *data, scope_t *scope);
node_t *tree_generate_load(variable_t *var, scope_t *scope);
node_t *tree_generate_sconst(char *string);
node_t *tree_generate_value( expression_t *expr, scope_t *scope);
node_t *tree_generate_branchz( node_t *condition, char *label);
node_t *tree_generate_jump(char* label);
node_t *tree_generate_nop(char *label);
node_t *tree_generate_for(node_t *prev, statement_for_t *_for, 
		scope_t *scope, char *label);
node_t *tree_generate_while(node_t *prev, statement_while_t *_while, 
		scope_t *scope, char *label);
node_t *tree_generate_if(node_t *prev, statement_if_t *_if, 
		scope_t *scope, char *label);
node_t *tree_generate_assignment(node_t *prev,
		statement_assignment_t *assign, scope_t *scope, char *label);
char *instr_label_unique(enum LabelType type);

node_t *tree_generate_nop(char *label)
{
	node_t *node;
	node = (node_t*) calloc(1, sizeof(node_t));
	node->type = NT_Nop;
	node->label = label;

	return node;
}




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


void list_add(node_list_t **root, node_list_t **tail, node_t *node)
{
	if ( *root == NULL ) {
		*tail = *root = (node_list_t*) calloc(1, sizeof(node_list_t));
		(*root)->next = NULL;
		(*root)->prev = NULL;
		(*root)->node = node;
		
		return;
	}

	(*tail)->next = (node_list_t*) calloc(1, sizeof(node_list_t));
	
	(*tail)->next->prev = *tail;

	(*tail) = (*tail)->next;
	(*tail)->node = node;
}

node_list_t *tree_generate_tree(statement_t *root, scope_t *scope)
{ 
	node_list_t *nroot, *ntail;
	node_t *node, *prev;
	statement_t *p;
	char *label = NULL;

	nroot = ntail = NULL;
	prev = NULL;

	p = root;

	while ( p ) {
		node = tree_generate_node(prev, p, scope, label);
		list_add(&nroot, &ntail, node);

		prev = ntail->node;

		p = p->next;
		
		switch( prev->type ) {
			case NT_While:
      case NT_For:
			case NT_If:
			{
				label = instr_label_last(Label_Join);
				if ( p == NULL )
					list_add(&nroot, &ntail, tree_generate_nop(label));
			}
			break;

			default:
				label = NULL;
		}

	}

	return nroot;
}

struct NODE_LOAD_STORE_T tree_generate_address(variable_t *parent,
	variable_t *var, scope_t *scope)
{
	int i, idim, j;
	int length;
	int type_size;
	typedefs_entry_t *ty = NULL;
	node_t *dim;
	node_t *accumulate = NULL;
	struct NODE_LOAD_STORE_T ret;
	int *factors = NULL;

	/*
		 Traverse var, and accumulate offsets based on the types of each
		 @child in var ( see variable_t in bison_union.h ) then add the base
		 address and return it as a icosnt type node_t.
	 */

	ret.address = NULL;
	
	if ( var->type.dataType == VT_User ) {
		ty = st_typedef_find(var->type.userType, scope);

		assert( (ty!=NULL) && "Type is not defined" );

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
			
			type_size = st_get_type_size(ty->array.typename.dataType
				, ty->array.typename.userType, scope);


			for ( i=0; i<ty->array.dims.size; i++ ) {
				factors[i] = 1;

				for (j=ty->array.dims.size-1; j>i; j-- ) {
					if ( ty->array.dims.limits[j].isRange )
						length = ty->array.dims.limits[j].range.to.iconst -
							ty->array.dims.limits[j].range.from.iconst;
					else
						length = ty->array.dims.limits[j].limit.iconst;

					factors[i] *= length;
				}
			}

			for ( i=0; i<var->expr.size; i++ )
			{
				if ( var->expr.exprs[i].type != ET_Constant) {

					if ( ty->array.dims.limits[j].isRange ) {
						// since this dimension is a range, we have to subtract
						// the beginning of the range from the expression that is used
						// as an index
						
						// but first the constant is generated

						int itype;
						
						switch( ty->array.dims.limits[j].range.from.type ) {
							case LT_Iconst:
								itype = VT_Iconst;
							break;

							case LT_Bconst:
								itype = VT_Bconst; // actually this might be overkill
							break;

							case LT_Cconst:
								itype = VT_Cconst;
							break;

							default:
								assert( 0 && "Invalid index type");
							break;
						}

						expression_t *constant = 	expression_constant(itype,
							&(ty->array.dims.limits[j].range.from.iconst));
						
						expression_t *sub = expression_binary(&(var->expr.exprs[i])
								, constant, AddopM);
						dim = tree_generate_value(sub, scope);
					} else 
						dim = tree_generate_value( &(var->expr.exprs[i]), scope);
					
					if ( accumulate == NULL )
						accumulate = dim;
					else {
						node_t *add = (node_t*) calloc(1, sizeof(node_t));
						add->type = NT_Add;
						add->bin.left  = accumulate;
						add->bin.right = dim;
						add->reg = rg_allocate();

						accumulate = add;
					}
				} else {
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
					if ( ty->array.dims.limits[j].isRange )
						idim -= ty->array.dims.limits[i].range.from.iconst;
					
					idim *= factors[i];
					ret.offset += idim;
				}	
			}
			
			ret.offset *= type_size;
		}

		// There's also dynamic parts that need to be calculated
		// at run-time
		if ( accumulate != NULL ) {
			expression_t *constant = expression_constant(VT_Iconst
				, &type_size);
			
			node_t *val = tree_generate_value(constant, NULL);

			ret.address = (node_t*) calloc(1, sizeof(node_t));
			ret.address->type = NT_Mult;
			ret.address->bin.left = val;
			ret.address->bin.right = accumulate;
			ret.address->reg = rg_allocate();

			free(constant);
		}


		
	} else { // Scalar variable
		ret.address = NULL;
		ret.offset = 0;
	}

	if ( parent && parent->type.dataType == VT_User ) {

		ty = st_typedef_find(parent->type.userType, scope);
		// Next add the offset that originates from record fields
 		record_t *rec = NULL;
		if ( ty->type == TT_Array ) {
			if ( ty->array.typename.dataType == VT_User ) {
				ty = st_typedef_find(ty->array.typename.userType, scope);
				
				assert( (ty!=NULL) && "Array typedef is not defined!");

				if ( ty->type == TT_Record )
					rec = &(ty->record);
			}
		} else if ( ty->type == TT_Record ) {
			rec = &(ty->record);
		} else
			rec = NULL;

		if ( rec ) {
			for ( i=0; strcasecmp(var->id,rec->ids[i])!=0 && i<rec->size
					 ;i++ )
				ret.offset += st_get_type_size(rec->types[i].dataType
					, rec->types[i].userType,scope);
		}
	}
	
	if ( factors )
		free(factors);

	if ( var->child ) {
		struct NODE_LOAD_STORE_T child = tree_generate_address(var, var->child,
		scope);
		ret.offset += child.offset;

		if ( ret.address == NULL )
			ret.address = child.address;
		else if ( child.address ) {
			// accumulate the dynamic parts of the memory address
			node_t *address;
			address = (node_t*) calloc(1, sizeof(node_t));
			address->type = NT_Add;
			address->bin.left  = ret.address;
			address->bin.right = child.address;
			address->reg = rg_allocate();

			ret.address = address;
		}
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
  var_t *v;
  
  v = st_var_find(var->id, scope);
    
  assert( v && "Unknown variable");

	ret->type = NT_Store;
	ret->store = tree_generate_address(NULL, var, scope);
  
  ret->store.offset += v->offset;
  ret->store.unique_id = v->unique_id;
	ret->store.data = data;
	return ret;
}

node_t *tree_generate_load(variable_t *var, scope_t *scope)
{
	node_t *ret = calloc(1, sizeof(node_t));
  var_t *v;
  
  v = st_var_find(var->id, scope);
    
  assert( v && "Unknown variable");
  
	ret->type = NT_Load;

	ret->load = tree_generate_address(NULL, var, scope);
  ret->load.offset += v->offset;
  ret->load.unique_id = v->unique_id;
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

	result->label = label;
	return result;
}

node_t *tree_generate_for(node_t *prev, statement_for_t * _for, 
		scope_t *scope, char *label)
{
	node_t *node = NULL;
	node_list_t *loop = NULL;
	node_t *ccon = NULL,
				 *jbranch = NULL;

	char *label_join = instr_label_last(Label_Join);
  char *label_iter_step = instr_label_unique(Label_Enter);
	

	ccon = tree_generate_value(_for->condition, scope);
	jbranch = tree_generate_branchz(ccon, label_join);
	jbranch->label = label_iter_step;

	loop = tree_generate_tree(_for->loop, scope);
  
  

	node = (node_t*) calloc(1, sizeof(node_t));
	node->type = NT_For;
	node->_for.loop = loop;
	node->_for.branch = jbranch;
  node->_for.init = tree_generate_node(prev, _for->init, scope, NULL);
	node->label = label;

	while ( loop->next )
		loop = loop->next;
	
	loop->next = (node_list_t*) calloc(1, sizeof(node_list_t));
	loop->next->prev = loop;
	loop = loop->next;
	loop->node = tree_generate_node(NULL, _for->iter_op, scope, NULL );

	loop->next = (node_list_t*) calloc(1, sizeof(node_list_t));
	loop->next->prev = loop;
	loop = loop->next;
	loop->node = tree_generate_jump(label_iter_step);

	return node;
}


node_t *tree_generate_while(node_t *prev, statement_while_t *_while, 
		scope_t *scope, char *label)
{
	node_t *node = NULL;
	node_list_t *loop = NULL;
	node_t *ccon = NULL,
				 *jbranch = NULL;

	char *label_join = instr_label_last(Label_Join);
	
	if ( label == NULL )
		label = instr_label_unique(Label_Enter);

	ccon = tree_generate_value(_while->condition, scope);
	jbranch = tree_generate_branchz(ccon, label_join);
	
	loop = tree_generate_tree(_while->loop, scope);


	node = (node_t*) calloc(1, sizeof(node_t));
	node->type = NT_While;
	node->_while.loop = loop;
	node->_while.branch = jbranch;
	node->label = label;
  
  if ( loop ) {
    while ( loop->next )
      loop = loop->next;
    
    loop->next = (node_list_t*) calloc(1, sizeof(node_list_t));
    loop->next->prev = loop;
    loop = loop->next;
  } else {
    loop = (node_list_t*) calloc(1, sizeof(node_list_t));
    node->_while.loop = loop;
  }

  loop->node = tree_generate_jump(label);
	return node;
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

	if ( bfalse ) {
    if ( btrue == NULL ) {
      btrue = (node_list_t*) calloc(1, sizeof(node_list_t));
      p = btrue;
    } else {
      for ( p = btrue; p->next != NULL; p=p->next );

  		p->next = (node_list_t*) calloc(1, sizeof(node_list_t));
	  	p->next->prev = p;
      p = p->next;
    }

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

// TODO @prev is not actually used, it's just passed around
node_t *tree_generate_node(node_t *prev, statement_t *stmt, scope_t *scope, char* label)
{  
	node_t *result = NULL;
	if ( stmt == NULL )
		return NULL;

	printf("node_type: %s\n", statement_type_to_string(stmt));

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
		{
			result = tree_generate_while(prev, &stmt->_while, scope, label);
			assert( result != NULL && "Failed to generate while statement");
			return result;
		}
		break;

		case ST_For:
			result  = tree_generate_for(prev, &stmt->_for, scope, label);
			assert ( (result != NULL ) && "Failed to generate for statement");
      return result;
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

  assert( 0 && "Unreachable Code");
}

