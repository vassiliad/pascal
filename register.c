#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "tree.h"
#include "register.h"

reg_file_t rf_saved, rf_temp,
rf_fsaved, rf_ftemp;
long int post_number = 0;
int stmt_id = 0;
void print_use_def(node_t *start);
void find_use_def(node_t *start , node_list_t *cur_stmt);


void rg_add(reg_file_t* rf, reg_t *reg)
{
	rf->regs = ( reg_t* ) realloc(rf->regs
				, ( rf->size+1 ) * sizeof(reg_t) );
	rf->regs[ rf->size++ ] = *reg;
}

reg_t rg_get_zero()
{
  reg_t ret;
  ret.name = "$0";
  ret.id = 0;
  ret.in_use = 0;

  return ret;
}

reg_t rg_allocate()
{
  reg_t ret;
  static int unique = 1;
  ret.id = unique;
  ret.name = (char*)malloc(sizeof(char)*30);
  sprintf(ret.name, "$%d", ret.id);

  unique++;
  return ret;
}

void rg_init()
{
	// Initialize temporary variables
	// info taken from 
	// http://msdn.microsoft.com/en-us/library/ms253512(v=vs.80).aspx

	reg_t s0 = { .name="$s0", .id=16, .in_use=0 };
	reg_t s1 = { .name="$s1", .id=17, .in_use=0 };
	reg_t s2 = { .name="$s2", .id=18, .in_use=0 };
	reg_t s3 = { .name="$s3", .id=19, .in_use=0 };
	reg_t s4 = { .name="$s4", .id=20, .in_use=0 };
	reg_t s5 = { .name="$s5", .id=21, .in_use=0 };
	reg_t s6 = { .name="$s6", .id=22, .in_use=0 };
	reg_t s7 = { .name="$s7", .id=23, .in_use=0 };
	reg_t s8 = { .name="$s8", .id=30, .in_use=0 };

	reg_t t0 = { .name="$t0", .id= 8, .in_use=0 };
	reg_t t1 = { .name="$t1", .id= 9, .in_use=0 };
	reg_t t2 = { .name="$t2", .id=10, .in_use=0 };
	reg_t t3 = { .name="$t3", .id=11, .in_use=0 };
	reg_t t4 = { .name="$t4", .id=12, .in_use=0 };
	reg_t t5 = { .name="$t5", .id=13, .in_use=0 };
	reg_t t6 = { .name="$t6", .id=14, .in_use=0 };
	reg_t t7 = { .name="$t7", .id=15, .in_use=0 };
	reg_t t8 = { .name="$t8", .id=24, .in_use=0 };
	reg_t t9 = { .name="$t9", .id=25, .in_use=0 };
	reg_t v0 = { .name="$v0", .id= 2, .in_use=0 };
	reg_t v1 = { .name="$v1", .id= 3, .in_use=0 };
	reg_t a0 = { .name="$a0", .id= 4, .in_use=0 };
	reg_t a1 = { .name="$a1", .id= 5, .in_use=0 };
	reg_t a2 = { .name="$a2", .id= 6, .in_use=0 };
	reg_t a3 = { .name="$a3", .id= 7, .in_use=0 };
	
	
	reg_t f0  = { .name="$f0", .id=32, .in_use=0 };
	reg_t f4  = { .name="$f4", .id=36, .in_use=0 };
	reg_t f6  = { .name="$f6", .id=38, .in_use=0 };
	reg_t f8  = { .name="$f8", .id=40, .in_use=0 };
	reg_t f10 = { .name="$f10", .id=42, .in_use=0 };
	reg_t f16 = { .name="$f16", .id=48, .in_use=0 };
	reg_t f18 = { .name="$f18", .id=50, .in_use=0 };

	reg_t f20 = { .name="$f20", .id=52, .in_use=0 };
	reg_t f22 = { .name="$f22", .id=54, .in_use=0 };
	reg_t f24 = { .name="$f24", .id=56, .in_use=0 };
	reg_t f26 = { .name="$f26", .id=58, .in_use=0 };
	reg_t f28 = { .name="$f28", .id=60, .in_use=0 };
	reg_t f30 = { .name="$f30", .id=62, .in_use=0 };

	memset(&rf_saved, 0, sizeof(reg_file_t));
	memset(&rf_temp, 0, sizeof(reg_file_t));

	memset(&rf_fsaved, 0, sizeof(reg_file_t));
	memset(&rf_ftemp, 0, sizeof(reg_file_t));
	
	rg_add(&rf_saved, &s0);
	rg_add(&rf_saved, &s1);
	rg_add(&rf_saved, &s2);
	rg_add(&rf_saved, &s3);
	rg_add(&rf_saved, &s4);
	rg_add(&rf_saved, &s5);
	rg_add(&rf_saved, &s6);
	rg_add(&rf_saved, &s7);
	rg_add(&rf_saved, &s8);

	rg_add(&rf_temp,&t0);
	rg_add(&rf_temp,&t1);
	rg_add(&rf_temp,&t2);
	rg_add(&rf_temp,&t3);
	rg_add(&rf_temp,&t4);
	rg_add(&rf_temp,&t5);
	rg_add(&rf_temp,&t6);
	rg_add(&rf_temp,&t7);
	rg_add(&rf_temp,&t8);
	rg_add(&rf_temp,&t9);
	rg_add(&rf_temp,&v0);
	rg_add(&rf_temp,&v1);
	rg_add(&rf_temp,&a0);
	rg_add(&rf_temp,&a1);
	rg_add(&rf_temp,&a2);
	rg_add(&rf_temp,&a3);

	rg_add(&rf_fsaved, &f0);
	rg_add(&rf_fsaved, &f4);
	rg_add(&rf_fsaved, &f6);
	rg_add(&rf_fsaved, &f8);
	rg_add(&rf_fsaved, &f10);
	rg_add(&rf_fsaved, &f16);
	rg_add(&rf_fsaved, &f18);
	
	rg_add(&rf_ftemp,&f20);
	rg_add(&rf_ftemp,&f22);
	rg_add(&rf_ftemp,&f24);
	rg_add(&rf_ftemp,&f26);
	rg_add(&rf_ftemp,&f28);
	rg_add(&rf_ftemp,&f30);

}

