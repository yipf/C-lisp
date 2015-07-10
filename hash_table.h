#ifndef HASH_TABLE_H 
#define HASH_TABLE_H

#include <string.h>
#include "basic_types.h"

typedef char* hash_key_t;

typedef struct{
	unsigned int num;
	node_t * data;
}hash_table_,*hash_table_t;

hash_table_t create_hash_table(unsigned int num);
unsigned int key2hash(hash_key_t key,hash_table_t ht);
static node_t get_prev(hash_table_t ht,hash_key_t key);
node_t get_hash_table(hash_table_t ht,hash_key_t key);
int set_hash_table(hash_table_t ht,hash_key_t key,node_t value);
int print_hash_table(hash_table_t ht,node_t sep);

#endif