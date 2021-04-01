#include "include/log.h"

//buffer
char log_to_read_buffer[LOG_BUFFER_SIZE];
char log_tmp_buffer[LOG_TMP_SIZE];

ssize_t log_read(struct file *file,char __user *ubuf,size_t count,loff_t *ppos){
	if(*ppos==0){
		raw_copy_to_user(ubuf,log_to_read_buffer,strlen(log_to_read_buffer)+1);
		*ppos+=strlen(log_to_read_buffer)+1;
		return strlen(log_to_read_buffer)+1;
	}
	return 0;
}

ssize_t log_write(struct file *file,const char __user *ubuf,size_t count,loff_t *ppos){
	return 0;
}