void givepostnumbers_tree(node_list_t *start){
  node_list_t *c;
  printf("giving post in list\n");
  for(c = start; c!=NULL ; c = c->next){
		c->id = stmt_id;
		stmt_id++;
    givepostnumbers(c->node);
	}
  return;
}

void givepostnumbers(node_t *start){
  if(!start)
    return ;
  switch (start->type){
    case NT_Iconst: 
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:{
      //propably constant nodes do not need post number id cause are going to be converted in opi instructions
      start->post= post_number;
      post_number++;
      printf("\t %ld\n",start->post);
      return;
      break;
    }
    case NT_Load:{
      givepostnumbers(start->load.address);
    //  givepostnumbers(start->load.data); load does not have data.
      start->post = post_number;
      printf("\t %ld\n",start->post);
      post_number++;
      break;
    }
    case NT_Store:{
      givepostnumbers(start->load.data); 
      givepostnumbers(start->load.address);
      start->post = post_number;
      printf("\t %ld\n",start->post);
      post_number++;
      break;
    }
    case NT_Add: //assuming that all cases of bin operations have left and tight childs with the same priority
    case NT_Sub:
    case NT_Div:
    case NT_Mult:
    case NT_Mod:{
      givepostnumbers(start->bin.left);
      givepostnumbers(start->bin.right);
      start->post = post_number;
      printf("\t %ld\n",start->post);
      post_number++;
      break;
    }
    case NT_String:// NT_string not implemented yet propably is going to be converted in a load of .data
      break;
    case NT_Not: //Not yes seen a valid implementation
      break;
    case NT_If:{
			start->_if.id = stmt_id;
			stmt_id++;
      givepostnumbers(start->_if.branch);
      givepostnumbers_tree(start->_if._true);
      givepostnumbers_tree(start->_if._false);
      break;
    }
    case NT_Jump:  
      break;
    case NT_BranchZ:{
      givepostnumbers(start->branchz.condition);
      break;
    }
    case NT_LessThan:{
      givepostnumbers(start->bin.left);
      givepostnumbers(start->bin.right);
      break;
    }
    case NT_While:{
			start->_while.id = stmt_id;
			stmt_id++;
      givepostnumbers(start->_while.branch);
      givepostnumbers_tree(start->_while.loop);
      break;
    }
    case NT_For:{
			start->_for.id = stmt_id;
			stmt_id++;
      givepostnumbers(start->_for.init);
      givepostnumbers(start->_for.branch);
      givepostnumbers_tree(start->_for.loop);
      break;
    }
    case NT_Nop:
      break;
    default:
      assert(0 && "Unhandled type in tree");
  }
  return;
}



int init_life(life_t **new){
	*new = (life_t*) malloc (sizeof(life_t));
	if(!*new){
		printf("Not enough memory abandoning program \"liveness.c \" \n");
		exit(0);
	}
	(*new)->vars = (int*) malloc (sizeof(int));
	(*new)->size = 1;
	return 1;
}

int resize (int new_size , life_t *life){
	int *temp= (int*) malloc(new_size * sizeof(int));
	if(!temp){
		printf("Not enough memory abandoning program \"liveness.c \" \n");
		exit(0);
	}
	int i;
	for(i = 0 ; i < life->size; i++)
		temp[i] = life->vars[i];
	free(life->vars);
	life->vars = temp;
	life->size = new_size;
	return 1;
	
}

