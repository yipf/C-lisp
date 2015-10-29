#include "node.h"

typedef node_t (*func_t)(node_t args);

typedef struct hash_node_{
	union{
		node_t value;
		char* key;
	};
	struct hash_node_ *top,*next;
}hash_node_,*hash_node_t;

static hash_node_t get_node(hash_node_t hnode,char* key);
static hash_node_t alloc_hash_node(char* key,node_t value);

hash_node_t init_hash_table(index_t num);
node_t hash_push_value(char* key,node_t value);
node_t hash_pop_value(char* key,int pop);

int register_function(char* key,func_t func);
int init_functions(void);

node_t eval(node_t args);
node_t car(node_t args);
node_t cdr(node_t args);
node_t quote(node_t args);

node_t list(node_t args);
node_t apply(node_t args);
node_t set(node_t args);
node_t cons(node_t args);
node_t lambda(node_t args);
node_t macro(node_t args);
node_t SIN(node_t args);