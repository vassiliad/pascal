#include <assert.h>
#include "printer.h"

void print_instruction(node_t *node, FILE* output);
void print_instruction_add(node_t *left, node_t *right, reg_t *reg, 
			FILE* output);
void print_instruction_sub(node_t *left, node_t *right, reg_t *reg,
			 FILE* output);
void print_instruction_store(int offset, node_t *data, 
			node_t *address, FILE* output);
void print_instruction_load(int offset, reg_t *dest,
		 node_t *address, FILE *output);
void print_instruction_less_than(node_t *left, node_t *right, 
		reg_t* reg, FILE *output);

void print_instruction_add(node_t *left, node_t *right, reg_t *reg,
			 FILE* output)
{
	if ( right == NULL ) {
		// this add is used as a move
		switch(left->type) {
			case NT_Iconst:
				if ( left->iconst != 0 )
					fprintf(output, "addi %s, $0, %d\n", left->reg->name, 
							left->iconst);
					
			break;

      case NT_Bconst:
        fprintf(output, "addi %s, $0, %d\n", left->reg->name, 
							left->bconst);
      break;

			default:
				assert(0 && "Unhandled type");
		}
	} else {

    print_instruction(left, output);
    
    if ( right->type == NT_Iconst ) {
      fprintf(output, "addi %s, %s, %d\n", reg->name, 
						left->reg->name, right->iconst);
    } else {
      print_instruction(right, output);
      fprintf(output, "add  %s, %s, %s\n", reg->name, 
						left->reg->name, right->reg->name);
    }
  }
}


void print_instruction_sub(node_t *left, node_t *right, reg_t *reg,
			 FILE* output)
{
  print_instruction(left, output);
  print_instruction(right, output);

  fprintf(output, "sub %s, %s, %s\n", reg->name, 
        left->reg->name, right->reg->name);
}

void print_instruction_store(int offset, node_t *data,
			node_t *address, FILE* output)
{
  print_instruction(data, output);
	
	if ( address ) {
		print_instruction(address, output);
	
  	fprintf(output, "sw   %s, %d(%s)\n", data->reg->name, offset, 
			address->reg->name);
	} else
		fprintf(output, "sw   %s, %d($0)\n", data->reg->name, offset);

}

void print_instruction_tree(node_list_t *tree, FILE* output)
{
	node_list_t *c;

	for ( c=tree; c!=NULL; c=c->next)
		print_instruction(c->node, output);
}

void print_instruction_less_than(node_t *left, node_t *right, 
		reg_t* reg, FILE *output)
{
  print_instruction(left, output);
  print_instruction(right, output);
  fprintf(output, "slt  %s, %s, %s\n", reg->name, left->reg->name,
				right->reg->name);
}

void print_instruction_load(int offset, reg_t *dest,
		 node_t *address, FILE *output)
{
	if ( address ) {
		print_instruction(address, output);
		fprintf(output, "lw   %s, %d(%s)\n", dest->name, offset, address->reg->name);
	} else
		fprintf(output, "lw   %s, %d($0)\n", dest->name, offset);
}
void print_instruction_mult(node_t *left, node_t *right, reg_t *reg, 
			FILE* output)
{
	print_instruction(left, output);
	print_instruction(right, output);
	fprintf(output, "mult %s, %s\n", left->reg->name, right->reg->name);
	fprintf(output, "mflo %s\n", reg->name);
}

void print_instruction(node_t *node, FILE* output)
{
	if ( node == NULL )
		return;
	
	if ( node->label )
		fprintf(output, "%s : ", node->label );

  // printf("#\t\tnode type: %d (%p)\n", node->type, node);
	
	switch ( node->type ) {

    case NT_Sub:
			print_instruction_sub(node->bin.left, node->bin.right,
					&node->reg, output);
		break;


		case NT_Add:
			print_instruction_add(node->bin.left, node->bin.right,
					&node->reg, output);
		break;

    case NT_Bconst:
		case NT_Iconst:
			print_instruction_add(node, NULL, NULL, output);
		break;
		
    case NT_Nop:
      fprintf(output, "add  $0, $0, $0\n");
    break;

    case NT_Load:
      print_instruction_load(node->load.offset, 
					&node->reg, node->load.address, output);
    break;

		case NT_Store:
			print_instruction_store(node->store.offset,
					node->store.data, node->store.address, output);
		break;

    case NT_LessThan:
      print_instruction_less_than(node->bin.left, node->bin.right,
					&node->reg, output);
    break;
    
    case NT_BranchZ:
      print_instruction(node->branchz.condition, output);
      fprintf(output, "bne  $0, %s, %s\n", 
					node->branchz.condition->reg->name, node->branchz.label);
    break;
    
		case NT_Mult:
			print_instruction_mult(node->bin.left, node->bin.right, 
					&node->reg, output);
		break;
    
    case NT_Jump:
      fprintf(output, "j    %s\n", node->jump_label);
    break;

		case NT_While:
			print_instruction(node->_while.branch, output);
			print_instruction_tree(node->_while.loop, output);
		break;
    
    case NT_For:
      print_instruction(node->_for.init, output);
      print_instruction(node->_for.branch, output);
      print_instruction_tree(node->_for.loop, output);
    break;

    case NT_If:
      print_instruction(node->_if.branch, output);
      if ( node->_if._true)
        print_instruction_tree(node->_if._true, output);
      if ( node->_if._false)
        print_instruction_tree(node->_if._false, output);
    break;

		default:
			printf("%d\n", node->type);
			assert(0 && "Unhandled node type" );
	}
}



