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
  node_t *node = NULL;

  switch ( expr->type )
  {
    case ET_Binary:
    {
      switch ( expr->binary.op )
      {
        case AddopM:
        case MuldivandopDiv:
        case MuldivandopM:
        case MuldivandopMod:
        case AddopP: {
          expression_t *e_left, *e_right;
          node_t *left, *right;
          
          // have constants always on the right site ( might prove usefull )
          if ( expr->binary.left->type == ET_Constant ) {
            e_right= expr->binary.left;
            e_left = expr->binary.right;
          } else {
            e_left = expr->binary.left;
            e_right = expr->binary.right;
          }
          
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
        } 
        break;
      }
    }
  }

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

