#ifndef __REGISTER_H
#define __REGISTER_H
typedef struct NODE_T node_t;
typedef struct NODE_LIST_T node_list_t;
typedef struct REG_FILE_T reg_file_t;
typedef struct REG_T reg_t;

struct REG_T
{
	char *name;
	int id;
	int in_use;
};

struct REG_FILE_T
{
	reg_t *regs;
	int size;
};



#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 


typedef struct liveness{
	int *vars;
	int size;
	}life_t;



void rg_init();
reg_t rg_get_zero();
reg_t rg_allocate();
void givepostnumbers_tree(node_list_t *start);
void givepostnumbers(node_t *start);



void find_use_def_stmt(node_list_t *start);	
void print_use_def_stmt(node_list_t *start);	


void print_nodes();
void assign_nodes_list(node_list_t *start);
void init_reg_lives();
void give_regs();
node_t **nodes;
int *reg_nodes_rep;

#endif //__REGISTER_H

