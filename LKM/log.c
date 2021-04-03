#include "include/log.h"
#include <linux/time.h>

extern log_vector_t log_vector; 
ssize_t log_read(struct file *file,char __user *ubuf,size_t count,loff_t *ppos){
	if(*ppos==0){
		raw_copy_to_user(ubuf,&log_vector,sizeof(log_vector_t));
		*ppos+=sizeof(log_vector_t);
		return sizeof(log_vector_t);
	}
	return 0;
}

ssize_t log_write(struct file *file,const char __user *ubuf,size_t count,loff_t *ppos){
	return 0;
}


int firewall_log(log_vector_ptr_t vector,log_level_t type,char* buffer){
	int count = strlen(buffer);

	printk("firewall log function");
	printk("1108");

	//the log is too long,drop it
	if ( count > LOG_BUFFER_SIZE - 1) return -1;
	
	//new a log_element
	log_element_t e;
	
	//init the element
	e.log_level = type;
	set_log_time (e.time_str);
	memcpy(e.content,buffer,count);
	e.content[count]='\0';

	//insert it to the vector
	memcpy(&vector->buffer[vector->index],&e,sizeof(log_element_t));
	printk("the log res the content: %s",vector->buffer[vector->index].content);

	if ( vector -> valid_size != VECTOR_SIZE ) {vector -> valid_size = vector -> valid_size + 1;}
	else vector -> start ++;

	if ( vector -> index == VECTOR_SIZE -1 ) {vector -> index = 0;}
	else vector -> index = vector -> index + 1;

	return 0;
}

//return 0 if succeed
int set_log_time(time_str_t time_str){
	struct timespec64 ts1;
	ktime_get_real_ts64(&ts1);

	struct tm local_time;
	time64_to_tm(ts1.tv_sec,0,&local_time);
	sprintf(time_str," %d-%d-%d %d:%d:%d",local_time.tm_year+1900,local_time.tm_mon+1,local_time.tm_mday,local_time.tm_hour+8,local_time.tm_min,local_time.tm_sec);
	printk("%s",time_str);

	return 0;
}
int init_log_vector(log_vector_ptr_t vector){
	vector->valid_size = 0;
	vector->index = 0;
	vector->start = 0;
	return 0;
}	
