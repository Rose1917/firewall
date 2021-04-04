#include "include/common.h"
cmd_ptr_t parse_cmd(int argc,char* argv[]){
	
	//printf("PARSE CMD FUNCTION.\n");
	
	//malloc for the cmd structure
	cmd_ptr_t cmd_ptr;
	cmd_ptr=(cmd_ptr_t)malloc(sizeof(cmd_t));
	if(!cmd_ptr){
		log_error_f("PARSE CMD ERROR:MALLOC FOR CMD TERMINATED");
		return NULL;
	}
	
	//detect the no argument error
	if(argc<2){
		log_error_f("firewall:no argument provided.\n\
			use firewall help for more information.");
		return NULL;
	}

	//set the command id
	char* first_arg = argv[1];
	int i;
	for(i = 0;i<COMMAND_COUNT;i++){
		if(!strcmp(first_arg,name_id_tables[i].name)){
			//set the command id
			cmd_ptr->id=name_id_tables[i].id;
			break;
		}
	}

	//handle the unrecognized command error
	if(i==COMMAND_COUNT){
		//did not match any command
		log_error_f("unrecognized command %s\n\
			use --help for more information.",first_arg);
		return NULL;
	}
	
	//set the init value to option
	memset(cmd_ptr->argument_fill_table,NOT_FILLED,sizeof(cmd_ptr->argument_fill_table));
	
	//parse the options and arguments
	int opt_index=1;
	int retval=-1;

	while((retval=getopt_long(argc,argv,"",optiontable,&opt_index))!=-1){
		switch (retval){
			case SIP:
				parse_ip(optarg,cmd_ptr->table.sip);	
				break;
			case DIP:
				parse_ip(optarg,cmd_ptr->table.dip);	
				break;
			case SPORT:
				//
				parse_port(optarg,&(cmd_ptr->table.sport));
				break;
			case DPORT:
				parse_port(optarg,&(cmd_ptr->table.dport));
				break;
			case PROTO:
				parse_proto(optarg,&(cmd_ptr->table.proto));
				break;
			case SERVICE:
				parse_service(optarg,&(cmd_ptr->table.service));
				break;
			case ALARM:
				parse_alarm(optarg,&(cmd_ptr->table.alarm));
				break;
			case CHAIN:
				parse_chain(optarg,&(cmd_ptr->table.chain_family));
				break;
			case TARGET:
				parse_target(optarg,&(cmd_ptr->table.target));
				break;
			case CHAIN_INDEX:
				parse_index(optarg,&(cmd_ptr->table.chain_index));
				break;
			case '?':
				//getopt_long will report the error
				log_error_f_tab(LEVEL_1,"--unrecoginized option %s ",optarg);
				break;

			default:
				log_error_f_tab(LEVEL_1,"an internal error occurred while parsing the option %s",optarg);
		}
		//set the argument fill table
		cmd_ptr->argument_fill_table[retval]=FILLED;
	
	}

	//check if the arguments are valid,it will be regarded as a invalid command in the following cases
	//	1):the command does not need the provided arguments
	//	2):the command does provide the arguments the it should provided
	
       	if(check_option(cmd_ptr))return cmd_ptr;
	else {
		free(cmd_ptr);
		return NULL;
	}	
}
int check_option(cmd_ptr_t cmd_ptr){
	int retval=1;

	//get the command permisson table
	permission* tmp_permisson_table = permission_table[cmd_ptr->id]; 	
	for(int i = 0;i < ARGUMENT_COUNT;i++){
		switch(cmd_ptr->argument_fill_table[i]){
			case NOT_FILLED:
				if(tmp_permisson_table[i]==REQUIRE){
					log_error_f("option %s is needed\n\t\tfor more help,please use firewall --help",optiontable[i].name);
					retval=0;
				}
				break;
			case FILLED:
				if(tmp_permisson_table[i]==NO_ARGU){
					log_error_f("option %s is not needed\n\t\tfor more help,please use firewall --help",optiontable[i].name);
					retval=0;
				}
				break;
			default:
				log_error_f("check option error:the is filled table invalid value with %s",optiontable[i].name);

		}

	}

	//parse the service
	
	if(cmd_ptr->argument_fill_table[SERVICE]==FILLED){
		log_info("service set");
		int dport_redundant=0,proto_redundant=0;
		if(cmd_ptr->argument_fill_table[DPORT]==FILLED){
			dport_redundant = 1;
			//but it is ok 
		}
		cmd_ptr->table.dport = service_info_table[cmd_ptr->table.service].port;
		log_info_f("service port number：%d",cmd_ptr->table.dport);
		cmd_ptr->argument_fill_table[DPORT] = FILLED;

		if(cmd_ptr->argument_fill_table[PROTO]==FILLED){
			proto_redundant = 1;
			//but it is ok 
		}

		//print the redundant info

		if(dport_redundant||proto_redundant)
		log_error_f("since you have specified the service %s,%s  %s will be ignored",service_info_table[cmd_ptr->table.service].name,dport_redundant ?"dport":"",proto_redundant ? "protocol":"");

		cmd_ptr->table.proto = service_info_table[cmd_ptr->table.service].proto;
		cmd_ptr->argument_fill_table[PROTO] = FILLED;
		log_info_f("service protocol number：%d",cmd_ptr->table.proto);
	}	

	#ifdef DEBUG
	if(retval)log_debug_f_tab(LEVEL_0,"check option successs");
	#endif
	return retval;
}
void parse_ip(char* s,ip_addr ip){
	//check the source string
	//the ip rule:
	//	1):IP->A.B.C.D
	//	2):A->0-255
	//	3):B->0-255
	//	4):C->0-255
	//	5):D->0-255
	char* token=NULL;
 	char  s_cp[IP_MAX_STRING_SIZE];

	strcpy(s_cp,s);//copy the data from s to s_cp since strtok will destroy the raw string	
	token=strtok(s_cp,".");
	

	int token_count=0;
	unsigned int ip_seg=0;
	while(token!=NULL){
		sscanf(token,"%u",&ip_seg);
		if(ip_seg<0||ip_seg>255){
			log_error_f("PARSE IP ERROR:INVALID IP RANGE '%d'",ip_seg);
			exit(-1);
		}

		//check if the token_count is valid
		if(token_count>3){
			log_error_f("PARSE IP ERROR:INVALID IP FORMAT %s",s);
			exit(-1);
		}

		//set the ip field
		ip[token_count]=ip_seg;
		token=strtok(NULL,".");

		token_count++;
	}

	//check
}
void parse_port(char* s,port_t* p){
	long long tmp_port;
	if(sscanf(s,"%ld",&tmp_port)==EOF){
		log_error_f("PARSE PORT ERROR:INVALID PORT STRING");
		exit(-1);
	}

	//check the port is valid
	if(tmp_port<0){
		log_error_f("PARSE PORT ERROR:PORT CAN NOT BE NEGATIVE");
		exit(-1);
	}

	//set the port value
	*p=tmp_port;
	
}
void parse_proto(char* s,proto_t* p){
	int i;
	for(i=0;i<PROTO_TABLE_SIZE;i++){
		if(!strcmp(s,proto_table[i].name)){
			*p=proto_table[i].proto;
			break;	
		}
	}

	//handle the protocal does not exist error
	if(i==PROTO_TABLE_SIZE){
		log_error_f_tab(LEVEL_2,"PARSE PROTOCAL ERROR:%s",s);
		exit(-1) ;
	}

	//log the result
}

