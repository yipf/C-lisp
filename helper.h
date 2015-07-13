#ifndef HELPER_H 
#define HELPER_H 

#include <stdlib.h>
#include <string.h>

#define SHOW(var,format) printf("\n%s:%d:\t%s\t:: ",__FILE__,__LINE__,#var);printf(format,var);

#define ALLOC(data_type,size) (data_type*)malloc(sizeof(data_type)*(size))

unsigned int key2hash(char* key,unsigned int num);
char * copy_string(char* src, unsigned int len);

#endif