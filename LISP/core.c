#include "core.h"
#include <stdlib.h>
#include <string.h>

static hash_node_t HASH;
static hash_size_t NUM;

hash_node_t alloc_hash_node(char* key,value_t value){
	hash_node_t node=calloc(1,sizeof(hash_node_));
	node->key=key; 	node->value=value;
	return node;
}

hash_node_t get_node(hash_node_t node,char* key){
	while(node && (key!=node->key)){	node=node->next; 	}
	return node;
}

hash_node_t init_hash_table(hash_size_t num){
	NUM=num?num:11;
	HASH=calloc(num,sizeof(hash_node_));
	return HASH;
}

value_t hash_push_value(char* key,value_t value){
	hash_node_t head,node;
	head=HASH+key2hash(key,NUM);
	node=get_node(head->next,key);
	if(!node){	node=alloc_hash_node(key,0);	node->next=head->next; head->next=node; }
	head=node;
	node=alloc_hash_node(key,value); 	node->top=head->top;	head->top=node; 	
	return value;
}

value_t hash_pop_value(char* key,int pop){
	hash_node_t head,node;
	head=HASH+key2hash(key,NUM);
	node=get_node(head->next,key);
	if(!node){return 0;}
	head=node->top;
	if(head&&pop){node->top=head->top;}
	return head?(head->value):0;
}

node_t mapf(func_t func,node_t src){
	node_t head,dst;
	ASSERT(src,src)
	printf("\n mapf: "); node2stream(src,stdout); 	printf("\n");
	dst=head=func(src);
	if(dst){		while(src=src->cdr){ dst->cdr=func(src); 	dst=dst->cdr; }	}
	printf("\n head: "); node2stream(head,stdout); 	printf("\n");
	return head;
}

node_t apply(node_t args){
	value_t value;
	ASSERT(args,args)
	printf("\n apply: "); node2stream(args,stdout); 	printf("\n");
	node_t car,cdr;
	car=eval(args);
	ASSERT(car,args)
	printf("\n first:"); node2stream(car,stdout); printf("\n");
	value=car->car;
	if(value){
		switch(value->type){
			case FUNCTION:
				if(value->ptr==quote){return quote(args->cdr);}
				return ((func_t)(value->ptr))(mapf(eval,args->cdr));
				break;
			case LAMBDA:
				break;
			default: break;
		}
	}
	printf("\n rest:"); node2stream(args->cdr,stdout); 	printf("\n");
	args=alloc_node(value,mapf(eval,args->cdr));
	return alloc_node(alloc_value(LIST,args),0);
}

node_t eval(node_t node){
	value_t value;
	node_t head,cdr;
	if(!node) return node;
	value=node->car;
	if(value){
		switch(value->type){
			case SYMBOL:	return alloc_node(hash_pop_value(value->string,0),0);			break;		
			case LIST:			
				printf("\n eval: "); node2stream(node,stdout); 	printf("\n");
				return apply(value->ptr);									
			break;
			default: break;
		}
	}
	return alloc_node(value,0);
}

int register_func(char* key,func_t func){
	value_t value;
	value=alloc_value(FUNCTION,func);
	key=unique_string(key);
	hash_push_value(key,value);
	return 0;
}

node_t car(node_t args){
	value_t value;
	ASSERT(args,args)
	value=args->car;
	args=value->ptr;
	value=args->car;
	return alloc_node(value,0);
}

node_t cdr(node_t args){
	value_t value;
	args=mapf(eval,args);
	value=args->car;
	args=value->ptr;
	value=alloc_value(LIST,args->cdr);
	return alloc_node(value,0);
}

node_t cons(node_t args){
	args=mapf(eval,args);
	return args;
}

node_t quote(node_t args){
	return alloc_node(args->car,0);
}

node_t label(node_t args){
	value_t value;
	printf("\nlabel\n");
	value=args->car;
	value=hash_push_value(value->string,(args->cdr)->car);
	return alloc_node(value,0);
}

int init_functions(void){
	FUNC(eval)
	FUNC(apply)
	
	FUNC(quote)
	FUNC(car)
	FUNC(cdr)
	FUNC(cons)
	
	FUNC(label)
	//~ register_func("sin",SIN)
	//~ register_func("+",SIN)
	//~ register_func("-",SIN)
	//~ register_func("*",SIN)
	//~ register_func("/",SIN)
	return 0;
}


