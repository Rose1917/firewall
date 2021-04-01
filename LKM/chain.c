#include "include/chain.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>

//defination of the chains of input/output/filter
chain_ptr_t income_chain;
chain_ptr_t outcome_chain;
chain_ptr_t forward_chain;

void init_chains(void){
	//malloc for the chain itself
	income_chain = (chain_ptr_t)vmalloc(sizeof(chain_t));
	outcome_chain = (chain_ptr_t)vmalloc(sizeof(chain_t));
	forward_chain = (chain_ptr_t)vmalloc(sizeof(chain_t));	

	//check the malloc
	if(!income_chain||!outcome_chain||!forward_chain){
		printk(KERN_DEBUG "INIT CHAIN ERROR:MALLOC CHAIN MEMORY FAILED");
		return ;
	}

	//malloc for the head rule
	income_chain->head = (rule_t*)vmalloc(sizeof(rule_t));
	outcome_chain->head = (rule_t*)vmalloc(sizeof(rule_t));
	forward_chain->head =(rule_t*)vmalloc(sizeof(rule_t));

	//check malloc
	if(!income_chain->head||!outcome_chain->head||!forward_chain->head){
		printk(KERN_DEBUG "INIT CHAIN ERROR:MALLOC HEAD RULE MEMORY FAILED");
		return ;
	}
	else printk(KERN_DEBUG "chains inited successfully\n");
	
	//set the description 
	strcpy(income_chain->desp,"income chain:all the rules in this chain will be applied to all the income packets sequentially");
	strcpy(outcome_chain->desp,"outcome chain:all the rules in this chain will be applied to all the outcome packets sequentially");
	strcpy(forward_chain->desp,"forward chain:all the rules in this chain will be applied to all the forward packets sequentially");
	
	//set the family
	income_chain->family=INCOME_FILTER;
	outcome_chain->family=OUTCOME_FILTER;
	forward_chain->family=FORWARD_FILTER;

	//set the rule number
	income_chain->rule_num=0;
	outcome_chain->rule_num=0;
	forward_chain->rule_num=0;

	//set the tail rule 
	income_chain->tail=income_chain->head;
	outcome_chain->tail=outcome_chain->head;
	forward_chain->tail=forward_chain->head;

	//set the default policy
	income_chain->default_policy=ACCEPT;
	outcome_chain->default_policy=ACCEPT;
	forward_chain->default_policy=ACCEPT;
}

//insert a rule to a specified chain
int append_rule(chain_family family,rule_t* rule){
	//get the chain pointer
	chain_ptr_t p = get_chain_by_family(family);
	if(!p){
		printk(KERN_DEBUG "APPEND RULE ERROR:CAN NOT GET THE CHAIN BY FAMILY");
		return -1;
	}
	
	//insert the rule to the tail of the chain
	p->tail->next=rule;
	p->tail=p->tail->next;

	p->rule_num++;
	return 0;
}

int remove_rule_by_index(chain_family family,index_t index){
	//get the chain pointer
	chain_ptr_t p = get_chain_by_family(family);
	if(!p){
		printk(KERN_DEBUG "REMOVE RULE ERROR:CAN NOT GET THE CHAIN BY FAMILY");
		//to-do:give this message to terminal
		return -1;
	}
	
	//init two pointer
	rule_t* pri=p->head;
	rule_t* cur=p->head->next;

	int i=p->rule_num;
	int index_p = index;

	//the index is out of range
	if(index_p>i-1||index_p<0){
		printk(KERN_DEBUG "REMOVE RULE ERROR:THE INDEX DOES NOT EXISTS");
		//to-do:give this message to the terminal
		return -1;
	}
	
	//move the pointer the node
	while(index_p--){
		pri=cur;
		cur=cur->next;
	}
	
	pri->next=cur->next;
	
	//set the tail
	if(cur==p->tail)p->tail=pri;
	vfree(cur);
	
	//set the rule_num
	p->rule_num--;
	return 0;
}
int insert_rule_by_index(chain_family family,index_t index,rule_ptr_t rule){
	
	//get the chain pointer
	chain_ptr_t p = get_chain_by_family(family);
	if(!p){
		printk(KERN_DEBUG "INSERT RULE ERROR:CAN NOT GET THE CHAIN BY FAMILY");
		//to-do:give this message to terminal
		return -1;
	}
	
	//init two pointer
	rule_t* pri=p->head;
	rule_t* cur=p->head->next;

	int i=p->rule_num;

	//the index is out of range
	if(index>i){
		printk(KERN_DEBUG "INSERT RULE ERROR:THE INDEX DOES NOT EXIST");
		//to-do:give this message to the terminal
		return -1;
	}
	
	//to append the rule to the end of the chain
	if(index==i){
		return append_rule(family,rule);
	}

	//move the pointer the node
	while(index--){
		pri=cur;
		cur=cur->next;
	}
	
	pri->next=rule;
	rule->next=cur;
	
	if(cur==NULL)p->tail=cur;
	p->rule_num++;
	return 0;
}


int modify_rule_by_index(chain_family family,index_t index,rule_t* rule){
	int res_remove=0;
	int res_insert=0;
	
	res_remove=remove_rule_by_index(family,index);
	res_insert=insert_rule_by_index(family,index,rule);

	return res_remove||res_insert;

}
chain_ptr_t get_chain_by_family(chain_family family){
	switch(family){
		case INCOME_FILTER:
			return income_chain;
		case OUTCOME_FILTER:
			return outcome_chain;
		case FORWARD_FILTER:
			return forward_chain;
		case INIT_FILTER:
			printk(KERN_DEBUG "GET CHAIN BY FAMILY ERROR:THE FILTER UNSET!");
			return NULL;
		default:
			printk(KERN_DEBUG "GET CHAIN BY FAMILY ERROR:THE FILTER UNRECOGNIZED");
			return NULL;
	}
}
int reset_chain(chain_family family){

	//get the chain pointer
	chain_ptr_t p = get_chain_by_family(family);
	if(!p){
		printk(KERN_DEBUG "RESET RULE ERROR:CAN NOT GET THE CHAIN BY FAMILY");
		//to-do:give this message to terminal
		return -1;
	}
	
	//init two pointer
	rule_t* pri=p->head;
	rule_t* cur=p->head->next;


	//move the pointer the node
	while(cur){
		pri=cur->next;
		vfree(cur);
		cur=pri;
	}
	
	p->rule_num=0;
	return 0;	
}
void print_chain(chain_family family,char* buffer,unsigned int count){
	
}
chain_ptr_t get_chain_by_id(chain_family_t t){
	if(t==INCOME_FILTER)return income_chain;
        if(t==OUTCOME_FILTER)return outcome_chain;
	if(t==FORWARD_FILTER)return forward_chain;
	return NULL;
}	
