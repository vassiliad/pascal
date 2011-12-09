#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"
#include "constants.h"

scope_t *st_init(scope_t *scope)
{
	scope_t *ret = ( scope_t * ) calloc(1, sizeof(scope_t));
	ret->parent = scope;
	return ret;
}

scope_t *st_destroy(scope_t *scope)
{
  //TODO free the contents of the scope
  return scope->parent;
}

typedefs_entry_t* st_typedef_find(char *name, scope_t *scope)
{
	int i;
	
	while ( scope ) {
		for ( i = 0; i < scope->typedefs_size; i ++ )
			if ( !strcasecmp(scope->typedefs[i].name, name ) )
				return scope->typedefs + i;
		scope = scope->parent;
	}
	
	return NULL;
}

#warning prepei na elegxw gia ta arrays an uparxoun oi diastaseis tous kai gia ta records an exoun fields\
warning pou einai array an uparxei to ka8e typedef gia to antistoixo field pou einai array ...

int st_typedef_register(typedefs_entry_t *entry, scope_t *scope)
{
  typedefs_entry_t *found = st_typedef_find(entry->name, scope);

	if ( found ) {
		printf("typedef %s is already defined\n", entry->name);
		return Failure;
	}
	
	scope->typedefs = ( typedefs_entry_t* ) realloc( scope->typedefs,
									sizeof(typedefs_entry_t) * ( scope->typedefs_size +1 ));
	scope->typedefs[ scope->typedefs_size ++ ] = *entry;
  printf("registered type %s\n", entry->name);
	return Success;
}

var_t *st_var_define(char* id, data_type_t type, scope_t *scope)
{
  int i;

   for ( i=0; i < scope->vars_size; i++ ) {
    
      if ( !strcasecmp(id, scope->vars[i].id))
        break;
    }

  if ( scope->vars_size && i!=scope->vars_size ) {
    printf("Error: %s has already been defined\n",id );
    return 0;
  }

  scope->vars = ( var_t * ) realloc(scope->vars, sizeof(var_t) * (scope->vars_size+1) );
  scope->vars[ scope->vars_size ].id = id;
  scope->vars[ scope->vars_size ].pass = 0;
  scope->vars[ scope->vars_size ].reference = NULL;
  scope->vars[ scope->vars_size ].type = type;

  return scope->vars + (scope->vars_size++);
}

var_t* st_var_find(char *id, scope_t *scope)
{
  int i;
  
  if ( id == NULL )
    return NULL;

  for ( ; scope; scope = scope->parent ) {
    for ( i = 0; i < scope->vars_size; i ++ )
      if ( !strcasecmp(id, scope->vars[i].id) )
        return scope->vars + i;
  }

  return NULL;
}

func_t *st_func_find(char *id, scope_t *global)
{
  int i = 0;
  
  if ( id == 0 )
    return 0;

  for ( i = 0 ; i < global->funcs_size; i ++ ) {
    if ( strcasecmp(id, global->funcs[i].id) == 0 )
      return global->funcs + i;
  }
  return NULL;
}


func_t *st_func_define(char *id, data_type_t type, var_t *params, int size, scope_t *scope)
{
  int i;
  func_t *func = NULL;

  for ( i = 0; i < scope->funcs_size; i ++ ) {
    if ( !strcasecmp(id, scope->funcs[i].id) ) {
      printf("Function %s is already defined\n", id);
      return NULL;
    }
  }
 
  scope->funcs = ( func_t* ) realloc( scope->funcs, (scope->funcs_size+1) * ( sizeof(func_t)));
  func = scope->funcs + scope->funcs_size++;

  func->id = id;
  func->type = type;
  func->params = params;
  func->size = size;


  return func;
}

const_t* st_const_define(char *id, constant_t *constant, scope_t *scope)
{
  if ( st_const_find(id, scope) ) {
    printf("constdef %s is already defined\n", id);
    return NULL;
  }

  scope->consts = ( const_t* ) realloc( scope->consts, (scope->consts_size+1) * sizeof(const_t));
  scope->consts[ scope->consts_size ].id = id;
  scope->consts[ scope->consts_size ].constant = *constant;

  return scope->consts + ( scope->consts_size++ );
}

const_t* st_const_find(char *id, scope_t *scope)
{
  int i;
  scope_t *p;

  for ( p = scope; p; p = p->parent )
    for ( i = 0; i < p->consts_size; i ++ )
      if ( !strcasecmp(id, p->consts[i].id) ) {
        return p->consts+i;
      }
    
  return NULL;
}


