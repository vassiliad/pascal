#ifndef _GRAPH_H
#define _GRAPH_H
#include <stdio.h>
#include "tree.h"

typedef struct GRAPH_T graph_t;

struct GRAPH_T {
  node_t **printed;
  int num;
  FILE *output;
};


graph_t graph_init(FILE* output);
void graph_finalize(graph_t *graph);
void graph_tree(node_list_t *n, graph_t *graph);
void graph_instruction(node_t *n, graph_t *graph);

#endif // _GRAPH_H
