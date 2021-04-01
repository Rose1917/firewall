#include "include/rule.h"
//malloc for a rule struct,init it and return its pointer.
rule_ptr_t get_rule(void){
	//malloc
	rule_ptr_t rule;
	rule=vmalloc(sizeof(rule_t));
	if(rule==NULL){
		printk("get rule error:can not malloc the memeory for a new rule");
		return NULL;
	}
	
	//init
	memset(rule->rule_behaviour.sip,0,sizeof(ip_addr));
	memset(rule->rule_behaviour.dip,0,sizeof(ip_addr));
	rule->rule_behaviour.sport=INIT_PORT;
	rule->rule_behaviour.dport=INIT_PORT;
	rule->rule_behaviour.proto=INIT_PROTO;
	rule->target=INIT_TARGET;
	rule->alarm=default_alarm;
	rule->next=NULL;

	return rule;

}
	
int if_init_ip(ip_addr ip){
	unsigned int *t =(unsigned int *)ip;
	if(*t==0)return 1;
	else return 0;
}

	
