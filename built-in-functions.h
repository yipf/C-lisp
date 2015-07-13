#ifndef BUILT_IN_FUNCTIONS_H 
#define BUILT_IN_FUNCTIONS_H

#include <stdio.h>
#include "node.h"
#include "core.h"

#define REGISTER_FUNC(name) set_value(global_string(#name),make_function_node(#name,lisp_##name));
#define NAME_FUNC(name) node_t lisp_##name(node_t arg,node_t extra)

node_t make_function_node(char* key,meta_func func);

/* function list */

NAME_FUNC(define);
NAME_FUNC(sin);
NAME_FUNC(lambda);




#endif