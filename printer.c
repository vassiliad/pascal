#include "printer.h"
void print_instruction_tree(node_t *node, FILE* output)
{
	if ( node == NULL )
		return;
	
	if ( node->label )
		fprintf("%s : ", node->label );
	
	switch ( node->type ) {
		case NT_Add:
			if ( node->left->type == NT_Iconst ) {

				if ( node->right->type == NT_Iconst ) {
					
				} else {
					// right is supposed to reside in a register
				}
			} else {

			}
		break;
	}
}


