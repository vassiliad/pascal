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
    if ( p->prev )
      fprintf(graph->output, "n%p -> n%p [style=dotted];\n",p->node, p->prev->node); 

    graph_instruction(p->node, graph);
  }
}

void graph_instruction(node_t *n, graph_t *graph)
{ 
  char *instr_name;
  int i = 0;
  
  if ( n == NULL ) 
    return;

  for ( i=0; i<graph->num; i++ )
    if ( graph->printed[i] == n )
      return;
  
  graph->printed = (node_t**) realloc(graph->printed, sizeof(node_t*)*(graph->num+1));
  graph->printed[ (graph->num) ++ ] = n;

  switch(n->type) {
    case NT_Iconst:
      fprintf(graph->output, "n%p [label=\"[%ld]  addi %s, $0, %d\"];\n", n, n->post, n->reg.name, n->iconst);
    break;
    

    case NT_Load:
      if ( n->load.address ) {
        fprintf(graph->output, "n%p -> n%p;\n", n, n->load.address);
        graph_instruction(n->load.address, graph);
        if ( n->label )
          fprintf(graph->output, "n%p [label=\"[%ld]  %s: lw %s, %d(%s)\"];\n", n, n->post, n->label, n->reg.name, n->load.offset,
          n->load.address->reg.name);
        else 
          fprintf(graph->output, "n%p [label=\"[%ld]  lw %s, %d(%s)\"];\n", n, n->post, n->reg.name, n->load.offset,
          n->load.address->reg.name);
      } else {
        fprintf(graph->output, "n%p [label=\"[%ld]  lw %s, %d($00)\"];\n", n, n->post, n->reg.name, n->load.offset);
      }

    break;

    
    case NT_Add:
      if ( n->bin.right->type != NT_Iconst ) {
        fprintf(graph->output, "n%p -> { n%p ; n%p };\n",n, n->bin.left, n->bin.right );
        

        graph_instruction(n->bin.left, graph);
        graph_instruction(n->bin.right, graph);
        
        if ( n->label )
          fprintf(graph->output, "n%p [label=\"[%ld]  %s: add %s, %s, %s\"];\n", n, n->post, n->label, n->reg.name,
                    n->bin.left->reg.name, n->bin.right->reg.name);
        else
          fprintf(graph->output, "n%p [label=\"[%ld]  add %s, %s, %s\"];\n", n, n->post, n->reg.name,
                    n->bin.left->reg.name, n->bin.right->reg.name);
     } else {
        fprintf(graph->output, "n%p -> n%p\n", n, n->bin.left);
       

        graph_instruction(n->bin.left, graph);

        fprintf(graph->output, "n%p [label=\"[%ld]  addi %s, %s, %d\"];\n", n, n->post, n->reg.name,
                  n->bin.left->reg.name, n->bin.right->iconst);
       
     }
    break;
    
    case NT_Mult:
      fprintf(graph->output, "n%p -> { n%p ; n%p };\n",n, n->bin.left, n->bin.right );
        

      graph_instruction(n->bin.left, graph);
      graph_instruction(n->bin.right, graph);

      fprintf(graph->output, "n%p [label=\"[%ld]  mult %s, %s\\nmflo %s\"];\n", n, 
                  n->post, n->bin.left->reg.name, n->bin.right->reg.name, n->reg.name);
      
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

      fprintf(graph->output, "n%p [label=\"[%ld]  %s %s, %s, %s\"];\n", n, n->post, instr_name, n->reg.name,
                n->bin.left->reg.name, n->bin.right->reg.name);

    break;
    
    case NT_If: {
      node_t *p;
      p = n->_if.branch;

      graph_instruction(p->branchz.condition, graph);

      assert(n->_if.branch->type == NT_BranchZ);

      fprintf(graph->output, "n%p -> n%p\n", n, p->branchz.condition);
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: bne %s, %s\"];\n", n, p->post, p->label, 
              p->branchz.condition->reg.name, p->branchz.label);
      else
       fprintf(graph->output, "n%p [label=\"[%ld]  bne %s, %s\"];\n", n,  p->post,
              p->branchz.condition->reg.name, p->branchz.label);

      if ( n->_if._true ) {
#warning fix this hack ?
        node_list_t *hack = n->_if._true->prev;
        n->_if._true->prev = NULL;
        fprintf(graph->output, "n%p -> n%p [style=dotted];\n", n->_if._true->node, n);
        graph_tree(n->_if._true, graph);
        n->_if._true->prev = hack;
      } if ( n->_if._false ) {
        node_list_t *hack = n->_if._true->prev;
        n->_if._false->prev = NULL;
        fprintf(graph->output, "n%p -> n%p [style=dotted];\n", n->_if._false->node, n);
        graph_tree(n->_if._false, graph);
        n->_if._false->prev = hack;
      }
    break;
  }

    case NT_BranchZ:
      graph_instruction(n->branchz.condition, graph);
      fprintf(graph->output, "n%p -> n%p\n", n, n->branchz.condition);
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: bne %s, %s\"];\n", n, n->post, n->label, 
              n->branchz.condition->reg.name, n->branchz.condition->label);
      else
       fprintf(graph->output, "n%p [label=\"[%ld]  bne %s, %s\"];\n", n,  n->post,
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
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: sw %s, %d(0)\"];\n", n,n->post, n->label, n->store.data->reg.name, n->store.offset);
      else
       fprintf(graph->output, "n%p [label=\"[%ld]  sw %s, %d(0)\"];\n", n,n->post, n->store.data->reg.name, n->store.offset);
    break;

    default: {
     printf("CANNOT HANDLE %d\n", n->type);
     // assert(0 && "Not implemented");
    }
  }

}

