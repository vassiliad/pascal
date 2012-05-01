#ifndef __REGISTER_H
#define __REGISTER_H


typedef struct NODE_LIST_T node_list_t;
typedef struct NODE_T node_t;
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

void rg_init();
reg_t rg_get_zero();
reg_t rg_allocate();
void givepostnumbers_tree(node_list_t *start);
void givepostnumbers(node_t *start);
#endif //__REGISTER_H
