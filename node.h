#ifndef NODE_H
#define NODE_H

#include "str.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

enum{NIL=0,LIST,SYMBOL,NUMBER,STRING,FUNCTION,LAMBDA};

typedef struct node_{
	union{
		double number;
		char* string;
		struct node_ * child;
		void* ptr;
	};
	int type;
	struct node_* cdr;
	struct node_* top; /* for gc */
}node_,*node_t;

node_t PUSH(node_t node);
node_t POP(node_t end,int release);
index_t count_nodes(node_t end);

node_t alloc_node(int type);
node_t new_node(int type,char* string);
node_t copy_node(node_t src);
node_t new_list(node_t child);

char* buffer2string(index_t n);
node_t quote_node(node_t node,index_t quote);
node_t stream2node(FILE* stream);
int node2stream(node_t node,FILE* stream);

#endif