void parse_service(char* s,service_t* p){
	int i;
	for(i=0;i<SERVICE_TABLE_SIZE;i++){
		if(!strcmp(s,service_table[i].name)){
			*p=service_table[i].service;
			break;	
		}
	}

	//handle the protocal does not exist error
	if(i==SERVICE_TABLE_SIZE){
		log_error_f_tab(LEVEL_2,"PARSE SERVICE ERROR:%s",s);
		exit(-1) ;
	}
	
	//log the result
}

void parse_alarm(char* s,alarm_switch_t* alarm){
	int i;
	for(i=0;i<ALARM_TABLE_SIZE;i++){
		if(!strcmp(s,alarm_table[i].name)){
			*alarm=alarm_table[i].alarm;
			break;	
		}
	}

	//handle the protocal does not exist error
	if(i==ALARM_TABLE_SIZE){
		log_error_f_tab(LEVEL_2,"PARSE ALARM ERROR:%s",s);
		exit(-1) ;
	}
	
	//log the result
}
	
void parse_chain(char* s,chain_family_t* family){
	int i;
	for(i=0;i<CHAIN_TABLE_SIZE;i++){
		if(!strcmp(s,chain_table[i].name)){
			*family=chain_table[i].family;
			break;	
		}
	}

	//handle the protocal does not exist error
	if(i==CHAIN_TABLE_SIZE){
		log_error_f_tab(LEVEL_2,"PARSE CHAIN ERROR:%s",s);
		exit(-1) ;
	}
	
	//log the result
}

void parse_target(char* s,target_t* target){
	int i;
	for(i=0;i<TARGET_TABLE_SIZE;i++){
		if(!strcmp(s,target_table[i].name)){
			*target=target_table[i].target;
			break;	
		}
	}

	//handle the protocal does not exist error
	if(i==TARGET_TABLE_SIZE){
		log_error_f_tab(LEVEL_2,"PARSE TARGET ERROR:%s",s);
		exit(-1) ;
	}
	
	//log the result
}
void parse_index(char* s,index_t* index){
	long long tmp_index;
	if(sscanf(s,"%ld",&tmp_index)==EOF){
		log_error_f("PARSE INDEX ERROR:INVALID INDEX STRING");
		exit(-1);
	}

	//check the port is valid
	if(tmp_index<0){
		log_error_f("PARSE INDEX ERROR:NEGATIVE INDEX IS NOT VALID");
		exit(-1);
	}

	//set the port value
	*index=tmp_index;
	
}
