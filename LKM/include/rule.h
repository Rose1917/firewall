#ifndef RULE_H
#define RULE_H
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include "service.h"
#include "proto.h"
#include <linux/netfilter.h>

//defination of some consts
#define IP_LENGTH 4 
#define INIT_PORT 0
#define INIT_IP_ADDR {0,0,0,0}
#define INIT_INDEX 9999999

#define DESCRIPTION_BUFFER_SIZE 200

//defination of ip address and port
typedef unsigned  char ip_addr [IP_LENGTH];
typedef unsigned int port_t;
typedef unsigned int index_t;

//defination of target
typedef enum target_t{
	DROP=0,
	ACCEPT=1,
	INIT_TARGET
}target_t,policy_t;

//defination of bebaviour which is a component of a rule
typedef struct behaviour{
	ip_addr sip;
	port_t sport;
	ip_addr dip;
	port_t dport;
	proto_t proto;
	service_t service;
}behaviour;

//defination of alarm_info_t which determine if a rule is matched,it will alarm or not
typedef enum alarm_switch_t{
	ALARM_OFF,
	ALARM_ON,
}alarm_switch_t,alarm_info_t;
static alarm_switch_t default_alarm=ALARM_OFF;

//defination of rule which should be defined by user aside from the index and next. these two variables are used in chains
typedef struct rule_t{
	behaviour rule_behaviour;
	target_t target;
	alarm_info_t alarm;

	struct rule_t*  next;//which linked to the next rule in a chain.
}rule_t,*rule_ptr_t;

//function declaration
rule_ptr_t get_rule(void);
int if_init_ip(ip_addr ip);
#endif
