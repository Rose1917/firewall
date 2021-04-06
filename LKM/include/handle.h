#ifndef HANDLE_H
#define HANDLE_H
#include "rule.h"
#include "main.h"
#include "chain.h"
#include "cmd.h"
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

//define handle functions
#define HANDLE_FUNCTION_NAME(x) x##_handle_function
#define HANDLE_FUNCTION(x) void HANDLE_FUNCTION_NAME(x) (cmd_ptr_t cmd_ptr)

HANDLE_FUNCTION(start);
HANDLE_FUNCTION(stop);
HANDLE_FUNCTION(enable);
HANDLE_FUNCTION(disable);
HANDLE_FUNCTION(append);
HANDLE_FUNCTION(remove);
HANDLE_FUNCTION(insert);
HANDLE_FUNCTION(modify);
HANDLE_FUNCTION(flush);
HANDLE_FUNCTION(policy);
HANDLE_FUNCTION(list_rules);
HANDLE_FUNCTION(status);
HANDLE_FUNCTION(log);
HANDLE_FUNCTION(help);


typedef struct name_id_map{
	char* name;
	command_id id;
	void (*handler) (cmd_ptr_t cmd_ptr);
}name_id_map;


//this table shows the name string and id
//well,it is apparent that we can add more information to this tale,for example the command description or things like that.
//to change that table,DO NOT FORGET TO change the name_id_map structure.
static name_id_map name_id_tables[]={
	{"start",START,HANDLE_FUNCTION_NAME(start)},
	{"stop",STOP,HANDLE_FUNCTION_NAME(stop)},
	{"enable",ENABLE,HANDLE_FUNCTION_NAME(enable)},
	{"disable",DISABLE,HANDLE_FUNCTION_NAME(disable)},
	{"append",APPEND_RULE,HANDLE_FUNCTION_NAME(append)},
	{"remove",REMOVE_RULE,HANDLE_FUNCTION_NAME(remove)},
	{"insert",INSERT_RULE,HANDLE_FUNCTION_NAME(insert)},
	{"modify",MODIFY_RULE,HANDLE_FUNCTION_NAME(modify)},
	{"flush",FLUSH_CHAIN,HANDLE_FUNCTION_NAME(flush)},
	{"policy",SET_CHAIN_POLICY,HANDLE_FUNCTION_NAME(policy)},
	{"list-rules",LIST_RULE,HANDLE_FUNCTION_NAME(list_rules)},
	{"status",STATUS_CHECK,HANDLE_FUNCTION_NAME(status)},
	{"log",FILTER_LOG,HANDLE_FUNCTION_NAME(log)},
	{"help",HELP,HANDLE_FUNCTION_NAME(help)}
};

//declaration of the handle functions
ssize_t handle_read(struct file *file,char __user *ubuf,size_t count,loff_t *ppos);
ssize_t handle_write(struct file *file,const char __user *ubuf,size_t count,loff_t *ppos);
//prepare the myops structure
/*file operation is deprecated since Kernel 5.6 or so. But we still leave it here for compatibility*/
#if LINUX_VERSION_CODE <= KERNEL_VERSION(5,6,0)
static struct file_operations myops={
		.owner = THIS_MODULE,
		.read = handle_read,
		.write = handle_write,
	};
#else
//prepare the proc_op
static struct proc_ops my_pops={
	.proc_read = handle_read,
	.proc_write = handle_write,
};
#endif

void print_cmd_info(void);
void set_rule_by_cmd(rule_ptr_t rule_ptr,cmd_ptr_t cmd_ptr,chain_family_t* chain,index_t *index);
#endif
