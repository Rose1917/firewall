#ifndef CMD_H
#define CMD_H
#include "service.h"
#include "handle.h"
#define COMMAND_COUNT 13
#define ARGUMENT_COUNT 10
//defination of the permission of command to arguments
#define REQUIRE 1
#define OPTIONAL 2
#define NO_ARGU 3
typedef int permission;

//some defiantion for argument fill table
#define FILLED 1
#define NOT_FILLED 0
typedef int if_filled;

//define handle functions
#define HANDLE_FUNCTION_NAME(x) x_handle_function
#define HANDLE_FUNCTION(x) void HANDLE_FUNCTION_NAME(x) (cmd_ptr_t cmd_ptr)


//every command has one id.we decide this id by the first argument,like start or stop
typedef enum command_id{
	//DO NOT change this table excpet that you know what you are doing.if you change this table please change the defination of COMMAND_COUNT
	//general setting
	START,
	STOP,
	ENABLE,
	DISABLE,
	//rule operations
	APPEND_RULE,
	REMOVE_RULE,
	INSERT_RULE,
	MODIFY_RULE,
	//chain operations
	FLUSH_CHAIN,
	SET_CHAIN_POLICY,
	
	//info operations
	LIST_RULE,//list all the rules
	STATUS_CHECK,//if activated
	FILTER_LOG//show the filter log
}command_id;

//all the argument possibilities
typedef enum argument_id{
//DO NOT change this table excpet that you know what you are doing.if you change this table please change the defination of ARGUMENT_COUNT
	SIP,
	DIP,
	SPORT,
	DPORT,
	PROTO,
	SERVICE,
	ALARM,
	CHAIN,
	TARGET,
	CHAIN_INDEX
}argument_id;

//this table shows the relationship between the command and the arguments.
	//eg:the value of a[START][SIP] is  one of the three types of permission:need argument,optional argument,no_argument which means
		//1)NEED_ARGUMENT:START needs argument SIP which is mandatory
		//2)OPTIONAL_ARGUMENT:the sip argument is optional.In most cases, SIP has a default value.
		//3)NO_ARGUMENT:START does not need argument SIP.And in most cases,the argument is not needed.
static permission permission_table[COMMAND_COUNT][ARGUMENT_COUNT]={
		//SIP	 DIP	 SPORT	 DPORT	 PROTO	 SERVICE   ALARM     CHAIN     TARGET	 CHAIN_INDEX
/*START*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  NO_ARGU,   NO_ARGU, NO_ARGU},
/*STOP*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  NO_ARGU,   NO_ARGU, NO_ARGU},
/*ENABLE*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  NO_ARGU,   NO_ARGU, NO_ARGU},
/*DISABLE*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  NO_ARGU,   NO_ARGU, NO_ARGU},
/*APPEND  */    {OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,REQUIRE,REQUIRE,NO_ARGU},
/*REMOVE*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  REQUIRE,   NO_ARGU, REQUIRE},
/*INSERT*/      {OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,REQUIRE,REQUIRE,REQUIRE},
/*MODIFY*/      {OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,OPTIONAL,REQUIRE,REQUIRE,REQUIRE},
/*FLUSH*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  REQUIRE,   NO_ARGU, NO_ARGU},
/*POLICY*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  REQUIRE,   REQUIRE, NO_ARGU},
/*LISTR*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  NO_ARGU,   NO_ARGU, NO_ARGU},
/*LISTS*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  NO_ARGU,   NO_ARGU, NO_ARGU},
/*LISTF*/	{NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,NO_ARGU,  NO_ARGU,  NO_ARGU,   NO_ARGU, NO_ARGU},
};
//defination of arguments table:
// the argument table defines all the possible arguments.you can get whatever you want according to the permission table
typedef struct argument_table{
	ip_addr sip;
	ip_addr dip;
	port_t sport;
	port_t dport;
	proto_t proto;
	service_t service;
	alarm_info_t alarm;
	chain_family_t chain_family;
	target_t target;
	index_t chain_index;//denote the index of a rule in a chain
}argument_table;

//defination of cmd
typedef struct cmd_t{
	command_id id;
	argument_table table;
	if_filled argument_fill_table[ARGUMENT_COUNT];
}cmd_t,*cmd_ptr_t;


//this is the map from id to the info of the argument
typedef  struct opt_id_info{
       char* name;
       /*
       void* ptr;
       int off_set;
       var_t type;
       */
}id_info_map;

static id_info_map id_info_table[]={
	{"sip"},
	{"dip"},
	{"sport"},
	{"dport"},
	{"protocal"},
	{"service"},
	{"alarm"},
	{"chain"},
	{"target"},
	{"index"}
};


static cmd_t cmd;
#endif
