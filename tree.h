#ifndef _TREE_H
#define _TREE_H
#include "statements.h"
#include "constants.h"
#include "symbol_table.h"

typedef struct NODE_T node_t;
typedef struct NODE_BIN_T node_bin_t;
typedef struct NODE_LOAD_STORE_T node_load_t;
typedef struct NODE_LOAD_STORE_T node_store_t;
typedef struct NODE_STRING_T node_string_t;
typedef struct NODE_IF_T node_if_t;
typedef struct NODE_BRANCHZ_T node_branchz_t;

enum NodeType
{
  NT_Iconst,  // int  ( iconst )
  NT_Bconst,  // char ( bconst )
  NT_Rconst,  // float ( rconst )
  NT_Cconst,  // char ( cconst )
  NT_Load,    // node_load_t ( load )
  NT_Store,   // node_store_t ( store )
  NT_Add,     // node_bin_t ( bin )
  NT_Sub,     // node_bin_t ( bin )
  NT_Div,     // node_bin_t ( bin )
  NT_Mod,     // node_bin_t ( bin )
  NT_Mult,    // node_bin_t ( bin )
  NT_String,  // node_string_t ( string )
  NT_Not,     // node_t ( not )
	NT_If,			// node_if_t ( _if )
	NT_Jump,		// node_t ( jump )
	NT_BranchZ, // node_branchz_t ( branchZ )
};

struct NODE_BRANCHZ_T // branch on zero
{
	node_t *condition;
	node_t *destination;
};

struct NODE_IF_T
{
	node_t *_true;
	node_t *_false;
	node_t *condition;
};

struct NODE_STRING_T
{
  char *mem;
  int size;
};

struct NODE_LOAD_STORE_T
{
  node_t *address;
  node_t *data;
  unsigned int size;
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
		node_t *jump;
		node_branchz_t *branchz;
  };

  node_t *next, *prev;
};


node_t *tree_generate_tree(statement_t *root, scope_t *scope);
#endif
