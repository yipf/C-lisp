#ifndef GLOBAL_STRING_H 
#define GLOBAL_STRING_H 

#include "helper.h"

struct str_element_;

typedef struct str_element_{
	char* key;
	struct str_element_ * next;
}str_element_, * str_element_t;

static str_element_t* STRINGS;
static unsigned int STR_COUNT;

unsigned int init_strings(unsigned int count);
str_element_t create_str_element(char* key);
char* global_string(char* str);

#endif