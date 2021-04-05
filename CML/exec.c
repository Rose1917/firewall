#include "include/exec.h"
#include "include/debug.h"
#include "include/help.h"
#include "include/log.h"
#include <stdlib.h>
int exec(cmd_ptr_t cmd_ptr){
	//open the proc file
	int fd = open("/proc/firewall",O_RDWR);
	if(fd == -1){
		log_error("exec error:an error occurred while opening /proc/firewall,please check if the firewall module is loaded");
		return -1;
	}

	
	//copy the cmd struct to the file
	write(fd,cmd_ptr,sizeof(cmd_t));
	
	
	//handle the enable and disable command
	if(cmd_ptr->id==ENABLE){
		enable_firewall();
		return ;
	}
	else if(cmd_ptr->id==DISABLE){
		disable_firewall();
		return ;
	}
	else if(cmd_ptr->id==FILTER_LOG){
		//check the conf file if exists
		int fd = open("/proc/firewall_log",O_RDONLY);
		log_vector_t log_vector;
		if(fd == -1){
			log_error("can not access the /proc/firewall_log");
			return -1;
		}
		if(read(fd,&log_vector,sizeof(log_vector_t))){
			int i;
			for(i = log_vector.start;i <log_vector.start + log_vector.valid_size;i++){
				int j = i%VECTOR_SIZE;
				switch(log_vector.buffer[j].log_level){
					case INFO:
						log_info_f("#%d\t INFO \t %s \t %s",i-log_vector.start,log_vector.buffer[j].time_str,log_vector.buffer[j].content);
						break;
					case DEBU:
						log_debug_f("#%d\t INFO \t %s \t %s",i-log_vector.start,log_vector.buffer[j].time_str,log_vector.buffer[j].content);
						break;
					case ERROR:
						log_error_f("#%d\t INFO \t %s \t %s",i-log_vector.start,log_vector.buffer[j].time_str,log_vector.buffer[j].content);
						break;
					default:
						log_error("an error while processing the firewall log");
				}
			}
				
		}
		return 0;
	}
	else if(cmd_ptr->id==STATUS_CHECK){
		//check the conf file if exists
		int fd = open("/etc/modules-load.d/firewall.conf",O_RDONLY|O_CREAT);
		char tmp_buf[BUFFER_SIZE];
		if(fd == -1){
			log_error("can not access the /etc/modules-load.d/firewall.conf");
			return -1;
		}
		if(read(fd,tmp_buf,BUFFER_SIZE)){
			log_info("firewall enable status:enabled");
		}
		else log_info("firewall enable status:disabled");
	}
	else if(cmd_ptr->id == HELP){
		print_manual();
		return 0;
	}

	//get the buffer response
	char buf[BUFFER_SIZE];
	lseek(fd,0,SEEK_SET);
	read(fd,buf,BUFFER_SIZE);

	log_info(buf);
}
//move the module to the startup list
//to do this,just create a file in /etc/modules-load.d/firewall.conf
void enable_firewall(){
	int fd = open("/etc/modules-load.d/firewall.conf",O_RDWR|O_CREAT,S_IRWXU);
	if(fd == -1){
		log_error("enable:an error occurred while creating the firewall.conf file");
		exit (-1);
	}
	
	char enable_str[]="firewall";
	int write_res;
	write_res=write(fd,enable_str,strlen(enable_str));
	
	if(write_res!=strlen(enable_str)){
		log_error("can not write to the firewall.conf");
		exit(-1);
	}
	log_info("enable firewall success");
}


//remove the module to the startup list;
void disable_firewall(){
	int fd = open("/etc/modules-load.d/firewall.conf",O_RDWR|O_TRUNC,S_IRWXU);
	if(fd == -1){
		log_error("disable:an error occurred while opening the firewall.conf file");
		exit (-1);
	}
	close(fd);
	log_info_f("disable the firewall success");
}

//print log info

