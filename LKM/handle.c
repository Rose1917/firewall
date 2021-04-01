#include "include/handle.h"
#include "include/rule.h"
#include "include/main.h"
#include "include/chain.h"
#include "include/cmd.h"
#include "include/common.h"
#include "include/parse.h"
#include "include/log.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/moduleparam.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/string.h>
#include <asm/uaccess.h>
extern char log_to_read_buffer[LOG_BUFFER_SIZE];
extern switch_t firewall_switch;
extern switch_t firewall_enable;
extern chain_ptr_t income_chain,outcome_chain,forward_chain;
//response buffer
static char to_read_buffer[BUFFER_SIZE];

//log buffer
extern char log_to_read_buffer[LOG_BUFFER_SIZE];
extern char log_tmp_buffer[LOG_TMP_SIZE];

ssize_t handle_read(struct file *file,char __user *ubuf,size_t count,loff_t *ppos){
	if(*ppos==0){
		raw_copy_to_user(ubuf,to_read_buffer,strlen(to_read_buffer)+1);
		*ppos+=strlen(to_read_buffer)+1;
		return strlen(to_read_buffer)+1;
	}
	return 0;
}

//this function receive the user-space command,and handle it to the corresponding functinos

ssize_t handle_write(struct file *file,const char __user *ubuf,size_t count,loff_t *ppos){
	printk(KERN_DEBUG "write handler\n");


	raw_copy_from_user(&cmd,ubuf,count);
	printk(KERN_DEBUG "received the user space command\n");

	print_cmd_info();
	
	//map the command to handler
	memset(to_read_buffer,'\0',sizeof(to_read_buffer));	//clean the read buffer
	memset(log_to_read_buffer,'\0',sizeof(log_to_read_buffer));
	
	if(firewall_switch == OFF&&cmd.id!=START){
		strcat(to_read_buffer,"the firewall is not active");
		*ppos=count;
		return count;
	}

	(name_id_tables[cmd.id].handler)(&cmd);
	*ppos=count;
	
	return count;
}

void print_cmd_info(){
	printk("id name:%s\n",name_id_tables[cmd.id].name);
	
	printk("option count:%d",ARGUMENT_COUNT);
	int i;
	for(i=0;i<ARGUMENT_COUNT;i++){
		printk("option %s is filled:%s\n",id_info_table[i],cmd.argument_fill_table[i]?"yes":"no");
	}

}
HANDLE_FUNCTION(start){
	firewall_switch = ON;
	memset(log_to_read_buffer,0,sizeof(log_to_read_buffer));
	register_hookfunctions();
	strcat(to_read_buffer,"started the firewall succesfully");
	return ;
}
HANDLE_FUNCTION(stop){
	firewall_switch = OFF;
	unregister_hookfunctions();
	strcat(to_read_buffer,"stopped the firewall successfully");
	return ;
}
HANDLE_FUNCTION(enable){
	firewall_enable=ON;
	printk("HANDLER ENABLE...");
	return ;
}
HANDLE_FUNCTION(disable){
	firewall_enable=OFF;
	printk("HANDLER DISABLE...");
	return ;
}
HANDLE_FUNCTION(append){
	rule_ptr_t rule = get_rule();	
	if(rule==NULL){
		printk("APPEN HANDLE FUNCTION ERROR:can not malloc memory for a new rule");
		return ;
	}

	chain_family_t chain_type;
	set_rule_by_cmd(rule,cmd_ptr,&chain_type,NULL);
	append_rule(chain_type,rule);
	
	strcat(to_read_buffer,"append the rule successfully");
	printk("HANDLER APPEND...");
	return ;
}
HANDLE_FUNCTION(remove){
	rule_ptr_t rule = get_rule();	
	if(rule==NULL){
		printk("REMOVE HANDLE FUNCTION ERROR:can not malloc memory for a new rule");
		return ;
	}

	chain_family_t chain_type;
	index_t idx;
	set_rule_by_cmd(rule,cmd_ptr,&chain_type,&idx);
	remove_rule_by_index(chain_type,idx);
	vfree(rule);
	return ;
}
HANDLE_FUNCTION(insert){
	rule_ptr_t rule = get_rule();	
	if(rule==NULL){
		printk("REMOVE HANDLE FUNCTION ERROR:can not malloc memory for a new rule");
		return ;
	}

	chain_family_t chain_type;
	index_t idx;
	set_rule_by_cmd(rule,cmd_ptr,&chain_type,&idx);
	insert_rule_by_index(chain_type,idx,rule);
	printk("HANDLER INSERT...");
	return ;
}
HANDLE_FUNCTION(modify){
	rule_ptr_t rule = get_rule();	
	if(rule==NULL){
		printk("REMOVE HANDLE FUNCTION ERROR:can not malloc memory for a new rule");
		return ;
	}

	chain_family_t chain_type;
	index_t idx;
	set_rule_by_cmd(rule,cmd_ptr,&chain_type,&idx);
	remove_rule_by_index(chain_type,idx);
	insert_rule_by_index(chain_type,idx,rule);

	printk("HANDLER MODIFY...");
	return ;
}
HANDLE_FUNCTION(flush){
	rule_ptr_t rule = get_rule();	
	if(rule==NULL){
		printk("POLICY HANDLE FUNCTION ERROR:can not malloc memory for a new rule");
		return ;
	}

	chain_family_t chain_type;
	set_rule_by_cmd(rule,cmd_ptr,&chain_type,NULL);

	reset_chain(chain_type);
	vfree(rule);
	printk("HANDLER FLUSH...");
	return ;
}
HANDLE_FUNCTION(policy){
	printk("HANDLER POLICY...");
	
	rule_ptr_t rule = get_rule();	
	if(rule==NULL){
		printk("POLICY HANDLE FUNCTION ERROR:can not malloc memory for a new rule");
		return ;
	}

	chain_family_t chain_type;
	set_rule_by_cmd(rule,cmd_ptr,&chain_type,NULL);

	chain_ptr_t tmp_chain = get_chain_by_id(chain_type);
	tmp_chain->default_policy=rule->target;
	vfree(rule);

	return ;
}


