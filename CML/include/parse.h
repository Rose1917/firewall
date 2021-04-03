#ifndef PARSE_H
#define PARSE_H
#include "common.h"
#include <getopt.h>
#include <string.h>

//option table
static struct option optiontable[]={
	{"sip",required_argument,NULL,SIP},
	{"dip",required_argument,NULL,DIP},
	{"sport",required_argument,NULL,SPORT},
	{"dport",required_argument,NULL,DPORT},
	{"proto",required_argument,NULL,PROTO},
	{"service",required_argument,NULL,SERVICE},
	{"alarm",required_argument,NULL,ALARM},
	{"chain",required_argument,NULL,CHAIN},
	{"target",required_argument,NULL,TARGET},
	{"index",required_argument,NULL,CHAIN_INDEX}
};
/*
typedef struct cmd_family_map{
	char* name;
	cmd_family_t family;
	int cmd_specific_family;
};	
static const cmd_family_map  cmd_family_table[]={
	//for the start cmd family
	{"start",STATUS_SWITCH,START},
	{"stop",STATUS_SWITCH,STOP},
	{"enable",STATUS_SWITCH,ENABLE},
	{"disable",STATUS_SWITCH,DISABLE},
	//for the log cmd family
	
	{"status",STATUS_SWITCH,STATUS_LOG},
	{"log",STATUS_SWITCH,FILTER_LOG},
	{"list",STATUS_SWITCH,LIST_RULE},
	/for the rule cmd family
	{"append",RULE_OP,APPEND},
	{"remove",RULE_OP,REMOVE},
	{"insert",RULE_OP,INSERT},
	{"modify",RULE_OP,MODIFY},
	//for the chain cmd family
	{"policy",CHAIN_OP,SET_POLICY},
	{"reset",CHAIN_OP,CLEAR_CHAIN}
};
*/
#define IP_MAX_STRING_SIZE 20
#define PROTO_TABLE_SIZE (sizeof(proto_table)/sizeof(proto_map))
#define SERVICE_TABLE_SIZE (sizeof(service_table)/sizeof(service_map))
#define ALARM_TABLE_SIZE (sizeof(alarm_table)/sizeof(alarm_map))
#define CHAIN_TABLE_SIZE (sizeof(chain_table)/sizeof(chain_map))
#define TARGET_TABLE_SIZE (sizeof(target_table)/sizeof(target_map))

typedef struct proto_map{
	char* name;
	proto_t proto;
}proto_map;
static proto_map proto_table[]={
	{"tcp",TCP},
	{"udp",UDP},
	{"icmp",ICMP},
	{"ANY",INIT_PROTO},
	{"TCP",TCP},
	{"UDP",UDP},
	{"ICMP",ICMP}
};


//service table map
typedef struct service_map{
	char* name;
	service_t service;
}service_map;
static service_map service_table[]={
	{"ssh",SSH},
	{"SSH",SSH},
	{"http",HTTP},
	{"HTTP",HTTP},
	{"https",HTTPS},
	{"HTTPS",HTTPS},
	{"mysql",MYSQL},
	{"MYSQL",MYSQL},
	{"oracle",ORACLE},
	{"ORACLE",ORACLE},
	{"dns",DNS},
	{"DNS",DNS}
};

//alarm table map
typedef struct alarm_map{
	char* name;
	alarm_switch_t alarm;
}alarm_map;

static alarm_map alarm_table[]={
	{"on",ALARM_ON},
	{"ON",ALARM_ON},
	{"OFF",ALARM_OFF},
	{"off",ALARM_OFF},
};


//chain table map
typedef struct chain_map{
	char* name;
	chain_family_t family;
}chain_map;

static chain_map chain_table[]={
	{"income",INCOME_FILTER},
	{"INCOME",INCOME_FILTER},
	{"outcome",OUTCOME_FILTER},
	{"OUTCOME",OUTCOME_FILTER},
	{"forward",FORWARD_FILTER},
	{"FORWARD",FORWARD_FILTER},
};


//target table map
typedef struct target_map{
	char* name;
	target_t target;
}target_map;

static target_map target_table[]={
	{"DROP",DROP},
	{"drop",DROP},
	{"ACCEPT",ACCEPT},
	{"accept",ACCEPT},
};


//function declaration
cmd_ptr_t parse_cmd(int argc,char* argv[]);
void parse_ip(char*,ip_addr);
void parse_port(char*,port_t*);
void parse_proto(char* s,proto_t* p);
void parse_alarm(char* s,alarm_switch_t* a);
void parse_chain(char* s,chain_family_t* t);
void parse_target(char* s,target_t *t);
void parse_index(char* s,index_t * t);
int check_option(cmd_ptr_t cmd_ptr);
#endif
