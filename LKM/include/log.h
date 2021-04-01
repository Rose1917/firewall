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
#define LOG_BUFFER_SIZE 100000
#define LOG_TMP_SIZE 1000
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
#define flog(...)  strcat(log_to_read_buffer,(sprintf(log_tmp_buffer,__VA_ARGS__),log_tmp_buffer))
#endif
