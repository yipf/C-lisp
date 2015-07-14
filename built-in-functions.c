#include "built-in-functions.h"

#include <math.h>
#include <string.h>
#include "global_string.h"


static char TRUE[]="T";
static char FALSE[]="F";

int init_functions(){
	register_function("define",lisp_define);
	register_function("lambda",lisp_lambda);
	
	register_function("car",lisp_car);
	register_function("cdr",lisp_cdr);
	register_function("cons",lisp_cons);
	register_function("cond",lisp_cond);

	register_function("empty?",lisp_empty);
	
	register_function("sin",lisp_sin);
	
	
	set_value(TRUE,create_atom(TRUE));
	set_value(FALSE,create_atom(FALSE));
	
	register_function("+",lisp_add);
	register_function("-",lisp_sub);
	register_function("*",lisp_mul);
	register_function("/",lisp_div);
	
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

NAME_FUNC(add){
	node_t next;
	value_t value;
	next=arg->next;
	SHOW_NODE(arg)
	SHOW_NODE(next)
	value.dvalue=(arg->value).dvalue+(next->value).dvalue;
	return create_node(0,DOUBLE,value,0);
}

NAME_FUNC(sub){
	return 0;
}

NAME_FUNC(mul){
	return 0;
}

NAME_FUNC(div){
	return 0;
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
	list=copy_node(arg->next,0);
 	arg=copy_node(arg,0);
	arg->next=(list->value).child;
	(list->value).child=arg;
	return list;
}


NAME_FUNC(cond){
	if(arg->key==TRUE){
		return eval(arg->next);
	}else if(arg=arg->next){
		return eval(arg->next);
	}
	return arg;
}

NAME_FUNC(empty){
	printf("EMPTY?");
	SHOW_NODE(arg);
	return create_atom((arg->value).child?FALSE:TRUE);
}