int add_var(int id , life_t* cur_life){
	int div = id/(sizeof(int)*8);
	int mod = (int) id % (sizeof(int)*8);
	int dummy = 1;
	dummy = dummy<<mod;
	if(div + 1 > (cur_life)->size)
		if (! resize(div+1,cur_life) )
			return 0;
	cur_life->vars[div] = cur_life->vars[div] | dummy;
	return 1;
}

void print_life(life_t life){
	int i;
	for(i = 0 ; i < life.size; i++){
			printf(""BYTETOBINARYPATTERN""BYTETOBINARYPATTERN""BYTETOBINARYPATTERN""BYTETOBINARYPATTERN" ",
			BYTETOBINARY(life.vars[i]>>24),BYTETOBINARY(life.vars[i]>>16) , BYTETOBINARY(life.vars[i]>>8) , BYTETOBINARY(life.vars[i]));
	}
}

void find_use_def_stmt(node_list_t *start){
  node_list_t *c;
  for(c = start; c!=NULL ; c = c->next){
		init_life(&(c->def));
		init_life(&(c->use));
    find_use_def(c->node,c);
	}
  return;
}

void find_use_def(node_t *start , node_list_t *cur_stmt){
  if(!start)
    return ;
  switch (start->type){
    case NT_Iconst: 
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:{
      break;
    }
    case NT_Load:{
      find_use_def(start->load.address , cur_stmt);
			add_var(start->load.unique_id,cur_stmt->use);
      break;
    }
    case NT_Store:{
      find_use_def(start->load.data , cur_stmt); 
      find_use_def(start->load.address , cur_stmt);
      add_var(start->load.unique_id,cur_stmt->def);
      break;
    }
    case NT_Add: //assuming that all cases of bin operations have left and tight childs with the same priority
    case NT_Sub:
    case NT_Div:
    case NT_Mult:
    case NT_Mod:{
      find_use_def(start->bin.left,cur_stmt);
      find_use_def(start->bin.right,cur_stmt);
      break;
    }
    case NT_String:// NT_string not implemented yet propably is going to be converted in a load of .data
      break;
    case NT_Not: //Not yes seen a valid implementation
      break;
    case NT_If:{
      find_use_def(start->_if.branch,cur_stmt);
      find_use_def_stmt(start->_if._true);
      find_use_def_stmt(start->_if._false);
      break;
    }
    case NT_Jump:  
      break;
    case NT_BranchZ:{
      find_use_def(start->branchz.condition,cur_stmt);
      break;
    }
    case NT_LessThan:{
      find_use_def(start->bin.left,cur_stmt);
      find_use_def(start->bin.right,cur_stmt);
      break;
    }
    case NT_While:{
      find_use_def(start->_while.branch,cur_stmt);
      find_use_def_stmt(start->_while.loop);
      break;
    }
    case NT_For:{
      find_use_def(start->_for.init,cur_stmt);
      find_use_def(start->_for.branch,cur_stmt);
      find_use_def_stmt(start->_for.loop);
      break;
    }
    case NT_Nop:
      break;
    default:
      assert(0 && "Unhandled type in tree");
  }
  return;
}

void print_use_def_stmt(node_list_t *start){
  node_list_t *c;
  for(c = start; c!=NULL ; c = c->next){
		printf(" %d : ",c->id);
		print_life(*(c->def));
		printf("\t");
		print_life(*(c->use));
		printf("\n");
    print_use_def(c->node);
	}
  return;
}

void print_use_def(node_t *start){
	  if(!start)
    return ;
  switch (start->type){
    case NT_Iconst: 
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:
    case NT_Load:
    case NT_Store:
    case NT_Add: 
		case NT_Sub:
    case NT_Div:
    case NT_Mult:
    case NT_Mod:
    case NT_String:
    case NT_Not: 
			break;
    case NT_If:{
			printf("TRUE\n");
      print_use_def_stmt(start->_if._true);
      printf("FALSE\n");
			print_use_def_stmt(start->_if._false);
			printf("JOIN\n");
      break;
    }
    case NT_Jump:  
      break;
    case NT_BranchZ:
    case NT_LessThan:
      break;
    case NT_While:{
      printf("WHILE\n");
      print_use_def_stmt(start->_while.loop);
			printf("END WHILE\n");
      break;
    }
    case NT_For:{
			printf("FOR\n");
      print_use_def_stmt(start->_for.loop);
			printf("END FOR\n");
      break;
    }
    case NT_Nop:
      break;
    default:
      assert(0 && "Unhandled type in tree");
  }
  return;
}

