
#ifndef STR_H
#define STR_H


#define MSG(var,format) printf("\n%s:%d:\t%s\t:: ",__FILE__,__LINE__,#var);printf(format,var);


typedef unsigned int index_t;

typedef struct string_node_{
	char* key;
	struct string_node_ * next;
}string_node_,*string_node_t;

int init_unique_strings(index_t num);
index_t key2hash(char* key,index_t num);
char* unique_string(char* string,int copy);
char* copy_string(char* src, index_t n);

int print_all_strings(void);

#endif