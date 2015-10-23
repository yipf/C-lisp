#include "node.h"

typedef node_t (*func_t)(node_t args);

node_t make_pair(node_t car,node_t cdr);
node_t get_first(node_t list);


int register_function(char* key,func_t func);

int init_funcs(void);

node_t map(func_t func,node_t child);
node_t eval(node_t node);

node_t CAR(node_t args);
node_t CDR(node_t args);
node_t CONS(node_t args);
