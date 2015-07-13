#ifndef ENVIRONMENT_H 
#define ENVIRONMENT_H 

#include <stdlib.h>
#include "node.h"

static node_t* ENV;
static unsigned int ENV_COUNT;

unsigned int init_environment(unsigned int count);

static node_t get_prev(char* key);
node_t set_value(char* key, node_t value);
node_t push_value(char* key, node_t value);
node_t get_value(char* key);
node_t pop_value(char* key);

#endif