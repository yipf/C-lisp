#include "core.h"

#include <stdio.h>      /* printf, scanf, puts */
#include <stdlib.h>      /* calloc memcpy */
#include <string.h>      /* calloc memcpy */

node_t new_node(value_t value,node_t cdr){
	node_t node;
	node=calloc(1,sizeof(node_));
	node->car=value;
	node->cdr=cdr;
	return node;
}

node_t string2node(char* string){
	return new_node(string2value(string),0);
}

node_t new_list(node_t child){
	value_t value;
	value=new_value(0);
	value->type=LIST;
	value->ptr=child;
	return new_node(value,0);
}

int value2stream(value_* value,FILE*stream){
	if(!value){
		fprintf(stream,"() ");
		return 0;
	}
	switch(value->type){
		case LIST:
			fprintf(stream,"( ");
			node2stream(value->ptr,stream);
			fprintf(stream,") ");
			break;
		case NUMBER:
			fprintf(stream,"%lf ",value->number);
			break;
		case INTEGER:
			fprintf(stream,"%ld ",value->integer);
			break;
		default: 
			fprintf(stream,"%s ",value->key);
			break;
	}
	return 0;
}

node_t node2stream(node_t node,FILE* stream){
	if(node){
		value2stream(node->car,stream);
		node2stream(node->cdr,stream);
	}
	return 0;
}

int init_buffer(index_t n){
	LEN=n>0?n:1;
	BUFFER=calloc(LEN,sizeof(char));
	return 0;
}

char* new_string(char* string,index_t n){
	char *str;
	str=calloc(n+1,sizeof(char));
	memcpy(str,string,n);
	return str;
}

node_t stream2node(FILE* stream){
	value_t head,cur;
	char ch;
	index_t cur;
	head=new_value(0);
	while
	return new_node(head->next,0);
}