int check_life(life_t *life1 , life_t *life2){
	if(life1->size != life2->size)
		return(0);
	int i;
	for(i = 0 ; i < life1->size ; i++)
		if(life1->vars[i] != life2->vars[i])
			return 0;
	return 1;
}

int check_lives(life_t **in_temp ,life_t** out_k){
	int i;
	for(i = 0; i < post_number  ; i++)
		if(!check_life(in_temp[i],out_k[i]))
			return 0;
	return 1;
}

void free_lives(life_t **lifes , int size){
	int i;
	for(  i = 0 ; i < size ; i++){
		free(lifes[i]->vars);
		free(lifes[i]);
	}
}

void life_and_life(life_t **result , life_t* temp1 ,  life_t* temp2){
	int i;
	int until;
	*result = (life_t*) malloc (sizeof(life_t));
	if(temp1->size > temp2->size){
		(*result)->vars = (int*) calloc (temp1->size,sizeof(int));
		(*result)->size = temp1->size;
		until = temp2->size;
	}
	else{
		(*result)->vars = (int*) calloc (temp2->size,sizeof(int));
		(*result)->size = temp2->size;
		until = temp1->size;
	}
	for(i = 0 ; i < until ; i++)
		(*result)->vars[i] = temp1->vars[i] & temp2->vars[i];
		
}

void lives_and_lives(life_t **result , life_t ** temp1 ,  life_t ** temp2 ){
	int i;
	for( i = 0 ; i < post_number ; i++){
		life_and_life(&result[i],temp2[i],temp1[i]);
	}
}

int check_convergence(life_t **in_k , life_t ** out_k ,  life_t **in_temp ,life_t **out_temp ){
	int k;
	life_t **result = (life_t**) malloc (post_number*sizeof(life_t*));
	if(!check_lives(in_temp,out_k))
		return (0);
	lives_and_lives(result,in_k,out_temp);
	k = check_lives(out_k,result);
	free_lives(result , post_number);
	free(result);
	return k;
}

void paste_lives(life_t *dest, life_t *source){
	dest->vars = (int*) calloc (source->size,sizeof(int));
	dest->size = source->size;
	int i;
	for( i = 0; i < source->size ; i++)
		dest->vars[i] = source->vars[i];
}

void pass_nodes_for_liveness(life_t** in_k ,life_t** out_k , life_t** in_temp, life_t** out_temp , node_list_t *cur_stmt ,node_t* start){
	if(!start)
    return ;
  switch (start->type){
    case NT_Iconst:
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:{
			paste_lives(in_temp[start->post],in_k[start->post]);
			paste_lives(out_temp[start->post],out_k[start->post]);
			break;
		}
    case NT_Load:
    case NT_Store:
    case NT_Add: 
		case NT_Sub:
    case NT_Div:
    case NT_Mult:
    case NT_Mod:
    case NT_String:
    case NT_Not: 
			break;
    case NT_If:{
			printf("TRUE\n");
      print_use_def_stmt(start->_if._true);
      printf("FALSE\n");
			print_use_def_stmt(start->_if._false);
			printf("JOIN\n");
      break;
    }
    case NT_Jump:  
      break;
    case NT_BranchZ:
    case NT_LessThan:
      break;
    case NT_While:{
      printf("WHILE\n");
      print_use_def_stmt(start->_while.loop);
			printf("END WHILE\n");
      break;
    }
    case NT_For:{
			printf("FOR\n");
      print_use_def_stmt(start->_for.loop);
			printf("END FOR\n");
      break;
    }
    case NT_Nop:
      break;
    default:
      assert(0 && "Unhandled type in tree");
  }
  return;
}

void pass_tree_for_liveness(life_t** in_k ,life_t** out_k , life_t** in_temp, life_t** out_temp , node_list_t *start ){
	node_list_t *c;
	for(c = start; c != NULL ; c++){
		pass_nodes_for_liveness(in_k ,out_k , in_temp, out_temp , start , c->node );
	}
	return;
}

void find_liveness(node_list_t* start ){
	life_t **in_temp, **out_temp;
	in = (life_t **) malloc (post_number*sizeof(life_t*));
	out = (life_t **) malloc (post_number * sizeof(life_t*));
	in_temp = (life_t **) malloc (post_number*sizeof(life_t*));
	out_temp = (life_t **) malloc (post_number * sizeof(life_t*));
	int i;
	
	if( !(out && in && in_temp && out_temp) ){
		printf("not enough memory for allocating in, out <register.c>\n");
		exit(0);
	}
	
	for(i = 0 ; i < post_number; i++){
		init_life(&out[i]);
		init_life(&in[i]);
		init_life(&out_temp[i]);
		init_life(&in_temp[i]);
	}
	do{
		pass_tree_for_liveness(in,out,in_temp,out_temp,start);
	}while( check_convergence(in,out,in_temp,out_temp) );
	
	
	
}




















