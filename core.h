#include "node.h"


node_t make_pair(node_t car,node_t cdr);
node_t first(node_t list);

typedef node_t (*func_t)(node_t args);

node_t CAR(node_t args);
node_t CDR(node_t args);
node_t CONS(node_t args);
