#ifndef CORE_H 
#define CORE_H

#include <stdio.h>
#include "basic_types.h"
#include "hash_table.h"


static char BUFFER[100];
static char QUOTE_STR[];
static size_t count;
static char string_stop_flag;
hash_table_t ENV,UNIQ_STRINGS;

string_t uniq_string(char str[]);
node_t register_node(hash_key_t key, node_t node);
int init_env(size_t num,size_t snum);
size_t append_char(char ch,char buffer[],size_t count);
node_t gen_atom_node(char buffer[],size_t count,int inside_string,int quote);
node_t quote_node(node_t node,int quote);
node_t append_node(node_t prev,node_t next);
node_t node2list(node_t node);
node_t stream2node(FILE* stream);

node_t eval(node_t node,node_t extra);

#endif