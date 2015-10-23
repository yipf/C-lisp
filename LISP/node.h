
#define MSG(var,format) printf("\n%s:%d:\t%s\t:: ",__FILE__,__LINE__,#var);printf(format,var);

#include <stdio.h>

typedef unsigned int index_t;

typedef struct mem_{
	void* ptr;
	struct mem_ * next;
}mem_,*mem_t;

mem_t push_ptr(mem_t head,void* ptr);

#define REGISTER_PTR(head,ptr) 	head=push_ptr(head,ptr);

/* value */

enum{LIST=0,SYMBOL,NUMBER,STRING,FUNCTION,LAMBDA};

typedef struct value_{
	union{
		double number;
		char* string;
		void* ptr;
	};
	int type;
}value_,*value_t;

value_t alloc_value(int type,void* ptr);

typedef struct string_node_{
	char* key;
	struct string_node_ * next;
}string_node_,*string_node_t;

int init_unique_strings(index_t num);
index_t key2hash(char* key,index_t num);
char* unique_string(char* string);

/* node */

typedef struct node_{
	value_t car;
	struct node_ * cdr;
}node_,*node_t;

node_t alloc_node(value_t car,node_t cdr);
node_t new_node(int type,char* string);
node_t node2stream(node_t node,FILE*stream);
int init_buffer(unsigned n);
char* new_string(char* string,unsigned n);
node_t stream2node(FILE*stream);