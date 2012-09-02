#ifndef _TREE_H
#define _TREE_H
#include "statements.h"
#include "constants.h"
#include "symbol_table.h"
#include "register.h"
#include "schedule.h"

typedef struct NODE_BIN_SEMI_T node_bin_semi_t;
typedef struct NODE_BIN_T node_bin_t;
typedef struct NODE_LOAD_STORE_T node_load_t;
typedef struct NODE_LOAD_STORE_T node_store_t;
typedef struct NODE_STRING_T node_string_t;
typedef struct NODE_IF_T node_if_t;
typedef struct NODE_WHILE_T node_while_t;
typedef struct NODE_FOR_T node_for_t;
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
  NT_BranchZ=15, // node_branchz_t ( branchz )
  NT_LessThan,// node_bin_t ( bin )
  NT_While,   // node_while_t (_while )
  NT_For,     // node_for_t ( _for )
  NT_Nop,     // none!!
  NT_Addi=20, // node_bin_semi_t ( bin_semi )
  NT_Subi,    // node_bin_semi_t ( bin_semi )
  NT_Ori,     // node_bin_semi_t ( bin_semi )
  NT_LessThani,  // node_bin_semi_t ( bin_semi )
  NT_Lui      // int ( iconst )

};

struct NODE_FOR_T
{
	node_t *branch;
  node_t *init;
	node_list_t *loop;
	life_t *def;
	life_t *use;
	int scheduled;
	unsigned int id;
};


struct NODE_WHILE_T
{
	node_t *branch;
	node_list_t *loop;
	life_t *def;
	life_t *use;
	int scheduled;	
	unsigned int id;
};

struct NODE_LIST_T {
	struct NODE_LIST_T *next, *prev;
	struct NODE_T *node;
	life_t *def;
	life_t *use;
	int scheduled;
	unsigned int id;
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
	int scheduled;
	unsigned int id;
};

struct NODE_STRING_T
{
  char *mem;
  int size;
};

struct NODE_LOAD_STORE_T
{
  int unique_id;
	node_t *address; // this will be used to generate the address for       //
									// the memory access at runtime, its result will       //
								 // be stored in @reg.                                  //
								// In the event that the generated address is constant //
							 // i.e. it does not depend on any variable             //
						  // it will be added to @offset                         //
  int offset;
  node_t *data;
};

struct NODE_BIN_SEMI_T
{
  node_t *left;
  int immediate;
};

struct NODE_BIN_T
{
  node_t *left, *right;
};

struct NODE_T
{
  char *label;
  enum NodeType type;
  long int post;
	int scheduled;
	long int time;
  union
  {
    node_bin_semi_t bin_semi;
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
		node_while_t _while;
		node_for_t _for;
  };
	reg_t reg;
	int reg_id;

  node_t *parent;
};


node_list_t *tree_generate_tree(statement_t *root, scope_t *scope);
#endif


