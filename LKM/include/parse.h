#ifndef PARSE_H
#define PARSE_H
#include "common.h"
//alarm table map
typedef struct alarm_map{
	char* name;
	alarm_switch_t alarm;
}alarm_map;

static alarm_map alarm_table[]={
	{"off",ALARM_OFF},
	{"on",ALARM_ON},
	{"ON",ALARM_ON},
	{"OFF",ALARM_OFF},
};


//chain table map
typedef struct chain_map{
	char* name;
	chain_family_t family;
}chain_map;

static chain_map chain_table[]={
	{"income",INCOME_FILTER},
	{"outcome",OUTCOME_FILTER},
	{"forward",FORWARD_FILTER},
	{"INCOME",INCOME_FILTER},
	{"OUTCOME",OUTCOME_FILTER},
	{"FORWARD",FORWARD_FILTER},
};


//target table map
typedef struct target_map{
	char* name;
	target_t target;
}target_map;

static target_map target_table[]={
	{"drop",DROP},
	{"accept",ACCEPT},
	{"DROP",DROP},
	{"ACCEPT",ACCEPT},
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
