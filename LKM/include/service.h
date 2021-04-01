#ifndef SERVICE_H
#define SERVICE_H
typedef enum service_t{
	SSH,
	HTTP,
	HTTPS,
	MYSQL,
	ORACLE,
	DNS,
	INIT_SERVICE
}service_t;
//service table map
typedef struct service_map{
	char* name;
	service_t service;
}service_map;
static service_map service_table[]={
	{"ssh",SSH},
	{"http",HTTP},
	{"https",HTTPS},
	{"mysql",MYSQL},
	{"oracle",ORACLE},
	{"dns",DNS},
	{"ANY",INIT_SERVICE},
	{"SSH",SSH},
	{"HTTP",HTTP},
	{"HTTPS",HTTPS},
	{"MYSQL",MYSQL},
	{"ORACLE",ORACLE},
	{"DNS",DNS}
};
#endif
