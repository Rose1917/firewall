#include "include/debug.h"

//Print the string with color.
int log_str(char* str,STR_COLOR color)
{
    switch (color){
        case RED_STR:
            printf(RED "%s" COLOR_NONE,str);
            break;
        case BLUE_STR:
            printf(BLUE "%s" COLOR_NONE,str);
            break;
        case GREEN_STR:
            printf(GREEN "%s" COLOR_NONE,str);
            break;
        case YELLOW_STR:
            printf(YELLOW "%s" COLOR_NONE,str);
            break;
        case NONE_STR:
            printf("%s",str);
            break;
        default:
            break;
        
    }
    printf("\n");
    return 0; 
}
int log_error(char* str){
    return log_str(str,RED_STR);
}
int log_info(char* str){
    return log_str(str,BLUE_STR);
}
int log_debug(char* str){
    return log_str(str,YELLOW_STR);
}

int log_error_tab(tab_level l,char* str){
	for(int i=0;i<l;i++){
		printf("\t");
	}
	log_error(str);
}


int log_debug_tab(tab_level l,char* str){
	for(int i=0;i<l;i++){
		printf("\t");
	}
	log_debug(str);
}

int log_info_tab(tab_level l,char* str){
	for(int i=0;i<l;i++){
		printf("\t");
	}
	log_info(str);
}
