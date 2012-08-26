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
  static int id = 0;
  int i = 0;
  int me = id;

  for ( i=0; i<graph->num; i++ )
    if ( graph->printed[i] == n )
      return;
  
  graph->printed = (node_t**) realloc(graph->printed, sizeof(node_t*)*(graph->num+1));
  graph->printed[ (graph->num) ++ ] = n;

  switch(n->type) {
    case NT_Iconst:
      fprintf(graph->output, "n%d [label=\"addi %s, $0, %d\"];\n",id++, n->reg.name, n->iconst);
    break;
    
    case NT_Add:
      if ( n->bin.right->type != NT_Iconst ) {
        fprintf(graph->output, "n%d -> { n%d ; n%d };\n", me, id+1, id+2);
        
        id++;

        graph_instruction(n->bin.left, graph);
        graph_instruction(n->bin.right, graph);

        fprintf(graph->output, "n%d [label=\"add %s, %s, %s\"];\n", me, n->reg.name,
                  n->bin.left->reg.name, n->bin.right->reg.name);
     } else {
        fprintf(graph->output, "n%d -> n%d\n", me, id+1);
       
        id++;

        graph_instruction(n->bin.left, graph);

        fprintf(graph->output, "n%d [label=\"addi %s, %s, %d\"];\n", me, n->reg.name,
                  n->bin.left->reg.name, n->bin.right->iconst);
       
     }
    break;


    case NT_Store:

      if ( n->store.address) {
        fprintf(graph->output, "n%d -> {n%d ; n%d};\n", me, id+1, id+2);
        id++;
        graph_instruction(n->store.address, graph);
      } else {
        fprintf(graph->output, "n%d -> n%d;\n",id, id+1);
        id++;
      }

      graph_instruction(n->store.data, graph);

      fprintf(graph->output, "n%d [label=\"sw %s, %d(0)\"];\n", me, n->store.data->reg.name, n->store.offset);
    break;

    default: {
     // assert(0 && "Not implemented");
    }
  }

}

