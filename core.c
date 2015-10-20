#include "core.h"

node_t make_pair(node_t car,node_t cdr){
	return new_node(car->car,cdr);
}

node_t first(node_t list){
	value_t value;
	value=list->car;
	return (value&&(value->type==LIST))?(value->ptr):0;
}

node_t CAR(node_t args){
	args=first(args);
	return args?new_node(args->car,0):0;
}

node_t CDR(node_t args){
	args=first(args);
	return args?new_list(args->cdr):0;
}

node_t CONS(node_t args){
	args=first(args);
	return args?make_pair(args,args->cdr):0;
}