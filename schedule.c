#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "register.h"
#include "tree.h"
#include "schedule.h"

/*
 The main purpsose of the scheduling algorithm is to increase the ILP of the output code
 It rearranges the sequnece of the instruction in each block seperately.
 The following steps are done in order to succeed:
 **Find the statementes which belong to a block;
 **Begin the scheduling algorithm for this block;
 **Start from the first statement of the block.
 ** Put the unscheduled stores into the pending_writes
 ** Do same for reads
 ** continue to next statement
 ** Do this untill all nodes of each statement have finished
 ** Find next block and start from second step
*/

void print_use_def(node_t *start);
void find_use_def(node_t *start , node_list_t *cur_stmt);
life_t *pending_writes, *pending_reads;
int real_time = 0;
int node_index = 0;

int init_life(life_t **new){
	*new = (life_t*) malloc (sizeof(life_t));
	if(!*new){
		printf("Not enough memory abandoning program \"liveness.c \" \n");
		exit(0);
	}
	(*new)->vars = (int*) malloc (sizeof(int));
	(*new)->vars[0] = 0;
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
	if(div + 1 > (cur_life)->size){
		if (! resize(div+1,cur_life) )
			return 0;
	}
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


int able_to_read(int id){
	int div = id/(sizeof(int)*8);
	int mod = (int) id % (sizeof(int)*8);
	int result;
	if(div > pending_writes->size )
		return 1;
	else{
		div = pending_writes->vars[div];
		result = (div)&(1<<mod);
		if(result == 1<<mod)
			return 0;
		else
			return 1;
		
	}
}

int able_to_write(int id){
	int div = id/(sizeof(int)*8);
	int mod = (int) id % (sizeof(int)*8);
	int result;
	int dummy;
	if(div > pending_reads->size && div > pending_writes->size )
		return 1;
	else if(div > pending_reads->size ){
		dummy = pending_writes->vars[div];
		result = dummy & (1<<mod);
	}
	else if(div > pending_writes->size ){
		dummy = pending_reads->vars[div];
		result = dummy & (1<<mod);
	}
	else{
		dummy = pending_reads->vars[div];
		result = dummy & (1<<mod);
		dummy = pending_writes->vars[div];
		result = (result | ( dummy & (1<<mod)  ));
	}
	if(result)
			return 0;
		else
			return 1;
}


void find_node_to_schedule_tree(node_t* start){
  if(!start)
    return ;

 if ( start->scheduled == 1 )
    return;

  switch (start->type){
    case NT_Iconst: 
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:{
			nodes[node_index] = start;
      start->scheduled = 1;
			node_index++;
      break;
    }
    case NT_Load:{
      if ( start->load.address ){
				if( start->load.address->scheduled == 0){
					find_node_to_schedule_tree(start->load.address);
					add_var(start->load.unique_id,pending_reads) ; //ADD THIS READ TO PENDING READS NOT YET SCHEDULED
				}
				else if(start->load.address->scheduled == 1){
					if(able_to_read(start->load.unique_id)){
						nodes[node_index] = start;
						start->scheduled = 1;
						node_index++;	
					}
				}
			}
			else{
				if(able_to_read(start->load.unique_id)){
					nodes[node_index] = start;
					start->scheduled = 1;
					node_index++;	
				}
			}
			break;
    }
    case NT_Store:{
			if(start->store.address){
					if(start->store.data->scheduled == 0 && start->store.address->scheduled == 0){
						find_node_to_schedule_tree(start->store.data);
						find_node_to_schedule_tree(start->store.address);
						add_var(start->store.unique_id,pending_writes) ;
					}
					else if(start->store.data->scheduled == 1 && start->store.address->scheduled == 0){
						find_node_to_schedule_tree(start->store.address);
						add_var(start->store.unique_id,pending_writes) ;	
					}
					else if(start->store.data->scheduled == 0 && start->store.address->scheduled == 1){
						find_node_to_schedule_tree(start->store.data);
						add_var(start->store.unique_id,pending_writes);
					}
					else if(start->store.data->scheduled == 1 && start->store.address->scheduled == 1){
						if(able_to_write(start->store.unique_id)){
							nodes[node_index] = start;
							start->scheduled = 1;
							node_index++;
						}
					}
			}
			else{
				if(start->store.data->scheduled == 0 ){
						find_node_to_schedule_tree(start->store.data);
						add_var(start->store.unique_id,pending_writes) ;
					}
					else if(start->store.data->scheduled == 1 ){
						if(able_to_write(start->store.unique_id)){
							nodes[node_index] = start;
							start->scheduled = 1;
							node_index++;
						}
					}
			}
			break;
    }
    
    case NT_Add: //assuming that all cases of bin operations have left and tight childs with the same priority
    case NT_Sub:
    case NT_Div:
    case NT_Mult:
    case NT_Mod:
		case NT_LessThan:{
      if(start->bin.left->scheduled == 0 && start->bin.right->scheduled == 0){
				find_node_to_schedule_tree(start->bin.left);
				find_node_to_schedule_tree(start->bin.right);
			}
			else if(start->bin.left->scheduled == 0 && start->bin.right->scheduled == 1){
				find_node_to_schedule_tree(start->bin.left);
			}
			else if(start->bin.left->scheduled == 1 && start->bin.right->scheduled == 0){
				find_node_to_schedule_tree(start->bin.right);
			}
			else if(start->bin.left->scheduled == 1 && start->bin.right->scheduled == 1){
				nodes[node_index] = start;
				start->scheduled = 1;
				node_index++;	
			}
			break;
    }
    case NT_String:// NT_string not implemented yet propably is going to be converted in a load of .data
    case NT_Not: //Not yet seen a valid implementation
    case NT_If:
    case NT_Jump:  
    case NT_BranchZ:
    case NT_While:
    case NT_For:
    case NT_Nop:
      break;
    default:
      assert(0 && "Unhandled type in tree");
  }
  return;
}


void set_life(life_t* life){
	int i; 
	for( i  = 0 ; i < life->size ; i++){
	//	printf("xxxxxxx : %d\n",i);
		life->vars[i] = 0;
	}
}


void find_node_to_schedule(node_list_t* start,node_list_t* end){
		node_list_t *c;
		int cur_index;
		int i = 0;
		init_life(&pending_writes);
		init_life(&pending_reads);
		do{
			set_life(pending_reads);
			set_life(pending_writes);
			cur_index = node_index;
			for(c = start;  c!=end->next && c!=NULL ; c= c->next){	
//				printf("schedule number of stmt : %d\n",i++);
				find_node_to_schedule_tree(c->node);
			}
//			printf("@@@@@@\tscheduled : %d \n",node_index - cur_index);
		
			
		
		}while(cur_index - node_index != 0 );	
		
}





node_list_t* find_start_end(node_list_t *start ){
	node_list_t *c, *k;
//	int i = 0;
	for(c = start ; c!=NULL ; c= c->next){
	//	printf("schedule number of stmt : %d\n",i++);
		switch (c->node->type){
			case NT_If:
			case NT_For:
			case NT_While:{
				return c;
			}
		}
		k = c;
	}
	return k;
}

void schedule(node_list_t *start){
	node_list_t *end = NULL;
	int i;
	node_index = 0;
	end = find_start_end(start);
	find_node_to_schedule(start,end);
	printf("***************************quick check***************************\n");
	for(i = 0; i < node_index ; i++){
		if(nodes[i])
			printf("\t\t %d %d %d\n",i,nodes[i]->post);
		else
			printf("\t\t\%d (NUL) fix \n",i);
	}
	printf("*************************** end of checking ***************************\n");
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
      find_use_def(start->store.data , cur_stmt); 
      find_use_def(start->store.address , cur_stmt);
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
