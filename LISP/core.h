
#include "node.h"
/* hash_table */


typedef index_t hash_size_t;

typedef node_t (*func_t) (node_t);

typedef struct hash_node_{
	value_t value;
	char* key;
	struct hash_node_ *top,*next;
}hash_node_,*hash_node_t;

static hash_node_t get_node(hash_node_t node,char* key);
static hash_node_t alloc_hash_node(char* key,value_t value);

hash_node_t init_hash_table(hash_size_t num);
value_t hash_push_value(char* key,value_t value);
value_t hash_pop_value(char* key,int pop);

int register_func(char* key,func_t func);

#define FUNC(name) 	register_func(#name,name);

#define ASSERT(var,ret) if(!var){ printf("\n%s:%d:\t`%s' must be NON-ZERO!\t:: ",__FILE__,__LINE__,#var); return ret;}


node_t mapf(func_t func,node_t src);
node_t apply(node_t args);
node_t eval(node_t args);

int init_functions(void);

node_t apply(node_t args);
node_t eval(node_t args);
node_t car(node_t args);
node_t cdr(node_t args);
node_t cons(node_t args);
node_t quote(node_t args);





