#include "built-in-functions.h"

#include <math.h>
#include <string.h>

int init_functions(){
	REGISTER_FUNC(sin)
	REGISTER_FUNC(define)
	REGISTER_FUNC(lambda)
	return 0;
}

node_t make_function_node(char* key,meta_func func){
	node_t node;
	node=create_atom(key);
	node->type=FUNCTION;
	(node->value).func=func;
	return node;
}

/* functions */
NAME_FUNC(define){
	node_t next;
	next=arg->next;
	if(next){		set_value(arg->key,next);	}
	return arg;
}

NAME_FUNC(sin){
	(arg->value).dvalue=sin(arg->value.dvalue);
	return arg;
}

NAME_FUNC(lambda){
	node_t list,child;
	child=create_atom("lambda");
	append_node(child,arg);
	list=create_list(child);
	list->type=LAMBDA;
	return list;
}