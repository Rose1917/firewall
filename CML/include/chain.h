#ifndef CHAIN_H
#define CHAIN_H
#include "rule.h"
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/netfilter.h>

//defination of some consts
#define DESCRIPTION_BUFFER_SIZE 200

//defination of ip address and port
typedef unsigned char ip_addr [IP_LENGTH];
typedef unsigned int port_t;
typedef unsigned int index_t;

//enum defination of three kinds of chains
typedef enum chain_family_t{
	INCOME_FILTER,
	OUTCOME_FILTER,
	FORWARD_FILTER,
	INIT_FILTER //should not reach here
}chain_family_t;

//defination of chain itself,head DO NOT store data.
typedef struct chain_t{
	char desp[DESCRIPTION_BUFFER_SIZE];
	chain_family_t chain_family;
	unsigned int rule_num;
	rule_ptr_t head;//head it self do not store infomation
	rule_ptr_t tail;//tail stores data compared to head
	policy_t default_policy;
}chain_t,*chain_ptr_t;

//function declaration
void init_chains(void);
int append_rule(chain_family_t family,rule_t* rule);
int remove_rule_by_index(chain_family_t family,index_t index);
int insert_rule_by_index(chain_family_t family,index_t index,rule_ptr_t rule);
int modify_rule_by_index(chain_family_t family,index_t index,rule_ptr_t rule);
chain_ptr_t get_chain_by_family(chain_family_t family);
void print_chain(chain_family_t family,char* buffer,unsigned int count);
int reset_chain(chain_family_t family);
#endif
