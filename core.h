#ifndef CORE_H 
#define CORE_H

#include <stdio.h>
#include "node.h"
#include "environment.h"


static char BUFFER[100];
static char QUOTE_STR[];
static size_t count;
static char string_stop_flag;

int init_env(size_t num,size_t snum);
size_t append_char(char ch,char buffer[],size_t count);
node_t gen_atom_node(char buffer[],size_t count,int inside_string,int quote);
node_t quote_node(node_t node,int quote);
node_t append_node(node_t prev,node_t next);
node_t node2list(node_t node);
node_t stream2node(FILE* stream);

node_t apply_function(node_t func,node_t arg);
node_t apply_lambda(node_t l,node_t inputs);
node_t eval(node_t node);

#endif