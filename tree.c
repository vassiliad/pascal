#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "expressions.h"

node_t *tree_generate_tree(statement_t *root, scope_t *scope)
{ 
  node_t *node, *t;
  statement_t *p;

  if ( root == NULL )
    return NULL;
  
  node = tree_generate_node( root, scope );
  
  if ( node == NULL )
    return NULL;

  for ( p = root->next; p ; p = p->next ) {
    t = tree_generate_node( p, scope );

    if ( t == NULL )
      return NULL;
  }
  
  return node;
}

node_t *tree_generate_store_str(expression_t *var, char *string, scope_t *scope)
{
  return NULL;
}

node_t *tree_generate_store(expression_t *var, expression_t *expr, scope_t *scope)
{
  
  return NULL;
}

node_t *tree_generate_load(expression_t *var, scope_t *scope)
{
  return NULL;
}

node_t *tree_generate_sconst(char *string)
{
  return NULL;
}

node_t *tree_generate_value( expression_t *expr, scope_t *scope)
{
  return NULL;
}

node_t *tree_generate_node(statement_t *node, scope_t *scope)
{  
  node_t *_node = NULL;
  

  if ( node == NULL )
    return NULL;

  printf("node_type: %s\n", statement_type_to_string(node));
  
  switch ( node->type )
  {
    case ST_Assignment:
    {
      node_t *var, *data;

      if ( node->assignment.type == AT_Expression ) {
        var = tree_generate_store( node->assignment.var, node->assignment.expr, scope);
        data = tree_generate_value( node->assignment.expr, scope );
      } else {
        var = tree_generate_store_str(node->assignment.var, node->assignment.string, scope);
        data = tree_generate_sconst( node->assignment.string );
      }

      _node = calloc (1, sizeof(node_t));
      _node->type = NT_Store;
      _node->load.address = var;
      _node->load.data = data;
    break;
   }
  }

  return _node;
}

