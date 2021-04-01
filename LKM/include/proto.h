#ifndef PROTO_H
#define PROTO_H


//defination of protocal type
typedef enum proto_t{
	TCP, 
	UDP,
	ICMP,
	INIT_PROTO
}proto_t;


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
#endif

