#include "core.h"

#include <stdlib.h>
#include <math.h>

node_t make_pair(node_t car,node_t cdr){
	return new_node(car->car,cdr);
}

node_t get_first(node_t list){
	value_t value;
	value=list->car;
	return (value&&(value->type==LIST))?(value->ptr):0;
}

node_t map(func_t func,node_t list){
	node_t head,cur,child;
	child=get_first(list);
	if(!child) return list;
	head=func(child);
	cur=head; 	child=child?(child->cdr):0;
	while(child){
		cur->cdr=func(child); 	cur=cur->cdr;
		child=child->cdr;
	}
	return new_list(head);
}

node_t eval(node_t node){
	value_t value,nvalue;
	node_t child;
	value=node->car;
	if(value){
		switch(value->type){
			case LIST:
				if(child=value->ptr){
					nvalue=child->car;
					switch(nvalue->type){
						case FUNCTION:
							return ((func_t)(nvalue->ptr))(child->cdr);
							break;
						case LAMBDA:
							break;
						default: break;
					}
				}
				break;
			case SYMBOL:
				return new_node(hash_pop_value(value->key,0),0);
				break;
			default: break;
		}
	}
	return new_node(value,0);
}

int register_function(char* key,func_t func){
	value_t value;
	value=new_value(0);
	value->type=FUNCTION;
	value->key=key;
	value->ptr=func;
	hash_push_value(key,value);
	return 0;
}

node_t CAR(node_t args){
	args=get_first(args);
	return args?new_node(args->car,0):0;
}

node_t CDR(node_t args){
	args=get_first(args);
	return args?new_list(args->cdr):0;
}

node_t CONS(node_t args){
	node_t list,cur;
	list=new_node(args->car,0);
	cur=list; 	args=get_first(args->cdr);
	while(args){
		cur->cdr=new_node(args->car,0);
		cur=cur->cdr; 	args=args->cdr;
	}
	return new_list(list);
}

node_t QUOTE(node_t args){
	return args;
}

node_t MAP(node_t args){
	value_t value;
	node_t node;
	node=eval(args);
	value=node->car;
	return map(value->ptr,args->cdr);
}

node_t SIN(node_t args){
	value_t value,nvalue;
	value=args->car;
	nvalue=new_value(0);
	nvalue->type=value->type;
	nvalue->number=sin(value->number);
	return new_node(nvalue,0);
}

node_t LABEL(node_t args){
	value_t key,value;
	key=args->car;
	args=args->cdr;
	value=args->car;
	value=hash_push_value(key->key,hash_pop_value(value->key,0));
	return new_node(value,0);
}


node_t ADD(node_t args){
	value_t value;
	double res;
	res=0.0;
	while(args){		value=args->car;		res+=value->number;	args=args->cdr;}
	value=new_value(0);	value->type=NUMBER;	value->number=res;
	return new_node(value,0);
}

node_t SUB(node_t args){
	value_t value;
	double res;
	res=0.0;
	while(args){		value=args->car;		res-=value->number;		args=args->cdr;}
	value=new_value(0);	value->type=NUMBER;	value->number=res;
	return new_node(value,0);
}

node_t MUL(node_t args){
	value_t value;
	double res;
	res=1.0;
	while(args){		value=args->car;		res*=value->number;	args=args->cdr;}
	value=new_value(0);	value->type=NUMBER;	value->number=res;
	return new_node(value,0);
}

node_t DIV(node_t args){
	value_t value;
	double res;
	res=1.0;
	while(args){		value=args->car;		res/=value->number; 	args=args->cdr;}
	value=new_value(0);	value->type=NUMBER;	value->number=res;
	return new_node(value,0);
}

int init_funcs(void){
	register_function("car",CAR);
	register_function("cdr",CDR);
	register_function("cons",CONS);
	register_function("quote",QUOTE);
	
	register_function("eval",eval);
	register_function("map",MAP);
	register_function("label",LABEL);
		
	register_function("sin",SIN);
	
	register_function("+",ADD);
	register_function("-",SUB);
	register_function("*",MUL);
	register_function("/",DIV);
	
	return 0;
}

