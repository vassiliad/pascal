#include <stdlib.h>
#include <assert.h>
#include "graph.h"

graph_t graph_init(FILE* output)
{
  graph_t ret;

  ret.output = output;
  ret.printed = NULL;
  ret.num = 0;

  fprintf(output, "digraph G {\n");

  return ret;
}

void graph_finalize(graph_t *graph) {
  fprintf(graph->output, "}\n");
  if ( graph->printed )
    free(graph->printed);
  graph->num = 0;
}

void graph_tree(node_list_t *n, graph_t *graph) {
  node_list_t *p;

  for (p=n; p; p=p->next ) {
    graph_instruction(p->node, graph);
  }
}

void graph_instruction(node_t *n, graph_t *graph)
{ 
  char *instr_name;
  int i = 0;

  for ( i=0; i<graph->num; i++ )
    if ( graph->printed[i] == n )
      return;
  
  graph->printed = (node_t**) realloc(graph->printed, sizeof(node_t*)*(graph->num+1));
  graph->printed[ (graph->num) ++ ] = n;

  switch(n->type) {
    case NT_Iconst:
      fprintf(graph->output, "n%p [label=\"addi %s, $0, %d\"];\n", n, n->reg.name, n->iconst);
    break;
    

    case NT_Load:
      if ( n->load.address ) {
        fprintf(graph->output, "n%p -> n%p;\n", n, n->load.address);
        graph_instruction(n->load.address, graph);
        if ( n->label )
          fprintf(graph->output, "n%p [label=\"%s: lw %s, %d(%s)\"];\n", n, n->label, n->reg.name, n->load.offset,
          n->load.address->reg.name);
        else 
          fprintf(graph->output, "n%p [label=\"lw %s, %d(%s)\"];\n", n, n->reg.name, n->load.offset,
          n->load.address->reg.name);
      } else {
        fprintf(graph->output, "n%p [label=\"lw %s, %d($00)\"];\n", n, n->reg.name, n->load.offset);
      }

    break;

    
    case NT_Add:
      if ( n->bin.right->type != NT_Iconst ) {
        fprintf(graph->output, "n%p -> { n%p ; n%p };\n",n, n->bin.left, n->bin.right );
        

        graph_instruction(n->bin.left, graph);
        graph_instruction(n->bin.right, graph);
        
        if ( n->label )
          fprintf(graph->output, "n%p [label=\"%s: add %s, %s, %s\"];\n", n, n->label, n->reg.name,
                    n->bin.left->reg.name, n->bin.right->reg.name);
        else
          fprintf(graph->output, "n%p [label=\"add %s, %s, %s\"];\n", n, n->reg.name,
                    n->bin.left->reg.name, n->bin.right->reg.name);
     } else {
        fprintf(graph->output, "n%p -> n%p\n", n, n->bin.left);
       

        graph_instruction(n->bin.left, graph);

        fprintf(graph->output, "n%p [label=\"addi %s, %s, %d\"];\n", n, n->reg.name,
                  n->bin.left->reg.name, n->bin.right->iconst);
       
     }
    break;
    
    case NT_Mult:
      fprintf(graph->output, "n%p -> { n%p ; n%p };\n",n, n->bin.left, n->bin.right );
        

      graph_instruction(n->bin.left, graph);
      graph_instruction(n->bin.right, graph);

      fprintf(graph->output, "n%p [label=\"mult %s, %s\\nmflo %s\"];\n", n, 
                  n->bin.left->reg.name, n->bin.right->reg.name, n->reg.name);
      
    break;

    case NT_Sub:
      instr_name = "sub";
      goto print_binary;

    case NT_LessThan:
      instr_name = "slt";
    print_binary:
      fprintf(graph->output, "n%p -> { n%p ; n%p };\n",n, n->bin.left, n->bin.right );
      

      graph_instruction(n->bin.left, graph);
      graph_instruction(n->bin.right, graph);

      fprintf(graph->output, "n%p [label=\"%s %s, %s, %s\"];\n", n, instr_name, n->reg.name,
                n->bin.left->reg.name, n->bin.right->reg.name);

    break;
    
    case NT_If:
      graph_instruction(n->_if.branch, graph);
      if ( n->_if._true )
        graph_tree(n->_if._true, graph);
      if ( n->_if._false )
        graph_tree(n->_if._false, graph);
    break;


    case NT_BranchZ:
      graph_instruction(n->branchz.condition, graph);
      fprintf(graph->output, "n%p -> n%p\n", n, n->branchz.condition);
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"%s: bne %s, %s\"];\n", n, n->label, 
              n->branchz.condition->reg.name, n->branchz.condition->label);
      else
       fprintf(graph->output, "n%p [label=\"bne %s, %s\"];\n", n, 
              n->branchz.condition->reg.name, n->branchz.label);

    break;

    case NT_Store:

      if ( n->store.address) {
        fprintf(graph->output, "n%p -> {n%p ; n%p};\n", n, n->store.address, n->store.data);
        graph_instruction(n->store.address, graph);
      } else {
        fprintf(graph->output, "n%p -> n%p;\n",n, n->store.data);
      }

      graph_instruction(n->store.data, graph);
      
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"%s: sw %s, %d(0)\"];\n", n, n->label, n->store.data->reg.name, n->store.offset);
      else
       fprintf(graph->output, "n%p [label=\"sw %s, %d(0)\"];\n", n, n->store.data->reg.name, n->store.offset);
    break;

    default: {
     // assert(0 && "Not implemented");
    }
  }

}

