#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H
#include "bison_union.h"
typedef struct SCOPE_T scope_t;
typedef struct VAR_T var_t;
typedef struct CONST_T const_t;
typedef sub_header_t func_t;

struct CONST_T
{
  char *id;
  constant_t constant;
};

struct VAR_T
{
	data_type_t type;
	char *id;
  int pass;
  struct VAR_T* reference; // used only when pass == 1
};

struct SCOPE_T
{
	typedefs_entry_t *typedefs;
	int typedefs_size;
	
  var_t *vars;
	int vars_size;
  
  func_t *funcs;
  int funcs_size;
  
  const_t *consts;
  int consts_size;
  
	scope_t *parent;
};

#warning na psaxnw kai sta CONST kai sta VARS kai sta functions(?) gia dipla ID
scope_t *st_init(scope_t *scope);
scope_t *st_destroy(scope_t *scope);
int st_typedef_register(typedefs_entry_t *entry, scope_t *scope);
typedefs_entry_t* st_typedef_find(char *name, scope_t *scope);
var_t* st_var_define(char* id, data_type_t type, scope_t *scope);
var_t* st_var_find(char *id, scope_t *scope);
func_t *st_func_define(char *id, data_type_t type, parameters_t *params, int size, scope_t *scope);
const_t* st_const_define(char *id, constant_t *constant, scope_t *scope);
const_t* st_const_find(char *id, scope_t *scope);

#endif // __SYMBOL_TABLE_H
