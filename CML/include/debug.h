#ifndef DEBUG_H
#define DEBUG_H
#define DEBUG_BUFFER_SIZE 100
typedef enum STR_COLOR{
    RED_STR,BLUE_STR,GREEN_STR,YELLOW_STR,NONE_STR
}STR_COLOR;
typedef enum tab_level{
	LEVEL_0,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3
}tab_level;

//The printf format control string
#define COLOR_NONE "\033[0m" //表示清除前面设置的格式
#define RED "\033[1;31m" //40表示背景色为黑色, 1 表示高亮
//#define BLUE "\033[1;34;40m"//with background color
#define BLUE "\033[1;34m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"

int log_str(char* str,STR_COLOR color);
int log_error(char* str);
int log_debug(char* str);
int log_info(char* str);

static char debug_buffer[DEBUG_BUFFER_SIZE];
#define log_error_f(...) log_error((sprintf(debug_buffer,__VA_ARGS__),debug_buffer))
#define log_debug_f(...) log_debug((sprintf(debug_buffer,__VA_ARGS__),debug_buffer))
#define log_info_f(...) log_info((sprintf(debug_buffer,__VA_ARGS__),debug_buffer))

int log_error_tab(tab_level l,char* str);
int log_debug_tab(tab_level l,char* str);
int log_info_tab(tab_level l,char* str);


#define log_error_f_tab(x,...) log_error_tab(x,(sprintf(debug_buffer,__VA_ARGS__),debug_buffer))
#define log_debug_f_tab(x,...) log_debug_tab(x,(sprintf(debug_buffer,__VA_ARGS__),debug_buffer))
#define log_info_f_tab(x,...) log_info_tab(x,(sprintf(debug_buffer,__VA_ARGS__),debug_buffer))

#endif
