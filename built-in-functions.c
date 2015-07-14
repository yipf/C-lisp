#include "built-in-functions.h"

#include <math.h>
#include <string.h>
#include "global_string.h"

int init_functions(){
	register_function("define",lisp_define);
	register_function("car",lisp_car);
	register_function("cdr",lisp_cdr);
	register_function("cons",lisp_cons);

	register_function("sin",lisp_sin);
	
/* 	register_function("atom?",is_atom)
 * 	register_function("+",add)
 * 	register_function("-",sub)
 * 	register_function("*",mul)
 * 	register_function("/",div)
 */
	return 0;
}

node_t register_function(char* key,meta_func func){
	node_t node;
	key=global_string(key);
	node=create_atom(key);
	node->type=FUNCTION;
	(node->value).func=func;
	set_value(key,node);
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

NAME_FUNC(car){
	return (arg->value.child);
}

NAME_FUNC(cdr){
	node_t child;
	child=(arg->value).child;
	return child?create_list(child->next):0;
}

NAME_FUNC(cons){
	node_t list;
	arg=copy_node(arg,0);
	list=copy_node(arg->next,0);
	arg->next=(list->value).child;
	(list->value).child=arg;
	return list;
}