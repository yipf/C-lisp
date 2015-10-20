
#include "value.h"
#include <stdio.h>

typedef struct node_{
	value_t car;
	struct node_ * cdr;
}node_,*node_t;

node_t new_node(value_t value,node_t cdr);
node_t new_list(node_t child);

node_t string2node(char* string);

int value2stream(value_* value,FILE*stream);
node_t node2stream(node_t node,FILE* stream);

typedef unsigned int index_t;

static char *BUFFER;
static index_t LEN;

int init_buffer(index_t n);
static char* new_string(char* string,index_t n);

node_t stream2node(FILE* stream);





