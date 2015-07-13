#include "global_string.h"

#include <string.h>

static str_element_t* STRINGS=0;
static unsigned int STR_COUNT=0;

unsigned int init_strings(unsigned int count){
	unsigned int i;
	count=count?count:101;
	STR_COUNT=count;
	STRINGS=ALLOC(str_element_t,count);

	for(i=0;i<count;i++){	STRINGS[i]=0; 	}
	return count;
}

str_element_t create_str_element(char* key){
	str_element_t s;
	s=ALLOC(str_element_,1);
	s->key=key;
	return s;
}

char* global_string(char* str){
	str_element_t s;
	unsigned int hash;
	hash=key2hash(str,STR_COUNT);
	s=STRINGS[hash];
	if(!s){/* if the first is not existed */
		str=copy_string(str,strlen(str));
		STRINGS[hash]=create_str_element(str);
	}
	while(s&&strcmp(s->key,str)){s=s->next;}
	if(s){ 	/* if found one */
		return s->key; 
	}else{
		str=copy_string(str,strlen(str));
		s=create_str_element(str);
		s->next=STRINGS[hash]->next;
		STRINGS[hash]->next=s;
	}
	return str;
}