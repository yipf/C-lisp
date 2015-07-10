#ifndef BASIC_TYPES_H 
#define BASIC_TYPES_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_


#define SHOW(var,format) printf("\n%s:%d:\t%s\t:: ",__FILE__,__LINE__,#var);printf(format,var);

#define SHOW_NODE(var) 	printf("\n%s:%d:\t%s\t:: ",__FILE__,__LINE__,#var);node2stream(var,stdout);printf("\n");;

#define ALLOC(data_type,size) (data_type*)malloc(sizeof(data_type)*(size))



enum TYPES{LIST=0,FUNCTION,LAMBDA,MACRO,ATOM,ARG,INTEGER,DOUBLE,STRING,STREAM};
char TYPE_STRINGS[20][20];

/* node_ & narray */

typedef char* string_t;

struct node_;

typedef struct node_ * (*meta_func) (struct node_ * n, struct node_ * extra);

typedef union{
		long int	ivalue;
		double 	dvalue;
		char*		svalue;
		struct node_ * child;
		FILE* stream;
		meta_func func;
} value_t;

typedef struct node_{
	char* key;	
	int type;	
	value_t value;
	struct node_ * next;
}node_,*node_t;

node_t create_node(char* key);
node_t create_list(node_t child);
node_t copy_node_props(node_t src,node_t dst);
node_t map_children(meta_func func,node_t child,node_t extra);
node_t copy_node(node_t src,node_t dst);
node_t destroy_node(node_t n);

static FILE* ostream;

int pretty_print_string(char * str,FILE* stream);
node_t node2stream_(node_t node,node_t extra);
node_t node2stream(node_t node,FILE* stream);

node_t make_stack(node_t bottom);
node_t push_stack(node_t stack,node_t element);
node_t pop_stack(node_t stack);
node_t get_top(node_t stack);

#endif