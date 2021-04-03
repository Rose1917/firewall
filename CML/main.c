#include "include/common.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <fcntl.h>

int main(int argc,char* argv[]){
	
	cmd_ptr_t cmd_ptr = parse_cmd(argc,argv);
	
	if(cmd_ptr!=NULL)
		exec(cmd_ptr);


	return 0;
}
