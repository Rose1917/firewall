#ifndef SERVICE_H
#define SERVICE_H
#include "rule.h"
typedef enum service_t{
	SSH,
	HTTP,
	HTTPS,
	MYSQL,
	ORACLE,
	DNS,
	INIT_SERVICE
}service_t;

typedef struct service_info_map{
	char* name;
	proto_t proto;
	port_t port;
}service_info_map;

static service_info_map service_info_table[]={
/*SSH*/	{"ssh",TCP,22},
/*HTTP*/{"http",TCP,80},
/*HTTPS*/{"https",TCP,443},
/*MYSQL*/{"mysql",TCP,3306},
/*ORACLE*/{"oracle",TCP,1521},
/*DNS*/{"dns",UDP,853},
/*INIT_SERVICE*/{"init service",INIT_PROTO,0},
};

#endif
