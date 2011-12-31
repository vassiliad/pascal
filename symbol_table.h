#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H
#include "bison_union.h"
typedef struct SCOPE_T scope_t;
typedef struct VAR_T var_t;
typedef struct CONST_T const_t;
typedef struct SUB_FUNC_T func_t;

struct SUB_FUNC_T 
{
  char *id;
  int isProcedure;
  data_type_t type;
  var_t *params;
  int size;
  statement_t *body;
	scope_t *scope; // used in forward declarated functions
};


struct CONST_T
{
  char *id;
  constant_t constant;
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

scope_t *st_init(scope_t *scope);
scope_t *st_destroy(scope_t *scope);
int st_typedef_register(typedefs_entry_t *entry, scope_t *scope);
typedefs_entry_t* st_typedef_find(char *name, scope_t *scope);
var_t* st_var_define(char* id, data_type_t type, scope_t *scope);
var_t* st_var_find(char *id, scope_t *scope);
func_t *st_func_define(char *id, data_type_t type, var_t *params, int size, scope_t *scope);
func_t *st_func_find(char *id, scope_t *global);
const_t* st_const_define(char *id, constant_t *constant, scope_t *scope);
const_t* st_const_find(char *id, scope_t *scope);

int st_get_type_size(int type, char* userType, scope_t *scope);
#endif // __SYMBOL_TABLE_H
