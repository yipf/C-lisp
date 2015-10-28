#include "core.h"

#include <math.h>

static hash_node_t HASH;
static index_t NUM;

hash_node_t alloc_hash_node(char* key,node_t value){
	hash_node_t node=calloc(1,sizeof(hash_node_));
	if(key)node->key=key; 	
	if(value)node->value=value;
	return node;
}

hash_node_t get_node(hash_node_t node,char* key){
	while(node && (key!=node->key)){	node=node->next; 	}
	return node;
}

hash_node_t init_hash_table(index_t num){
	NUM=num?num:11;
	HASH=calloc(num,sizeof(hash_node_));
	return HASH;
}

node_t hash_push_value(char* key,node_t value){
	hash_node_t head,node;
	head=HASH+key2hash(key,NUM);
	node=get_node(head->next,key);
	if(!node){	node=alloc_hash_node(key,0);	node->next=head->next; head->next=node; }
	head=node;
	node=alloc_hash_node(0,value); 	node->top=head->top;	head->top=node; 	
	return value;
}

node_t hash_pop_value(char* key,int pop){
	hash_node_t head,node;
	head=HASH+key2hash(key,NUM);
	node=get_node(head->next,key);
	if(!node){return 0;}
	head=node->top;
	if(head&&pop){node->top=head->top;}
	return head?(head->value):0;
}

int register_function(char* key,func_t func){
	node_t node=alloc_node(FUNCTION);
	node->ptr=func;
	hash_push_value(unique_string(key,0),node);
	return 0;
}

int push_values(node_t names,node_t args){
	while(names&&args){
		hash_push_value(names->string,args); 	
		names=names->cdr; args=args->cdr;
	}
	return 0;
}

int pop_values(node_t names){
	while(names){		hash_pop_value(names->string,1); 	names=names->cdr;	}
	return 0;
}

static node_t call_func(func_t func,node_t node){
	return func(node);
}

static node_t call_lambda(node_t names,node_t args){
	push_values(names->child,args);
	args=eval(names->cdr);
	pop_values(names);
	return args;
}

static node_t call(node_t F,node_t args){
	node_t names;
	func_t func;
	switch(F->type){
		case FUNCTION:  	return call_func(F->ptr,args);			break;
		case LAMBDA:		return call_lambda(F->child,args);	break;
		default:
			break;
	}
	MSG(call,"The 1st arg must be callable function or a lambda !")
	return alloc_node(NIL);
}

node_t mapf(func_t func,node_t start){
	node_t head,cur;
	cur=head=func(start); 
	while(start=start->cdr){  cur->cdr=func(start); 	cur=cur->cdr;	}
	return head;
}

int is_special_func(func_t func){
	return func==quote || func==lambda ;
}

node_t eval(node_t args){
	node_t first;
	func_t func;
	if(args){
		switch(args->type){
			case SYMBOL:
				return copy_node(hash_pop_value(args->string,0));			
				break;
			case LIST:			
				args=args->child;
				first=eval(args);
				if((first->type==FUNCTION)&&is_special_func(func=first->ptr)){return func(args->cdr);}
				return call(first,mapf(eval,args->cdr));			
				break;
			default: 
				break;
		}
	}
	return copy_node(args);
}

node_t apply(node_t args){
	if(args){		return call(args,args->cdr);	}
	MSG("APPLY","The 1st arg must be exist!")
	return alloc_node(NIL);
}

node_t lambda(node_t args){
	node_t cdr;
	cdr=args?args->cdr:0;
	if(args&&(args->type==LIST)&&cdr&&(cdr->type==LIST)){
		args=new_list(args);
		args->type=LAMBDA;
		return args;
	}
	MSG(lambda,"The 2 arg must be both lists !")
	return alloc_node(NIL);
}

node_t car(node_t args){
	if(args&&args->type==LIST){
		return copy_node(args->child);
	}
	MSG(car,"The 1st arg must be a list!");
	return alloc_node(NIL);
}

node_t SIN(node_t args){
	double number;
	if(args&&args->type==NUMBER){
		number=sin(args->number);
		args=alloc_node(NUMBER);
		args->number=number;
		return args;
	}
	MSG(sin,"The 1st arg must be a number!");
	return alloc_node(NIL);
}

node_t quote(node_t args){
	return copy_node(args);
}

node_t list(node_t args){
	return new_list(args);
}

node_t set(node_t args){
	if(args&&(args->type==SYMBOL)&&args->cdr){
		return copy_node(hash_push_value(args->string,args->cdr));
	}
	MSG(set,"The 1st arg must be a symbol and the second one must exist!");
	return alloc_node(NIL);
}

node_t cons(node_t args){
	node_t node;
	if( (node=args->cdr) && node->type==LIST ){
		args=copy_node(args);
		args->cdr=node->child;
		return new_list(args);
	}
	MSG(cons,"The second arg must be a list!");
	return alloc_node(NIL);
}

node_t cdr(node_t args){
	if(args&&args->type==LIST){
		return new_list((args->child)->cdr);
	}
	MSG(cdr,"The 1st arg must be a list!");
	return alloc_node(NIL);
}

node_t map(node_t args){
	node_t head,cur,start;
	if(args&&(start=args->cdr)&&start->type==LIST){
		start=start->child;
		cur=head=call(args,start);
		while(start=start->cdr){  cur->cdr=call(args,start); 	cur=cur->cdr;	}
		return new_list(head);
	}
	MSG(map,"The 2nd arg must be a list !")
	return alloc_node(NIL);
}

int init_functions(void){
	register_function("car",car);
	register_function("quote",quote);
	register_function("sin",SIN);
	
	register_function("eval",eval);
	register_function("apply",apply);
	register_function("list",list);
	register_function("set",set);
	register_function("cons",cons);
	register_function("lambda",lambda);
	register_function("map",map);
}