#include <assert.h>
#include "printer.h"

void print_instruction(node_t *node, FILE* output);

void print_instruction_add(node_t *left, node_t *right, FILE* output)
{
	if ( right == NULL ) {
		// this add is used as a move
		switch(left->type) {
			case NT_Iconst:
				fprintf(output, "addi %s, $0, %d\n", left->reg.name, left->iconst);
			break;

			default:
				assert(0 && "Unhandled type");
		}
	}

	print_instruction(left, output);
	
	if ( right->type == NT_Iconst ) {
		// this is an addi
	} else {
		// this is an add
	}
}

void print_instruction_store(node_t *address, node_t *data, FILE* output)
{
}

void print_instruction_tree(node_list_t *tree, FILE* output)
{
	node_list_t *c;

	for ( c=tree; c!=NULL; c=c->next)
		print_instruction(c->node, output);
}

void print_instruction(node_t *node, FILE* output)
{
	if ( node == NULL )
		return;
	
	if ( node->label )
		fprintf(output, "%s : ", node->label );
	
	switch ( node->type ) {
		case NT_Add:
			print_instruction_add(node->bin.left, node->bin.right, output);
		break;

		case NT_Iconst:
			print_instruction_add(node, NULL, output);
		break;
		
		case NT_Store:
			print_instruction_store(node->store.address, node->store.data, output);
		break;
		default:
			printf("%d\n", node->type);
			assert(0 && "Unhandled node type" );
	}
}


