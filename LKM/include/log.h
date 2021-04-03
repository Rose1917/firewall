#ifndef LOG_H
#define LOG_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/moduleparam.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/fcntl.h>
#define LOG_BUFFER_SIZE 1000
#define VECTOR_SIZE 100

//defiantion of log level
typedef enum log_level_t{
	INFO,
	DEBUG,
	ERROR
}log_level_t;


//defination of the time string
typedef char time_str_t [40] ;


//defiantion of log element
typedef struct log_element_t{
	log_level_t log_level;
	time_str_t time_str;
	char content[LOG_BUFFER_SIZE];
}log_element_t,*log_element_ptr_t;


//defination of buffer_vector size
typedef struct log_vector_t{
	log_element_t buffer[VECTOR_SIZE];
	unsigned int valid_size;
	int index;	//indicate the start index.it is valid only when the valid_size == VECTOR SIZE
	int start;
}log_vector_t,*log_vector_ptr_t;


//declaration of the handle functions
ssize_t log_read(struct file *file,char __user *ubuf,size_t count,loff_t *ppos);
ssize_t log_write(struct file *file,const char __user *ubuf,size_t count,loff_t *ppos);//a fake function

//prepare the myops structure
static struct file_operations myops_1={
		.owner = THIS_MODULE,
		.read = log_read,
		.write = log_write,
};


void clean_log_buffer(void);
int firewall_log(log_vector_ptr_t vector,log_level_t type,char* buffer);
int set_log_time(time_str_t time);
int init_log_vector(log_vector_ptr_t vector);

//defination of intermediate buffer
static char int_buffer[LOG_BUFFER_SIZE];

#define firewall_flog(vector,type,...) firewall_log(vector,type,(sprintf(int_buffer,__VA_ARGS__),int_buffer))
#endif
