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
  node_t *prev, *cur;

  for (p=n; p; p=p->next ) {
    if ( p->node->type == NT_For )
      cur = p->node->_for.init;
    else if ( p->node->type == NT_While )
      cur = p->node->_while.branch;
    else
      cur = p->node;

    if ( p->prev ) {
      if ( p->prev->node->type == NT_For )
        prev = p->prev->node->_for.init;
      else if ( p->prev->node->type == NT_While )
        prev = p->prev->node->_while.branch;
      else
        prev = p->prev->node;

      fprintf(graph->output, "n%p -> n%p [style=dotted];\n",cur, prev); 
    }

    graph_instruction(p->node, graph);
  }
}

void graph_instruction(node_t *n, graph_t *graph)
{ 
  char *instr_name;
  int i = 0;

  if ( n == NULL ) 
    return;

  printf("parent: of %p (%ld--%d) %p-(%d)\n", n, n->post, n->type,  n->parent[0], (n->parent[0]?n->parent[0]->type : -1));

  for ( i=0; i<graph->num; i++ )
    if ( graph->printed[i] == n )
      return;

  graph->printed = (node_t**) realloc(graph->printed, sizeof(node_t*)*(graph->num+1));
  graph->printed[ (graph->num) ++ ] = n;

  switch(n->type) {
    case NT_Iconst:
      fprintf(graph->output, "n%p [label=\"[%ld]  addi %s, $0, %d\"];\n", n, n->post, n->reg->name, n->iconst);
      break;


    case NT_Load:
      if ( n->load.address ) {
        fprintf(graph->output, "n%p -> n%p;\n", n, n->load.address);
        graph_instruction(n->load.address, graph);
        if ( n->label )
          fprintf(graph->output, "n%p [label=\"[%ld]  %s: lw %s, %d(%s)\"];\n", n, n->post, n->label, n->reg->name, n->load.offset,
              n->load.address->reg->name);
        else 
          fprintf(graph->output, "n%p [label=\"[%ld]  lw %s, %d(%s)\"];\n", n, n->post, n->reg->name, n->load.offset,
              n->load.address->reg->name);
      } else {
        fprintf(graph->output, "n%p [label=\"[%ld]  lw %s, %d($00)\"];\n", n, n->post, n->reg->name, n->load.offset);
      }

      break;



    case NT_Mult:
      fprintf(graph->output, "n%p -> { n%p ; n%p };\n",n, n->bin.left, n->bin.right );


      graph_instruction(n->bin.left, graph);
      graph_instruction(n->bin.right, graph);

      fprintf(graph->output, "n%p [label=\"[%ld]  mult %s, %s\\nmflo %s\"];\n", n, 
          n->post, n->bin.left->reg->name, n->bin.right->reg->name, n->reg->name);

      break;

    case NT_Add:
      instr_name = "add";
      goto print_binary;

    case NT_Sub:
      instr_name = "sub";
      goto print_binary;

    case NT_LessThan:
      instr_name = "slt";
print_binary:
      fprintf(graph->output, "n%p -> { n%p ; n%p };\n",n, n->bin.left, n->bin.right );


      graph_instruction(n->bin.left, graph);
      graph_instruction(n->bin.right, graph);

      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: %s %s, %s, %s\"];\n", n, n->post, n->label, instr_name, n->reg->name,
            n->bin.left->reg->name, n->bin.right->reg->name);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  %s %s, %s, %s\"];\n", n, n->post, instr_name, n->reg->name,
            n->bin.left->reg->name, n->bin.right->reg->name);

      break;


    case NT_While:
      {
        node_t *p = n->_while.branch;
        assert(n->_while.branch->type == NT_BranchZ);

        graph_instruction(p->branchz.condition, graph);

        fprintf(graph->output, "n%p -> n%p\n", p, p->branchz.condition);

        if ( n->label )
          fprintf(graph->output, "n%p [label=\"[%ld]  %s: bne %s, %s\"];\n", p, p->post, n->label, 
              p->branchz.condition->reg->name, p->branchz.label);
        else
          fprintf(graph->output, "n%p [label=\"[%ld]  bne %s, %s\"];\n", p,  p->post,
              p->branchz.condition->reg->name, p->branchz.label);

        node_list_t hack, *temp = n->_while.loop->prev;
        hack.prev = hack.next = NULL;

        hack.node = p;

        n->_while.loop->prev = &hack;
        graph_tree(n->_while.loop, graph);
        n->_while.loop->prev = temp;

      }
      break;

    case NT_For:
      {
        node_t *branch, *init;

        init  =n->_for.init;
        branch=n->_for.branch;

        graph_instruction(init, graph);
        graph_instruction(branch, graph);
#warning THIS IS A BUG, if a for has a label it will NOT be displayed
        fprintf(graph->output, "n%p -> n%p [style=dotted];\n", init, branch);

        node_list_t hack, *temp = n->_for.loop->prev;
        hack.prev = hack.next = NULL;

        hack.node = branch;

        n->_for.loop->prev = &hack;
        graph_tree(n->_for.loop, graph);
        n->_for.loop->prev = temp;
      } 
      break;

    case NT_Nop:
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: nop\"];\n", n, n->post, n->label);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  nop\"];\n", n, n->post);
      break;


    case NT_If: 
      {
        node_t *p;
        p = n->_if.branch;

        graph_instruction(p->branchz.condition, graph);

        assert(n->_if.branch->type == NT_BranchZ);

        fprintf(graph->output, "n%p -> n%p\n", n, p->branchz.condition);
        if ( n->label )
          fprintf(graph->output, "n%p [label=\"[%ld]  %s: bne %s, %s\"];\n", n, p->post, n->label, 
              p->branchz.condition->reg->name, p->branchz.label);
        else
          fprintf(graph->output, "n%p [label=\"[%ld]  bne %s, %s\"];\n", n,  p->post,
              p->branchz.condition->reg->name, p->branchz.label);

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
      }
      break;


    case NT_BranchZ:
      graph_instruction(n->branchz.condition, graph);
      fprintf(graph->output, "n%p -> n%p\n", n, n->branchz.condition);
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: bne %s, %s\"];\n", n, n->post, n->label, 
            n->branchz.condition->reg->name, n->branchz.label);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  bne %s, %s\"];\n", n,  n->post,
            n->branchz.condition->reg->name, n->branchz.label);

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
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: sw %s, %d(0)\"];\n", n,n->post, n->label, n->store.data->reg->name, n->store.offset);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  sw %s, %d(0)\"];\n", n,n->post, n->store.data->reg->name, n->store.offset);
      break;

    case NT_Addi:
      graph_instruction(n->bin_semi.left, graph);
      fprintf(graph->output, "n%p -> n%p\n", n, n->bin_semi.left);
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: addi %s, %s, %d\"];\n", n, n->post, n->label, 
            n->reg->name, n->bin_semi.left->reg->name, n->bin_semi.immediate);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  addi %s, %s, %d\"];\n", n, n->post,
            n->reg->name, n->bin_semi.left->reg->name, n->bin_semi.immediate);
      break;

    case NT_LessThani:
      graph_instruction(n->bin_semi.left, graph);
      fprintf(graph->output, "n%p -> n%p\n", n, n->bin_semi.left);
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: slti %s, %s, %d\"];\n", n, n->post, n->label, 
            n->reg->name, n->bin_semi.left->reg->name, n->bin_semi.immediate);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  slti %s, %s, %d\"];\n", n, n->post,
            n->reg->name, n->bin_semi.left->reg->name, n->bin_semi.immediate);
      break;

    case NT_Subi:
      graph_instruction(n->bin_semi.left, graph);
      fprintf(graph->output, "n%p -> n%p\n", n, n->bin_semi.left);
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: subi %s, %s, %d\"];\n", n, n->post, n->label, 
            n->reg->name, n->bin_semi.left->reg->name, n->bin_semi.immediate);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  subi %s, %s, %d\"];\n", n, n->post,
            n->reg->name, n->bin_semi.left->reg->name, n->bin_semi.immediate);
      break;

    case NT_Ori:
      graph_instruction(n->bin_semi.left, graph);
      fprintf(graph->output, "n%p -> n%p\n", n, n->bin_semi.left);
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: ori %s, %s, %d\"];\n", n, n->post, n->label, 
            n->reg->name, n->bin_semi.left->reg->name, n->bin_semi.immediate);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  ori %s, %s, %d\"];\n", n, n->post,
            n->reg->name, n->bin_semi.left->reg->name, n->bin_semi.immediate);
      break;

    case NT_Lui:
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: lui %s, %d\"];\n", n, n->post, n->label, 
            n->reg->name, n->iconst);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  lui %s, %d\"];\n", n, n->post,
            n->reg->name, n->iconst);
      break;

    case NT_Jump:
      if ( n->label )
        fprintf(graph->output, "n%p [label=\"[%ld]  %s: j %s\"];\n", n, n->post, n->label, 
            n->jump_label);
      else
        fprintf(graph->output, "n%p [label=\"[%ld]  j %s\"];\n", n, n->post,
            n->jump_label);
      break;

    default: 
      printf("CANNOT HANDLE %d\n", n->type);
      assert(0 && "Not implemented");

  }

}

