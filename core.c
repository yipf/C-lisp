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

static int push_values(node_t names,node_t args){
	while(names&&args){
		hash_push_value(names->string,args); 	
		names=names->cdr; args=args->cdr;
	}
	return 0;
}

static int pop_values(node_t names){
	while(names){		hash_pop_value(names->string,1); 	names=names->cdr;	}
	return 0;
}

static node_t mapf(func_t func,node_t start){
	node_t head,cur;
	cur=head=func(start); 
	while(start=start->cdr){  cur->cdr=func(start); 	cur=cur->cdr;	}
	return head;
}

static node_t get_value(node_t node){
	return copy_node((node->type==SYMBOL)?hash_pop_value(node->string,0):node);
}

static node_t get_values(node_t node){
	if(node&&node->type==LIST){
		return new_list(mapf(get_value,node->child));
	}
	return get_value(node);
}

node_t eval(node_t args){
	node_t first;
	func_t func;
	MSG(eval,"") 	node2stream(args,stdout);
	if(args&&(args->type==LIST)){
		args=args->child;
		first=eval(args);
		args=args->cdr;
		switch(first->type){
			case FUNCTION:
				func=first->ptr;
				if( func==quote || func==macro || func==lambda ) {return func(args);}
				return func(mapf(eval,args));
				break;
			case LAMBDA: 	
				first=first->child;
				push_values(first->child,mapf(eval,args));
				args=eval(first->cdr);
				pop_values(first->child);
				return args;
				break;
			case MACRO:
				first=first->child;
				push_values(first->child,args);
				args=eval(get_values(first->cdr));
				pop_values(first->child);
				return args;
				break;
			default: break;
		}
		MSG(eval,"The 1st arg must be callable function or a lambda !")
		return alloc_node(NIL);
	}
	return get_value(args);
}

node_t apply(node_t args){
	if(args){		return eval(new_list(args));	}
	MSG("APPLY","The 1st arg must be exist!")
	return alloc_node(NIL);
}

node_t lambda(node_t args){
	node_t cdr;
	cdr=args?args->cdr:0;
	if(args&&(args->type==LIST)&&cdr){
		args=new_list(args);
		args->type=LAMBDA;
		return args;
	}
	MSG(lambda,"The 1st arg must be a  list and the 2nd one must exist!")
	return alloc_node(NIL);
}

node_t macro(node_t args){
	node_t cdr;
	cdr=args?args->cdr:0;
	if(args&&(args->type==LIST)&&cdr&&(cdr->type==LIST)){
		args=new_list(args);
		args->type=MACRO;
		return args;
	}
	MSG(macro,"The 1st arg must be a  list and the 2nd one must exist!")
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
		args->cdr=start;
		cur=head=eval(new_list(args));
		while(start=start->cdr){  args->cdr=start; cur->cdr=eval(new_list(args));	cur=cur->cdr;	}
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
	register_function("macro",macro);
	register_function("map",map);
}