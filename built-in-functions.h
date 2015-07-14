#ifndef BUILT_IN_FUNCTIONS_H 
#define BUILT_IN_FUNCTIONS_H

#include <stdio.h>
#include "node.h"
#include "core.h"

#define REGISTER_FUNC(key,fname) set_value(global_string(key),make_function_node(key,lisp_##fname));
#define NAME_FUNC(name) node_t lisp_##name(node_t arg)

node_t register_function(char* key,meta_func func);

/* function list */

NAME_FUNC(define);
NAME_FUNC(sin);
NAME_FUNC(lambda);
NAME_FUNC(car);
NAME_FUNC(cdr);
NAME_FUNC(cons);

#endif