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

 if ( start->scheduled != 0 )
    return;
	start->label = NULL;
  switch (start->type){
    case NT_Lui:
    case NT_Iconst: 
    case NT_Bconst:
    case NT_Rconst:
    case NT_Cconst:{
			nodes[node_index] = start;
      start->scheduled = 1;
			start->time = node_index;
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
						start->time = node_index;
						node_index++;	
					}
				}
			}
			else{
				if(able_to_read(start->load.unique_id)){
					nodes[node_index] = start;
					start->scheduled = 1;
					start->time = node_index;
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
					else if(start->store.data->scheduled == 2 && start->store.address->scheduled == 0){
						find_node_to_schedule_tree(start->store.address);
						add_var(start->store.unique_id,pending_writes) ;	
					}
					else if(start->store.data->scheduled == 0 && start->store.address->scheduled == 2){
						find_node_to_schedule_tree(start->store.data);
						add_var(start->store.unique_id,pending_writes);
					}
					else if(start->store.data->scheduled == 2 && start->store.address->scheduled == 2){
						if(able_to_write(start->store.unique_id)){
							nodes[node_index] = start;
							start->scheduled = 1;
							start->time = node_index;
							node_index++;
						}
					}
			}
			else{
				if(start->store.data->scheduled == 0 ){
						find_node_to_schedule_tree(start->store.data);
						add_var(start->store.unique_id,pending_writes) ;
					}
					else if(start->store.data->scheduled == 2 ){
						if(able_to_write(start->store.unique_id)){
							nodes[node_index] = start;
							start->scheduled = 1;
							start->time = node_index;
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
			else if(start->bin.left->scheduled == 0 && start->bin.right->scheduled == 2){
				find_node_to_schedule_tree(start->bin.left);
			}
			else if(start->bin.left->scheduled == 2 && start->bin.right->scheduled == 0){
				find_node_to_schedule_tree(start->bin.right);
			}
			else if(start->bin.left->scheduled == 2 && start->bin.right->scheduled == 2){
				nodes[node_index] = start;
				start->scheduled = 1;
				start->time = node_index;
				node_index++;	
			}
			break;
    }
    case NT_String:// NT_string not implemented yet propably is going to be converted in a load of .data
    case NT_Not: //Not yet seen a valid implementation
    case NT_If:
    case NT_Jump:
			if(node_index == start->post){
				nodes[node_index] = start;
				start->scheduled = 1;
				start->time = node_index;
				node_index++;	
			}
			break;
    case NT_BranchZ:
			if(start->branchz.condition->scheduled == 2){
				nodes[node_index] = start;
				start->scheduled = 1;
				start->time = node_index;
				node_index++;	
			}
			else{
				find_node_to_schedule_tree(start->branchz.condition);
			}
			break;
    case NT_Addi:
    case NT_LessThani:
    case NT_Ori:
    case NT_Subi:
      if ( start->bin_semi.left->scheduled == 0 )
        find_node_to_schedule_tree(start->bin_semi.left);
      else if ( start->bin_semi.left->scheduled == 2 ) {
        nodes[node_index] = start;
        start->scheduled = 1;
        start->time = node_index;
        node_index++;
      }
    break;
    
    

    case NT_While:
    case NT_For:
    case NT_Nop:
      break;
    default:
			printf("start->type : %d\n",start->type);
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

void final_schedule(int scheduled, int new){
	node_t **temp = (node_t**) malloc (sizeof(node_t*)*(new-scheduled  ));
//	if(temp)
//		exit(0);
	int size = new-scheduled;
	int counter = 0;
	int i;
	printf( "%d %d %d\n",scheduled, new , size );
	for( i = scheduled ; i < new; i++)
		if(nodes[i]->type == NT_Load){
			temp[counter] = nodes[i];
			temp[counter]->scheduled = 2;
			counter++;
		}
	
	for( i = scheduled ; i < new; i++)
		if(nodes[i]->type == NT_Mult || nodes[i]->type == NT_Div){
			temp[counter] = nodes[i];
			temp[counter]->scheduled = 2;
			counter++;
		}
	
	for( i = scheduled ; i < new; i++)
		if(nodes[i]->type != NT_Mult && nodes[i]->type != NT_Div && nodes[i]->type != NT_Load){
			temp[counter] = nodes[i];
			temp[counter]->scheduled = 2;
			counter++;
		}
		
		assert(counter-size == 0);
		
		for(i = 0 ; i < size ; i++){
			nodes[scheduled+i] = temp[i];
			nodes[scheduled+i]->time = scheduled+i;
		}
		
//	if(counter != size  ){
//		printf("major problem in scheduling : %d %d \n",size,counter);
//		exit(0);
//	}
	free(temp);
}

void find_node_to_schedule(node_list_t* start,node_list_t* end ){
		node_list_t *c;
		node_t *k;
		int cur_index;
		if(!start)
			return;
		init_life(&pending_writes);
		init_life(&pending_reads);
		do{
			set_life(pending_reads);
			set_life(pending_writes);
			cur_index = node_index;
			for(c = start;  c!=end && c!=NULL ; c= c->next){	
				find_node_to_schedule_tree(c->node);
			}
			final_schedule(cur_index,node_index);
			printf("schedule number of stmt :%d %d %d\n", cur_index,node_index,cur_index - node_index);
		}while( cur_index - node_index !=0 );	
		
		if(end){
			switch (end->node->type){
				case NT_If:{
					printf("scheduling condition\n");
					set_life(pending_reads);
					set_life(pending_writes);
					k  = end->node->_if.branch;
					do{
						set_life(pending_reads);
						set_life(pending_writes);
						cur_index = node_index;
						find_node_to_schedule_tree(k);
						final_schedule(cur_index,node_index);
					}while(k->scheduled != 2);
					printf("DONE scheduling condition\n");
					break;	
				}
				case NT_For:{
					k = end->node->_for.init;
					do{
						set_life(pending_reads);
						set_life(pending_writes);
						cur_index = node_index;
						find_node_to_schedule_tree(k);
						final_schedule(cur_index,node_index);
						printf("stacked here %d\n",k->type);
					}while(k->scheduled != 2);
					k = end->node->_for.branch;
					do{
						set_life(pending_reads);
						set_life(pending_writes);
						cur_index = node_index;
						find_node_to_schedule_tree(k);
						final_schedule(cur_index,node_index);
					}while(k->scheduled != 2);
					break;
				}
				case NT_While:{
					k = end->node->_while.branch;
					do{
						set_life(pending_reads);
						set_life(pending_writes);
						cur_index = node_index;
						find_node_to_schedule_tree(k);
						final_schedule(cur_index,node_index);
					}while(k->scheduled != 2);
					break;
				}
        default:
        break;
			}
			
		}
			printf("@@@@@@\tscheduled : %d \n",node_index - cur_index);
		
			
		
		
		
}





void find_start_end(node_list_t *start ,node_list_t *end ){
	node_list_t *c , *begin;
	char *label;
	node_t *dummy;
//	int i = 0;
	if(!start)
		return;
	begin = start;
	for(c = start ; c!=end && c!=NULL ; c= c->next){
		
		if(c->node){
			switch ( c->node->type){
				case NT_If:{
						if(c->node->_if.branch->type == NT_BranchZ){
							dummy = c->node->_if.branch;
							label = dummy->branchz.label;
						}
						printf("NT IF ENCOUNTERED label %s\n",label);
						find_node_to_schedule(begin,c);						
						printf("NT IF ENCOUNTERED TRUE\n");
						find_start_end(c->node->_if._true,c->next);
						nodes[node_index-1]->label = label;
						printf("NT IF ENCOUNTERED FALSE\n");
						if(nodes[node_index-1]->type == NT_Jump)
							label = nodes[node_index-1]->jump_label;
						if(c->node->_if._false){
							find_start_end(c->node->_if._false,c->next);
							nodes[node_index-1]->label = label;
						}
						begin = c->next;
					break;	
				}
				case NT_For:{
					if(c->node->_for.branch->type == NT_BranchZ){
							dummy = c->node->_for.branch;
							label = dummy->branchz.label;
						}
						printf("NT _FOR ENCOUNTERED label: %s\n",label);
						find_node_to_schedule(begin,c);
						printf("NT WHILE ENCOUNTERED LOOP\n");
						nodes[node_index-1]->label = label;
						find_start_end(c->node->_for.loop,c->next);
						begin = c->next;
					break;
				}
				case NT_While:{
					if(c->node->_while.branch->type == NT_BranchZ){
							dummy = c->node->_while.branch;
							label = dummy->branchz.label;
					}
					printf("NT WHILE ENCOUNTERED label : %s\n",label);
					find_node_to_schedule(begin,c);
					printf("NT WHILE ENCOUNTERED LOOP\n");
					find_start_end(c->node->_while.loop,c->next);
					nodes[node_index-1]->label = label;
					begin = c->next;
					break;
				}
        default:
        break;
			}
		}
	}
	find_node_to_schedule(begin,end);
	return ;
}

/*
void update_fathers(){
	int i;
	for( i = 0 ; i < node_index ; i++ ){
		switch(nodes[i]->type){
			case NT_LessThani:
			case NT_Subi:
			case NT_Ori:
			case NT_Addi: {
				nodes[i]->bin_semi.left->num_parents++;
				if(nodes[i]->bin_semi.left->parent != nodes[i]){
					if(nodes[i]->bin_semi.left->parent->time < i)
						nodes[i]->bin_semi.left->parent = nodes[i];
				}
				break;
			}
			case NT_Iconst: 
			case NT_Bconst:
			case NT_Rconst:
			case NT_Cconst:
			case NT_Lui:{
				break;
			}
			case NT_Load:{
				if(nodes[i]->load.address&&nodes[i]->load.address->parent != nodes[i]){
					nodes[i]->load.address->num_parents++;
					if(nodes[i]->load.address->parent ->time < i)
						nodes[i]->load.address->parent = nodes[i];
				}
				break;
			}
			case NT_Store:{
				if(nodes[i]->load.data->parent != nodes[i]){
					nodes[i]->load.data->num_parents++;
					if(nodes[i]->load.data->parent ->time < i)
						nodes[i]->load.data->parent = nodes[i];	
				}

				if ( nodes[i]->load.address &&  nodes[i]->load.address->parent != nodes[i]){
					nodes[i]->load.address->num_parents++;
					if(nodes[i]->load.address->parent ->time < i)
						nodes[i]->load.address->parent = nodes[i];		
				}

				break;
			}
			case NT_LessThan:
			case NT_Add: 
			case NT_Sub:
			case NT_Div:
			case NT_Mult:
			case NT_Mod:{
				nodes[i]->bin.left->num_parents++;
				nodes[i]->bin.right->num_parents++;
				if(nodes[i]->bin.left->parent != nodes[i])
						if(nodes[i]->bin.left->parent ->time < i)
							nodes[i]->bin.left->parent = nodes[i];		

				if(nodes[i]->bin.right->parent != nodes[i])
					if(nodes[i]->bin.right->parent ->time < i)
						nodes[i]->bin.right->parent = nodes[i];		

				break;
			}
			case NT_String:
			case NT_Not: 
			case NT_If:
			case NT_Jump:
				break;
				
			case NT_BranchZ:{
				nodes[i]->branchz.condition->num_parents++;
				if(nodes[i]->branchz.condition->parent != nodes[i])
					if(nodes[i]->branchz.condition->parent ->time < i)
						nodes[i]->branchz.condition->parent = nodes[i];		
				break;
			}
			case NT_While:
				break;
			case NT_For:
				break;
			case NT_Nop:
				break;
			default:
				assert(0 && "Unhandled type in tree");
			}
	}
}
*/

void schedule(node_list_t *start){
	int i;
	node_index = 0;
	find_start_end(start,NULL);
//	update_fathers();
	
	printf("***************************quick check***************************\n");
	for(i = 0; i < node_index ; i++){
		if(nodes[i])
			printf("\t\t %d %ld %d\n",i,nodes[i]->post,nodes[i]->num_parents);
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
		case NT_Lui:
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
    case NT_LessThani:
    case NT_Subi:
    case NT_Ori:
    case NT_Addi: {
      find_use_def(start->bin_semi.left,cur_stmt);
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
