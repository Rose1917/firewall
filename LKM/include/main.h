#ifndef MAIN_H
#include "common.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/moduleparam.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#define MAIN_H

//function declarations
void init_filters(void);
void register_hookfunctions(void);
void unregister_hookfunctions(void);
void init_proc(void);
target_t check_rule(struct sk_buff* skb,rule_ptr_t rule);
#endif
