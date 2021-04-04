#ifndef LOG_H
#define LOG_H

#include <string.h>
#include <fcntl.h>
#define LOG_BUFFER_SIZE 1000
#define VECTOR_SIZE 100

//defiantion of log level
typedef enum log_level_t{
	INFO,
	DEBU,
	ERROR,
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



#endif
