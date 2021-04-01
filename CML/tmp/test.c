#include <stdio.h>
#include <string.h>
int main(){
	char* token;
	char s[]="192.168.0.1";
	token=strtok(s,".");

	while(token!=NULL){
		printf("%s\n",token);
		token=strtok(NULL,".");
	}

	printf("raw string: %s\n",s);
	return 0;
}
