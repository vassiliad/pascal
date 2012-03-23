#ifndef _TREE_H
#define _TREE_H
#include "statements.h"
#include "constants.h"
#include "symbol_table.h"
#include "register.h"

typedef struct NODE_T node_t;
typedef struct NODE_BIN_T node_bin_t;
typedef struct NODE_LOAD_STORE_T node_load_t;
typedef struct NODE_LOAD_STORE_T node_store_t;
typedef struct NODE_STRING_T node_string_t;
typedef struct NODE_LIST_T node_list_t;
typedef struct NODE_IF_T node_if_t;
typedef struct NODE_BRANCHZ_T node_branchz_t;

enum NodeType
{
  NT_Iconst,  // int  ( iconst )
  NT_Bconst,  // char ( bconst )
  NT_Rconst,  // float ( rconst )
  NT_Cconst,  // char ( cconst )
  NT_Load,    // node_load_t ( load )
  NT_Store=5,   // node_store_t ( store )
  NT_Add,     // node_bin_t ( bin )
  NT_Sub,     // node_bin_t ( bin )
  NT_Div,     // node_bin_t ( bin )
  NT_Mod,     // node_bin_t ( bin )
  NT_Mult=10,    // node_bin_t ( bin )
  NT_String,  // node_string_t ( string )
  NT_Not,     // node_t ( not )
	NT_If,			// node_if_t ( _if )
	NT_Jump,		// char *( jump_label )
	NT_BranchZ=15, // node_branchz_t ( branchZ )
  NT_LessThan,// node_bin_t ( bin )
  NT_Nop,     // none!!
};


struct NODE_LIST_T {
	struct NODE_LIST_T *next, *prev;
	struct NODE_T *node;
};
struct NODE_BRANCHZ_T // branch on zero
{
	node_t *condition;
	char *label;
};

struct NODE_IF_T
{
	node_list_t *_true;
	node_list_t *_false;
	node_t *branch;
};

struct NODE_STRING_T
{
  char *mem;
  int size;
};

struct NODE_LOAD_STORE_T
{
  reg_t reg;
  int offset;
  node_t *data;
};

struct NODE_BIN_T
{
  node_t *left, *right;
};

struct NODE_T
{
  char *label;
  int type;
  union
  {
    node_bin_t bin;
    node_load_t load;
    node_store_t store;
    node_string_t string;
    node_t *not;
    int iconst;
    char cconst;
    char bconst;
    float rconst;
		char *jump_label;
		node_branchz_t branchz;
		node_if_t _if;
  };
	reg_t reg;
  node_t *next, *prev;
};


node_list_t *tree_generate_tree(statement_t *root, scope_t *scope);
#endif