HANDLE_FUNCTION(status){
	strcat(to_read_buffer,"the firewall active :");
	strcat(to_read_buffer,firewall_switch?"on":"off");
	return ;
}
HANDLE_FUNCTION(log){
	printk("HANDLER LOG...");
	return ;
}
HANDLE_FUNCTION(list_rules){
	printk("HANDLER LIST RULES...");
	
	int cur_chain;
	for(cur_chain=INCOME_FILTER;cur_chain<INIT_FILTER;cur_chain++){

		//income chain
		chain_ptr_t tmp_chain=get_chain_by_id(cur_chain);
		
		char local_buffer[BUFFER_SIZE];
		sprintf(local_buffer,"chain name:%s\n",chain_table[cur_chain].name);
		strcat(to_read_buffer,local_buffer);
		
		
		sprintf(local_buffer,"chain rule number:%d\n",tmp_chain->rule_num);
		strcat(to_read_buffer,local_buffer);
	
		sprintf(local_buffer,"chain description:%s\n",tmp_chain->desp);
		strcat(to_read_buffer,local_buffer);

		sprintf(local_buffer,"chain default policy: %s\n",target_table[tmp_chain->default_policy]);
		strcat(to_read_buffer,local_buffer);

		//strcat(to_read_buffer,"index\t source-port\t destination-port\t source-ip\t destination-ip\t protocal\t service\t target\t alarm\n");
		rule_ptr_t tmp_rule=tmp_chain->head->next;
		int i;
		for( i=0;i<tmp_chain->rule_num;i++,tmp_rule=tmp_rule->next){
			//the index of the rule
			sprintf(local_buffer,"   %d \t",i);
			strcat(to_read_buffer,local_buffer);
			//source port
			if(tmp_rule->rule_behaviour.sport==INIT_PORT){
				strcat(to_read_buffer,"ANY  \t");
			}
			else{
				sprintf(local_buffer," %d   \t",tmp_rule->rule_behaviour.sport);
				strcat(to_read_buffer,local_buffer);
			}
	
			//destination port
			if(tmp_rule->rule_behaviour.dport==INIT_PORT){
				strcat(to_read_buffer,"ANY  \t");
			}
			else{
				sprintf(local_buffer,"  %d \t",tmp_rule->rule_behaviour.dport);
				strcat(to_read_buffer,local_buffer);
			}
			
			
			//source ip addr
			if(if_init_ip(tmp_rule->rule_behaviour.sip)){
				strcat(to_read_buffer," ANY \t");
			}
			else{
				unsigned char* addr=tmp_rule->rule_behaviour.sip;
				sprintf(local_buffer," %u.%u.%u.%u ",addr[0],addr[1],addr[2],addr[3]);
				strcat(to_read_buffer,local_buffer);
			}
	
	
			//destination ip addr
			if(if_init_ip(tmp_rule->rule_behaviour.dip)){
				strcat(to_read_buffer," ANY \t");
			}
			else{
				unsigned char* addr=tmp_rule->rule_behaviour.dip;
				sprintf(local_buffer," %u.%u.%u.%u ",addr[0],addr[1],addr[2],addr[3]);
				strcat(to_read_buffer,local_buffer);
			}
			//protocal
				strcat(to_read_buffer,proto_table[tmp_rule->rule_behaviour.proto].name);
				strcat(to_read_buffer,"\t");
	
			//service
				strcat(to_read_buffer,service_table[tmp_rule->rule_behaviour.service].name);
				strcat(to_read_buffer,"\t");
			//target
				strcat(to_read_buffer,target_table[tmp_rule->target].name);
				strcat(to_read_buffer,"\t");
			//alarm
				strcat(to_read_buffer,alarm_table[tmp_rule->alarm].name);
				strcat(to_read_buffer,"\n");
		}
	
		strcat(to_read_buffer,"\n\n");		
	}
}



//some utility functions
void set_rule_by_cmd(rule_ptr_t rule_ptr,cmd_ptr_t cmd_ptr,chain_family_t* chain,index_t* index){
	int i;
	for(i=0;i<ARGUMENT_COUNT;i++){
		if(cmd_ptr->argument_fill_table[i]==NOT_FILLED)continue;
		printk("handle.c:set rule by cmd:the i=%d",i);
		switch(i){
			case SIP:
				memcpy(rule_ptr->rule_behaviour.sip,cmd_ptr->table.sip,sizeof(ip_addr));
		       		break;
			case DIP:
				memcpy(rule_ptr->rule_behaviour.dip,cmd_ptr->table.dip,sizeof(ip_addr));
				break;
			case SPORT:
			        rule_ptr->rule_behaviour.sport=cmd_ptr->table.sport;
		       	       	break;
			case DPORT:
			        rule_ptr->rule_behaviour.dport=cmd_ptr->table.dport;
		       	       	break;
			case PROTO:
				rule_ptr->rule_behaviour.proto=cmd_ptr->table.proto;
				break;
			case SERVICE:
				//to-do
				break;
			case ALARM:
				rule_ptr->alarm=cmd_ptr->table.alarm;
				break;
			case CHAIN:
				*chain=cmd_ptr->table.chain_family;
				break;
			case TARGET:
				rule_ptr->target=cmd_ptr->table.target;
				break;
			case CHAIN_INDEX:
			        *index=cmd_ptr->table.chain_index;
			default:
				printk("set rule error:an unexpected option");	
		}
	}
	

}
