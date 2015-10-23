#define SHOW(var,format) printf("\n%s:%d:\t%s\t:: ",__FILE__,__LINE__,#var);printf(format,var);

struct node_;

typedef struct node_ * (*func_t) (struct node_ *);

typedef struct node_{
	union{
		double number;
		struct node_ *child;
		char* string;
		func_t func;
	};
	char* key;
	struct node_ *cdr;
	struct node_ *next; /* for gc, hash_table, ...*/
};

static node_t push_node(node_t top,node_t node);
static node_t pop_node(node_t top, int pop);
static node_t alloc_node(char* key);

typedef unsigned int hash_size_t;